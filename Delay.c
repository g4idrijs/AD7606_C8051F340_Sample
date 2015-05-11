#include"Delay.h"

void delay0us(void)
{
	unsigned char xdata i;
	for(i=1;i>0;i--);
}
void delay1us(void)
{
	unsigned char xdata i;
	for(i=6;i>0;i--);
}
void delay_us(unsigned char T)
{
    while(T--)
	{
		delay1us();	  
	}
}