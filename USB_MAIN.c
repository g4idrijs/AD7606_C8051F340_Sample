//#include "compiler_defs.h"
#include <c8051f340.h>
#include <stddef.h>
#include <stdio.h>
#include "USB_MAIN.h"
#include "USB_API.h"
#include "F34x_USB_Register.h"
#include "Delay.h"
#include "C8051F340_AD7606.h"
#include "stdio.h"

sbit Led = P2^3;

U8 In_Packet[2];

extern U8 xdata out1[256];
extern U8 xdata out2[256];
extern U8 xdata out3[256];
extern U8 xdata out4[256];
extern U8 xdata out5[256];
extern U8 xdata out6[256];
extern U8 xdata out7[256];
extern U8 xdata out8[256];
//extern U8 xdata out[2048];

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

void Sysclk_Init(void);
void Oscillator_Init(void);
void USB0_Init(void);
void Port_Init(void);
void Suspend_Device(void);	

void main(void)
{
   PCA0MD &= ~0x40;
   Sysclk_Init(); 
   //Oscillator_Init();
   Port_Init();
   USB0_Init(); 
   //USB_Clock_Start();
   USB_Init(USB_VID,USB_PID,USB_MfrStr,USB_ProductStr,USB_SerialStr,USB_MaxPower,USB_PwAttributes,USB_bcdDevice);   

   AD7606_Init();
   USB_Int_Enable();
   
   while (1)
   {
		if(In_Packet[0]==1)
			Led=1;
		else
			Led=0;

		AD7606_ContinuesRead();
		Block_Write(out1, 256);
		Block_Write(out2, 256);
		Block_Write(out3, 256);
		Block_Write(out4, 256);
		Block_Write(out5, 256);
		Block_Write(out6, 256);
		Block_Write(out7, 256);
		Block_Write(out8, 256);
//		Block_Write(out, 2048);      
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
	CLKSEL = SYS_4X_DIV_2; 
}

void Oscillator_Init()
{							   	
    CLKSEL = 0x00; // Select the internal osc. as the SYSCLK source
    OSCICN = 0x83; // configure internal oscillator for 12MHz / 1
	RSTSRC = 0x04; // enable missing clock detector
}

void USB0_Init(void)
{
   POLL_WRITE_BYTE(POWER,  0x08);          // Force Asynchronous USB Reset
   POLL_WRITE_BYTE(IN1IE,  0x07);          // Enable Endpoint 0-2 in interrupts
   POLL_WRITE_BYTE(OUT1IE, 0x07);          // Enable Endpoint 0-2 out interrupts
   POLL_WRITE_BYTE(CMIE,   0x07);          // Enable Reset, Resume, and Suspend interrupts

   USB0XCN = 0xE0;                         // Enable transceiver; select full speed
   POLL_WRITE_BYTE(CLKREC, 0x80);          // Enable clock recovery, single-step mode disabled

   EIE1 |= 0x02; // Enable USB0 Interrupts;Global Interrupt enable;Enable USB0 by clearing the USB Inhibit bit
   POLL_WRITE_BYTE(POWER,  0x01); // and enable suspend detection
}

void Port_Init(void)
{
	P0MDIN |= 0x40;// Port 0 pin 3 set as BUSY input
	P0MDOUT = 0xcf; //0x10 : Set TX pin to push-pull	

	P1MDIN |= 0xff; 
	P1MDOUT = 0x00;
	P1 |= 0xff;//Set port latches to '1'

	P2MDOUT = 0xf8;
	//P2 |= 0x00;

	P3MDIN |= 0xff;	
	P3MDOUT = 0x00;
	P3 |= 0xff;//Set port latches to '1'		  

	XBR0 = 0x01;// Enable UART0
	XBR1 = 0x40;// Route CEX0 to P0.0,Enable crossbar and weak pull-ups
}

void Suspend_Device(void)
{
   // Disable peripherals before calling USB_Suspend()
//   P0MDIN = 0x00;                       // Port 0 configured as analog input
//   P1MDIN = 0x00;                       // Port 1 configured as analog input
//   P2MDIN = 0x00;                       // Port 2 configured as analog input
//   P3MDIN = 0x00;                       // Port 3 configured as analog input

   USB_Suspend();                       // Put the device in suspend state

   // Once execution returns from USB_Suspend(), device leaves suspend state.Reenable peripherals
//   P0MDIN = 0xFF;
//   P1MDIN = 0xFF;                       // Port 1 pin 7 set as analog input
//   P2MDIN = 0xFF;
//   P3MDIN = 0xFF;
}

// Example ISR for USB_API
void  USB_API_TEST_ISR(void) interrupt 17
{
   BYTE INTVAL = Get_Interrupt_Source();

   if (INTVAL & RX_COMPLETE)
   {
      Block_Read(In_Packet, 1);
   }

   if (INTVAL & DEV_SUSPEND)
   {
        Suspend_Device();
   }

   if (INTVAL & DEV_CONFIGURED)
   {
		Oscillator_Init();
		Port_Init();
   }
}

void Delay (void)
{
   int x;
   for(x = 0;x < 500;x)
      x++;
}