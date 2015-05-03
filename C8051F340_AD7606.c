#include <c8051f340.h>
#include "C8051F340_AD7606.h"
#include "delay24M.h"
#include <intrins.h>
sbit CS=P0^0;
sbit RD=P0^1;
sbit CONVSTA=P0^2;
sbit CONVSTB=P0^3;
sbit BUSY=P0^6;
sbit REST=P0^7;
unsigned char H_Data,L_Data;
unsigned char Data[2];

unsigned char j;
unsigned char k = 0;
unsigned char TempA, TempB;
unsigned char Busy;

void AD7606_Init()
{	
	delay1ms();
	REST=0;
	CS=1;
	CONVSTA=1;
	CONVSTB=1;
	RD=1;
	delay1ms();
	REST=1;
	delay10us();
	//delay1ms();
	REST=0;
	delay10us();
	//delay1ms();
	
}
void AD7606_Read()
{
	CONVSTA=0;
	CONVSTB=0;
	delay10us();
	CONVSTA=1;
	CONVSTB=1;
	delay10us();
	Busy=BUSY;
	while(Busy==1)
	{
		delay10us();
		Busy=BUSY;
	}
	CS = 0;
	for(j=0; j<1; j++)
	{
	for(k=0; k<1; k++)
	{
		TempA=0;
		TempB=0;
		RD=0;
		//TempA = (TempA<<1) + P3;
		//TempB = (TempB<<1) + P1;
		TempA = P3;
		TempB = P1;
		RD=1;
		Data[0] = TempA;
		Data[1] = TempB;
		}
	}
	CS=1;
}