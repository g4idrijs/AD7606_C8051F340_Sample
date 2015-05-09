using System;
using System.Runtime.InteropServices;
using System.Text;

namespace USBXpress_TestPanel
{
    internal class SLUSBXpressDLL
    {
        // Return Codes
        public const Int32 SI_SUCCESS = 0x00;
        public const Int32 SI_DEVICE_NOT_FOUND = 0xFF;
        public const Int32 SI_INVALID_HANDLE = 0x01;
        public const Int32 SI_READ_ERROR = 0x02;
        public const Int32 SI_RX_QUEUE_NOT_READY = 0x03;
        public const Int32 SI_WRITE_ERROR = 0x04;
        public const Int32 SI_RESET_ERROR = 0x05;
        public const Int32 SI_INVALID_PARAMETER = 0x06;
        public const Int32 SI_INVALID_REQUEST_LENGTH = 0x07;
        public const Int32 SI_DEVICE_IO_FAILED = 0x08;
        public const Int32 SI_INVALID_BAUDRATE = 0x09;
        public const Int32 SI_FUNCTION_NOT_SUPPORTED = 0x0a;
        public const Int32 SI_GLOBAL_DATA_ERROR = 0x0b;
        public const Int32 SI_SYSTEM_ERROR_CODE = 0x0c;
        public const Int32 SI_READ_TIMED_OUT = 0x0d;
        public const Int32 SI_WRITE_TIMED_OUT = 0x0e;
        public const Int32 SI_IO_PENDING = 0x0f;
        // GetProductString() function flags
        public const Int32 SI_RETURN_SERIAL_NUMBER = 0x00;
        public const Int32 SI_RETURN_DESCRIPTION = 0x01;
        public const Int32 SI_RETURN_LINK_NAME = 0x02;
        public const Int32 SI_RETURN_VID = 0x03;
        public const Int32 SI_RETURN_PID = 0x04;
        // RX Queue status flags
        public const Int32 SI_RX_NO_OVERRUN = 0x00;
        public const Int32 SI_RX_EMPTY = 0x00;
        public const Int32 SI_RX_OVERRUN = 0x01;
        public const Int32 SI_RX_READY = 0x02;
        // Buffer size limits
        public const Int32 SI_MAX_DEVICE_STRLEN = 256;
        public const Int32 SI_MAX_READ_SIZE = 4096*1;
        public const Int32 SI_MAX_WRITE_SIZE = 4096;
        // Input and Output pin Characteristics
        public const Int32 SI_HELD_INACTIVE = 0x00;
        public const Int32 SI_HELD_ACTIVE = 0x01;
        public const Int32 SI_FIRMWARE_CONTROLLED = 0x02;
        public const Int32 SI_RECEIVE_FLOW_CONTROL = 0x02;
        public const Int32 SI_TRANSMIT_ACTIVE_SIGNAL = 0x03;
        public const Int32 SI_STATUS_INPUT = 0x00;
        public const Int32 SI_HANDSHAKE_LINE = 0x01;
        // Mask and Latch value bit definitions
        public const Int32 SI_GPIO_0 = 0x01;
        public const Int32 SI_GPIO_1 = 0x02;
        public const Int32 SI_GPIO_2 = 0x04;
        public const Int32 SI_GPIO_3 = 0x08;
        // GetDeviceVersion() return codes
        public const Int32 SI_CP2101_VERSION = 0x01;
        public const Int32 SI_CP2102_VERSION = 0x02;
        public const Int32 SI_CP2103_VERSION = 0x03;
        public static Int32 Status;
        public static UInt32 hUSBDevice;

        [DllImport("SiUSBXp.dll")]
        public static extern int SI_GetNumDevices
            (ref Int32 lpdwNumDevices);

        [DllImport("SiUSBXp.dll")]
        public static extern int SI_GetProductString
            (Int32 dwDeviceNum,
                StringBuilder lpvDeviceString,
                Int32 dwFlags);

        [DllImport("SiUSBXp.dll")]
        public static extern int SI_Open
            (Int32 dwDevice,
                ref UInt32 cyHandle);

        [DllImport("SiUSBXp.dll")]
        public static extern int SI_Close
            (UInt32 cyHandle);

        [DllImport("SiUSBXp.dll")]
        public static extern int SI_Read
            (UInt32 cyHandle,
                ref Byte lpBuffer,
                Int32 dwBytesToRead,
                ref Int32 lpdwBytesReturned,
                Int32 lpOverlapped);

        [DllImport("SiUSBXp.dll")]
        public static extern int SI_Write
            (UInt32 cyHandle,
                ref Byte lpBuffer,
                Int32 dwBytesToWrite,
                ref Int32 lpdwBytesWritten,
                Int32 lpOverlapped);

        [DllImport("SiUSBXp.dll")]
        public static extern int SI_SetTimeouts
            (Int32 dwReadTimeout,
                Int32 dwWriteTimeout);

        [DllImport("SiUSBXp.dll")]
        public static extern int SI_GetTimeouts
            (ref Int32 dwReadTimeout,
                ref Int32 dwWriteTimeout);

        [DllImport("SiUSBXp.dll")]
        public static extern int SI_CheckRXQueue
            (UInt32 cyHandle,
                ref UInt32 lpdwNumBytesInQueue,
                ref UInt32 lpdwQueueStatus);
    }
}