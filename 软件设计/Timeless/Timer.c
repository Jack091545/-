#include "stc12c5a60s2.h"
#include "DS1302.h"
#include <stdio.h>

unsigned char Timer_1000ms = 0;
unsigned int nian,yue,ri,shi,fen,miao,zhou; //�洢��ʾֵ��ȫ�ֱ���

void Timer0_Init(void)
{														
	TMOD |= 0x01; //16λ��ʱ��
    TH0=(65536-50000)/256;
    TL0=(65536-50000)%256;
    ET0 = 1; //����T0�ж�
//    EA  = 1; //�����ж�
    TR0 = 1;
}

void Timer_SER(void) interrupt 1
{
    TH0=(65536-50000)/256;
    TL0=(65536-50000)%256;
}
