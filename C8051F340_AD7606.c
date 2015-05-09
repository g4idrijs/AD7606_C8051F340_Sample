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
sbit Led = P2^3;
U8 Out_Packet[32];

U8 j;
U8 k;
U8 Busy;
U8 temp;
U8 t = 0;

void AD7606_Init()
{	
	Led=1;
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
	delay0us();
	CONVSTA=1; CONVSTB=1;
	delay0us();
	Busy=BUSY;
	while(Busy==1)
	{
		delay0us();
		Busy=BUSY;
	}
	CS = 0;
	for(k=0; k<4; k++)
	{
		RD=0;
		if(k==0)
		{
			Out_Packet[t*2] = P1;
			Out_Packet[(t++)*2+1] = P3;	
		}			
		RD=1;
	}

//	RD=0;
//	Out_Packet[t*2] = P1;
//	Out_Packet[(t++)*2+1] = P3;
//	RD=1;

	CS=1;
}