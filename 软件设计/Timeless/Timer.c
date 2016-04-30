#include "stc12c5a60s2.h"
#include "DS1302.h"
#include "Data_store.h"
#include "UART.h"
#include <stdio.h>

unsigned char Timer_Counter0 = 0; 
unsigned char Timer_Counter1 = 0; 


sbit LED = P1^4;

void Timer_Init(void)
{														
	TMOD |= 0x01; //16位定时器

    TH0 = (65536-50000)/256;
    TL0 = (65536-50000)%256;

    ET0 = 1; //允许T0中断
    TR0 = 1;

	LED = 1;
}

void Timer0_SER(void) interrupt 1
{
	unsigned char i = 0;
    TH0=(65536-50000)/256;
    TL0=(65536-50000)%256;
	Timer_Counter0++;
	Timer_Counter1++;
	if(Timer_Counter0 == 20)
	{
		Timer_Counter0 = 0;
		LED = !LED;
			SendByte_UART(IAP_ReadByte(0x0010));
			SendByte_UART(IAP_ReadByte(0x0011));
	}  
	
}

