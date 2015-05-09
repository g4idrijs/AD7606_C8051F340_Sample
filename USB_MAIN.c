//#include "compiler_defs.h"
#include <c8051f340.h>
#include <stddef.h>
#include "USB_API.h"
#include "C8051F340_AD7606.h"
#include "stdio.h"

U8 In_Packet[2]; // Last packet received from host
extern U8 Out_Packet[32];
extern U8 t;
U8 Ax[16] ={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
U8 i;

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

void Oscillator_Init();
void Port_Init(void);
void Suspend_Device(void);

void main(void)
{
   PCA0MD &= ~0x40;
   
   USB_Clock_Start();
   USB_Init(USB_VID,USB_PID,USB_MfrStr,USB_ProductStr,USB_SerialStr,USB_MaxPower,USB_PwAttributes,USB_bcdDevice);
   USB_Int_Enable();

   Oscillator_Init();
   Port_Init();

   AD7606_Init();
   
   while (1)
   {
	  AD7606_Read();
	  if(t==16)
	  {
	  	Block_Write(Out_Packet, 32);
		t=0;
	  }
	  //Block_Write(Ax, 16);      
   }
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
	XBR1 = 0x40;// Enable crossbar and weak pull-ups
}

void Oscillator_Init()
{							   	
    CLKSEL = 0x00; // Select the internal osc. as the SYSCLK source
    OSCICN = 0x83; // configure internal oscillator for 12MHz / 1
	RSTSRC = 0x04; // enable missing clock detector
}

void Suspend_Device(void)
{
   // Disable peripherals before calling USB_Suspend()
   P0MDIN = 0x00;                       // Port 0 configured as analog input
   P1MDIN = 0x00;                       // Port 1 configured as analog input
   P2MDIN = 0x00;                       // Port 2 configured as analog input
   P3MDIN = 0x00;                       // Port 3 configured as analog input

   USB_Suspend();                       // Put the device in suspend state

   // Once execution returns from USB_Suspend(), device leaves suspend state.Reenable peripherals
   P0MDIN = 0xFF;
   P1MDIN = 0x7F;                       // Port 1 pin 7 set as analog input
   P2MDIN = 0xFF;
   P3MDIN = 0x01;
}

// Example ISR for USB_API
void  USB_API_TEST_ISR(void) interrupt 17
{
   BYTE INTVAL = Get_Interrupt_Source();

   if (INTVAL & RX_COMPLETE)
   {
      Block_Read(In_Packet, 2);
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