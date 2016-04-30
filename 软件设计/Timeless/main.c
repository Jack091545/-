#include "stc12c5a60s2.h"
#include "Display.h"
#include "DS1302.h"
#include "UART.h"
#include "Timer.h"
#include "Data_store.h"
#include <intrins.h> 
#include <stdio.h>


void Init_Task(void)
{

	Ds1302_Init();
#ifdef SET
	Ds1302_Write_Time();
#endif
	Timer_Init();
	UART_Init();
	EA = 1;			
//	SendStr("Timeless V1.0\n");
}

/*------------------------------------------------
                   Ö÷³ÌÐò
------------------------------------------------*/
void main(void)
{
	Init_Task();
	IAP_ProgramByte(0x0010,0x55);
	IAP_ProgramByte(0x0011,0xAA);



	while(1)
	{
		Read_Time_Task();		
		Display_Task();
	}
}


