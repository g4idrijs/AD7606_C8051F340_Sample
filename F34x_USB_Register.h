//-----------------------------------------------------------------------------
// F34x_USB_Register.h
//-----------------------------------------------------------------------------
// Copyright 2005 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Header file for USB firmware. Includes all USB core register
// addresses, register access macros, and register bit masks.
//
//
// How To Test:    See Readme.txt
//
//
// FID:            34X000021
// Target:         C8051F34x
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
//                 Silicon Laboratories IDE version 2.6
// Command Line:   See Readme.txt
// Project Name:   F34x_USB_Interrupt
//
//
// Release 1.0
//    -Initial Revision (GP)
//    -22 NOV 2005
//    -Ported from 'F320_USB_Bulk
//

#ifndef  F34x_USB_REGISTER_H
#define  F34x_USB_REGISTER_H

// BYTE type definition
#ifndef _BYTE_DEF_
#define _BYTE_DEF_
typedef unsigned char BYTE;
#endif   // _BYTE_DEF_

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

// USB Core Registers
#define  BASE     0x00
#define  FADDR    BASE
#define  POWER    BASE + 0x01
#define  IN1INT   BASE + 0x02
#define  OUT1INT  BASE + 0x04
#define  CMINT    BASE + 0x06
#define  IN1IE    BASE + 0x07
#define  OUT1IE   BASE + 0x09
#define  CMIE     BASE + 0x0B
#define  FRAMEL   BASE + 0x0C
#define  FRAMEH   BASE + 0x0D
#define  INDEX    BASE + 0x0E
#define  CLKREC   BASE + 0x0F
#define  E0CSR    BASE + 0x11
#define  EINCSR1  BASE + 0x11
#define  EINCSR2  BASE + 0x12
#define  EOUTCSR1 BASE + 0x14
#define  EOUTCSR2 BASE + 0x15
#define  E0CNT    BASE + 0x16
#define  EOUTCNTL BASE + 0x16
#define  EOUTCNTH BASE + 0x17
#define  FIFO_EP0 BASE + 0x20
#define  FIFO_EP1 BASE + 0x21
#define  FIFO_EP2 BASE + 0x22
#define  FIFO_EP3 BASE + 0x23

// USB Core Register Bits

// POWER
#define  rbISOUD        0x80
#define  rbSPEED        0x40
#define  rbUSBRST       0x08
#define  rbRESUME       0x04
#define  rbSUSMD        0x02
#define  rbSUSEN        0x01

// IN1INT
#define  rbIN3          0x08
#define  rbIN2          0x04
#define  rbIN1          0x02
#define  rbEP0          0x01

// OUT1INT
#define  rbOUT3         0x08
#define  rbOUT2         0x04
#define  rbOUT1         0x02

// CMINT
#define  rbSOF          0x08
#define  rbRSTINT       0x04
#define  rbRSUINT       0x02
#define  rbSUSINT       0x01

// IN1IE
#define  rbIN3E         0x08
#define  rbIN2E         0x04
#define  rbIN1E         0x02
#define  rbEP0E         0x01

// OUT1IE
#define  rbOUT3E        0x08
#define  rbOUT2E        0x04
#define  rbOUT1E        0x02

// CMIE
#define  rbSOFE         0x08
#define  rbRSTINTE      0x04
#define  rbRSUINTE      0x02
#define  rbSUSINTE      0x01

// E0CSR
#define  rbSSUEND       0x80
#define  rbSOPRDY       0x40
#define  rbSDSTL        0x20
#define  rbSUEND        0x10
#define  rbDATAEND      0x08
#define  rbSTSTL        0x04
#define  rbINPRDY       0x02
#define  rbOPRDY        0x01

// EINCSR1
#define  rbInCLRDT      0x40
#define  rbInSTSTL      0x20
#define  rbInSDSTL      0x10
#define  rbInFLUSH      0x08
#define  rbInUNDRUN     0x04
#define  rbInFIFONE     0x02
#define  rbInINPRDY     0x01

// EINCSR2
#define  rbInDBIEN      0x80
#define  rbInISO        0x40
#define  rbInDIRSEL     0x20
#define  rbInFCDT       0x08
#define  rbInSPLIT      0x04

// EOUTCSR1
#define  rbOutCLRDT     0x80
#define  rbOutSTSTL     0x40
#define  rbOutSDSTL     0x20
#define  rbOutFLUSH     0x10
#define  rbOutDATERR    0x08
#define  rbOutOVRUN     0x04
#define  rbOutFIFOFUL   0x02
#define  rbOutOPRDY     0x01

// EOUTCSR2
#define  rbOutDBOEN     0x80
#define  rbOutISO       0x40

//-----------------------------------------------------------------------------
// Register Read/Write Macros
//-----------------------------------------------------------------------------

// These first two macros do not poll for busy, and can be used to increase
// program speed where necessary, but should never be used for successive
// reads or writes.

#define READ_BYTE(addr, target) { USB0ADR = (0x80 | addr); \
                                  while(USB0ADR & 0x80);   \
                                  target = USB0DAT; }

#define WRITE_BYTE(addr, data)  { USB0ADR = (addr);         \
                                  USB0DAT = data; }

// These two macros are polling versions of the above macros, and can be
// used for successive reads writes without taking the chance that the
// Interface Engine is busy from the last register access.

#define POLL_READ_BYTE(addr, target) { while(USB0ADR & 0x80); \
                                       READ_BYTE(addr, target); }

#define POLL_WRITE_BYTE(addr, data) { while(USB0ADR & 0x80);  \
                                      WRITE_BYTE(addr, data); }

#endif                                 // F34x_USB_REGISTER_H

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------