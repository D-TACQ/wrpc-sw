/*
 * This work is part of the White Rabbit project
 *
 * Copyright (C) 2016 GSI (www.gsi.de)
 * Author: Alessandro Rubini <a.rubini@gsi.de>
 *
 * Released according to the GNU GPL, version 2 or any later version.
 */
#include <wrc.h>
#include <wrpc.h>
#include <ptpd_netif.h>
#include <shell.h>
#include "ipv4.h"

#ifdef CONFIG_LATENCY_SYSLOG
#define HAS_SYSLOG 1
static int lat_verbose = 0;
#else
#define HAS_SYSLOG 0
static int lat_verbose = 1;
#endif

static unsigned long prios[] = {7, 6, 0}; /* the prio for the 3 frames */

/* latency probe: we need to enqueue 3 short frames: 64*3+overhead = 256 */
static uint8_t __latency_queue[256];
static struct wrpc_socket *latency_socket, __static_latency_socket = {
	.queue.buff = __latency_queue,
	.queue.size = sizeof(__latency_queue),
};

static struct wr_sockaddr latency_addr = {
	.mac = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, /* for sending */
	.ethertype = 0, /* htons(CONFIG_LATENCY_ETHTYPE) -- not constant! */
};

static void latency_init(void)
{
	latency_addr.ethertype = htons(CONFIG_LATENCY_ETHTYPE);
	latency_socket = ptpd_netif_create_socket(&__static_latency_socket,
						  &latency_addr,
						  PTPD_SOCK_RAW_ETHERNET, 0);
}

static struct latency_frame {
	uint32_t type;   /* 1, 2, 3 */
	uint32_t sequence;
	struct wr_timestamp ts[2];
} frame;

static uint32_t prev_sequence, prev_type;

static void ts_sub(struct wr_timestamp *t2, struct wr_timestamp *t1,
		      struct wr_timestamp *res)
{
	int64_t resll; /* every field is signed: good */

	resll = t2->sec - t1->sec;
	resll *= 1000 * 1000 * 1000;
	resll += t2->nsec - t1->nsec;
	resll *= 1000;
	resll += t2->phase - t1->phase;
	/* seconds must be 0 and result must be positive */
	res->sec = 0;
	res->phase = __div64_32((uint64_t *)&resll, 1000);
	res->nsec = resll;
}

static void latency_warning(void) {
	if (!lat_verbose)
		return;
	pp_printf("lat: unexpected %i.%i after %i.%i\n",
		  frame.sequence, frame.type, prev_sequence, prev_type);
}

