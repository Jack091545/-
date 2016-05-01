#include "stc12c5a60s2.h"
#include "UART.h"
#include "DS1302.h"
#include <stdlib.h>
#include <string.h>

static unsigned char *time_buf_data;
static unsigned char time_buf2[8];

/*------------------------------------------------
              ����ͨѶ��ʼ��
------------------------------------------------*/
void UART_Init(void)
{
    SCON  = 0x50;		        // SCON: ģʽ 1, 8-bit UART, ʹ�ܽ���  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit ��װ
    TH1   = 0xFD;               // TH1:  ��װֵ 9600 ������ ���� 11.0592MHz  
    TR1   = 1;                  // TR1:  timer 1 �� 
    ES    = 1;                  //�򿪴����ж�  
	TI=1;                     
}



/*------------------------------------------------
                    ����һ���ֽ�
------------------------------------------------*/
void SendByte_UART(unsigned char dat)
{
 SBUF = dat;
 while(!TI);
      TI = 0;
}
/*------------------------------------------------
                    ����һ���ַ���
------------------------------------------------*/
void SendStr(unsigned char *s)
{
 while(*s!='\0')// \0 ��ʾ�ַ���������־��
                //ͨ������Ƿ��ַ���ĩβ
  {
  SendByte_UART(*s);
  s++;
  }
}

/*------------------------------------------------
                  �����жϳ���
------------------------------------------------*/
void UART_SER (void) interrupt 4 //�����жϷ������
{
    unsigned char Temp;          //������ʱ���� 
	static unsigned char i = 0,j = 0;

	if(RI)                        //�ж��ǽ����жϲ���
	{
		RI=0;                      //��־λ����
		Temp = SBUF;                 //���뻺������ֵ

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
			Set_SetFlag(1);               //������ɱ�־λ��1
		}		
	}
	if(TI)  //����Ƿ��ͱ�־λ������
	{
//		TI=0;
	}
} 

