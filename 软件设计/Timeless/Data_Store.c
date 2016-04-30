#include "stc12c5a60s2.h"
#include "Data_Store.h"
#include<intrins.h>


#define CMD_IDLE 0
#define CMD_READ 1
#define CMD_PROGRAM 2
#define CMD_ERASE 3

#define	ENABLE_IAP 0x82

#define IAP_ADDRESS 0x0000



void Delay(unsigned char n)
{
	unsigned int x;
	while(n--)
	{
		x = 0;
		while(++x);
	}
}

void IAP_Idle(void)
{
	IAP_CONTR = 0;
	IAP_CMD = 0;
	IAP_TRIG = 0;
	IAP_ADDRH = 0x80;
	IAP_ADDRL = 0x00;
}

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




