#include <c8051f340.h>
#include "C8051F340_AD7606.h"
#include "Delay.h"
#include <intrins.h>
typedef unsigned char BYTE;

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

U8 xdata out1[256];
U8 xdata out2[256];
U8 xdata out3[256];
U8 xdata out4[256];
U8 xdata out5[256];
U8 xdata out6[256];
U8 xdata out7[256];
U8 xdata out8[256];
U8 t1 = 0;
U8 t2 = 0;
U8 t3 = 0;
U8 t4 = 0;
U8 t5 = 0;
U8 t6 = 0;
U8 t7 = 0;
U8 t8 = 0;

//U8 xdata out[2048];
U8 t;

U8 Busy;
U16 i;

void AD7606_Init()
{
	delay80us();
	REST=0;
//	delay80us();
	OA=0;OB=0;OC=0;RAGE=0;
//	delay80us();
	CS=1;RD=1;
	CONVSTA=1; CONVSTB=1;
//	delay80us();
	REST=1;
	delay80us();
	REST=0;
	delay80us();
	
}
void AD7606_Read()
{
//	CONVSTA=0; CONVSTB=0;
//	delay0us();
//	CONVSTA=1; CONVSTB=1;
//	Busy=BUSY;
//	while(Busy==1)
//	{
//		delay1us();
//		Busy=BUSY;
//	}	
//	CS=0;RD=0;
//	out[0] = P3;
//	out[1] = P1;
//	RD=1;CS=1;	
}

void AD7606_ContinuesRead()
{
	t=0;t1=0;t2=0;t3=0;t4=0;t5=0;t6=0;t7=0;t8=0;
	for(i=0;i<1024;i++)
	{
		CONVSTA=0; CONVSTB=0;
		delay0us();
		CONVSTA=1; CONVSTB=1;
		//delay0us();
		Busy=BUSY;
		while(Busy==1)
		{
			delay0us();
			Busy=BUSY;
		}	
		CS=0;RD=0;
//		out[t*2]=P3;
//		out[(t++)*2+1]=P1;
		if(i<128)
		{
			out1[t1*2] = P3;
			out1[(t1++)*2+1] = P1;
		}
		else if(128<=i&&i<256)
		{
			out2[t2*2] = P3;
			out2[(t2++)*2+1] = P1;
		}
		else if(256<=i&&i<384)
		{
			out3[t3*2] = P3;
			out3[(t3++)*2+1] = P1;
		}
		else if(384<=i&&i<512)
		{
			out4[t4*2] = P3;
			out4[(t4++)*2+1] = P1;
		}
		else if(512<=i&&i<640)
		{
			out5[t5*2] = P3;
			out5[(t5++)*2+1] = P1;
		}
		else if(640<=i&&i<768)
		{
			out6[t6*2] = P3;
			out6[(t6++)*2+1] = P1;
		}
		else if(768<=i&&i<896)
		{
			out7[t7*2] = P3;
			out7[(t7++)*2+1] = P1;
		}
		else if(896<=i&&i<1024)
		{
			out8[t8*2] = P3;
			out8[(t8++)*2+1] = P1;
		}
		RD=1;CS=1;
	}		
}