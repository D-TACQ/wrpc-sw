/*
 * This work is part of the White Rabbit project
 *
 * Copyright (C) 2011 CERN (www.cern.ch)
 * Copyright (C) 2012 GSI (www.gsi.de)
 * Author: Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
 * Author: Wesley W. Terpstra <w.terpstra@gsi.de>
 *
 * Released according to the GNU GPL, version 2 or any later version.
 */

/* Endpoint Packet Filter/Classifier driver

   A little explanation: The WR core needs to classify the incoming packets into
   two (or more categories):
   - PTP, ARP, DHCP packets, which should go to the WRCore CPU packet queue (mini-nic)
   - Other packets matching user's provided pattern, which shall go to the external fabric
     port - for example to Etherbone, host network controller, etc.
   - packets to be dropped (used neither by the WR Core or the user application)

  WR Endpoint (WR MAC) inside the WR Core therefore contains a simple microprogrammable
  packet filter/classifier. The classifier processes the incoming packet, and assigns it
  to one of 8 classes (an 8-bit word, where each bit corresponds to a particular class) or
  eventually drops it. Hardware implementation of the unit is a simple VLIW processor with
  32 single-bit registers (0 - 31). The registers are organized as follows:
  - 0: don't touch (always 0)
  - 1 - 22: general purpose registers
  - 23: drop packet flag: if 1 at the end of the packet processing, the packet will be dropped.
  - 24..31: packet class (class 0 = reg 24, class 7 = reg 31).

  Program memory has 64 36-bit words. Packet filtering program is restarted every time a new packet comes.
  There are 5 possible instructions:

  1. CMP offset, value, mask, oper, Rd:
  ------------------------------------------

  * Rd = Rd oper ((((uint16_t *)packet) [offset] & mask) == value)

  Examples:
  * CMP 3, 0xcafe, 0xffff, MOV, Rd
  will compare the 3rd word of the packet (bytes 6, 7) against 0xcafe and if the words are equal,
  1 will be written to Rd register.

  * CMP 4, 0xbabe, 0xffff, AND, Rd
  will do the same with the 4th word and write to Rd its previous value ANDed with the result
  of the comparison. Effectively, Rd now will be 1 only if bytes [6..9] of the payload contain word
  0xcafebabe.

  Note that the mask value is nibble-granular. That means you can choose a particular
  set of nibbles within a word to be compared, but not an arbitrary set of bits (e.g. 0xf00f, 0xff00
  and 0xf0f0 masks are ok, but 0x8001 is wrong.

  2. BTST offset, bit_number, oper, Rd
  ------------------------------------------

  * Rd = Rd oper (((uint16_t *)packet) [offset] & (1<<bit_number) ? 1 : 0)

  Examples:
  * BTST 3, 10, MOV, 11
  will write 1 to reg 11 if the 10th bit in the 3rd word of the packet is set (and 0 if it's clear)

  3. Logic opearations:
  -----------------------------------------
  * LOGIC2 Rd, Ra, OPER Rb - 2 argument logic (Rd = Ra OPER Rb). If the operation is MOV or NOT, Ra is
  taken as the source register.

  * LOGIC3 Rd, Ra, OPER Rb, OPER2, Rc - 3 argument logic Rd = (Ra OPER Rb) OPER2 Rc.

  4. Misc
  -----------------------------------------
  FIN instruction terminates the program.
  NOP executes a dummy instruction (LOGIC2 0, 0, AND, 0)

  IMPORTANT:
  - the program counter is advanved each time a 16-bit words of the packet arrives.
  - the CPU doesn't have any interlocks to simplify the HW, so you can't compare the
    10th word when  PC = 2. Max comparison offset is always equal to the address of the instruction.
  - Code may contain up to 64 operations, but it must classify shorter packets faster than in
    32 instructions (there's no flow throttling)
*/

#include <stdio.h>

#include "board.h"
#include <endpoint.h>
#include <hw/endpoint_regs.h>

#define PFILTER_MAX_CODE_SIZE      32

#define pfilter_dbg pp_printf

extern volatile struct EP_WB *EP;

static const uint64_t PF_MODE_LOGIC = (1ULL << 34);
static const uint64_t PF_MODE_CMP = 0ULL;

static int code_pos;
static uint64_t code_buf[PFILTER_MAX_CODE_SIZE];

