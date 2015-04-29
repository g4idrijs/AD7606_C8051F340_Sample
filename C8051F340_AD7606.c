#include <c8051f340.h>
#include "C8051F340_AD7606.h"
#include "delay24M.h"
#include <intrins.h>
sbit AD7606_CS=P0^0;
sbit AD7606_RD=P0^1;
sbit AD7606_CONVSTA=P0^2;
sbit AD7606_BUSY=P0^3;
sbit AD7606_REST=P0^4;
sbit INT=P0^5;
int byte_num;
unsigned char H_Data,L_Data;
unsigned char Data[16];

unsigned char j, k;
unsigned short int TempA, TempB;
unsigned char Busy;

void AD7606_Init()
{	
	delay1ms();
	AD7606_REST=0;
	AD7606_CS=1;
	AD7606_CONVSTA=1;
	AD7606_RD=1;
	delay1ms();
	AD7606_REST=1;
	delay1us();
	AD7606_REST=0;
	delay1us();
	
}
void AD7606_Read()
{
	AD7606_CONVSTA=0;
	delay1us();
	byte_num=0;
	AD7606_CONVSTA=1;
	delay1us();

	Busy=AD7606_BUSY;

	while(Busy==1)
	{
		delay1us();
		Busy=AD7606_BUSY;
	}

	AD7606_CS = 0;
	for(j=0; j<8; j++)
	{
		TempA=0;
		TempB=0;
		
		AD7606_RD=0;
		TempA= P3;
		TempB= P1;
		AD7606_RD=1;
		
		Data[byte_num++] =TempA;
		Data[byte_num++] =TempB;
	}
	AD7606_CS=1;
}