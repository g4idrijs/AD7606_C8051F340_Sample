/// USBXpress Test Panel Application
/// 
/// Author: BD
/// Date: 20 MAY 2007
/// Language: Visual C#.NET 2005 Express Edition
/// 
/// This works together with the USBXpress Test Panel firmware to show a simple example
/// of interfacing to the USBXpress .DLL

using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace USBXpress_TestPanel
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new SelectScreen());
        }
    }
}