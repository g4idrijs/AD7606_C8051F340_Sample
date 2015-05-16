#include <c8051f340.h>
#include "C8051F340_AD7606.h"
#include "Delay.h"
#include <intrins.h>

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

U8 Busy;

U8 xdata out[1024];
U16 t;
U16 i;

void AD7606_Init()
{
	delay80us();
	REST=0;
	OA=0;OB=0;OC=0;RAGE=0;
	CS=1;RD=1;
	CONVSTA=1; CONVSTB=1;
	REST=1;
	delay1us(); /* AD7606是高电平复位，要求最小脉宽50ns */
	REST=0;
	
}					   

void AD7606_ContinuesRead()
{
	t=0;
	for(i=0;i<512;i++)
	{
		CONVSTA=1; CONVSTB=1;		
		Busy=BUSY;
		while(Busy==1)
		{
			Busy=BUSY;
		}	
		CS=0;RD=0;
		out[t*2]=P3;
		out[(t++)*2+1]=P1;
		RD=1;CS=1;
		CONVSTA=0; CONVSTB=0;
	}		
}