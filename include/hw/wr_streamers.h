/*
  Register definitions for slave core: WR Transmission control, status and debug

  * File           : ./doc/wr_streamers.h
  * Author         : auto-generated by wbgen2 from wr_streamers_wb.wb
  * Created        : Thu May 23 16:11:14 2019
  * Version        : 0x00000002
  * Standard       : ANSI C

    THIS FILE WAS GENERATED BY wbgen2 FROM SOURCE FILE wr_streamers_wb.wb
    DO NOT HAND-EDIT UNLESS IT'S ABSOLUTELY NECESSARY!

*/

#ifndef __WBGEN2_REGDEFS_WR_STREAMERS_WB_WB
#define __WBGEN2_REGDEFS_WR_STREAMERS_WB_WB

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <inttypes.h>
#endif

#if defined( __GNUC__)
#define PACKED __attribute__ ((packed))
#else
#error "Unsupported compiler?"
#endif

#ifndef __WBGEN2_MACROS_DEFINED__
#define __WBGEN2_MACROS_DEFINED__
#define WBGEN2_GEN_MASK(offset, size) (((1<<(size))-1) << (offset))
#define WBGEN2_GEN_WRITE(value, offset, size) (((value) & ((1<<(size))-1)) << (offset))
#define WBGEN2_GEN_READ(reg, offset, size) (((reg) >> (offset)) & ((1<<(size))-1))
#define WBGEN2_SIGN_EXTEND(value, bits) (((value) & (1<<bits) ? ~((1<<(bits))-1): 0 ) | (value))
#endif

/* version definition */
#define WBGEN2_WR_STREAMERS_VERSION 0x00000002


/* definitions for register: Version register */

/* definitions for field: Version identifier in reg: Version register */
#define WR_STREAMERS_VER_ID_MASK              WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_VER_ID_SHIFT             0
#define WR_STREAMERS_VER_ID_W(value)          WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_VER_ID_R(reg)            WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Statistics status and ctrl register */

/* definitions for field: Reset statistics in reg: Statistics status and ctrl register */
#define WR_STREAMERS_SSCR1_RST_STATS          WBGEN2_GEN_MASK(0, 1)

/* definitions for field: Reset tx seq id in reg: Statistics status and ctrl register */
#define WR_STREAMERS_SSCR1_RST_SEQ_ID         WBGEN2_GEN_MASK(1, 1)

/* definitions for field: Snapshot statistics in reg: Statistics status and ctrl register */
#define WR_STREAMERS_SSCR1_SNAPSHOT_STATS     WBGEN2_GEN_MASK(2, 1)

/* definitions for field: Latency accumulator overflow in reg: Statistics status and ctrl register */
#define WR_STREAMERS_SSCR1_RX_LATENCY_ACC_OVERFLOW WBGEN2_GEN_MASK(3, 1)

/* definitions for field: Reset timestamp cycles in reg: Statistics status and ctrl register */
#define WR_STREAMERS_SSCR1_RST_TS_CYC_MASK    WBGEN2_GEN_MASK(4, 28)
#define WR_STREAMERS_SSCR1_RST_TS_CYC_SHIFT   4
#define WR_STREAMERS_SSCR1_RST_TS_CYC_W(value) WBGEN2_GEN_WRITE(value, 4, 28)
#define WR_STREAMERS_SSCR1_RST_TS_CYC_R(reg)  WBGEN2_GEN_READ(reg, 4, 28)

/* definitions for register: Statistics status and ctrl register */

/* definitions for field: Reset timestamp 32 LSB of TAI in reg: Statistics status and ctrl register */
#define WR_STREAMERS_SSCR2_RST_TS_TAI_LSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_SSCR2_RST_TS_TAI_LSB_SHIFT 0
#define WR_STREAMERS_SSCR2_RST_TS_TAI_LSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_SSCR2_RST_TS_TAI_LSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Statistics status and ctrl register */

