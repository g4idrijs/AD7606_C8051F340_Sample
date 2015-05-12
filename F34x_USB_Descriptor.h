// Program Description:
//
// Header file for USB firmware.  Defines standard descriptor structures.

#ifndef  F34x_USB_DESCRIPTORS_H
#define  F34x_USB_DESCRIPTORS_H

//-----------------------------------------------------------------------------
// Global Definitions
//-----------------------------------------------------------------------------

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

//------------------------------------------
// Standard Device Descriptor Type Defintion
//------------------------------------------
typedef code struct
{
   BYTE bLength;                       // Size of this Descriptor in Bytes
   BYTE bDescriptorType;               // Descriptor Type (=1)
   WORD bcdUSB;                        // USB Spec Release Number in BCD
   BYTE bDeviceClass;                  // Device Class Code
   BYTE bDeviceSubClass;               // Device Subclass Code
   BYTE bDeviceProtocol;               // Device Protocol Code
   BYTE bMaxPacketSize0;               // Maximum Packet Size for EP0
   WORD idVendor;                      // Vendor ID
   WORD idProduct;                     // Product ID
   WORD bcdDevice;                     // Device Release Number in BCD
   BYTE iManufacturer;                 // Index of String Desc for Manufacturer
   BYTE iProduct;                      // Index of String Desc for Product
   BYTE iSerialNumber;                 // Index of String Desc for SerNo
   BYTE bNumConfigurations;            // Number of possible Configurations
} device_descriptor;                   // End of Device Descriptor Type

//--------------------------------------------------
// Standard Configuration Descriptor Type Definition
//--------------------------------------------------
typedef code struct
{
   BYTE bLength;                       // Size of this Descriptor in Bytes
   BYTE bDescriptorType;               // Descriptor Type (=2)
   WORD wTotalLength;                  // Total Length of Data for this Conf
   BYTE bNumInterfaces;                // # of Interfaces supported by Conf
   BYTE bConfigurationValue;           // Designator Value for *this* Conf
   BYTE iConfiguration;                // Index of String Desc for this Conf
   BYTE bmAttributes;                  // Configuration Characteristics
   BYTE bMaxPower;                     // Max. Power Consumption in Conf (*2mA)
} configuration_descriptor;            // End of Configuration Descriptor Type

//----------------------------------------------
// Standard Interface Descriptor Type Definition
//----------------------------------------------
typedef code struct
{
   BYTE bLength;                       // Size of this Descriptor in Bytes
   BYTE bDescriptorType;               // Descriptor Type (=4)
   BYTE bInterfaceNumber;              // Number of *this* Interface (0..)
   BYTE bAlternateSetting;             // Alternative for this Interface
   BYTE bNumEndpoints;                 // No of EPs used by this IF (excl. EP0)
   BYTE bInterfaceClass;               // Interface Class Code
   BYTE bInterfaceSubClass;            // Interface Subclass Code
   BYTE bInterfaceProtocol;            // Interface Protocol Code
   BYTE iInterface;                    // Index of String Desc for Interface
} interface_descriptor;                // End of Interface Descriptor Type

//---------------------------------------------
// Standard Endpoint Descriptor Type Definition
//---------------------------------------------
typedef code struct
{
   BYTE bLength;                       // Size of this Descriptor in Bytes
   BYTE bDescriptorType;               // Descriptor Type (=5)
   BYTE bEndpointAddress;              // Endpoint Address (Number + Direction)
   BYTE bmAttributes;                  // Endpoint Attributes (Transfer Type)
   WORD wMaxPacketSize;                // Max. Endpoint Packet Size
   BYTE bInterval;                     // Polling Interval (Interrupt) ms
} endpoint_descriptor;                 // End of Endpoint Descriptor Type


//-----------------------------
// Setup Packet Type Definition
//-----------------------------
typedef struct
{
   BYTE bmRequestType;                 // Request recipient, type, direction
   BYTE bRequest;                      // Specific standard request number
   WORD wValue;                        // varies according to request
   WORD wIndex;                        // varies according to request
   WORD wLength;                       // Number of bytes to transfer
} setup_buffer;                        // End of Setup Packet Type

#endif                                 // F34x_USB_DESCRIPTORS_H

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------