enum pf_regs {
	R_ZERO = 1024,
	R_1, R_2, R_3, R_4, R_5, R_6, R_7, R_8, R_9, R_10,
	R_11, R_12, R_13, R_14, R_15, R_16, R_17, R_18, R_19, R_20,
	R_21, R_22,
	R_DROP,
	R_C0, R_C1, R_C2, R_C3, R_C4, R_C5, R_C6, R_C7
};
#define R_CLASS(x) (R_C0 + x)

/* Give also "symbolic" names, to the roles of each register. */
enum pf_symbolic_regs {

	/* The first set is used for straight comparisons */
	FRAME_BROADCAST = R_1,
	FRAME_PTP_MCAST = R_2,
	FRAME_OUR_MAC = R_3,
	FRAME_TYPE_IPV4 = R_4,
	FRAME_TYPE_PTP2 = R_5,
	FRAME_TYPE_ARP = R_6,
	FRAME_ICMP = R_7,
	FRAME_UDP = R_8,
	FRAME_TYPE_STREAMER = R_9, /* An ethtype by Tom, used in gateware */
	FRAME_PORT_ETHERBONE = R_16,

	/* These are results of logic over the previous bits  */
	FRAME_IP_UNI = R_10,
	FRAME_IP_OK = R_11, /* unicast or broadcast */

	/* A temporary register, and the CPU target */
	R_TMP = R_14,
	FRAME_FOR_CPU = R_15

};

int v1[R_C7 - (R_ZERO + 31)]; /* fails if we lost  a register */
int v2[(R_ZERO + 31) - R_C7]; /* fails if we added a register */



/* begins assembling a new packet filter program */
static void pfilter_new(void)
{
	code_pos = 0;
}

static void check_size(void)
{
	if (code_pos == PFILTER_MAX_CODE_SIZE - 1) {
		pfilter_dbg("microcode: code too big (max size: %d)\n",
			    PFILTER_MAX_CODE_SIZE);
	}
}

static void check_reg_range(int val, int minval, int maxval, char *name)
{
	if (val < minval || val > maxval) {
		pfilter_dbg("microcode: %s register out of range (%d to %d)",
			    name, minval, maxval);
	}
}

static void pfilter_cmp(int offset, int value, int mask, pfilter_op_t op,
			int rd)
{
	uint64_t ir;

	check_size();

	if (offset > code_pos)
		pfilter_dbg
		    ("microcode: comparison offset is bigger than current PC. Insert some nops before comparing");

	check_reg_range(rd, R_1, R_15, "ra/rd");
	rd -= R_ZERO;

	ir = (PF_MODE_CMP | ((uint64_t) offset << 7)
	      | ((mask & 0x1) ? (1ULL << 29) : 0)
	      | ((mask & 0x10) ? (1ULL << 30) : 0)
	      | ((mask & 0x100) ? (1ULL << 31) : 0)
	      | ((mask & 0x1000) ? (1ULL << 32) : 0))
	    | op | (rd << 3);

	ir = ir | ((uint64_t) value & 0xffffULL) << 13;

	code_buf[code_pos++] = ir;
}

static void pfilter_nop(void)
{
	uint64_t ir;
	check_size();
	ir = PF_MODE_LOGIC;
	code_buf[code_pos++] = ir;
}

  // rd  = ra op rb
static void pfilter_logic2(int rd, int ra, pfilter_op_t op, int rb)
{
	uint64_t ir;
	check_size();
	check_reg_range(ra, R_ZERO, R_C7, "ra"); ra -= R_ZERO;
	check_reg_range(rb, R_ZERO, R_C7, "rb"); rb -= R_ZERO;
	check_reg_range(rd, R_1,    R_C7, "rd"); rd -= R_ZERO;

	ir = ((uint64_t) ra << 8) | ((uint64_t) rb << 13) |
	    (((uint64_t) rd & 0xf) << 3) | (((uint64_t) rd & 0x10) ? (1ULL << 7)
					    : 0) | (uint64_t) op;
	ir = ir | PF_MODE_LOGIC | (3ULL << 23);
	code_buf[code_pos++] = ir;
}

