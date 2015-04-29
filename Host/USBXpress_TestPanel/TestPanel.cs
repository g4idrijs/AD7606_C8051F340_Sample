using System;
using System.Windows.Forms;

namespace USBXpress_TestPanel
{
    public partial class TestPanel : Form
    {
        public TestPanel()
        {
            InitializeComponent();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            var IOBufSize = 16;
            var IOBuf = new Byte[IOBufSize];
            var BytesSucceed = 0;
            var BytesWriteRequest = IOBufSize - 8;
            var BytesReadRequest = IOBufSize;

            // Get information from form to write to device
            if (checkBox_LED.Checked)
            {
                IOBuf[1] = 1;
            }
            else
            {
                IOBuf[1] = 0;
            }

            // Send output data out to the board
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Write(SLUSBXpressDLL.hUSBDevice, ref IOBuf[0], BytesWriteRequest,
                ref BytesSucceed, 0);

            if ((BytesSucceed != BytesWriteRequest) || (SLUSBXpressDLL.Status != SLUSBXpressDLL.SI_SUCCESS))
            {
                MessageBox.Show("Error writing to USB. Wrote " + BytesSucceed + " of " + BytesWriteRequest +
                                " bytes. Application is aborting. Reset hardware and try again.");
                Application.Exit();
            }

            //clear out bytessucceed for the next read
            BytesSucceed = 0;

            //read data from the board
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Read(SLUSBXpressDLL.hUSBDevice, ref IOBuf[0], BytesReadRequest,
                ref BytesSucceed, 0);

            if ((BytesSucceed != BytesReadRequest) || (SLUSBXpressDLL.Status != SLUSBXpressDLL.SI_SUCCESS))
            {
                MessageBox.Show("Error writing to USB. Read " + BytesSucceed + " of " + BytesReadRequest +
                                " bytes. Application is aborting. Reset hardware and try again.");
                Application.Exit();
            }

            //take the newly received array and put it into the form
            //textBox1.Text += (IOBuf[0]+IOBuf[1] * 256).ToString() + " ";
            for (int i = 0; i < 16; i++)
            {
                textBox1.Text += IOBuf[i].ToString() + " ";
            }

        }


        private void button2_Click(object sender, EventArgs e)
        {
            timer1.Start();
        }
        private void button1_Click(object sender, EventArgs e)
        {
            timer1.Stop();
        }
        private void button_Exit_Click(object sender, EventArgs e)
        {
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Close(SLUSBXpressDLL.hUSBDevice);
            Application.Exit(); // Exit program
        }

    }
}