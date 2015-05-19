#include "compiler_defs.h"
#include <c8051f340.h>
#include <stddef.h>
#include <stdio.h>
#include "USB_MAIN.h"
#include "USB_API.h"
#include "F34x_USB_Register.h"
#include "Delay.h"
#include "stdio.h"

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

sbit CS_RD=P0^0;
sbit CONVSTAB=P0^1;
sbit BUSY=P0^6;
sbit REST=P0^7;
sbit OA=P2^4;
sbit OB=P2^5;
sbit OC=P2^6;
sbit RAGE= P2^7;
sbit Led = P2^3;

#define N 32

#define SYSCLK	12000000/8
#define TIMER_PRESCALER	12  // Based on Timer2 CKCON and TMR2CN settings
#define RATE	40000 // if LED_TOGGLE_RATE = 1, the LED will be on for 1  second and off for 1 second
// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so SYSCLK/TIMER_PRESCALER timer ticks per second.
#define TIMER_TICKS_PER_S  SYSCLK/TIMER_PRESCALER
// Note: LED_TOGGLE_RATE*TIMER_TICKS_PERS should not exceed 65535 (0xFFFF)for the 16-bit timer
#define AUX1 TIMER_TICKS_PER_S/RATE
#define AUX2 -AUX1
#define TIMER2_RELOAD AUX2  // Reload value for Timer2
sfr16 TMR2RL = 0xCA; // Timer2 Reload Register
sfr16 TMR2 = 0xCC; // Timer2 Register


U8 temp[2];
U8 Busy;
U8 xdata out[N];
U16 t;


void Sysclk_Init(void);
void USB0_Init(void);
void Port_Init(void);
void Suspend_Device(void);
void Delay(void);	
void AD7606_Init(void);
void AD7606_Read(void);
void Timer2_Init();

void main(void)
{
	PCA0MD &= ~0x40;
	//Sysclk_Init(); 
	//USB0_Init();
	Port_Init(); 
	USB_Clock_Start();
	USB_Init(USB_VID,USB_PID,USB_MfrStr,USB_ProductStr,USB_SerialStr,USB_MaxPower,USB_PwAttributes,USB_bcdDevice);   

	CLKSEL |= 0x02;

	AD7606_Init();
	//USB_Int_Enable();

	Timer2_Init(); 
	EA=1;

	t=0;   
	while (1)
	{	
	}
}

void Sysclk_Init(void)
{
    CLKSEL = 0x00; // Select the internal osc. as the SYSCLK source
    OSCICN = 0x83; // configure internal oscillator for 12MHz / 1
	RSTSRC = 0x04; // enable missing clock detector 

	OSCICN = 0x83;
	CLKMUL  = 0x00; 
	CLKMUL |= 0x80; // Enable clock multiplier   
	Delay(); // Delay for clock multiplier to begin   
	CLKMUL |= 0xC0; // Initialize the clock multiplier   
	Delay(); // Delay for clock multiplier to begin   
	while(!(CLKMUL & 0x20)); // Wait for multiplier to lock
	CLKSEL  = SYS_INT_OSC;
	CLKSEL = SYS_4X_DIV_2; 
}

void Oscillator_Init()
{							   	
    CLKSEL = 0x00; // Select the internal osc. as the SYSCLK source
    OSCICN = 0x83; // configure internal oscillator for 12MHz / 1
	RSTSRC = 0x04; // enable missing clock detector
}

void AD7606_Init()
{
	delay80us();
	REST=0;
	OA=0;OB=0;OC=0;RAGE=0;
	CS_RD=1;
	CONVSTAB=0;
	REST=1;
	delay1us();
	REST=0;
	
}

void AD7606_Read()
{
	CONVSTAB=1;	
	if(t<N/2)
	{
		out[t*2]=temp[0];
		out[(t++)*2+1]=temp[1];
	}
	else
	{
		Block_Write(out,N);
		t=0;
	}
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

void USB0_Init(void)
{
   POLL_WRITE_BYTE(POWER,  0x08); // Force Asynchronous USB Reset
   POLL_WRITE_BYTE(IN1IE,  0x07); // Enable Endpoint 0-2 in interrupts
   POLL_WRITE_BYTE(OUT1IE, 0x07); // Enable Endpoint 0-2 out interrupts
   POLL_WRITE_BYTE(CMIE,   0x07); // Enable Reset, Resume, and Suspend interrupts
   USB0XCN = 0xE0; // Enable transceiver; select full speed
   POLL_WRITE_BYTE(CLKREC, 0x80); // Enable clock recovery, single-step mode disabled
   EIE1 |= 0x02; // Enable USB0 Interrupts;Global Interrupt enable;Enable USB0 by clearing the USB Inhibit bit
   POLL_WRITE_BYTE(POWER,  0x01); // and enable suspend detection
}


// This function configures Timer2 as a 16-bit reload timer, interrupt enabled.
// Using the SYSCLK at 12MHz/8 with a 1:12 prescaler.
// Note: The Timer2 uses a 1:12 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
void Timer2_Init ()
{
   CKCON &= ~0x60; // Timer2 uses SYSCLK/12
   TMR2CN &= ~0x01;

   TMR2RL = TIMER2_RELOAD;             // Reload value to be used in Timer2
   TMR2 = TMR2RL;                      // Init the Timer2 register

   TMR2CN = 0x04;                      // Enable Timer2 in auto-reload mode
   ET2 = 1; 
}

void Port_Init(void)
{
	P0MDIN |= 0x40;// 0x40:BUSY input
	P0MDOUT = 0xcc; //0x10 : Set TX pin to push-pull	

	P1MDIN |= 0xff; 
	P1MDOUT = 0x00;
	P1 |= 0xff;//Set port latches to '1'

	P2MDOUT = 0xfb;

	P3MDIN |= 0xff;	
	P3MDOUT = 0x00;
	P3 |= 0xff;//Set port latches to '1'		  

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