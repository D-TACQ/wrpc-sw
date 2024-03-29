/*
 * Link script for Lattice Mico32. Very loosely based on
 * code contributed by Jon Beniston <jon@beniston.com>
 *
 * Jon's license (BSD-style):
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

OUTPUT_FORMAT("elf32-lm32")
ENTRY(_start)

MEMORY
{
    ram :
	ORIGIN = 0x00000000,
	LENGTH = CONFIG_RAMSIZE - CONFIG_STACKSIZE - 2048
	hprams :
 	ORIGIN = CONFIG_RAMSIZE - CONFIG_STACKSIZE - 2048,
	LENGTH = 2048
    stack :
	ORIGIN = CONFIG_RAMSIZE - CONFIG_STACKSIZE,
	LENGTH = CONFIG_STACKSIZE
}

SECTIONS
{
	.boot : { *(.boot) } > ram

	.text : { *(.text .text.*) } > ram =0

	.rodata : { *(.rodata .rodata.*) } > ram

	.data : {
		*(.data .data.*)
		*(.stats)
	} > ram

	.bss : {
		_fbss = .;
		*(.bss .bss.*)
		*(COMMON)
		_ebss = .;
	} > ram

	/* _hostprams : parameters from host */
	PROVIDE(_hostprams = ORIGIN(hprams));
	PROVIDE(sfp_deltaTx = ORIGIN(hprams)+0x20);
	PROVIDE(sfp_deltaRx = ORIGIN(hprams)+0x24);

	/* End of RAM for checking stack overflows */
	PROVIDE(_endram = ORIGIN(stack));

	/* First location in stack is highest address in STACK */
	PROVIDE(_fstack = ORIGIN(stack) + LENGTH(stack) - 4);

	/* This may be missing, according to .config */
	PROVIDE(fifo_log = 0);

	/* A vlan rule-set may be missing (if no CONFIG_VLAN) */
	PROVIDE(_binary_rules_vlan_bin_start = 0);
	PROVIDE(_binary_rules_vlan_bin_end = 0);
}
