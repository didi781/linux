#ifndef __MQTT_H
#define __MQTT_H

void MQTT_Init(void);
void MQTT_Publish(char *topic, char *data);
uint8_t MQTT_GetMessage(char *topic, char *data, uint16_t maxLen);

#endif