/* definitions for field: Reset timestamp 8 MSB of TAI in reg: Statistics status and ctrl register */
#define WR_STREAMERS_SSCR3_RST_TS_TAI_MSB_MASK WBGEN2_GEN_MASK(0, 8)
#define WR_STREAMERS_SSCR3_RST_TS_TAI_MSB_SHIFT 0
#define WR_STREAMERS_SSCR3_RST_TS_TAI_MSB_W(value) WBGEN2_GEN_WRITE(value, 0, 8)
#define WR_STREAMERS_SSCR3_RST_TS_TAI_MSB_R(reg) WBGEN2_GEN_READ(reg, 0, 8)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer frame latency in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT0_RX_LATENCY_MAX_MASK WBGEN2_GEN_MASK(0, 28)
#define WR_STREAMERS_RX_STAT0_RX_LATENCY_MAX_SHIFT 0
#define WR_STREAMERS_RX_STAT0_RX_LATENCY_MAX_W(value) WBGEN2_GEN_WRITE(value, 0, 28)
#define WR_STREAMERS_RX_STAT0_RX_LATENCY_MAX_R(reg) WBGEN2_GEN_READ(reg, 0, 28)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer frame latency in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT1_RX_LATENCY_MIN_MASK WBGEN2_GEN_MASK(0, 28)
#define WR_STREAMERS_RX_STAT1_RX_LATENCY_MIN_SHIFT 0
#define WR_STREAMERS_RX_STAT1_RX_LATENCY_MIN_W(value) WBGEN2_GEN_WRITE(value, 0, 28)
#define WR_STREAMERS_RX_STAT1_RX_LATENCY_MIN_R(reg) WBGEN2_GEN_READ(reg, 0, 28)

/* definitions for register: Tx statistics */

/* definitions for field: WR Streamer frame sent count (LSB) in reg: Tx statistics */
#define WR_STREAMERS_TX_STAT2_TX_SENT_CNT_LSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_TX_STAT2_TX_SENT_CNT_LSB_SHIFT 0
#define WR_STREAMERS_TX_STAT2_TX_SENT_CNT_LSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_TX_STAT2_TX_SENT_CNT_LSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Tx statistics */

/* definitions for field: WR Streamer frame sent count (MSB) in reg: Tx statistics */
#define WR_STREAMERS_TX_STAT3_TX_SENT_CNT_MSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_TX_STAT3_TX_SENT_CNT_MSB_SHIFT 0
#define WR_STREAMERS_TX_STAT3_TX_SENT_CNT_MSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_TX_STAT3_TX_SENT_CNT_MSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer frame received count (LSB) in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT4_RX_RCVD_CNT_LSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_STAT4_RX_RCVD_CNT_LSB_SHIFT 0
#define WR_STREAMERS_RX_STAT4_RX_RCVD_CNT_LSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_STAT4_RX_RCVD_CNT_LSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer frame received count (MSB) in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT5_RX_RCVD_CNT_MSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_STAT5_RX_RCVD_CNT_MSB_SHIFT 0
#define WR_STREAMERS_RX_STAT5_RX_RCVD_CNT_MSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_STAT5_RX_RCVD_CNT_MSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer frame loss count (LSB) in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT6_RX_LOSS_CNT_LSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_STAT6_RX_LOSS_CNT_LSB_SHIFT 0
#define WR_STREAMERS_RX_STAT6_RX_LOSS_CNT_LSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_STAT6_RX_LOSS_CNT_LSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer frame loss count (MSB) in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT7_RX_LOSS_CNT_MSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_STAT7_RX_LOSS_CNT_MSB_SHIFT 0
#define WR_STREAMERS_RX_STAT7_RX_LOSS_CNT_MSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_STAT7_RX_LOSS_CNT_MSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer block loss count (LSB) in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT8_RX_LOST_BLOCK_CNT_LSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_STAT8_RX_LOST_BLOCK_CNT_LSB_SHIFT 0
#define WR_STREAMERS_RX_STAT8_RX_LOST_BLOCK_CNT_LSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_STAT8_RX_LOST_BLOCK_CNT_LSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer block loss count (MSB) in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT9_RX_LOST_BLOCK_CNT_MSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_STAT9_RX_LOST_BLOCK_CNT_MSB_SHIFT 0
#define WR_STREAMERS_RX_STAT9_RX_LOST_BLOCK_CNT_MSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_STAT9_RX_LOST_BLOCK_CNT_MSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer frame latency (LSB) in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT10_RX_LATENCY_ACC_LSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_STAT10_RX_LATENCY_ACC_LSB_SHIFT 0
#define WR_STREAMERS_RX_STAT10_RX_LATENCY_ACC_LSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_STAT10_RX_LATENCY_ACC_LSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer frame latency (MSB) in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT11_RX_LATENCY_ACC_MSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_STAT11_RX_LATENCY_ACC_MSB_SHIFT 0
#define WR_STREAMERS_RX_STAT11_RX_LATENCY_ACC_MSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_STAT11_RX_LATENCY_ACC_MSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer frame latency counter (LSB) in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT12_RX_LATENCY_ACC_CNT_LSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_STAT12_RX_LATENCY_ACC_CNT_LSB_SHIFT 0
#define WR_STREAMERS_RX_STAT12_RX_LATENCY_ACC_CNT_LSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_STAT12_RX_LATENCY_ACC_CNT_LSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer frame latency counter (MSB) in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT13_RX_LATENCY_ACC_CNT_MSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_STAT13_RX_LATENCY_ACC_CNT_MSB_SHIFT 0
#define WR_STREAMERS_RX_STAT13_RX_LATENCY_ACC_CNT_MSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_STAT13_RX_LATENCY_ACC_CNT_MSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Tx Config Reg 0 */

