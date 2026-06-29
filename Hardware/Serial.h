#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);
void Serial_SendATCommand(char *cmd);

uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);

void Serial_RxClear(void);
uint8_t Serial_WaitResponse(char *keyword, uint32_t timeout_ms);

extern char Serial_RxBuf[];

void Serial3_Init(void);
void Serial3_SendByte(uint8_t Byte);
void Serial3_SendString(char *String);
void Serial3_Printf(char *format, ...);

#define DEBUG(fmt, ...) Serial3_Printf(fmt, ##__VA_ARGS__)

#endif