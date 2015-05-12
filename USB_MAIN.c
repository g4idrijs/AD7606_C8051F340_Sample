//#include "compiler_defs.h"
#include <c8051f340.h>
#include <stddef.h>
#include "USB_MAIN.h"
#include "USB_API.h"
#include "F34x_USB_Register.h"
#include "Delay.h"
#include "C8051F340_AD7606.h"
#include "stdio.h"

sbit Led = P2^3;

U8 In_Packet[2];
U8 i;
extern U8 t;
extern U8 xdata out[2];
U8 xdata out_test[128];

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

void Oscillator_Init(void);
//void Sysclk_Init(void);
void Port_Init(void);
void Suspend_Device(void);	

void main(void)
{
   PCA0MD &= ~0x40;
   Oscillator_Init(); 
   //Sysclk_Init();
    
   USB_Clock_Start();
   USB_Init(USB_VID,USB_PID,USB_MfrStr,USB_ProductStr,USB_SerialStr,USB_MaxPower,USB_PwAttributes,USB_bcdDevice);
   USB_Int_Enable();  

   Port_Init();
   AD7606_Init();

//   for(i=0;i<128;i++)
//   {
//   	out_test[i]=i;
//   }
   
   while (1)
   {
		if(In_Packet[0]==1)
			Led=1;
		else
			Led=0;
		AD7606_Read();
		if(t==1)
		{
			Block_Write(out, 2);
			t=0;
		}
	  //Block_Write(out_test, 128);      
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

	XBR0 = 0x00;// Enable UART0
	XBR1 = 0x40;// Enable crossbar and weak pull-ups
}

void Sysclk_Init(void)
{
//   OSCICN |= 0x03;    
//   CLKMUL  = 0x00;     
//   CLKMUL |= 0x80; 
//   Delay(); 
//   CLKMUL |= 0xC0;        
//   Delay();                            
//   while(!(CLKMUL & 0x20));
//   CLKSEL  = 0x02; 
}

void Oscillator_Init()
{				
	
//	int i = 0;
//	//OSCICN = 0x83;
//    FLSCL = 0x90;
//	CLKMUL  = 0x00;
//    CLKMUL |= 0x80;
//    for (i = 0; i < 20; i++);    // Wait 5us for initialization
//    CLKMUL |= 0xC0;
//    while ((CLKMUL & 0x20) == 0);
//    //CLKSEL    = 0x03;
//    //OSCICN    = 0x83;
//	CLKSEL  = 0x00; // Select to use internal osc. 
//	//CLKSEL |= 0x00; // Select USB clock 
	
				   	
    CLKSEL = 0x00; // Select the internal osc. as the SYSCLK source
    OSCICN = 0x83; // configure internal oscillator for 12MHz / 1
	RSTSRC = 0x04; // enable missing clock detector 

	CLKMUL  = 0x00; // Select internal oscillator as input to clock multiplier   
	CLKMUL |= 0x80; // Enable clock multiplier   
	Delay(); // Delay for clock multiplier to begin   
	CLKMUL |= 0xC0; // Initialize the clock multiplier   
	Delay(); // Delay for clock multiplier to begin   
	while(!(CLKMUL & 0x20)); // Wait for multiplier to lock 
	CLKSEL  = 0x02;
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

//-----------------------------------------------------------------------------
// Delay(void)
//-----------------------------------------------------------------------------
// Used for a small pause, approximately 80 us in Full Speed,
// and 1 ms when clock is configured for Low Speed
//
//-----------------------------------------------------------------------------
void Delay (void)
{
   int x;
   for(x = 0;x < 500;x)
      x++;
}