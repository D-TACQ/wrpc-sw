/*
 * This work is part of the White Rabbit project
 *
 * Copyright (C) 2011,2012 CERN (www.cern.ch)
 * Author: Aurelio Colosimo <aurelio@aureliocolosimo.it>
 *
 * Released according to the GNU GPL, version 2 or any later version.
 */

#include <inttypes.h>
#include <wrc.h>
#include <w1.h>
#include <ppsi/ppsi.h>
#include <wrpc.h>
#include <wr-api.h>
#include <minic.h>
#include <softpll_ng.h>
#include <syscon.h>
#include <pps_gen.h>
#include <onewire.h>
#include <temperature.h>
#include "wrc_ptp.h"
#include "hal_exports.h"
#include "lib/ipv4.h"
#include "shell.h"

extern struct pp_servo servo;
extern struct pp_instance ppi_static;
struct pp_instance *ppi = &ppi_static;
const char *ptp_unknown_str= "unknown";

static void wrc_mon_std_servo(void);

#define PRINT64_FACTOR	1000000000LL
static char* print64(uint64_t x)
{
	uint32_t h_half, l_half;
	static char buf[2*10+1];	//2x 32-bit value + \0
	if (x < PRINT64_FACTOR)
		sprintf(buf, "%u", (uint32_t)x);
	else{
		l_half = __div64_32(&x, PRINT64_FACTOR);
		h_half = (uint32_t) x;
		sprintf(buf, "%u%09u", h_half, l_half);
	}
	return buf;

}

static const char* wrc_ptp_state(void)
{
	struct pp_state_table_item *ip = NULL;
	for (ip = pp_state_table; ip->state != PPS_END_OF_TABLE; ip++) {
		if (ip->state == ppi->state)
			break;
	}
	
	if(!ip)
		return ptp_unknown_str;
	return ip->name;
}

static int wrc_mon_status(void)
{
	struct wr_servo_state *s =
			&((struct wr_data *)ppi->ext_data)->servo_state;

	cprintf(C_BLUE, "\n\nPTP status: ");
	cprintf(C_WHITE, "%s", wrc_ptp_state());

	if ((!s->flags & WR_FLAG_VALID) || (ppi->state != PPS_SLAVE)) {
		cprintf(C_RED,
			"\n\nSync info not valid\n");
		return 0;
	}

	/* show_servo */
	cprintf(C_BLUE, "\n\nSynchronization status:\n");

	return 1;
}

