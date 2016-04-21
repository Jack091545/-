#ifndef __UART_H__
#define __UART_H__

void UART_Init(void);
void SendByte_UART(unsigned char dat);
void SendStr(unsigned char *s);

#endif