/* definitions for field: Ethertype in reg: Tx Config Reg 0 */
#define WR_STREAMERS_TX_CFG0_ETHERTYPE_MASK   WBGEN2_GEN_MASK(0, 16)
#define WR_STREAMERS_TX_CFG0_ETHERTYPE_SHIFT  0
#define WR_STREAMERS_TX_CFG0_ETHERTYPE_W(value) WBGEN2_GEN_WRITE(value, 0, 16)
#define WR_STREAMERS_TX_CFG0_ETHERTYPE_R(reg) WBGEN2_GEN_READ(reg, 0, 16)

/* definitions for register: Tx Config Reg 1 */

/* definitions for field: MAC Local LSB in reg: Tx Config Reg 1 */
#define WR_STREAMERS_TX_CFG1_MAC_LOCAL_LSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_TX_CFG1_MAC_LOCAL_LSB_SHIFT 0
#define WR_STREAMERS_TX_CFG1_MAC_LOCAL_LSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_TX_CFG1_MAC_LOCAL_LSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Tx Config Reg 2 */

/* definitions for field: MAC Local MSB in reg: Tx Config Reg 2 */
#define WR_STREAMERS_TX_CFG2_MAC_LOCAL_MSB_MASK WBGEN2_GEN_MASK(0, 16)
#define WR_STREAMERS_TX_CFG2_MAC_LOCAL_MSB_SHIFT 0
#define WR_STREAMERS_TX_CFG2_MAC_LOCAL_MSB_W(value) WBGEN2_GEN_WRITE(value, 0, 16)
#define WR_STREAMERS_TX_CFG2_MAC_LOCAL_MSB_R(reg) WBGEN2_GEN_READ(reg, 0, 16)

/* definitions for register: Tx Config Reg 3 */

/* definitions for field: MAC Target LSB in reg: Tx Config Reg 3 */
#define WR_STREAMERS_TX_CFG3_MAC_TARGET_LSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_TX_CFG3_MAC_TARGET_LSB_SHIFT 0
#define WR_STREAMERS_TX_CFG3_MAC_TARGET_LSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_TX_CFG3_MAC_TARGET_LSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Tx Config Reg 4 */

/* definitions for field: MAC Target MSB in reg: Tx Config Reg 4 */
#define WR_STREAMERS_TX_CFG4_MAC_TARGET_MSB_MASK WBGEN2_GEN_MASK(0, 16)
#define WR_STREAMERS_TX_CFG4_MAC_TARGET_MSB_SHIFT 0
#define WR_STREAMERS_TX_CFG4_MAC_TARGET_MSB_W(value) WBGEN2_GEN_WRITE(value, 0, 16)
#define WR_STREAMERS_TX_CFG4_MAC_TARGET_MSB_R(reg) WBGEN2_GEN_READ(reg, 0, 16)

