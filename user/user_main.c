#include <ets_sys.h>
#include <osapi.h>
#include <os_type.h>
#include <gpio.h>
#include "driver/uart.h"

LOCAL os_timer_t myTimer;

LOCAL void timerCallback(void *pArg)
{
	ets_uart_printf("Hello World!\r\n");
} 
void user_init(void)
{
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	os_timer_disarm(&myTimer);
	os_timer_setfn(&myTimer, (os_timer_func_t *)timerCallback, NULL);
	os_timer_arm(&myTimer, 1000, 1);
} 
