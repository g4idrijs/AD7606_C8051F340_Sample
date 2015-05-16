#ifndef  F34x_USB_MAIN_H
#define  F34x_USB_MAIN_H

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK                   12000000    // SYSCLK frequency in Hz

// USB clock selections (SFR CLKSEL)
#define USB_4X_CLOCK             0x00        // Select 4x clock multiplier,
                                             // for USB Full Speed
#define USB_INT_OSC_DIV_2        0x10        // See Oscillators in Datasheet
#define USB_EXT_OSC              0x20
#define USB_EXT_OSC_DIV_2        0x30
#define USB_EXT_OSC_DIV_3        0x40
#define USB_EXT_OSC_DIV_4        0x50

// System clock selections (SFR CLKSEL)
#define SYS_INT_OSC              0x00        // Select to use internal osc.
#define SYS_EXT_OSC              0x01        // Select to use external osc.
#define SYS_4X_DIV_2             0x02

// BYTE type definition
#ifndef _BYTE_DEF_
#define _BYTE_DEF_
typedef unsigned char BYTE;
#endif   // _BYTE_DEF_

// WORD type definition, for KEIL Compiler
#ifndef _WORD_DEF_                     // Compiler Specific, for Little Endian
#define _WORD_DEF_
typedef union {unsigned int i; unsigned char c[2];} WORD;

// All words sent to and received from the host are little endian, this is
// switched by software when necessary.  These sections of code have been
// marked with "Compiler Specific" as above for easier modification

#define LSB 1
#define MSB 0

#endif   // _WORD_DEF_

#endif                                 // F340_USB_Interrupt