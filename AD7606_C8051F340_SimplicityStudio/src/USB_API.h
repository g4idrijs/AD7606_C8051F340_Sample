#ifndef  _USB_API_H_
#define  _USB_API_H_

// UINT type definition
#ifndef _UINT_DEF_
#define _UINT_DEF_
typedef unsigned int UINT;
#endif  /* _UINT_DEF_ */

// BYTE type definition
#ifndef _BYTE_DEF_
#define _BYTE_DEF_
typedef unsigned char BYTE;
#endif   /* _BYTE_DEF_ */

// Get_Interrupt_Source() return value bit masks
// Note: More than one bit can be set at the same time.
#define	USB_RESET		0x01  // USB Reset Interrupt has occurred
#define	TX_COMPLETE		0x02  // Transmit Complete Interrupt has occurred
#define	RX_COMPLETE		0x04  // Receive Complete Interrupt has occurred
#define	FIFO_PURGE		0x08  // Command received (and serviced) from the host 
                              // to purge the USB buffers
#define	DEVICE_OPEN		0x10  // Device Instance Opened on host side
#define	DEVICE_CLOSE	0x20  // Device Instance Closed on host side
#define	DEV_CONFIGURED	0x40  // Device has entered configured state
#define	DEV_SUSPEND		0x80  // USB suspend signaling present on bus

// Function prototypes
void  USB_Clock_Start(void) large;
void  USB_Init(UINT,UINT,BYTE*,BYTE*,BYTE*,BYTE,BYTE,UINT) large;        
UINT	Block_Write(BYTE*, UINT) large;
BYTE	Block_Read(BYTE*, BYTE) large;
BYTE	Get_Interrupt_Source(void) large;
void	USB_Int_Enable(void) large;
void	USB_Int_Disable(void) large;
void	USB_Disable(void) large;
void	USB_Suspend(void) large;
UINT  USB_Get_Library_Version(void) large;

#endif  /* _USB_API	_H_ */