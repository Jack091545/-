#include "stc12c5a60s2.h"
#include "DS1302.h"
#include <stdio.h>

unsigned char Timer_1000ms = 0;
unsigned int nian,yue,ri,shi,fen,miao,zhou; //存储显示值的全局变量

void Timer0_Init(void)
{														
	TMOD |= 0x01; //16位定时器
    TH0=(65536-50000)/256;
    TL0=(65536-50000)%256;
    ET0 = 1; //允许T0中断
//    EA  = 1; //开放中断
    TR0 = 1;
}

void Timer_SER(void) interrupt 1
{
    TH0=(65536-50000)/256;
    TL0=(65536-50000)%256;
}
