#include "stc12c5a60s2.h"

unsigned char  segout[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; //8列
/*------------------------------------------------
                硬件端口定义
------------------------------------------------*/

sbit LATCH = P2^6; 
sbit SRCLK= P2^5;
sbit SER  = P2^7;

//行信号控制
sbit LATCH_B = P2^3;
sbit SRCLK_B= P2^4;
sbit SER_B= P2^2;

/*------------------------------------------------
 uS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编,大致延时
 长度如下 T=tx2+5 uS 
------------------------------------------------*/
void DelayUs2x(unsigned char t)
{   
 while(--t);
}
/*------------------------------------------------
 mS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编
------------------------------------------------*/
void DelayMs(unsigned char t)
{
     
 while(t--)
 {
     //大致延时1mS
     DelayUs2x(245);
	 DelayUs2x(245);
 }
}
/*------------------------------------------------
                发送字节程序
------------------------------------------------*/
void SendByte(unsigned char dat)
{    
  unsigned char i; 
        
	for(i=0;i<8;i++)
	{
		SRCLK_B=0;
		SER_B=dat&0x80;
		dat<<=1;
		SRCLK_B=1;
	}
	
         
}
/*------------------------------------------------
                发送双字节序
     595级联，n个595，就需要发送n字节后锁存
------------------------------------------------*/
void Send4Byte(unsigned char dat1,unsigned char dat2,unsigned char dat3,unsigned char dat4)
{    
   SendByte(dat1);
   SendByte(dat2);
   SendByte(dat3);
   SendByte(dat4);
   LATCH_B=0;
   LATCH_B=1;     
}

/*------------------------------------------------
	行信号
------------------------------------------------*/
void SendSeg(unsigned char dat)
{    
unsigned char i; 
        
   for(i=0;i<8;i++)  //发送8行对应的数据
	{
		SRCLK=0;
		SER=dat&0x80;
		dat<<=1;
		SRCLK=1;
	}
	LATCH=0;    //锁存
	LATCH=1;
}

void Send_Data(unsigned char dat1[],unsigned char dat2[],unsigned char dat3[],unsigned char dat4[])
{
	unsigned char i = 0;
	for(i = 0;i < 8;i++)
	{
		SendSeg(~segout[i]); //0xfd
		Send4Byte(dat4[i],dat3[i],dat2[i],dat1[i]);
		DelayMs(1);
		Send4Byte(0x00,0x00,0x00,0x00);//delay(10); //防止重
		DelayMs(1);
	}
}
