#include <c8051f340.h>
#include "C8051F340_AD7606.h"
#include "delay24M.h"
#include <intrins.h>
typedef unsigned char     u8;
typedef unsigned short    u16;
sbit CS=P0^0;
sbit RD=P0^1;
sbit CONVSTA=P0^2;
sbit CONVSTB=P0^3;
sbit BUSY=P0^6;
sbit REST=P0^7;
sbit OA=P2^4;
sbit OB=P2^5;
sbit OC=P2^6;
sbit RAGE= P2^7;
unsigned char Data[32];

unsigned char j;
unsigned char k = 0;
unsigned char Busy;

void AD7606_Init()
{	
	delay1ms();
	REST=0;
	OA=0;
	OB=0;
	OC=0;
	RAGE=0;
	CS=1;
	CONVSTA=1; CONVSTB=1;
	RD=1;
	delay1ms();
	REST=1;
	delay1ms();
	REST=0;
	delay1ms();
}
void AD7606_Read()
{
	CONVSTA=0; CONVSTB=0;
	delay1us();
	CONVSTA=1; CONVSTB=1;
	delay1us();
	Busy=BUSY;
	while(Busy==1)
	{
		delay1us();
		Busy=BUSY;
	}
	CS = 0;
	for(j=0; j<1; j++)
	{
		for(k=0; k<8; k++)
		{
			RD=0;
			Data[k*2] = P1;
			Data[k*2+1] = P3;
			RD=1;
		}
	}
	CS=1;
}