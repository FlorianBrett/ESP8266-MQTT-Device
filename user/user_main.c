#include <ets_sys.h>
#include <osapi.h>
#include <os_type.h>
#include <gpio.h>
#include "driver/uart.h"

LOCAL os_timer_t myTimer;

LOCAL void timerCallback(void *pArg)
{
	ets_uart_printf("Hello World!\r\n");
	if (GPIO_INPUT_GET(4))
		GPIO_OUTPUT_SET(4,0);
	else
		GPIO_OUTPUT_SET(4,1);
	//GPIO_REG_READ(GPIO_OUT_ADDRESS) & BIT4
} 
void user_init(void)
{
	uart_init(BIT_RATE_115200, BIT_RATE_115200);

	gpio_init();
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
	//gpio_output_set(0, BIT4, BIT4, 0);
	GPIO_OUTPUT_SET(4,1);
	os_timer_disarm(&myTimer);
	os_timer_setfn(&myTimer, (os_timer_func_t *)timerCallback, NULL);
	os_timer_arm(&myTimer, 1000, 1);
} 
