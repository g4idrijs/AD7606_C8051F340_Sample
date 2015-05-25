#include "compiler_defs.h"
#include <c8051f340.h>
#include <stddef.h>
#include <stdio.h>
#include "USB_API.h"

/*** [BEGIN] USB Descriptor Information [BEGIN] ***/
code const UINT USB_VID = 0x10C4;								
code const UINT USB_PID = 0xEA61;
code const BYTE USB_MfrStr[] = {0x1A,0x03,'S',0,'i',0,'l',0,'i',0,'c',0,'o',0,'n',0,' ',0,'L',0,'a',0,'b',0,'s',0};
code const BYTE USB_ProductStr[] = {0x10,0x03,'U',0,'S',0,'B',0,' ',0,'A',0,'P',0,'I',0};
code const BYTE USB_SerialStr[] = {0x0A,0x03,'i',0,'d',0,'r',0,'i',0};
code const BYTE USB_MaxPower = 15;
code const BYTE USB_PwAttributes = 0x80; // Bus-powered, remote wakeup not supported
code const UINT USB_bcdDevice = 0x0100;
/*** [ END ] USB Descriptor Information [ END ] ***/

/*** [BEGIN]  [BEGIN] ***/
sbit CS_RD=P0^0;
sbit CONVSTAB=P0^1;
sbit BUSY=P0^6;
sbit REST=P0^7;
sbit OA=P2^4;
sbit OB=P2^5;
sbit OC=P2^6;
sbit RAGE= P2^7;
sbit Led = P2^3;
/*** [ END ]  [ END ] ***/

/*** [BEGIN]  [BEGIN] ***/
sfr16 TMR2RL = 0xca; // Timer2 reload value 
sfr16 TMR2 = 0xcc; // Timer2 counter

#define SYSCLK 24000000
#define TIMER_PRESCALER	12  // Based on Timer2 CKCON and TMR2CN settings
#define RATE	50000
// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER timer ticks per second.
#define TIMER_TICKS_PER_S  SYSCLK/TIMER_PRESCALER
// NoteRATE*TIMER_TICKS_PERS should not exceed 65535 (0xFFFF)for the 16-bit timer
#define AUX1 TIMER_TICKS_PER_S/RATE
/*** [ END ]  [ END ] ***/

/*** [BEGIN]  [BEGIN] ***/
#define N 16
U8 Busy;
U8 xdata out1[N];
U8 xdata out2[N];
U16 T1;
U16 T2;
U8 Flag1;
U8 Flag2;
U16 i;
/*** [ END ]  [ END ] ***/

/*** [BEGIN]  [BEGIN] ***/
void Oscilitator_Init(void);
void Port_Init(void);
void Suspend_Device(void);
void Delay(void);	
void AD7606_Init(void);
void AD7606_Read(void);
void Timer2_Init(U16 counts);
void Timer2_ISR (void);
void Ext_Interrupt_Init();
/*** [ END ]  [ END ] ***/

void main(void)
{
	PCA0MD &= ~0x40;

   	Oscilitator_Init();
	Port_Init(); 
	USB_Clock_Start();
	CLKSEL |= 0x02;
	USB_Init(USB_VID,USB_PID,USB_MfrStr,USB_ProductStr,USB_SerialStr,USB_MaxPower,USB_PwAttributes,USB_bcdDevice);   

	AD7606_Init();
	USB_Int_Enable();
	T1=0;T2=0;Flag1=1;Flag2=0;
	Timer2_Init(AUX1); 
	Ext_Interrupt_Init();
	Led=0;
	EA = 1;
	//IE=0xA1;
	IP=0x21;
		 

	while (1)
	{
		if(Flag1==1)
		{
			Block_Write(out1,N);
			Flag1=0;
			Flag2=0;
		}
		if(Flag2==1)
		{
			Block_Write(out2,N);
			Flag1=0;
			Flag2=0;
		}	
	}
}


void AD7606_Init()
{
	Delay();
	REST=0;
	OA=0;OB=0;OC=0;RAGE=0;
	CS_RD=1;
	CONVSTAB=0;
	REST=1;
	Delay();
	REST=0;	
}

void AD7606_Read()
{
	CONVSTAB=1;		
}

void Oscilitator_Init()
{
	CLKMUL = 0x80;
    for (i = 0; i < 20; i++);
    CLKMUL |= 0xC0;
    while ((CLKMUL & 0x20) == 0);
    OSCICN = 0x83;
	CLKSEL |= 0x02;	
}

void Timer2_Init(U16 counts)
{
   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // Use SYSCLK/12 as timebase
   CKCON  &= ~0x60;                    // Timer2 clocked based on T2XCLK;

   TMR2RL  = -counts;                  // Init reload values
   TMR2    = 0xffff;                   // Set to reload immediately
   ET2     = 1;                        // Enable Timer2 interrupts
   TR2     = 1;                        // Start Timer2
}

void Port_Init(void)
{
	P0MDIN |= 0x40;
	P0MDOUT = 0xcc; 
	P1MDIN |= 0xff; 
	P1MDOUT = 0x00;
	P1 |= 0xff;
	P2MDOUT = 0xfb;
	P3MDIN |= 0xff;	
	P3MDOUT = 0x00;
	P3 |= 0xff;		  

	XBR0 = 0x01;
	XBR1 = 0x40;
}

void Ext_Interrupt_Init (void)
{
   TCON = 0x03;
   IT01CF = 0x06; 
   EX0 = 1; 
}

void INT0_ISR (void) interrupt 0
{
	CS_RD=0;
	if(Flag1==0)
	{
		out1[T1]=P3;
		T1=T1+1;
		out1[T1]=P1;
		T1=T1+1;
		if(T1==N)
		{
			Flag1=1;
			T1=0;
		}
	}
	if(Flag2==0)
	{
		out2[T2]=P3;
		T2=T2+1;
		out2[T2]=P1;
		T2=T2+1;
		if(T2==N)
		{
			Flag2=1;
			T2=0;
		}
	}
	CS_RD=1;
	CONVSTAB=0;	
}

void Suspend_Device(void)
{
   USB_Suspend();

}

void  USB_API_TEST_ISR(void) interrupt 17
{
   BYTE INTVAL = Get_Interrupt_Source();

   if (INTVAL & DEV_SUSPEND)
   {
        Suspend_Device();
   }

   if (INTVAL & DEV_CONFIGURED)
   {
		Port_Init();
   }
}

void Timer2_ISR (void) interrupt 5
{
	Led=~Led;
	TF2H = 0; // Clear Timer2 interrupt flag
	AD7606_Read();
}

void Delay (void)
{
   int x;
   for(x = 0;x < 500;x)
      x++;
}

void delay_us(unsigned char T)
{
	unsigned char xdata i;
    while(T--)
	{
		for(i=6;i>0;i--);	  
	}
}