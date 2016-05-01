#include "stc12c5a60s2.h"
#include "UART.h"
#include "DS1302.h"
#include <stdlib.h>
#include <string.h>

static unsigned char *time_buf_data;
static unsigned char time_buf2[8];

/*------------------------------------------------
              串口通讯初始化
------------------------------------------------*/
void UART_Init(void)
{
    SCON  = 0x50;		        // SCON: 模式 1, 8-bit UART, 使能接收  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit 重装
    TH1   = 0xFD;               // TH1:  重装值 9600 波特率 晶振 11.0592MHz  
    TR1   = 1;                  // TR1:  timer 1 打开 
    ES    = 1;                  //打开串口中断  
	TI=1;                     
}



/*------------------------------------------------
                    发送一个字节
------------------------------------------------*/
void SendByte_UART(unsigned char dat)
{
 SBUF = dat;
 while(!TI);
      TI = 0;
}
/*------------------------------------------------
                    发送一个字符串
------------------------------------------------*/
void SendStr(unsigned char *s)
{
 while(*s!='\0')// \0 表示字符串结束标志，
                //通过检测是否字符串末尾
  {
  SendByte_UART(*s);
  s++;
  }
}

/*------------------------------------------------
                  串口中断程序
------------------------------------------------*/
void UART_SER (void) interrupt 4 //串行中断服务程序
{
    unsigned char Temp;          //定义临时变量 
	static unsigned char i = 0,j = 0;

	if(RI)                        //判断是接收中断产生
	{
		RI=0;                      //标志位清零
		Temp = SBUF;                 //读入缓冲区的值

		time_buf2[i++] = Temp;
		if(i == 8)                  
		{
			time_buf_data = Get_time_buf1();
			for(j = 0;j<8;j++)
			{
				time_buf_data[j] = time_buf2[j];
			}
			i = 0;
			memset(time_buf2,0,sizeof(time_buf2)/sizeof(unsigned char));
			Set_SetFlag(1);               //接收完成标志位置1
		}		
	}
	if(TI)  //如果是发送标志位，清零
	{
//		TI=0;
	}
} 

