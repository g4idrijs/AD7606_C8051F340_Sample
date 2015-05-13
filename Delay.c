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
void delay2us (void)
{
   int x;
   for(x = 0;x < 12;x)
      x++;
}
void delay80us (void)
{
   int x;
   for(x = 0;x < 500;x)
      x++;
}