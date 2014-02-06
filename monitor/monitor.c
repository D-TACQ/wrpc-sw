/*
 * This work is part of the White Rabbit project
 *
 * Copyright (C) 2011,2012 CERN (www.cern.ch)
 * Author: Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
 *
 * Released according to the GNU GPL, version 2 or any later version.
 */
#include <stdio.h>
#include <time.h>
#include <wrc.h>
#include <w1.h>

#include "board.h"
#include "ptpd_exports.h"
#include "hal_exports.h"
#include "softpll_ng.h"
#include "minic.h"
#include "pps_gen.h"
#include "util.h"
#include "syscon.h"
#include "onewire.h"
#include "lib/ipv4.h"


int UI_REFRESH_PERIOD = TICS_PER_SECOND;	/* 1 sec */

#define PRINT64_FACTOR	1000000000
char* print64(unsigned long long x)
{
	uint32_t h_half, l_half;
	static char buf[2*10+1];	//2x 32-bit value + \0
	if (x < PRINT64_FACTOR)
		sprintf(buf, "%u", (uint32_t)x);
	else{
		h_half = (uint32_t)(x/PRINT64_FACTOR);
		l_half = (uint32_t)(x-h_half*PRINT64_FACTOR);
		sprintf(buf, "%u%u", h_half, l_half);
	}
	return buf;
}


extern ptpdexp_sync_state_t cur_servo_state;
extern int wrc_man_phase;

