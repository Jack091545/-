#include "stc12c5a60s2.h"
#include "Display.h"
#include "DS1302.h"
#include "UART.h"
#include "Timer.h"
#include "Data_store.h"
#include <intrins.h> 
#include <stdio.h>


/*
 * 初始化任务
 */
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

/*
 * 主进程任务
 */
void Process_Task(void)
{
	if(Get_Boot_Flag())
	{
		if(Get_Read_Time_Flag())
		{
			Read_Time_Task();
		}	
		else
		{
			Display_Task();
			Update_Time_Task();		
		}	

	}
	else
	{
		 SendStr("Timeless Imformations has been stolen!\n");
	}	
}

/*------------------------------------------------
                   主程序
------------------------------------------------*/
void main(void)
{
	Init_Task();
	while(1)
	{
		 Process_Task();
	}
}


