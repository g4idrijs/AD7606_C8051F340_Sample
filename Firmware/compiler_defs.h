//-----------------------------------------------------------------------------
// compiler_defs.h
//-----------------------------------------------------------------------------
// Portions of this file are copyright Maarten Brock
// http://sdcc.sourceforge.net
// Portions of this file are copyright 2010, Silicon Laboratories, Inc.
// http://www.silabs.com
//
// GNU LGPL boilerplate:
/** This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
  *
  * In other words, you are welcome to use, share and improve this program.
  * You are forbidden to forbid anyone else to use, share and improve
  * what you give them. Help stamp out software-hoarding!
**/
// Program Description:
//
// **Important Note**: This header file should be included before including
// a device-specific header file such as C8051F300_defs.h.
//
// Macro definitions to accomodate 8051 compiler differences in specifying
// special function registers and other 8051-specific features such as NOP
// generation, and locating variables in memory-specific segments.  The
// compilers are identified by their unique predefined macros. See also:
// http://predef.sourceforge.net/precomp.html
//
// SBIT and SFR define special bit and special function registers at the given
// address. SFR16 and SFR32 define sfr combinations at adjacent addresses in
// little-endian format. SFR16E and SFR32E define sfr combinations without
// prerequisite byte order or adjacency. None of these multi-byte sfr
// combinations will guarantee the order in which they are accessed when read
// or written.
//
// SFR16X and SFR32X for 16 bit and 32 bit xdata registers are not defined
// to avoid portability issues because of compiler endianness.
//
// Example:
// // my_mcu.c: main 'c' file for my mcu
// #include <compiler_defs.h>  // this file
// #include <C8051xxxx_defs.h> // SFR definitions for specific MCU target
//
// SBIT  (P0_1, 0x80, 1);      // Port 0 pin 1
// SFR   (P0, 0x80);           // Port 0
// SFRX  (CPUCS, 0xE600);      // Cypress FX2 Control and Status register in
//                             // xdata memory at 0xE600
// SFR16 (TMR2, 0xCC);         // Timer 2, lsb at 0xCC, msb at 0xCD
// SFR16E(TMR0, 0x8C8A);       // Timer 0, lsb at 0x8A, msb at 0x8C
// SFR32 (MAC0ACC, 0x93);      // SiLabs C8051F120 32 bits MAC0 Accumulator,
//                             // lsb at 0x93, msb at 0x96
// SFR32E(SUMR, 0xE5E4E3E2);   // TI MSC1210 SUMR 32 bits Summation register,
//                             // lsb at 0xE2, msb at 0xE5
//
// Target:         C8051xxxx
// Tool chain:     Generic
// Command Line:   None
// 
// Release 2.6 - 14 DEC 2012 (GO)
// 	  -Added define for deprecated SDCC keyword 'at'
// Release 2.5 - 12 SEP 2012 (TP)
//    -Added defines for deprecated SDCC keywords bit and code
// Release 2.4 - 27 AUG 2012 (TP)
//    -Added defines for deprecated SDCC keywords interrupt, _asm, and _endasm
// Release 2.3 - 27 MAY 2010 (DM)
//    -Removed 'LOCATED_VARIABLE' pragma from Keil because it is not supported
// Release 2.2 - 06 APR 2010 (ES)
//    -Removed 'PATHINCLUDE' pragma from Raisonance section
// Release 2.1 - 16 JUL 2009 (ES)
//    -Added SEGMENT_POINTER macro definitions for SDCC, Keil, and Raisonance
//    -Added LOCATED_VARIABLE_NO_INIT macro definitions for Raisonance
// Release 2.0 - 19 MAY 2009 (ES)
//    -Added LOCATED_VARIABLE_NO_INIT macro definitions for SDCC and Keil
// Release 1.9 - 23 OCT 2008 (ES)
//    -Updated Hi-Tech INTERRUPT and INTERRUPT_USING macro definitions
//    -Added SFR16 macro defintion for Hi-Tech
// Release 1.8 - 31 JUL 2008 (ES)
//    -Added INTERRUPT_USING and FUNCTION_USING macro's
//    -Added macro's for IAR
//    -Corrected Union definitions for Hi-Tech and added SFR16 macro defintion
// Release 1.7 - 11 SEP 2007 (BW)
//    -Added support for Raisonance EVAL 03.03.42 and Tasking Eval 7.2r1
// Release 1.6 - 27 AUG 2007 (BW)
//    -Updated copyright notice per agreement with Maartin Brock
//    -Added SDCC 2.7.0 "compiler.h" bug fixes
//    -Added memory segment defines (SEG_XDATA, for example)
// Release 1.5 - 24 AUG 2007 (BW)
//    -Added support for NOP () macro
//    -Added support for Hi-Tech ver 9.01
// Release 1.4 - 07 AUG 2007 (PKC)
//    -Removed FID and fixed formatting.
// Release 1.3 - 30 SEP 2007 (TP)
//    -Added INTERRUPT_PROTO_USING to properly support ISR context switching
//     under SDCC.
// Release 1.2 - (BW)
//    -Added support for U8,U16,U32,S8,S16,S32,UU16,UU32 data types
// Release 1.1 - (BW)
//    -Added support for INTERRUPT, INTERRUPT_USING, INTERRUPT_PROTO,
//     SEGMENT_VARIABLE, VARIABLE_SEGMENT_POINTER,
//     SEGMENT_VARIABLE_SEGMENT_POINTER, and LOCATED_VARIABLE
// Release 1.0 - 29 SEP 2006 (PKC)
//    -Initial revision