int wrc_mon_gui(void)
{
	static uint32_t last_jiffies;
	static uint32_t last_servo_count;
	struct hal_port_state state;
	int tx, rx;
	int aux_stat;
	uint64_t sec;
	uint32_t nsec;
	struct wr_servo_state *s =
			&((struct wr_data *)ppi->ext_data)->servo_state;
	int64_t crtt;
	int64_t total_asymmetry;
	char buf[20];

	if (!last_jiffies)
		last_jiffies = timer_get_tics() - 1 -  wrc_ui_refperiod;
	if (time_before(timer_get_tics(), last_jiffies + wrc_ui_refperiod)
		&& last_servo_count == s->update_count)
		return 0;
	last_jiffies = timer_get_tics();
	last_servo_count = s->update_count;

	term_clear();

	cprintf(C_BLUE, "WR PTP Core Sync Monitor v 1.0");
	cprintf(C_GREY, "\nEsc = exit");

	shw_pps_gen_get_time(&sec, &nsec);

	cprintf(C_BLUE, "\n\nTAI Time:                  ");
	cprintf(C_WHITE, "%s", format_time(sec, TIME_FORMAT_LEGACY));

	/*show_ports */
	wrpc_get_port_state(&state, NULL);
	cprintf(C_BLUE, "\n\nLink status:");

	cprintf(C_WHITE, "\n%s: ", "wru1");
	if (state.state)
		cprintf(C_GREEN, "Link up   ");
	else
		cprintf(C_RED,   "Link down ");

	minic_get_stats(&tx, &rx);
	cprintf(C_GREY, "(RX: %d, TX: %d)", rx, tx);

	if (!state.state) {
		return 1;
	}

	if (HAS_IP) {
		uint8_t ip[4];

		cprintf(C_WHITE, " IPv4: ");
		getIP(ip);
		format_ip(buf, ip);
		switch (ip_status) {
		case IP_TRAINING:
			cprintf(C_RED, "BOOTP running");
			break;
		case IP_OK_BOOTP:
			cprintf(C_GREEN, "%s (from bootp)", buf);
			break;
		case IP_OK_STATIC:
			cprintf(C_GREEN, "%s (static assignment)", buf);
			break;
		}
	}

	cprintf(C_GREY, "\nMode: ");

	if (!WR_DSPOR(ppi)->wrModeOn) {
		cprintf(C_RED, "WR Off");
		wrc_mon_std_servo();
		return 1;
	}

	switch (ptp_mode) {
	case WRC_MODE_GM:
	case WRC_MODE_MASTER:
		cprintf(C_WHITE, "WR Master  ");
		break;
	case WRC_MODE_SLAVE:
		cprintf(C_WHITE, "WR Slave   ");
		break;
	default:
		cprintf(C_RED,   "WR Unknown ");
	}

	if (state.locked)
		cprintf(C_GREEN, "Locked ");
	else
		cprintf(C_RED,   "NoLock ");
	if (state.calib.rx_calibrated && state.calib.tx_calibrated)
		cprintf(C_GREEN, "Calibrated");
	else
		cprintf(C_RED, "Uncalibrated");


	if (wrc_mon_status() == 0)
		return 1;

	cprintf(C_GREY, "Servo state:               ");
	cprintf(C_WHITE, "%s\n", s->servo_state_name);
	cprintf(C_GREY, "Phase tracking:            ");
	if (s->tracking_enabled)
		cprintf(C_GREEN, "ON\n");
	else
		cprintf(C_RED, "OFF\n");
	/* sync source not implemented */
	/*cprintf(C_GREY, "Synchronization source:    ");
	cprintf(C_WHITE, "%s\n", cur_servo_state.sync_source);*/
	cprintf(C_GREY, "Aux clock status:          ");

	aux_stat = spll_get_aux_status(0);

	if (aux_stat & SPLL_AUX_ENABLED)
		cprintf(C_GREEN, "enabled");

	if (aux_stat & SPLL_AUX_LOCKED)
		cprintf(C_GREEN, ", locked");
	pp_printf("\n");

	cprintf(C_BLUE, "\nTiming parameters:\n");

	cprintf(C_GREY, "Round-trip time (mu):    ");
	cprintf(C_WHITE, "%s ps\n", print64(s->picos_mu));
	cprintf(C_GREY, "Master-slave delay:      ");
	cprintf(C_WHITE, "%s ps\n", print64(s->delta_ms));

	cprintf(C_GREY, "Master PHY delays:       ");
	cprintf(C_WHITE, "TX: %d ps, RX: %d ps\n",
		(int32_t) s->delta_tx_m,
		(int32_t) s->delta_rx_m);

	cprintf(C_GREY, "Slave PHY delays:        ");
	cprintf(C_WHITE, "TX: %d ps, RX: %d ps\n",
		(int32_t) s->delta_tx_s,
		(int32_t) s->delta_rx_s);
	total_asymmetry = s->picos_mu - 2LL * s->delta_ms;
	cprintf(C_GREY, "Total link asymmetry:    ");
	cprintf(C_WHITE, "%9d ps\n", (int32_t) (total_asymmetry));

	crtt = s->picos_mu - s->delta_tx_m - s->delta_rx_m
		- s->delta_tx_s - s->delta_rx_s;
	cprintf(C_GREY, "Cable rtt delay:         ");
	cprintf(C_WHITE, "%s ps\n", print64(crtt));

	cprintf(C_GREY, "Clock offset:            ");
	cprintf(C_WHITE, "%9d ps\n", (int32_t) (s->offset));

	cprintf(C_GREY, "Phase setpoint:          ");
	cprintf(C_WHITE, "%9d ps\n", (s->cur_setpoint));

	cprintf(C_GREY, "Skew:                    ");
	cprintf(C_WHITE, "%9d ps\n", (int32_t) (s->skew));

	cprintf(C_GREY, "Update counter:          ");
	cprintf(C_WHITE, "%9d\n", (int32_t) (s->update_count));

	return 1;
}

static inline void cprintf_time(int color, struct pp_time *time)
{
	int s, ns;

	s = (int)time->secs;
	ns = (int)(time->scaled_nsecs >> 16);
	if (s > 0 || (s == 0 && ns >= 0)) {
		cprintf(color, "%2i.%09i s", s, ns);
	} else { /* negative */
		if (time->secs == 0)
			cprintf(color, "-%i.%09i s", s, -ns);
		else
			cprintf(color, "%i.%09i s", s, -ns);
	}
}

