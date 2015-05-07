//#include "compiler_defs.h"
#include <c8051f340.h>
#include <stddef.h>
#include "USB_API.h"
#include "C8051F340_AD7606.h"
#include "stdio.h"
																					
sbit Led = P2^3;




// Constants Definitions
#define  NUM_STG_PAGES  20 // Total number of flash pages to be used for file storage
#define  MAX_BLOCK_SIZE_READ     64 // Use the maximum read block size of 64 bytes
#define  MAX_BLOCK_SIZE_WRITE    4096   // Use the maximum write block size of 4096 bytes
#define  FLASH_PAGE_SIZE         512    //  Size of each flash page
#define  BLOCKS_PR_PAGE  FLASH_PAGE_SIZE/MAX_BLOCK_SIZE_READ  // 512/64 = 8
#define  MAX_NUM_BYTES   FLASH_PAGE_SIZE*NUM_STG_PAGES
#define  MAX_NUM_BLOCKS  BLOCKS_PR_PAGE*NUM_STG_PAGES
// Message Types
#define  READ_MSG          0x00  // Message types for communication with host
#define  WRITE_MSG         0x01
#define  SIZE_MSG          0x02
#define  DELAYED_READ_MSG  0x05
// Machine States
#define  ST_WAIT_DEV    0x01  // Wait for application to open a device instance
#define  ST_IDLE_DEV    0x02  // Device is open, wait for Setup Message from host
#define  ST_RX_SETUP    0x04  // Received Setup Message, decode and wait for data
#define  ST_RX_FILE     0x08  // Receive file data from host
#define  ST_TX_FILE     0x10  // Transmit file data to host
#define  ST_TX_ACK      0x20  // Transmit ACK 0xFF back to host after every 8 packets
#define  ST_ERROR       0x80  // Error state
typedef struct {        // Structure definition of a block of data
   unsigned char  Piece[MAX_BLOCK_SIZE_READ];
}  BLOCK;
typedef struct {        // Structure definition of a flash memory page
   U8  FlashPage[FLASH_PAGE_SIZE];
}  PAGE;

SEGMENT_VARIABLE(TempStorage[BLOCKS_PR_PAGE], BLOCK, SEG_XDATA); //Temporary storage of between flash writes

SEGMENT_VARIABLE_SEGMENT_POINTER(PageIndices[20], U8, SEG_CODE, SEG_DATA);

SEGMENT_VARIABLE(BytesToRead, U16, SEG_DATA); // Total number of bytes to read from host
SEGMENT_VARIABLE(WriteStageLength, U16, SEG_DATA); //  Current write transfer stage length
SEGMENT_VARIABLE(ReadStageLength, U16, SEG_DATA);  //  Current read transfer stage length
SEGMENT_VARIABLE(Buffer[3], U8, SEG_DATA);   // Buffer for Setup messages
SEGMENT_VARIABLE(NumBytes, U16, SEG_DATA);   // Number of Blocks for this transfer
SEGMENT_VARIABLE(NumBlocks, U8, SEG_DATA);
SEGMENT_VARIABLE(BytesRead, U16, SEG_DATA);  // Number of Bytes Read
SEGMENT_VARIABLE(M_State, U8, SEG_DATA);     // Current Machine State
SEGMENT_VARIABLE(BytesWrote, U16, SEG_DATA); // Number of Bytes Written
SEGMENT_VARIABLE(BlockIndex, U8, SEG_DATA);  // Index of Current Block in Page
SEGMENT_VARIABLE(PageIndex, U8, SEG_DATA);   // Index of Current Page in File
SEGMENT_VARIABLE(BlocksWrote, U8, SEG_DATA); // Total Number of Blocks Written
#if defined __C51__
   SEGMENT_VARIABLE(ReadIndex, U8*, SEG_DATA);
#elif defined SDCC
   SEGMENT_VARIABLE_SEGMENT_POINTER(ReadIndex, U8, SEG_CODE, SEG_DATA);
#endif
SEGMENT_VARIABLE(BytesToWrite, U16, SEG_DATA);

LOCATED_VARIABLE_NO_INIT(LengthFile[3], U8, SEG_CODE, 0x2000);
   // {Length(Low Byte), Length(High Byte), Number of Blocks}



U8 In_Packet[2]; // Last packet received from host
extern U8 Out_Packet[16];
//U8 Ax[32] ={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};
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
      if (In_Packet[0] == 1) Led = 1;
      else Led = 0;
	  AD7606_Read();
	  Block_Write(Out_Packet, 16);
	  //Block_Write(Ax, 32);      
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