//-----------------------------------------------------------------------------
// Header File Preprocessor Directive
// Macro definitions
//-----------------------------------------------------------------------------

// Keil C51

//#error Keil C51 detected.

# define SEG_GENERIC
# define SEG_FAR   xdata
# define SEG_DATA  data
# define SEG_NEAR  data
# define SEG_IDATA idata
# define SEG_XDATA xdata
# define SEG_PDATA pdata
# define SEG_CODE  code
# define SEG_BDATA bdata

# define SBIT(name, addr, bit)  sbit  name = addr^bit
# define SFR(name, addr)        sfr   name = addr
# define SFR16(name, addr)      sfr16 name = addr
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr)  /* not supported */
# define SFR32E(name, fulladdr) /* not supported */

# define INTERRUPT(name, vector) void name (void) interrupt vector
# define INTERRUPT_USING(name, vector, regnum) void name (void) interrupt vector using regnum
# define INTERRUPT_PROTO(name, vector) void name (void)
# define INTERRUPT_PROTO_USING(name, vector, regnum) void name (void)

# define FUNCTION_USING(name, return_value, parameter, regnum) return_value name (parameter) using regnum
# define FUNCTION_PROTO_USING(name, return_value, parameter, regnum) return_value name (parameter)
// Note: Parameter must be either 'void' or include a variable type and name. (Ex: char temp_variable)

# define SEGMENT_VARIABLE(name, vartype, locsegment) vartype locsegment name
# define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) vartype targsegment * name
# define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) vartype targsegment * locsegment name
# define SEGMENT_POINTER(name, vartype, locsegment) vartype * locsegment name
# define LOCATED_VARIABLE_NO_INIT(name, vartype, locsegment, addr) vartype locsegment name _at_ addr

// used with UU16
# define LSB 1
# define MSB 0

// used with UU32 (b0 is least-significant byte)
# define b0 3
# define b1 2
# define b2 1
# define b3 0

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;

typedef signed char S8;
typedef signed int S16;
typedef signed long S32;

typedef union UU16
{
   U16 U16;
   S16 S16;
   U8 U8[2];
   S8 S8[2];
} UU16;

typedef union UU32
{
   U32 U32;
   S32 S32;
   UU16 UU16[2];
   U16 U16[2];
   S16 S16[2];
   U8 U8[4];
   S8 S8[4];
} UU32;

// NOP () macro support
extern void _nop_ (void);
#define NOP() _nop_()
//-----------------------------------------------------------------------------