#include <ets_sys.h>
#include <osapi.h>
#include <os_type.h>
#include <gpio.h>
#include "driver/uart.h"

LOCAL os_timer_t myTimer;

LOCAL void timerCallback(void *pArg)
{
	ets_uart_printf("Hello World!%d\r\n", GPIO_INPUT_GET(5));
	if (GPIO_INPUT_GET(4))
		GPIO_OUTPUT_SET(4,0);
	else
		GPIO_OUTPUT_SET(4,1);
	//GPIO_REG_READ(GPIO_OUT_ADDRESS) & BIT4
	os_printf("testing\r\n");
} 
void user_init(void)
{
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	//os_delay_us(1000000);
	os_printf("test");
	gpio_init();
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
	GPIO_DIS_OUTPUT(5);
	//gpio_output_set(0, BIT4, BIT4, 0);
	GPIO_OUTPUT_SET(4,1);
	os_timer_disarm(&myTimer);
	os_timer_setfn(&myTimer, (os_timer_func_t *)timerCallback, NULL);
	os_timer_arm(&myTimer, 100, 1);
} 
