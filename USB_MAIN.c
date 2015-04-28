#include <c8051f340.h>
#include <stddef.h>
#include "USB_API.h"
#include "C8051F340_AD7606.h"

sbit Led1 = P2^2;                         // LED='1' means ON
sbit Led2 = P2^3;

BYTE Out_Packet[8] = {0,0,0,0,0,0,0,0};   // Last packet received from host

extern unsigned char Data[16]; //调用C8051F340——AD7606.c中的数组Date

/*** [BEGIN] USB Descriptor Information [BEGIN] ***/
code const UINT USB_VID = 0x10C4;
code const UINT USB_PID = 0xEA61;
code const BYTE USB_MfrStr[] = {0x1A,0x03,'S',0,'i',0,'l',0,'i',0,'c',0,'o',0,'n',0,' ',0,'L',0,'a',0,'b',0,'s',0};                       // Manufacturer String
code const BYTE USB_ProductStr[] = {0x10,0x03,'U',0,'S',0,'B',0,' ',0,'A',0,'P',0,'I',0}; // Product Desc. String
code const BYTE USB_SerialStr[] = {0x0A,0x03,'i',0,'d',0,'r',0,'i',0};
code const BYTE USB_MaxPower = 15;            // Max current = 30 mA (15 * 2)
code const BYTE USB_PwAttributes = 0x80;      // Bus-powered, remote wakeup not supported
code const UINT USB_bcdDevice = 0x0100;       // Device release number 1.00
/*** [ END ] USB Descriptor Information [ END ] ***/

void Port_Init(void);
void Suspend_Device(void);
void Initialize(void);
void Oscillator_Init();

void main(void)
{
   PCA0MD &= ~0x40;
   Oscillator_Init();
   USB_Clock_Start();
   USB_Init(USB_VID,USB_PID,USB_MfrStr,USB_ProductStr,USB_SerialStr,USB_MaxPower,USB_PwAttributes,USB_bcdDevice);

   Initialize();
   USB_Int_Enable();
   AD7606_Init();
 
   while (1)
   {
      if (Out_Packet[0] == 1) Led1 = 1;   // Update status of LED #1
      else Led1 = 0;
      if (Out_Packet[1] == 1) Led2 = 1;   // Update status of LED #2
      else Led2 = 0;

	  AD7606_Read();
	  Block_Write(Data, 16);
       
   }
}

void Port_Init(void)
{
	P0MDOUT = 0xFD;
	P0MDIN = 0x02;
	P1MDOUT = 0x00;
	P1MDIN = 0xff;
	P2MDOUT |= 0x0C;
	P3MDOUT = 0x00;
	P3MDIN = 0xff;	 

	XBR0 = 0x00;
	XBR1 = 0x40; // Enable Crossbar
}

void Oscillator_Init()
{
    int i = 0;
    FLSCL     = 0x90;
    CLKMUL    = 0x80;
    for (i = 0; i < 20; i++);    // Wait 5us for initialization
    CLKMUL    |= 0xC0;
    while ((CLKMUL & 0x20) == 0);
    CLKSEL    = 0x03;
    OSCICN    = 0x83;
}

void Suspend_Device(void)
{
   // Disable peripherals before calling USB_Suspend()
   P0MDIN = 0x00;                       // Port 0 configured as analog input
   P1MDIN = 0x00;                       // Port 1 configured as analog input
   P2MDIN = 0x00;                       // Port 2 configured as analog input
   P3MDIN = 0x00;                       // Port 3 configured as analog input

   USB_Suspend();                       // Put the device in suspend state

   // Once execution returns from USB_Suspend(), device leaves suspend state.
   // Reenable peripherals
   P0MDIN = 0xFF;
   P1MDIN = 0x7F;                       // Port 1 pin 7 set as analog input
   P2MDIN = 0xFF;
   P3MDIN = 0x01;
}

//-------------------------
// Initialize
//-------------------------
// Called when a DEV_CONFIGURED interrupt is received.
// - Enables all peripherals needed for the application
//
void Initialize(void)
{
   Port_Init();                           // Initialize crossbar and GPIO
   //Timer_Init();                          // Initialize timer2
   //Adc_Init();                            // Initialize ADC
}

// Example ISR for USB_API
void  USB_API_TEST_ISR(void) interrupt 17
{
   BYTE INTVAL = Get_Interrupt_Source();

   if (INTVAL & RX_COMPLETE)
   {
      Block_Read(Out_Packet, 8);
   }

   if (INTVAL & DEV_SUSPEND)
   {
        Suspend_Device();
   }

   if (INTVAL & DEV_CONFIGURED)
   {
      Initialize();
   }
}