#include <c8051f340.h>
#include "C8051F340_AD7606.h"
#include "Delay.h"
#include <intrins.h>

sbit CS_RD=P0^0;
sbit CONVSTAB=P0^1;
sbit BUSY=P0^6;
sbit REST=P0^7;
sbit OA=P2^4;
sbit OB=P2^5;
sbit OC=P2^6;
sbit RAGE= P2^7;

U8 Busy;

U8 xdata out[1024];
U16 t;
U16 i;

void AD7606_Init()
{
	delay80us();
	REST=0;
	OA=0;OB=0;OC=0;RAGE=0;
	CS_RD=1;
	CONVSTAB=1;
	REST=1;
	delay1us();
	REST=0;
	
}					   

void AD7606_Read()
{
	CONVSTAB=1;	
	Block_Write(out,1024);
	Busy=BUSY;
	while(Busy==1)
	{
		Busy=BUSY;
	}	
	CS_RD=0;
	out[0]=P3;
	out[1]=P1;
	CS_RD=1;
	CONVSTAB=0;
}