static void wrc_mon_std_servo(void)
{
	if (wrc_mon_status() == 0)
		return;

	cprintf(C_GREY, "\nClock offset:                 ");

	if (DSCUR(ppi)->offsetFromMaster.secs)
		cprintf_time(C_WHITE, &DSCUR(ppi)->offsetFromMaster);
	else {
		cprintf(C_WHITE, "%9i ns",
			(int)(DSCUR(ppi)->offsetFromMaster.scaled_nsecs >> 16));

		cprintf(C_GREY, "\nOne-way delay averaged:       ");
		cprintf(C_WHITE, "%9i ns",
			(int)(DSCUR(ppi)->meanPathDelay.scaled_nsecs >> 16));

		cprintf(C_GREY, "\nObserved drift:               ");
		cprintf(C_WHITE, "%9i ns",SRV(ppi)->obs_drift);
	}
}


/* internal "last", exported to shell command */
uint32_t wrc_stats_last;

static int wrc_log_stats(void)
{
	struct hal_port_state state;
	int tx, rx;
	int aux_stat;
	uint64_t sec;
	uint32_t nsec;
	struct wr_servo_state *s =
			&((struct wr_data *)ppi->ext_data)->servo_state;
	static uint32_t last_jiffies;

	if (!wrc_stat_running)
		return 0;

	if (!last_jiffies)
		last_jiffies = timer_get_tics() - 1 -  wrc_ui_refperiod;
	/* stats update condition for Slave mode */
	if (wrc_stats_last == s->update_count && ptp_mode==WRC_MODE_SLAVE)
		return 0;
	/* stats update condition for Master mode */
	if (time_before(timer_get_tics(), last_jiffies + wrc_ui_refperiod) &&
			ptp_mode != WRC_MODE_SLAVE)
		return 0;
	last_jiffies = timer_get_tics();
	wrc_stats_last = s->update_count;

	shw_pps_gen_get_time(&sec, &nsec);
	wrpc_get_port_state(&state, NULL);
	minic_get_stats(&tx, &rx);
	pp_printf("lnk:%d rx:%d tx:%d ", state.state, rx, tx);
	pp_printf("lock:%d ", state.locked ? 1 : 0);
	pp_printf("ptp:%s ", wrc_ptp_state());
	if(ptp_mode == WRC_MODE_SLAVE) {
		pp_printf("sv:%d ", (s->flags & WR_FLAG_VALID) ? 1 : 0);
		pp_printf("ss:'%s' ", s->servo_state_name);
	}
	aux_stat = spll_get_aux_status(0);
	pp_printf("aux:%x ", aux_stat);
	/* fixme: clock is not always 125 MHz */
	pp_printf("sec:%d nsec:%d ", (uint32_t) sec, nsec);
	if(ptp_mode == WRC_MODE_SLAVE) {
		pp_printf("mu:%s ", print64(s->picos_mu));
		pp_printf("dms:%s ", print64(s->delta_ms));
		pp_printf("dtxm:%d drxm:%d ", (int32_t) s->delta_tx_m,
			(int32_t) s->delta_rx_m);
		pp_printf("dtxs:%d drxs:%d ", (int32_t) s->delta_tx_s,
			(int32_t) s->delta_rx_s);
		int64_t total_asymmetry = s->picos_mu -
				  2LL * s->delta_ms;
		pp_printf("asym:%d ", (int32_t) (total_asymmetry));
		pp_printf("crtt:%s ", print64(s->picos_mu -
					s->delta_tx_m -
					s->delta_rx_m -
					s->delta_tx_s -
					s->delta_rx_s));
		pp_printf("cko:%d ", (int32_t) (s->offset));
		pp_printf("setp:%d ", (int32_t) (s->cur_setpoint));
		pp_printf("ucnt:%d ", (int32_t) s->update_count);
	}
	pp_printf("hd:%d md:%d ad:%d ", spll_get_dac(-1), spll_get_dac(0),
		spll_get_dac(1));

	if (1) {
		int32_t temp;

		temp = wrc_temp_get("pcb");
		pp_printf("temp: %d.%04d C", temp >> 16,
			  (int)((temp & 0xffff) * 10 * 1000 >> 16));
	}

	pp_printf("\n");

	return 1;
}
DEFINE_WRC_TASK(stats) = {
	.name = "stats",
	.job = wrc_log_stats,
};
