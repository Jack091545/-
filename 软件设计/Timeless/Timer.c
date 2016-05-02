#include "stc12c5a60s2.h"
#include "DS1302.h"



sbit LED = P1^4;  /* 用来指示MCU还活着 */

static unsigned char Timer_Counter0 = 0; 
static unsigned char Timer_Counter1 = 0; 
static unsigned char Display_Switch = 1; 


/*------------------------------------------------
	获取显示时间标志位
------------------------------------------------*/
unsigned char *Get_Display_Switch(void)
{
	return Display_Switch;
}



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
	if(Timer_Counter0 == 20)//1s
	{
		Timer_Counter0 = 0;
		LED = !LED;		
	} 
	if(Timer_Counter1 == 200)//10s
	{
		Display_Switch = !Display_Switch;
	} 
	
}

