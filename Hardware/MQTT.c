#include "stm32f10x.h"
#include <string.h>
#include "Delay.h"
#include "Serial.h"

#define CONNECT_WIFI "AT+CWJAP=\"WIFI名字\",\"密码\"\r\n"
#define MQTT_USER_CFG "AT+MQTTUSERCFG=0,1,\"客户端名字\",\"服务端名字\",\"密码\",0,0,\"\"\r\n"
#define MQTT_CONN "AT+MQTTCONN=0,\"服务端地址\",1883,0\r\n"
#define MQTT_SUB "AT+MQTTSUB=0,\"dt_mqtt/led\",1\r\n"

void MQTT_Init(void)
{
	printf("Reset ESP32...\r\n");
	Serial_RxClear();
	Serial_SendString("AT+RST\r\n");
	if (Serial_WaitResponse("ready", 5000))
		printf("  -> ESP32 Ready\r\n");
	else
		printf("  -> ESP32 RST FAIL\r\n");
	Delay_ms(500);
	
	printf("ESP32 AT Test...\r\n");
	Serial_RxClear();
	Serial_SendString("AT\r\n");
	if (Serial_WaitResponse("OK", 2000))
		printf("  -> AT OK\r\n");
	else
		printf("  -> AT FAIL\r\n");
	
	printf("Setting WiFi Mode...\r\n");
	Serial_RxClear();
	Serial_SendString("AT+CWMODE=1\r\n");
	if (Serial_WaitResponse("OK", 2000))
		printf("  -> CWMODE OK\r\n");
	else
		printf("  -> CWMODE FAIL\r\n");
	
	printf("Connecting WiFi...\r\n");
	while (1)
	{
		Serial_RxClear();
		Serial_SendString(CONNECT_WIFI);
		if (Serial_WaitResponse("WIFI GOT IP", 15000))
		{
			printf("  -> WiFi OK\r\n");
			break;
		}
		printf("  -> WiFi FAIL, retrying...\r\n");
		Delay_ms(1000);
	}
	
	printf("Config MQTT User...\r\n");
	Serial_RxClear();
	Serial_SendString(MQTT_USER_CFG);
	if (Serial_WaitResponse("OK", 2000))
		printf("  -> USERCFG OK\r\n");
	else
		printf("  -> USERCFG FAIL\r\n");
	
	printf("Connecting MQTT Broker...\r\n");
	Serial_RxClear();
	Serial_SendString(MQTT_CONN);
	if (Serial_WaitResponse("OK", 5000))
		printf("  -> MQTT OK\r\n");
	else
		printf("  -> MQTT FAIL\r\n");
	
	printf("Subscribing Topic...\r\n");
	Serial_RxClear();
	Serial_SendString(MQTT_SUB);
	if (Serial_WaitResponse("OK", 2000))
		printf("  -> SUB OK\r\n");
	else
		printf("  -> SUB FAIL\r\n");
	
	printf("MQTT Init Done!\r\n");
}

void MQTT_Publish(char *topic, char *data)
{
	char cmd[128];
	Serial_RxClear();
	sprintf(cmd, "AT+MQTTPUB=0,\"%s\",\"%s\",1,0\r\n", topic, data);
	printf("[TX] %s", cmd);
	Serial_SendString(cmd);
	if (Serial_WaitResponse("OK", 3000))
		printf("  -> PUB OK\r\n");
	else
		printf("  -> PUB FAIL\r\n");
}

uint8_t MQTT_GetMessage(char *topic, char *data, uint16_t maxLen)
{
	char *p;
	uint16_t i = 0;
	p = strstr(Serial_RxBuf, "+MQTTSUBRECV:");
	if (p == NULL) return 0;
	p = strstr(p, topic);
	if (p == NULL) return 0;
	p = strstr(p, "\",");
	if (p == NULL) return 0;
	p += 2;
	p = strchr(p, ',');
	if (p == NULL) return 0;
	p++;
	while (*p != '\0' && *p != '\r' && *p != '\n' && i < maxLen - 1)
	{
		data[i] = *p;
		i++;
		p++;
	}
	data[i] = '\0';
	Serial_RxClear();
	return 1;
}
