#include"delay24M.h"
//*******************************************************************************//
//��������:24M��ʱ����,��ʱ���׼ȷ������С,�����ĸ�Ϊ�Ӻ���������ʱʹ���±��ĸ����κ���
//*******************************************************************************//	
void delay1us(void)   
{
    unsigned char a;
	a=0;
}
void delay10us(void)   
{
    unsigned char a,b;
    for(b=5;b>0;b--)
        for(a=10;a>0;a--);
}

void delay1ms(void) 
{
    unsigned char a,b,c;
    for(c=2;c>0;c--)
        for(b=222;b>0;b--)
            for(a=13;a>0;a--);
}
void delay1s(void)   
{
    unsigned char a,b,c;
    for(c=140;c>0;c--)
        for(b=168;b>0;b--)
            for(a=250;a>0;a--);
}
//*******************************************************************************//
//��������:24M��ʱ����
//*******************************************************************************//	
void delay_us(unsigned char T)  //��ʱT=1,ʵ����ʱ2us;T=5 or 10 ��ʱ���С�������ڶ�ʱ����ʱ���ϳ���ʱ��������ʱ����
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