static void pfilter_logic3(int rd, int ra, pfilter_op_t op, int rb,
			   pfilter_op_t op2, int rc)
{
	uint64_t ir;
	check_size();
	check_reg_range(ra, R_ZERO, R_C7, "ra"); ra -= R_ZERO;
	check_reg_range(rb, R_ZERO, R_C7, "rb"); rb -= R_ZERO;
	check_reg_range(rc, R_ZERO, R_C7, "rc"); rc -= R_ZERO;
	check_reg_range(rd, R_1,    R_C7, "rd"); rd -= R_ZERO;

	ir = (ra << 8) | (rb << 13) | (rc << 18) | ((rd & 0xf) << 3) |
	    ((rd & 0x10) ? (1 << 7) : 0) | op;
	ir = ir | PF_MODE_LOGIC | (op2 << 23);
	code_buf[code_pos++] = ir;
}

/* Terminates the microcode, loads it to the endpoint and enables the pfilter */
static void pfilter_load(void)
{
	int i;
	code_buf[code_pos++] = (1ULL << 35);	// insert FIN instruction

	EP->PFCR0 = 0;		// disable pfilter

	for (i = 0; i < code_pos; i++) {
		pfilter_dbg("pos %02i: %x.%08x\n", i, (uint32_t)(code_buf[i] >> 32), (uint32_t)(code_buf[i]));
		uint32_t cr0, cr1;
		cr1 = EP_PFCR1_MM_DATA_LSB_W(code_buf[i] & 0xfff);
		cr0 =
		    EP_PFCR0_MM_ADDR_W(i) | EP_PFCR0_MM_DATA_MSB_W(code_buf[i]
								   >> 12) |
		    EP_PFCR0_MM_WRITE_MASK;

		EP->PFCR1 = cr1;
		EP->PFCR0 = cr0;
	}

	EP->PFCR0 = EP_PFCR0_ENABLE;
}

/* sample packet filter initialization:
- redirects broadcasts and PTP packets to the WR Core
- redirects unicasts addressed to self with ethertype 0xa0a0 to the external fabric */

