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
#ifdef __DEBUG__
	Ds1302_Write_Time();
#endif
	Timer_Init();
	UART_Init();
	IAP_Task();
	EA = 1;			
	SendStr(" Timeless V2.0,Compiled about: ");
	SendStr(__DATE__);
	SendStr(",");
	SendStr(__TIME__);
	SendStr(".\n");
}

void Process_Task(void)
{
	if(Get_Boot_Flag())
	{
		Update_Time_Task();
		Read_Time_Task();		
		Display_Task();
	}
	else
	{
		 SendStr("Timeless Imformations has been stolen!\n");
	}	
}

/*------------------------------------------------
                   Ö÷³ÌÐò
------------------------------------------------*/
void main(void)
{
	Init_Task();
	while(1)
	{
		 Process_Task();
	}
}