static int latency_poll_rx(void)
{
	static struct wr_timestamp ts[2];
	static int nframes;
	static int lost[2];
	struct wr_timestamp ts_tmp, lat[2];
	struct wr_sockaddr addr;
	int i, j;

	i = ptpd_netif_recvfrom(latency_socket, &addr,
				  &frame, sizeof(frame), &ts_tmp);
	if (i < sizeof(frame))
		return 0;

	/* check sequence and type is ok */
	switch(frame.type) {
	case 1:
		if (frame.sequence != prev_sequence + 1 || prev_type != 3)
			latency_warning();
		ts[0] = ts_tmp; /* 0 is always valid, it's a new set */
		nframes = 1;
		ts[1].sec = 0; /* but still incomplete info */

		break;

	case 2:
		if (frame.sequence != prev_sequence || prev_type != 1) {
			latency_warning();
		} else {
			ts[1] = ts_tmp;
			nframes++;
		}
		break;

	case 3:
		if (frame.sequence != prev_sequence || prev_type != 2) {
			latency_warning();
		} else {
			nframes++;
		}
		break;
	}

	/* count lost frames */
	i = prev_sequence * 3 + prev_type - 1; /* type: 1..3 -> 0..2 */
	j = frame.sequence * 3 + frame.type - 2;
	if (HAS_SYSLOG && j != i && j < i + 100) {
		lost[1] = j;
		lost[0] += j;
		syslog_latency_report(-1, NULL, lost);
	}

	prev_sequence = frame.sequence;
	prev_type = frame.type;
	if (frame.type != 3 || nframes != 3)
		return 1;


	net_verbose("ts_rx 1: %9li.%09i.%03i\n", (long)ts[0].sec,
		    ts[0].nsec, ts[0].phase);
	net_verbose("ts_tx 1: %9li.%09i.%03i\n", (long)frame.ts[0].sec,
		    frame.ts[0].nsec, frame.ts[0].phase);

	net_verbose("ts_rx 2: %9li.%09i.%03i\n", (long)ts[1].sec,
		    ts[1].nsec, ts[1].phase);
	net_verbose("ts_tx 2: %9li.%09i.%03i\n", (long)frame.ts[1].sec,
		    frame.ts[1].nsec, frame.ts[1].phase);

	ts_sub(ts + 0, frame.ts + 0, lat + 0);
	ts_sub(ts + 1, frame.ts + 1, lat + 1);

	if (lat[0].sec || lat[1].sec)
		return 1; /* not synchronized for sure */

	if (lat_verbose) {
		pp_printf("lat: %9i %6i.%03i %6i.%03i\n",
			  frame.sequence,
			  lat[0].nsec, lat[0].phase,
			  lat[1].nsec, lat[1].phase);
	} else {
		/* keep a running average of 8 samples */
		static unsigned long avg_ns[2];
		int i;

		for (i = 0; i < 2; i++) {
			if (!avg_ns[i])
				avg_ns[i] = 2 * lat[i].nsec;
			if (lat[i].nsec > avg_ns[i] * 2)
				syslog_latency_report(prios[i], lat + i, lost);
			avg_ns[i] = (7 * avg_ns[i] + lat[i].nsec) / 8;
		}
	}
	return 1;
}

static int latency_poll_tx(void)
{
	static uint32_t sequence;
	static uint32_t lasts;

	/* Send three frames -- lazily in native byte order */
	memset(&frame, 0, sizeof(frame));
	frame.sequence = sequence++;

	frame.type = 1;
	latency_socket->prio = prios[0];
	ptpd_netif_sendto(latency_socket, &latency_addr, &frame, sizeof(frame),
			  frame.ts + 0);

	frame.type = 2;
	latency_socket->prio = prios[1];
	ptpd_netif_sendto(latency_socket, &latency_addr, &frame, sizeof(frame),
			  frame.ts + 1);

	frame.type = 3;
	latency_socket->prio = prios[2];
	ptpd_netif_sendto(latency_socket, &latency_addr, &frame, sizeof(frame),
			  NULL);

	/* Every 10s remind we are sending ltest */
	if (!lasts) {
		lasts = frame.ts->sec;
	} else if (frame.ts->sec - lasts >= 10) {
		lasts = frame.ts->sec;
		pp_printf("latency: seq %9i sent @ %9i\n",
			  sequence, lasts);
	}
	return 1;

}

static uint32_t lastt;
static uint32_t latency_period_ms;

static int latency_poll(void)
{
	if (!latency_period_ms)
		return latency_poll_rx();

	/* Periodically send the frames */
	if (task_not_yet(&lastt, latency_period_ms))
		return 0;
	return latency_poll_tx();
}

DEFINE_WRC_TASK(uptime) = {
	.name = "latency-probe",
	.init = latency_init,
	.job = latency_poll,
};


static int cmd_ltest(const char *args[])
{
	int v = 0, v1 = 0;

	if (args[1]) {
		fromdec(args[1], &v1); /* ms */
	}
	if (args[0]) {
		if (HAS_SYSLOG && !strcmp(args[0], "verbose"))
			lat_verbose = 1;
		else if (HAS_SYSLOG && !strcmp(args[0], "quiet"))
			lat_verbose = 0;
		else {
			fromdec(args[0], &v);
			latency_period_ms = v * 1000 + v1;
			lastt = 0; /* reset, so it fires immediately */
		}
	}
	pp_printf("%i.%03i (%s)\n", latency_period_ms / 1000,
		  latency_period_ms % 1000, lat_verbose ? "verbose" : "quiet");
	return 0;
}

DEFINE_WRC_COMMAND(ltest) = {
	.name = "ltest",
	.exec = cmd_ltest,
};

