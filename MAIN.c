#include "compiler_defs.h"
#include <c8051f340.h>
#include <stddef.h>
#include <stdio.h>
#include "USB_API.h"
#include "Delay.h"

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
#define SYSCLK 12000000/8
#define TIMER_PRESCALER	12  // Based on Timer2 CKCON and TMR2CN settings
#define RATE	40000 //if LED_TOGGLE_RATE = 1, the LED will be on for 1 second and off for 1 second
// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER timer ticks per second.
#define TIMER_TICKS_PER_S  SYSCLK/TIMER_PRESCALER
// NoteRATE*TIMER_TICKS_PERS should not exceed 65535 (0xFFFF)for the 16-bit timer
#define AUX1 TIMER_TICKS_PER_S/RATE
#define AUX2 -AUX1
#define TIMER2_RELOAD AUX2  // Reload value for Timer2

sfr16 TMR2RL = 0xca; // Timer2 reload value 
sfr16 TMR2 = 0xcc; // Timer2 counter
/*** [ END ]  [ END ] ***/

/*** [BEGIN]  [BEGIN] ***/
#define N 16
U8 temp[2]={0,0};
U8 Busy;
U8 xdata out[N];
U16 T;
U8 i;
/*** [ END ]  [ END ] ***/

/*** [BEGIN]  [BEGIN] ***/
void Oscilitator_Init(void);
void Port_Init(void);
void Suspend_Device(void);
void Delay(void);	
void AD7606_Init(void);
void AD7606_Read(void);
void Timer2_1_Init();
void Timer2_2_Init();
/*** [ END ]  [ END ] ***/

void main(void)
{
	PCA0MD &= ~0x40;

   	//Oscilitator_Init();
	Port_Init(); 
	USB_Clock_Start();
	CLKSEL |= 0x02;
	USB_Init(USB_VID,USB_PID,USB_MfrStr,USB_ProductStr,USB_SerialStr,USB_MaxPower,USB_PwAttributes,USB_bcdDevice);   

	AD7606_Init();
	USB_Int_Enable();
	T=0;
	//Timer2_1_Init(); 
	EA=1;
	 
	while (1)
	{
		AD7606_Read();	
	}
}

void AD7606_Init()
{
	delay80us();
	REST=0;
	OA=0;OB=0;OC=0;RAGE=0;
	CS_RD=1;
	CONVSTAB=0;
	REST=1;
	delay80us();
	REST=0;
	
}

void AD7606_Read()
{
	CONVSTAB=1;	
	out[T]=temp[0];
	T=T+1;
	out[T]=temp[1];
	T=T+1;
	if(T==N)
	{
		Block_Write(out,N);
		T=0;
	}
	//Block_Write(temp,2);
	Busy=BUSY;
	while(Busy==1)
	{
		Busy=BUSY;
	}	
	CS_RD=0;
	temp[0]=P3;
	temp[1]=P1;
	CS_RD=1;
	CONVSTAB=0;
}

void Oscilitator_Init()
{
	CLKMUL    = 0x80;
    for (i = 0; i < 20; i++);    // Wait 5us for initialization
    CLKMUL    |= 0xC0;
    while ((CLKMUL & 0x20) == 0);
    OSCICN    = 0x83;
	CLKSEL |= 0x02;	
}

// This function configures Timer2 as a 16-bit reload timer, interrupt enabled.
// Using the SYSCLK at 12MHz/8 with a 1:12 prescaler.
// Note: The Timer2 uses a 1:12 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
void Timer2_1_Init ()
{
   CKCON &= ~0x60; // Timer2 uses SYSCLK/12
   TMR2CN &= ~0x01;

   TMR2RL = TIMER2_RELOAD; // Reload value to be used in Timer2
   TMR2 = TMR2RL; // Init the Timer2 register

   TMR2CN = 0x04; // Enable Timer2 in auto-reload mode
   ET2 = 1; 
}

// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 100uS 
// intervals.  Timer 2 overflow automatically triggers ADC0 conversion.
void Timer2_2_Init ()
{
   TMR2CN  = 0x00; // Stop Timer2; Clear TF2;use SYSCLK as timebase, 16-bit auto-reload
   CKCON  |= 0x10; // select SYSCLK for timer 2 source
   TMR2RL  = 65535 - (24000000 / 100000);
   TMR2    = 0xffff; // set to reload immediately
   TR2     = 1; // start Timer2
}

void Port_Init(void)
{
	P0MDIN |= 0x40;// 0x40:BUSY input
	P0MDOUT = 0xcc; //0x10 : Set TX pin to push-
	P1MDIN |= 0xff; 
	P1MDOUT = 0x00;
	P1 |= 0xff;
	P2MDOUT = 0xfb;
	P3MDIN |= 0xff;	
	P3MDOUT = 0x00;
	P3 |= 0xff;		  

	XBR0 = 0x01;// Enable UART0
	XBR1 = 0x40;// Route CEX0 to P0.0,Enable crossbar and weak pull-ups
}

void Suspend_Device(void)
{
   USB_Suspend();	// Put the device in suspend state

}

// Example ISR for USB_API
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
   TF2H = 0; // Clear Timer2 interrupt flag
   AD7606_Read();
}

void Delay (void)
{
   int x;
   for(x = 0;x < 500;x)
      x++;
}