void pfilter_init_default()
{
	pfilter_new();
	pfilter_nop();

	pfilter_cmp(0, 0xffff, 0xffff, MOV, FRAME_BROADCAST);
	pfilter_cmp(1, 0xffff, 0xffff, AND, FRAME_BROADCAST);
	pfilter_cmp(2, 0xffff, 0xffff, AND, FRAME_BROADCAST);	/* r1 = 1 when dst mac is broadcast */
	pfilter_cmp(0, 0x011b, 0xffff, MOV, FRAME_PTP_MCAST);
	pfilter_cmp(1, 0x1900, 0xffff, AND, FRAME_PTP_MCAST);
	pfilter_cmp(2, 0x0000, 0xffff, AND, FRAME_PTP_MCAST);	/* r2 = 1 when dst mac is PTP multicast (01:1b:19:00:00:00) */
	pfilter_cmp(0, EP->MACH & 0xffff, 0xffff, MOV, FRAME_OUR_MAC);
	pfilter_cmp(1, EP->MACL >> 16, 0xffff, AND, FRAME_OUR_MAC);
	pfilter_cmp(2, EP->MACL & 0xffff, 0xffff, AND, FRAME_OUR_MAC);	/* r3 = 1 when the packet is unicast to our own MAC */
	pfilter_cmp(6, 0x0800, 0xffff, MOV, FRAME_TYPE_IPV4);	/* r4 = 1 when ethertype = IPv4 */
	pfilter_cmp(6, 0x88f7, 0xffff, MOV, FRAME_TYPE_PTP2);	/* r5 = 1 when ethertype = PTPv2 */
	pfilter_cmp(6, 0x0806, 0xffff, MOV, FRAME_TYPE_ARP);	/* r6 = 1 when ethertype = ARP */
	pfilter_cmp(6, 0xdbff, 0xffff, MOV, FRAME_TYPE_STREAMER);	/* r9 = 1 when ethertype = streamer */

	/* Ethernet = 14 bytes, Offset to type in IP: 8 bytes = 22/2 = 11 */
	pfilter_cmp(11, 0x0001, 0x00ff, MOV, FRAME_ICMP);	/* r7 = 1 when IP type = ICMP */
	pfilter_cmp(11, 0x0011, 0x00ff, MOV, FRAME_UDP);	/* r8 = 1 when IP type = UDP */

#ifdef CONFIG_ETHERBONE

	pfilter_logic3(FRAME_IP_UNI, FRAME_OUR_MAC, OR, R_ZERO, AND, FRAME_TYPE_IPV4);	/* r10 = IP(unicast) */
	pfilter_logic3(FRAME_IP_OK, FRAME_BROADCAST, OR, FRAME_OUR_MAC, AND, FRAME_TYPE_IPV4);	/* r11 = IP(unicast+broadcast) */

	pfilter_logic3(R_TMP, FRAME_BROADCAST, AND, FRAME_TYPE_ARP, OR, FRAME_TYPE_PTP2);	/* r14 = ARP(broadcast) or PTPv2 */
	pfilter_logic3(FRAME_FOR_CPU, FRAME_IP_UNI, AND, FRAME_ICMP, OR, R_TMP);	/* r15 = ICMP/IP(unicast) or ARP(broadcast) or PTPv2 */

	/* Ethernet = 14 bytes, IPv4 = 20 bytes, offset to dport: 2 = 36/2 = 18 */
	pfilter_cmp(18, 0x0044, 0xffff, MOV, R_TMP);	/* r14 = 1 when dport = BOOTPC */

	pfilter_logic3(R_TMP, R_TMP, AND, FRAME_UDP, AND, FRAME_IP_OK);	/* r14 = BOOTP/UDP/IP(unicast|broadcast) */
	pfilter_logic2(FRAME_FOR_CPU, R_TMP, OR, FRAME_FOR_CPU);	/* r15 = BOOTP/UDP/IP(unicast|broadcast) or ICMP/IP(unicast) or ARP(broadcast) or PTPv2 */

	#ifdef CONFIG_NIC_PFILTER

		pfilter_cmp(18,0xebd0,0xffff,MOV, FRAME_PORT_ETHERBONE); /* r16 = 1 when dport = ETHERBONE */

		//pfilter_cmp(21,0x4e6f,0xffff,MOV,R_9); /* r9 = 1 when magic number = ETHERBONE */
		//pfilter_logic2(R_6,R_6,AND,R_9);

		pfilter_logic2(R_CLASS(0), FRAME_FOR_CPU, MOV, R_ZERO); /* class 0: ICMP/IP(unicast) or ARP(broadcast) or PTPv2 => PTP LM32 core */
		pfilter_logic2(R_CLASS(5), FRAME_PORT_ETHERBONE, OR, R_ZERO); /* class 5: Etherbone packet => Etherbone Core */
		pfilter_logic3(R_CLASS(7), FRAME_FOR_CPU, OR, FRAME_PORT_ETHERBONE, NOT, R_ZERO); /* class 7: Rest => NIC Core */

	#else
		pfilter_logic3(R_TMP, FRAME_IP_OK, AND, FRAME_UDP, OR, FRAME_FOR_CPU);	/* Something we accept */

		pfilter_logic3(R_DROP, R_TMP, OR, FRAME_TYPE_STREAMER, NOT, R_ZERO);	/* None match? drop */

		pfilter_logic2(R_CLASS(7), FRAME_IP_OK, AND, FRAME_UDP);	/* class 7: UDP/IP(unicast|broadcast) => external fabric */
		pfilter_logic2(R_CLASS(6), FRAME_BROADCAST, AND, FRAME_TYPE_STREAMER);	/* class 6: streamer broadcasts => external fabric */
		pfilter_logic2(R_CLASS(0), FRAME_FOR_CPU, MOV, R_ZERO);	/* class 0: ICMP/IP(unicast) or ARP(broadcast) or PTPv2 => PTP LM32 core */

	#endif

#else
	pfilter_logic3(FRAME_IP_UNI, FRAME_OUR_MAC, OR, FRAME_PTP_MCAST, AND, FRAME_TYPE_PTP2);	/* r10 = PTP (multicast or unicast) */
	pfilter_logic2(FRAME_IP_OK, FRAME_BROADCAST, AND, FRAME_TYPE_STREAMER);		/* r11 = streamer broadcast */
	pfilter_logic3(R_TMP, FRAME_IP_UNI, OR, FRAME_IP_OK, NOT, R_ZERO); /* all non-PTP and non-streamer traffic */

	pfilter_logic2(R_CLASS(7), R_TMP, MOV, R_ZERO);	/* class 7: all non PTP and non-streamer
						   traffic => external fabric */
	pfilter_logic2(R_CLASS(6), FRAME_IP_OK, MOV, R_ZERO); /* class 6: streamer broadcasts =>
						   external fabric */
	pfilter_logic2(R_CLASS(0), FRAME_IP_UNI, MOV, R_ZERO); /* class 0: PTP frames => LM32 */

#endif

	pfilter_load();

}