void wrc_mon_gui(void)
{
	static uint32_t last = 0;
	hexp_port_state_t ps;
	int tx, rx;
	uint64_t sec;
	uint32_t nsec;
#ifdef CONFIG_ETHERBONE
	uint8_t ip[4];
#endif

	if (timer_get_tics() - last < UI_REFRESH_PERIOD)
		return;

	last = timer_get_tics();

	term_clear();

	pcprintf(1, 1, C_BLUE, "WR PTP Core Sync Monitor v 1.0");
	pcprintf(2, 1, C_GREY, "Esc = exit");

	shw_pps_gen_get_time(&sec, &nsec);

	cprintf(C_BLUE, "\n\nTAI Time:                  ");
	cprintf(C_WHITE, "%s", format_time(sec));

	/*show_ports */
	halexp_get_port_state(&ps, NULL);
	pcprintf(4, 1, C_BLUE, "\n\nLink status:");

	pcprintf(6, 1, C_WHITE, "%s: ", "wru1");
	if (ps.up)
		cprintf(C_GREEN, "Link up   ");
	else
		cprintf(C_RED, "Link down ");

	if (ps.up) {
		minic_get_stats(&tx, &rx);
		cprintf(C_GREY, "(RX: %d, TX: %d), mode: ", rx, tx);

		switch (ps.mode) {
		case HEXP_PORT_MODE_WR_MASTER:
			cprintf(C_WHITE, "WR Master  ");
			break;
		case HEXP_PORT_MODE_WR_SLAVE:
			cprintf(C_WHITE, "WR Slave   ");
			break;
		}

		if (ps.is_locked)
			cprintf(C_GREEN, "Locked  ");
		else
			cprintf(C_RED, "NoLock  ");
		if (ps.rx_calibrated && ps.tx_calibrated)
			cprintf(C_GREEN, "Calibrated  ");
		else
			cprintf(C_RED, "Uncalibrated  ");

#ifdef CONFIG_ETHERBONE
		cprintf(C_WHITE, "\nIPv4: ");
		getIP(ip);
		if (needIP)
			cprintf(C_RED, "BOOTP running");
		else
			cprintf(C_GREEN, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
#endif		
		
		/* show_servo */
		cprintf(C_BLUE, "\n\nSynchronization status:\n\n");

		if (!cur_servo_state.valid) {
			cprintf(C_RED,
				"Master mode or sync info not valid\n\n");
			return;
		}

		cprintf(C_GREY, "Servo state:               ");
		cprintf(C_WHITE, "%s\n", cur_servo_state.slave_servo_state);
		cprintf(C_GREY, "Phase tracking:            ");
		if (cur_servo_state.tracking_enabled)
			cprintf(C_GREEN, "ON\n");
		else
			cprintf(C_RED, "OFF\n");
		cprintf(C_GREY, "Synchronization source:    ");
		cprintf(C_WHITE, "%s\n", cur_servo_state.sync_source);

		cprintf(C_GREY, "Aux clock status:          ");

		int n_ref, n_out, i;
		spll_get_num_channels(&n_ref, &n_out);

		for(i = 0; i<n_out - 1; i++)		
		{
			const char *aux_stat = spll_get_aux_status_string(i);

			cprintf(C_GREY,"%d:", i);
			cprintf(C_GREEN,"%s ", aux_stat);
		}
		mprintf("\n");

		cprintf(C_BLUE, "\nTiming parameters:\n\n");

		cprintf(C_GREY, "Round-trip time (mu):    ");
		cprintf(C_WHITE, "%s ps\n", print64(cur_servo_state.mu));
		cprintf(C_GREY, "Master-slave delay:      ");
		cprintf(C_WHITE, "%s ps\n", print64(cur_servo_state.delay_ms));
		cprintf(C_GREY, "Master PHY delays:       ");
		cprintf(C_WHITE, "TX: %d ps, RX: %d ps\n",
			(int32_t) cur_servo_state.delta_tx_m,
			(int32_t) cur_servo_state.delta_rx_m);
		cprintf(C_GREY, "Slave PHY delays:        ");
		cprintf(C_WHITE, "TX: %d ps, RX: %d ps\n",
			(int32_t) cur_servo_state.delta_tx_s,
			(int32_t) cur_servo_state.delta_rx_s);
		cprintf(C_GREY, "Total link asymmetry:    ");
		cprintf(C_WHITE, "%9d ps\n",
			(int32_t) (cur_servo_state.total_asymmetry));
		cprintf(C_GREY, "Cable rtt delay:         ");
		cprintf(C_WHITE, "%9s ps\n", print64(cur_servo_state.mu -
					cur_servo_state.delta_tx_m -
					cur_servo_state.delta_rx_m -
					cur_servo_state.delta_tx_s -
					cur_servo_state.delta_rx_s));

		cprintf(C_GREY, "Clock offset:            ");
		cprintf(C_WHITE, "%9d ps\n",
			(int32_t) (cur_servo_state.cur_offset));
		cprintf(C_GREY, "Phase setpoint:          ");
		cprintf(C_WHITE, "%9d ps\n",
			(int32_t) (cur_servo_state.cur_setpoint));
		cprintf(C_GREY, "Skew:                    ");
		cprintf(C_WHITE, "%9d ps\n",
			(int32_t) (cur_servo_state.cur_skew));
		cprintf(C_GREY, "Manual phase adjustment: ");
		cprintf(C_WHITE, "%9d ps\n", (int32_t) (wrc_man_phase));

		cprintf(C_GREY, "Update counter:          ");
		cprintf(C_WHITE, "%9d \n",
			(int32_t) (cur_servo_state.update_count));

	}

	cprintf(C_GREY, "--");

	return;
}

int wrc_log_stats(uint8_t onetime)
{
	static uint32_t last = 0;
	hexp_port_state_t ps;
	int tx, rx;
	int aux_stat;
	uint64_t sec;
	uint32_t nsec;

	if (!onetime && timer_get_tics() - last < UI_REFRESH_PERIOD)
		return 0;

	last = timer_get_tics();

	shw_pps_gen_get_time(&sec, &nsec);
	halexp_get_port_state(&ps, NULL);
	minic_get_stats(&tx, &rx);
	mprintf("lnk:%d rx:%d tx:%d ", ps.up, rx, tx);
	mprintf("lock:%d ", ps.is_locked ? 1 : 0);
	mprintf("sv:%d ", cur_servo_state.valid ? 1 : 0);
	mprintf("ss:'%s' ", cur_servo_state.slave_servo_state);
	aux_stat = spll_get_aux_status(0);
	mprintf("aux:%x ", aux_stat);
	mprintf("sec:%d nsec:%d ", (uint32_t) sec, nsec);	/* fixme: clock is not always 125 MHz */
	mprintf("mu:%s ", print64(cur_servo_state.mu));
	mprintf("dms:%s ", print64(cur_servo_state.delay_ms));
	mprintf("dtxm:%d drxm:%d ", (int32_t) cur_servo_state.delta_tx_m,
		(int32_t) cur_servo_state.delta_rx_m);
	mprintf("dtxs:%d drxs:%d ", (int32_t) cur_servo_state.delta_tx_s,
		(int32_t) cur_servo_state.delta_rx_s);
	mprintf("asym:%d ", (int32_t) (cur_servo_state.total_asymmetry));
	mprintf("crtt:%s ", print64(cur_servo_state.mu -
				cur_servo_state.delta_tx_m -
				cur_servo_state.delta_rx_m -
				cur_servo_state.delta_tx_s -
				cur_servo_state.delta_rx_s));
	mprintf("cko:%d ", (int32_t) (cur_servo_state.cur_offset));
	mprintf("setp:%d ", (int32_t) (cur_servo_state.cur_setpoint));
	mprintf("hd:%d md:%d ad:%d ", spll_get_dac(-1), spll_get_dac(0),
		spll_get_dac(1));
	mprintf("ucnt:%d ", (int32_t) cur_servo_state.update_count);

	if (1) {
		int32_t temp;

		//first read the value from previous measurement,
		//first one will be random, I know
		temp = w1_read_temp_bus(&wrpc_w1_bus, W1_FLAG_COLLECT);
		//then initiate new conversion for next loop cycle
		w1_read_temp_bus(&wrpc_w1_bus, W1_FLAG_NOWAIT);
		mprintf("temp: %d.%04d C", temp >> 16,
			  (int)((temp & 0xffff) * 10 * 1000 >> 16));
	}

	mprintf("\n");
	return 0;
}
