#include "stc12c5a60s2.h"
#include "Data_Store.h"
#include "UART.h"
#include <intrins.h>
#include <stdio.h>


#define CMD_IDLE 0
#define CMD_READ 1
#define CMD_PROGRAM 2
#define CMD_ERASE 3

#define	ENABLE_IAP 0x83

#define IAP_ADDRESS_SEC1 0x0000	 /* STC12C5A60S2总共只有两个扇区，大小1k	*/
#define IAP_ADDRESS_SEC2 0x0200	 /* 每个扇区512个字节 */

struct Private_Data
{
	unsigned char Account[20];
	unsigned char Passwd[20]; 
};

static unsigned char Boot_Flag = 0;


void Delay(unsigned char n)
{
	unsigned int x;
	while(n--)
	{
		x = 0;
		while(++x);
	}
}

/*
 * 扇区擦写读操作初始化
 */
void IAP_Idle(void)
{
	IAP_CONTR = 0;
	IAP_CMD = 0;
	IAP_TRIG = 0;
	IAP_ADDRH = 0x80;
	IAP_ADDRL = 0x00;
}


/*
 * 扇区读，地址addr 0x0000-0x03FF，返回数据0x00-0xFF
 */
unsigned char IAP_ReadByte(unsigned int addr)
{
	unsigned char dat;

	IAP_CONTR = ENABLE_IAP;
	IAP_CMD = CMD_READ;
	IAP_ADDRL = addr;
	IAP_ADDRH = addr >> 8;
	IAP_TRIG = 0x5A;
	IAP_TRIG = 0XA5;
	_nop_();

	dat = IAP_DATA;
	IAP_Idle();

	return dat;
}

/*
 * 扇区写，地址addr 0x0000-0x03FF，数据0x00-0xFF
 */
void IAP_ProgramByte(unsigned int addr,unsigned char dat)
{
	IAP_CONTR = ENABLE_IAP;
	IAP_CMD = CMD_PROGRAM;
	IAP_ADDRL = addr;
	IAP_ADDRH = addr >> 8;
	IAP_DATA = dat;
	IAP_TRIG = 0x5A;
	IAP_TRIG = 0XA5;
	_nop_();


	IAP_Idle();	
}


/*
 * 扇区擦除，地址addr 0x0000-0x03FF
 */
void IAP_EraseSector(unsigned int addr)
{
	IAP_CONTR = ENABLE_IAP;
	IAP_CMD = CMD_ERASE;
	IAP_ADDRL = addr;
	IAP_ADDRH = addr >> 8;
	IAP_TRIG = 0x5A;
	IAP_TRIG = 0XA5;
	_nop_();

	IAP_Idle();		
}

/*
 * 获取启动标志
 */
unsigned char Get_Boot_Flag(void)
{
	return Boot_Flag;
}

/*
 * 获取剩余天数
 */
unsigned int Get_Timeless(void)
{
	return (IAP_ReadByte(IAP_ADDRESS_SEC1+2)<<8)|(IAP_ReadByte(IAP_ADDRESS_SEC1+3))	;
}


/*
 * 写入启动标志和剩余天数
 */
void Set_Timeless(unsigned int data_Time)
{
	IAP_EraseSector(IAP_ADDRESS_SEC1); //擦除扇区1
	Delay(10);

	IAP_ProgramByte(IAP_ADDRESS_SEC1,0x55);
	IAP_ProgramByte(IAP_ADDRESS_SEC1+1,0xAA);


	IAP_ProgramByte(IAP_ADDRESS_SEC1+2,data_Time>>8);
	IAP_ProgramByte(IAP_ADDRESS_SEC1+3,(data_Time&0x00ff));		
}


/*
 * EEPROM读写任务
 */
void IAP_Task(void)
{

#ifdef __DEBUG__

#ifdef __WEI__	/* 定义了__DEBUG__，又定义__WEI__ */
	IAP_EraseSector(IAP_ADDRESS_SEC1); /*擦除扇区1.2	*/
	Delay(10);
	IAP_EraseSector(IAP_ADDRESS_SEC2); 
	Delay(10);
	IAP_ProgramByte(IAP_ADDRESS_SEC1,0x55);			  /* 写入启动标志 */
	IAP_ProgramByte(IAP_ADDRESS_SEC1+1,0xAA);

	IAP_ProgramByte(IAP_ADDRESS_SEC1+2,0x07);		/* 写入Timeless */
	IAP_ProgramByte(IAP_ADDRESS_SEC1+3,0xC4);
	Delay(10);
	SendByte_UART(IAP_ReadByte(IAP_ADDRESS_SEC1+2));
	SendByte_UART(IAP_ReadByte(IAP_ADDRESS_SEC1+3));
	SendByte_UART(IAP_ReadByte(IAP_ADDRESS_SEC1+2));
	SendByte_UART(IAP_ReadByte(IAP_ADDRESS_SEC1+3));
#else	   /* 定义了__DEBUG__，但没有定义__WEI__ */
	IAP_EraseSector(IAP_ADDRESS_SEC1); 	  /* 擦除扇区 */
	Delay(10);
	IAP_EraseSector(IAP_ADDRESS_SEC2); 
	Delay(10);
	IAP_ProgramByte(IAP_ADDRESS_SEC1,0x55);	 /* 写入启动标志 */
	IAP_ProgramByte(IAP_ADDRESS_SEC1+1,0xAA);
	Delay(10);
#endif

#endif

	if((IAP_ReadByte(IAP_ADDRESS_SEC1)!= 0x55) || (IAP_ReadByte(IAP_ADDRESS_SEC1+1) != 0xAA))	  /* 如果不是有效的，那么就擦除EEPROM上所有的数据，以防盗窃	  */
	{
		IAP_EraseSector(IAP_ADDRESS_SEC1); 
		IAP_EraseSector(IAP_ADDRESS_SEC2); 
		Boot_Flag = 0;		
	}
	else /*是有效的启动标志	 */
	{
		Boot_Flag = 1;
	}
}





