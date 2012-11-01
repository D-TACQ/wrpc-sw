#ifndef __WRC_H__
#define __WRC_H__

/*
 * This header includes all generic prototypes that were missing
 * in earlier implementations. For example, the monitor is only
 * one function and doesn't deserve an header of its own.
 * Also, this brings in very common and needed headers
 */
int mprintf(char const *format, ...)
	__attribute__((format(printf,1,2)));

void wrc_mon_gui(void);
void shell_init(void);
int wrc_log_stats(uint8_t onetime);


/* This is in the library, somewhere */
extern int abs(int val);

/* The following from ptp-noposix */
extern void wr_servo_reset(void);
void update_rx_queues(void);

#endif /* __WRC_H__ */
