#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"
#include "Serial.h"
#include "MQTT.h"
#include <string.h>

uint8_t KeyNum;
int8_t Speed;
char MqttData[32];

int main(void)
{
	OLED_Init();
	Motor_Init();
	Key_Init();
	Serial_Init();
	Serial3_Init();
	Motor_SetSpeed(20);
	OLED_ShowString(1, 1, "Speed:");
	printf("System Init OK!\r\n");
	
	MQTT_Init();
	OLED_ShowString(2, 1, "MQTT OK");
	
	while (1)
	{
		MQTT_Publish("dt_mqtt/led", "0");
		Delay_ms(10000);
		MQTT_Publish("dt_mqtt/led", "1");
		Delay_ms(10000);
	}
}