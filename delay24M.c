#include"delay24M.h"

/*void delay1us(void)   
{
    unsigned char a;
	a=0;
} */
void delay1us(void)
{
unsigned char xdata i;
for(i=6;i>0;i--);
 
}
void delay10us(void)   
{
    unsigned char a,b;
    for(b=10;b>0;b--)
        for(a=12;a>0;a--);
}

void delay1ms(void) 
{
    unsigned char a,b,c;
    for(c=100;c>0;c--)
        for(b=10;b>0;b--)
            for(a=12;a>0;a--);
}
void delay1s(void)   
{
    unsigned char a,b,c;
    for(c=140;c>0;c--)
        for(b=168;b>0;b--)
            for(a=250;a>0;a--);
}
//*******************************************************************************//
//函数功能:24M延时函数
//*******************************************************************************//	
void delay_us(unsigned char T)  //延时T=1,实际延时2us;T=5 or 10 延时误差小，仅用于短时间延时，较长延时用其它延时函数
{
    while(T--)
	{
		delay1us();	  
	}
}
void delay_10us(unsigned char T)  
{
    while(T--)
	{
		delay10us();	  
	}
}
void delay_ms(unsigned char T)  
{
    while(T--)
	{
		delay1ms();	  
	}
}
void delay_s(unsigned char T)  	
{
    while(T--)
	{
		delay1s();	  
	}
}
