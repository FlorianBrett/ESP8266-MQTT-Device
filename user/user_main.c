#include <ets_sys.h>
#include <osapi.h>
#include <os_type.h>
#include <gpio.h>
#include "driver/uart.h"

#include "mqtt.h"
#include "wifi.h"
#include "config.h"
#include "debug.h"
#include "user_interface.h"
#include "mem.h"

LOCAL os_timer_t myTimer;
MQTT_Client mqttClient;
void wifiConnectedCallBack(uint8_t status)
{
	ets_uart_printf("WiFi connected\r\n");
	if(status == STATION_GOT_IP)
		MQTT_Connect(&mqttClient);
	else
		MQTT_Disconnect(&mqttClient);

}
void mqttConnectedCallBack(uint32_t *arg)
{
	ets_uart_printf("mqttConnected!\r\n");
	MQTT_Client* client = (MQTT_Client*)arg;
	MQTT_Subscribe(client, "/esp1/led1", 0);
	MQTT_Subscribe(client, "/esp1/switch1", 1);

	MQTT_Publish(client, "/esp1/led1", "hello", 6, 0, 0);
	MQTT_Publish(client, "/esp1/switch1", "hello", 6, 1, 0);

}


void mqttDisconnectedCallBack(uint32_t *arg)
{
	ets_uart_printf("mqttDisconnected!\r\n");
}
void mqttPublishCallBack(uint32_t *arg)
{
	ets_uart_printf("mqttPublished!\r\n");
}
void mqttSubCallBack(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
	char *topicBuf = (char*)os_zalloc(topic_len+1),
			*dataBuf = (char*)os_zalloc(data_len+1);

	MQTT_Client* client = (MQTT_Client*)args;

	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;

	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;
	if (strcmp(topicBuf,"/esp1/led1") == 0)
	{
		if (dataBuf[0] == '1')
			GPIO_OUTPUT_SET(4,1);
		else if (dataBuf[0] == '0')
			GPIO_OUTPUT_SET(4,0);

	}
	INFO("Receive topic: %s, data: %s \r\n", topicBuf, dataBuf);
	ets_uart_printf("Receive topic: %s, data: %s \r\n", topicBuf, dataBuf);

	os_free(topicBuf);
	os_free(dataBuf);
}
void timerCallBack(void *pArg)
{

	char gpioVal[1] = {GPIO_INPUT_GET(5) + '0'};
	//char gpioVal[1] = {GPIO_INPUT_GET(5) + '0'};
	MQTT_Publish(&mqttClient, "/esp1/switch1", gpioVal, 1, 0, 0);

	ets_uart_printf("Switch value updated to:%d\r\n", GPIO_INPUT_GET(5));
} 

void user_init(void)
{
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	//os_delay_us(1000000);

	gpio_init();

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
	GPIO_DIS_OUTPUT(5);
	//gpio_output_set(0, BIT4, BIT4, 0);

	GPIO_OUTPUT_SET(4,1);
	os_timer_disarm(&myTimer);
	os_timer_setfn(&myTimer, (os_timer_func_t *)timerCallBack, NULL);
	os_timer_arm(&myTimer, 100, 1);

	MQTT_InitConnection(&mqttClient, "m10.cloudmqtt.com", 10308, 0);
	MQTT_InitClient(&mqttClient, "esp1", "avvppads", "ORn9fPTI1hgq", 120, 1);
	//Last will and testament
	MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
	//On connection call back
	MQTT_OnConnected(&mqttClient, mqttConnectedCallBack);
	//On disconnection call back
	MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCallBack);
	//On published call back
	MQTT_OnPublished(&mqttClient, mqttPublishCallBack);
	//On subscription data received call back
	MQTT_OnData(&mqttClient, mqttSubCallBack);

	WIFI_Connect("apnet1", "brettnet", wifiConnectedCallBack);


} 
