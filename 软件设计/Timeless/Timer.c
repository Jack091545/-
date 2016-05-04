#include "stc12c5a60s2.h"
#include "DS1302.h"



sbit LED = P1^4;  /* ����ָʾMCU������ */

static unsigned char Timer_Counter0 = 0;	/* ��LED�л�����״̬ */ 
static unsigned char Timer_Counter1 = 0; 	/* ����ʾ����ʱ��ʱ���л���ʾ״̬ */
static unsigned char Timer_Counter2 = 0;	/* ��ȡDS1302 */
static unsigned char Display_Switch = 1; 
static unsigned char Read_Time_Flag = 0;

/*------------------------------------------------
	��ȡ�л���ʾʱ���־λ
------------------------------------------------*/
unsigned char Get_Display_Switch(void)
{
	return Display_Switch;
}

/*------------------------------------------------
	��ȡ��ȡDS1302ʱ���־λ
------------------------------------------------*/
unsigned char Get_Read_Time_Flag(void)
{
	return Read_Time_Flag;
}


void Timer_Init(void)
{														
	TMOD |= 0x01; //16λ��ʱ��

    TH0 = (65536-50000)/256;
    TL0 = (65536-50000)%256;

    ET0 = 1; //����T0�ж�
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
	Timer_Counter2++;

	if(Timer_Counter0 == 20)//1s
	{
		Timer_Counter0 = 0;
		LED = !LED;		
	}
	 
	if(Timer_Counter1 == 200)//10s
	{
		Display_Switch = !Display_Switch;
	} 
	if(Timer_Counter2 == 2)/* 100ms */
	{
		Read_Time_Flag = 1;
	}
	
}

