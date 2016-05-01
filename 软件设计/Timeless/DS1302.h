#ifndef __DS1302_H__
#define __DS1302_H__




/*------------------------------------------------
           向DS1302写入一字节数据
------------------------------------------------*/
static void Ds1302_Write_Byte(unsigned char addr, unsigned char d);
/*------------------------------------------------
           从DS1302读出一字节数据
------------------------------------------------*/
static unsigned char Ds1302_Read_Byte(unsigned char addr) ;
/*------------------------------------------------
           向DS1302写入时钟数据
------------------------------------------------*/
void Ds1302_Write_Time(void) ;
/*------------------------------------------------
           从DS1302读出时钟数据
------------------------------------------------*/
static void Ds1302_Read_Time(void)  ;
/*------------------------------------------------
                DS1302初始化
------------------------------------------------*/
void Ds1302_Init(void);

void Read_Time_Task(void);

unsigned char Get_SetFlag(void);
void Set_SetFlag(unsigned char flag_data);
unsigned char *Get_time_buf1(void);
void Update_Time_Task(void);
#endif