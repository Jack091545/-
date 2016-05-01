#ifndef __DATA_STORE_H__
#define __DATA_STORE_H__

void Delay(unsigned char n);
void IAP_Idle(void);
unsigned char IAP_ReadByte(unsigned int addr);
void IAP_ProgramByte(unsigned int addr,unsigned char dat);
void IAP_EraseSector(unsigned int addr);
void IAP_Task(void);
//unsigned char IAP_Test_Task(void);
unsigned int Get_Timeless(void);
void Set_Timeless(unsigned int data_Time);
unsigned char Get_Boot_Flag(void);
void Set_Boot_Flag(unsigned char Boot_Flag_data);
#endif