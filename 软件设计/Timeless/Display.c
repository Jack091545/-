#include "stc12c5a60s2.h"

unsigned char  segout[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; //8��
/*------------------------------------------------
                Ӳ���˿ڶ���
------------------------------------------------*/

sbit LATCH = P2^6; 
sbit SRCLK= P2^5;
sbit SER  = P2^7;

//���źſ���
sbit LATCH_B = P2^3;
sbit SRCLK_B= P2^4;
sbit SER_B= P2^2;

/*------------------------------------------------
 uS��ʱ����������������� unsigned char t���޷���ֵ
 unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
 0~255 ����ʹ�þ���12M����ȷ��ʱ��ʹ�û��,������ʱ
 �������� T=tx2+5 uS 
------------------------------------------------*/
void DelayUs2x(unsigned char t)
{   
 while(--t);
}
/*------------------------------------------------
 mS��ʱ����������������� unsigned char t���޷���ֵ
 unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
 0~255 ����ʹ�þ���12M����ȷ��ʱ��ʹ�û��
------------------------------------------------*/
void DelayMs(unsigned char t)
{
     
 while(t--)
 {
     //������ʱ1mS
     DelayUs2x(245);
	 DelayUs2x(245);
 }
}
/*------------------------------------------------
                �����ֽڳ���
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
                ����˫�ֽ���
     595������n��595������Ҫ����n�ֽں�����
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
	���ź�
------------------------------------------------*/
void SendSeg(unsigned char dat)
{    
unsigned char i; 
        
   for(i=0;i<8;i++)  //����8�ж�Ӧ������
	{
		SRCLK=0;
		SER=dat&0x80;
		dat<<=1;
		SRCLK=1;
	}
	LATCH=0;    //����
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
		Send4Byte(0x00,0x00,0x00,0x00);//delay(10); //��ֹ��
		DelayMs(1);
	}
}
