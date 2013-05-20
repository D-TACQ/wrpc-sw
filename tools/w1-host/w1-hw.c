/*
 * This work is part of the White Rabbit project
 *
 * Copyright (C) 2013 CERN (www.cern.ch)
 * Author: Alessandro Rubini <rubini@gnudd.com>
 *
 * Released according to the GNU GPL, version 2 or any later version.
 */
#include <string.h>
#include "w1.h"
#include "../../include/hw/sockit_owm_regs.h"

extern void *BASE_ONEWIRE;

static inline uint32_t __wait_cycle(void *base)
{
	uint32_t reg;

	while ((reg = IORD_SOCKIT_OWM_CTL(base)) & SOCKIT_OWM_CTL_CYC_MSK)
		;
	return reg;
}

static int w1_reset(struct w1_bus *bus)
{
	int portnum = bus->detail;
	uint32_t reg;

	IOWR_SOCKIT_OWM_CTL(BASE_ONEWIRE, (portnum << SOCKIT_OWM_CTL_SEL_OFST)
			    | (SOCKIT_OWM_CTL_CYC_MSK)
			    | (SOCKIT_OWM_CTL_RST_MSK));
	reg = __wait_cycle(BASE_ONEWIRE);
	/* return presence-detect pulse (1 if true) */
	return (reg & SOCKIT_OWM_CTL_DAT_MSK) ? 0 : 1;
}

static int w1_read_bit(struct w1_bus *bus)
{
	int portnum = bus->detail;
	uint32_t reg;

	IOWR_SOCKIT_OWM_CTL(BASE_ONEWIRE, (portnum << SOCKIT_OWM_CTL_SEL_OFST)
			    | (SOCKIT_OWM_CTL_CYC_MSK)
			    | (SOCKIT_OWM_CTL_DAT_MSK));
	reg = __wait_cycle(BASE_ONEWIRE);
	return (reg & SOCKIT_OWM_CTL_DAT_MSK) ? 1 : 0;
}

static void w1_write_bit(struct w1_bus *bus, int bit)
{
	int portnum = bus->detail;

	IOWR_SOCKIT_OWM_CTL(BASE_ONEWIRE, (portnum << SOCKIT_OWM_CTL_SEL_OFST)
			    | (SOCKIT_OWM_CTL_CYC_MSK)
			    | (bit ? SOCKIT_OWM_CTL_DAT_MSK : 0));
	__wait_cycle(BASE_ONEWIRE);
}

struct w1_ops wrpc_w1_ops = {
	.reset = w1_reset,
	.read_bit = w1_read_bit,
	.write_bit = w1_write_bit,
};

struct w1_bus wrpc_w1_bus;