/* definitions for register: Tx Config Reg 4 */

/* definitions for field: Enable tagging with Qtags in reg: Tx Config Reg 4 */
#define WR_STREAMERS_TX_CFG5_QTAG_ENA         WBGEN2_GEN_MASK(0, 1)

/* definitions for field: VLAN ID in reg: Tx Config Reg 4 */
#define WR_STREAMERS_TX_CFG5_QTAG_VID_MASK    WBGEN2_GEN_MASK(8, 12)
#define WR_STREAMERS_TX_CFG5_QTAG_VID_SHIFT   8
#define WR_STREAMERS_TX_CFG5_QTAG_VID_W(value) WBGEN2_GEN_WRITE(value, 8, 12)
#define WR_STREAMERS_TX_CFG5_QTAG_VID_R(reg)  WBGEN2_GEN_READ(reg, 8, 12)

/* definitions for field: Priority in reg: Tx Config Reg 4 */
#define WR_STREAMERS_TX_CFG5_QTAG_PRIO_MASK   WBGEN2_GEN_MASK(24, 3)
#define WR_STREAMERS_TX_CFG5_QTAG_PRIO_SHIFT  24
#define WR_STREAMERS_TX_CFG5_QTAG_PRIO_W(value) WBGEN2_GEN_WRITE(value, 24, 3)
#define WR_STREAMERS_TX_CFG5_QTAG_PRIO_R(reg) WBGEN2_GEN_READ(reg, 24, 3)

/* definitions for register: Rx Config Reg 0 */

/* definitions for field: Ethertype in reg: Rx Config Reg 0 */
#define WR_STREAMERS_RX_CFG0_ETHERTYPE_MASK   WBGEN2_GEN_MASK(0, 16)
#define WR_STREAMERS_RX_CFG0_ETHERTYPE_SHIFT  0
#define WR_STREAMERS_RX_CFG0_ETHERTYPE_W(value) WBGEN2_GEN_WRITE(value, 0, 16)
#define WR_STREAMERS_RX_CFG0_ETHERTYPE_R(reg) WBGEN2_GEN_READ(reg, 0, 16)

/* definitions for field: Accept Broadcast in reg: Rx Config Reg 0 */
#define WR_STREAMERS_RX_CFG0_ACCEPT_BROADCAST WBGEN2_GEN_MASK(16, 1)

/* definitions for field: Filter Remote in reg: Rx Config Reg 0 */
#define WR_STREAMERS_RX_CFG0_FILTER_REMOTE    WBGEN2_GEN_MASK(17, 1)

/* definitions for register: Rx Config Reg 1 */

/* definitions for field: MAC Local LSB in reg: Rx Config Reg 1 */
#define WR_STREAMERS_RX_CFG1_MAC_LOCAL_LSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_CFG1_MAC_LOCAL_LSB_SHIFT 0
#define WR_STREAMERS_RX_CFG1_MAC_LOCAL_LSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_CFG1_MAC_LOCAL_LSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx Config Reg 2 */

/* definitions for field: MAC Local MSB in reg: Rx Config Reg 2 */
#define WR_STREAMERS_RX_CFG2_MAC_LOCAL_MSB_MASK WBGEN2_GEN_MASK(0, 16)
#define WR_STREAMERS_RX_CFG2_MAC_LOCAL_MSB_SHIFT 0
#define WR_STREAMERS_RX_CFG2_MAC_LOCAL_MSB_W(value) WBGEN2_GEN_WRITE(value, 0, 16)
#define WR_STREAMERS_RX_CFG2_MAC_LOCAL_MSB_R(reg) WBGEN2_GEN_READ(reg, 0, 16)

/* definitions for register: Rx Config Reg 3 */

/* definitions for field: MAC Remote LSB in reg: Rx Config Reg 3 */
#define WR_STREAMERS_RX_CFG3_MAC_REMOTE_LSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_CFG3_MAC_REMOTE_LSB_SHIFT 0
#define WR_STREAMERS_RX_CFG3_MAC_REMOTE_LSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_CFG3_MAC_REMOTE_LSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx Config Reg 4 */

