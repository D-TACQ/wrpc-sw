#ifndef __BOARD_H
#define __BOARD_H

#define BASE_MINIC    0x20000
#define BASE_EP   	  0x20100 
#define BASE_SOFTPLL  0x20200 
#define BASE_PPSGEN   0x20300
#define BASE_SYSCON   0x20400
#define BASE_UART 	  0x20500
#define BASE_ONEWIRE  0x20600

#define CPU_CLOCK 62500000ULL

#define UART_BAUDRATE 115200ULL /* not a real UART */

static inline int delay(int x)
{
  while(x--) asm volatile("nop");
}
  
#endif
