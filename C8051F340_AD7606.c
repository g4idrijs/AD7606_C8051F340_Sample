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

void AD7606_Init()
{	AD7606_REST=0;
	AD7606_RD=1;
	AD7606_CS=0;
	AD7606_CONVSTA=1;
	AD7606_REST=1;
	AD7606_REST=0;
}
void AD7606_Read()
{
	int i;
	AD7606_CONVSTA=0;
	byte_num=0;
	AD7606_CONVSTA=1;
	
	while(AD7606_BUSY);
	
	//AD7606_CS=0;
	
	for(i=0;i<1;i++)
	{	
		AD7606_RD=0;
		H_Data=P3;
	    L_Data=P1;
		AD7606_RD=1;
		Data[byte_num++] =H_Data;
		Data[byte_num++] =L_Data;
	}
	//AD7606_CS=1;
}