/* definitions for field: MAC Remote MSB in reg: Rx Config Reg 4 */
#define WR_STREAMERS_RX_CFG4_MAC_REMOTE_MSB_MASK WBGEN2_GEN_MASK(0, 16)
#define WR_STREAMERS_RX_CFG4_MAC_REMOTE_MSB_SHIFT 0
#define WR_STREAMERS_RX_CFG4_MAC_REMOTE_MSB_W(value) WBGEN2_GEN_WRITE(value, 0, 16)
#define WR_STREAMERS_RX_CFG4_MAC_REMOTE_MSB_R(reg) WBGEN2_GEN_READ(reg, 0, 16)

/* definitions for register: Rx Config Reg 5 */

/* definitions for field: Fixed Latency in reg: Rx Config Reg 5 */
#define WR_STREAMERS_RX_CFG5_FIXED_LATENCY_MASK WBGEN2_GEN_MASK(0, 28)
#define WR_STREAMERS_RX_CFG5_FIXED_LATENCY_SHIFT 0
#define WR_STREAMERS_RX_CFG5_FIXED_LATENCY_W(value) WBGEN2_GEN_WRITE(value, 0, 28)
#define WR_STREAMERS_RX_CFG5_FIXED_LATENCY_R(reg) WBGEN2_GEN_READ(reg, 0, 28)

/* definitions for register: TxRx Config Override */

/* definitions for field: Tx Ethertype in reg: TxRx Config Override */
#define WR_STREAMERS_CFG_OR_TX_ETHTYPE        WBGEN2_GEN_MASK(0, 1)

/* definitions for field: Tx MAC Local in reg: TxRx Config Override */
#define WR_STREAMERS_CFG_OR_TX_MAC_LOC        WBGEN2_GEN_MASK(1, 1)

/* definitions for field: Tx MAC Target in reg: TxRx Config Override */
#define WR_STREAMERS_CFG_OR_TX_MAC_TAR        WBGEN2_GEN_MASK(2, 1)

/* definitions for field: QTAG in reg: TxRx Config Override */
#define WR_STREAMERS_CFG_OR_TX_QTAG           WBGEN2_GEN_MASK(3, 1)

/* definitions for field: Rx Ethertype in reg: TxRx Config Override */
#define WR_STREAMERS_CFG_OR_RX_ETHERTYPE      WBGEN2_GEN_MASK(16, 1)

/* definitions for field: Rx MAC Local in reg: TxRx Config Override */
#define WR_STREAMERS_CFG_OR_RX_MAC_LOC        WBGEN2_GEN_MASK(17, 1)

/* definitions for field: Rx MAC Remote in reg: TxRx Config Override */
#define WR_STREAMERS_CFG_OR_RX_MAC_REM        WBGEN2_GEN_MASK(18, 1)

/* definitions for field: Rx Accept Broadcast in reg: TxRx Config Override */
#define WR_STREAMERS_CFG_OR_RX_ACC_BROADCAST  WBGEN2_GEN_MASK(19, 1)

/* definitions for field: Rx Filter Remote in reg: TxRx Config Override */
#define WR_STREAMERS_CFG_OR_RX_FTR_REMOTE     WBGEN2_GEN_MASK(20, 1)

/* definitions for field: Rx Fixed Latency  in reg: TxRx Config Override */
#define WR_STREAMERS_CFG_OR_RX_FIX_LAT        WBGEN2_GEN_MASK(21, 1)

/* definitions for register: DBG Control register */

/* definitions for field: Debug Tx (0) or Rx (1) in reg: DBG Control register */
#define WR_STREAMERS_DBG_CTRL_MUX             WBGEN2_GEN_MASK(0, 1)

