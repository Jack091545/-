#ifndef __DS1302_H__
#define __DS1302_H__




/*------------------------------------------------
           ��DS1302д��һ�ֽ�����
------------------------------------------------*/
static void Ds1302_Write_Byte(unsigned char addr, unsigned char d);
/*------------------------------------------------
           ��DS1302����һ�ֽ�����
------------------------------------------------*/
static unsigned char Ds1302_Read_Byte(unsigned char addr) ;
/*------------------------------------------------
           ��DS1302д��ʱ������
------------------------------------------------*/
void Ds1302_Write_Time(void) ;
/*------------------------------------------------
           ��DS1302����ʱ������
------------------------------------------------*/
static void Ds1302_Read_Time(void)  ;
/*------------------------------------------------
                DS1302��ʼ��
------------------------------------------------*/
void Ds1302_Init(void);

void Read_Time_Task(void);

unsigned char Get_SetFlag(void);
void Set_SetFlag(unsigned char flag_data);
unsigned char *Get_time_buf1(void);
void Update_Time_Task(void);
#endif