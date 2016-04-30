#ifndef __DATA_STORE_H__
#define __DATA_STORE_H__

void Delay(unsigned char n);
void IAP_Idle(void);
unsigned char IAP_ReadByte(unsigned int addr);
void IAP_ProgramByte(unsigned int addr,unsigned char dat);
void IAP_EraseSector(unsigned int addr);


#endif