/* definitions for field: Debug Start byte in reg: DBG Control register */
#define WR_STREAMERS_DBG_CTRL_START_BYTE_MASK WBGEN2_GEN_MASK(8, 8)
#define WR_STREAMERS_DBG_CTRL_START_BYTE_SHIFT 8
#define WR_STREAMERS_DBG_CTRL_START_BYTE_W(value) WBGEN2_GEN_WRITE(value, 8, 8)
#define WR_STREAMERS_DBG_CTRL_START_BYTE_R(reg) WBGEN2_GEN_READ(reg, 8, 8)

/* definitions for register: DBG Data */

/* definitions for register: Test value */

/* definitions for field: DUMMY value to read in reg: Test value */
#define WR_STREAMERS_DUMMY_DUMMY_MASK         WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_DUMMY_DUMMY_SHIFT        0
#define WR_STREAMERS_DUMMY_DUMMY_W(value)     WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_DUMMY_DUMMY_R(reg)       WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Reset Register */

/* definitions for field: Software reset streamers in reg: Reset Register */
#define WR_STREAMERS_RSTR_RST_SW              WBGEN2_GEN_MASK(0, 1)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer RX Late Frames Count (LSB) in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT15_RX_LATE_FRAMES_CNT_LSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_STAT15_RX_LATE_FRAMES_CNT_LSB_SHIFT 0
#define WR_STREAMERS_RX_STAT15_RX_LATE_FRAMES_CNT_LSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_STAT15_RX_LATE_FRAMES_CNT_LSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer RX Late Frames Count (MSB) in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT16_RX_LATE_FRAMES_CNT_MSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_STAT16_RX_LATE_FRAMES_CNT_MSB_SHIFT 0
#define WR_STREAMERS_RX_STAT16_RX_LATE_FRAMES_CNT_MSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_STAT16_RX_LATE_FRAMES_CNT_MSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer RX Timed-out Frames Count (LSB) in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT17_RX_TIMEOUT_FRAMES_CNT_LSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_STAT17_RX_TIMEOUT_FRAMES_CNT_LSB_SHIFT 0
#define WR_STREAMERS_RX_STAT17_RX_TIMEOUT_FRAMES_CNT_LSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_STAT17_RX_TIMEOUT_FRAMES_CNT_LSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer RX Timed-out Frames Count (MSB) in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT18_RX_TIMEOUT_FRAMES_CNT_MSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_STAT18_RX_TIMEOUT_FRAMES_CNT_MSB_SHIFT 0
#define WR_STREAMERS_RX_STAT18_RX_TIMEOUT_FRAMES_CNT_MSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_STAT18_RX_TIMEOUT_FRAMES_CNT_MSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer RX OK Frames Count (LSB) in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT19_RX_MATCH_FRAMES_CNT_LSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_STAT19_RX_MATCH_FRAMES_CNT_LSB_SHIFT 0
#define WR_STREAMERS_RX_STAT19_RX_MATCH_FRAMES_CNT_LSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_STAT19_RX_MATCH_FRAMES_CNT_LSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx statistics */

/* definitions for field: WR Streamer RX OK Frames Count (MSB) in reg: Rx statistics */
#define WR_STREAMERS_RX_STAT20_RX_MATCH_FRAMES_CNT_MSB_MASK WBGEN2_GEN_MASK(0, 32)
#define WR_STREAMERS_RX_STAT20_RX_MATCH_FRAMES_CNT_MSB_SHIFT 0
#define WR_STREAMERS_RX_STAT20_RX_MATCH_FRAMES_CNT_MSB_W(value) WBGEN2_GEN_WRITE(value, 0, 32)
#define WR_STREAMERS_RX_STAT20_RX_MATCH_FRAMES_CNT_MSB_R(reg) WBGEN2_GEN_READ(reg, 0, 32)

/* definitions for register: Rx Config Reg 6 */

/* definitions for field: RX Fixed Latency Timeout (Default: 0x1000000=~134ms) in reg: Rx Config Reg 6 */
#define WR_STREAMERS_RX_CFG6_RX_FIXED_LATENCY_TIMEOUT_MASK WBGEN2_GEN_MASK(0, 28)
#define WR_STREAMERS_RX_CFG6_RX_FIXED_LATENCY_TIMEOUT_SHIFT 0
#define WR_STREAMERS_RX_CFG6_RX_FIXED_LATENCY_TIMEOUT_W(value) WBGEN2_GEN_WRITE(value, 0, 28)
#define WR_STREAMERS_RX_CFG6_RX_FIXED_LATENCY_TIMEOUT_R(reg) WBGEN2_GEN_READ(reg, 0, 28)

PACKED struct WR_STREAMERS_WB {
  /* [0x0]: REG Version register */
  uint32_t VER;
  /* [0x4]: REG Statistics status and ctrl register */
  uint32_t SSCR1;
  /* [0x8]: REG Statistics status and ctrl register */
  uint32_t SSCR2;
  /* [0xc]: REG Statistics status and ctrl register */
  uint32_t SSCR3;
  /* [0x10]: REG Rx statistics */
  uint32_t RX_STAT0;
  /* [0x14]: REG Rx statistics */
  uint32_t RX_STAT1;
  /* [0x18]: REG Tx statistics */
  uint32_t TX_STAT2;
  /* [0x1c]: REG Tx statistics */
  uint32_t TX_STAT3;
  /* [0x20]: REG Rx statistics */
  uint32_t RX_STAT4;
  /* [0x24]: REG Rx statistics */
  uint32_t RX_STAT5;
  /* [0x28]: REG Rx statistics */
  uint32_t RX_STAT6;
  /* [0x2c]: REG Rx statistics */
  uint32_t RX_STAT7;
  /* [0x30]: REG Rx statistics */
  uint32_t RX_STAT8;
  /* [0x34]: REG Rx statistics */
  uint32_t RX_STAT9;
  /* [0x38]: REG Rx statistics */
  uint32_t RX_STAT10;
  /* [0x3c]: REG Rx statistics */
  uint32_t RX_STAT11;
  /* [0x40]: REG Rx statistics */
  uint32_t RX_STAT12;
  /* [0x44]: REG Rx statistics */
  uint32_t RX_STAT13;
  /* [0x48]: REG Tx Config Reg 0 */
  uint32_t TX_CFG0;
  /* [0x4c]: REG Tx Config Reg 1 */
  uint32_t TX_CFG1;
  /* [0x50]: REG Tx Config Reg 2 */
  uint32_t TX_CFG2;
  /* [0x54]: REG Tx Config Reg 3 */
  uint32_t TX_CFG3;
  /* [0x58]: REG Tx Config Reg 4 */
  uint32_t TX_CFG4;
  /* [0x5c]: REG Tx Config Reg 4 */
  uint32_t TX_CFG5;
  /* [0x60]: REG Rx Config Reg 0 */
  uint32_t RX_CFG0;
  /* [0x64]: REG Rx Config Reg 1 */
  uint32_t RX_CFG1;
  /* [0x68]: REG Rx Config Reg 2 */
  uint32_t RX_CFG2;
  /* [0x6c]: REG Rx Config Reg 3 */
  uint32_t RX_CFG3;
  /* [0x70]: REG Rx Config Reg 4 */
  uint32_t RX_CFG4;
  /* [0x74]: REG Rx Config Reg 5 */
  uint32_t RX_CFG5;
  /* [0x78]: REG TxRx Config Override */
  uint32_t CFG;
  /* [0x7c]: REG DBG Control register */
  uint32_t DBG_CTRL;
  /* [0x80]: REG DBG Data */
  uint32_t DBG_DATA;
  /* [0x84]: REG Test value */
  uint32_t DUMMY;
  /* [0x88]: REG Reset Register */
  uint32_t RSTR;
  /* [0x8c]: REG Rx statistics */
  uint32_t RX_STAT15;
  /* [0x90]: REG Rx statistics */
  uint32_t RX_STAT16;
  /* [0x94]: REG Rx statistics */
  uint32_t RX_STAT17;
  /* [0x98]: REG Rx statistics */
  uint32_t RX_STAT18;
  /* [0x9c]: REG Rx statistics */
  uint32_t RX_STAT19;
  /* [0xa0]: REG Rx statistics */
  uint32_t RX_STAT20;
  /* [0xa4]: REG Rx Config Reg 6 */
  uint32_t RX_CFG6;
};

#endif
