using System;
using System.Text;
using System.Windows.Forms;

namespace USBXpress_TestPanel
{
    public partial class SelectScreen : Form
    {
        public SelectScreen()
        {
            InitializeComponent();
        }

        private void button_Reject_Click(object sender, EventArgs e)
        {
            Application.Exit(); // Exit program
        }

        private void SelectScreen_Load(object sender, EventArgs e)
        {
            //Variables we will use when loading this form
            var DevNum = 0;
            var DevStr = new StringBuilder(SLUSBXpressDLL.SI_MAX_DEVICE_STRLEN);
            int i, iMax;

            comboBox_Device.Items.Clear();
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_GetNumDevices(ref DevNum);

            // if we find a device, obtain the name of each device
            // and add to the combo list, otherwise display the error
            // and close the application

            if (SLUSBXpressDLL.Status == SLUSBXpressDLL.SI_SUCCESS)
            {
                for (i = 0; i < DevNum; i++)
                {
                    SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_GetProductString(i, DevStr,
                        SLUSBXpressDLL.SI_RETURN_SERIAL_NUMBER);
                    comboBox_Device.Items.Insert(i, DevStr);
                }
                comboBox_Device.SelectedIndex = 0; // then set combo list to first item
            }
            else
            {
                MessageBox.Show("Error finding USB device.  Aborting application.");
                Application.Exit();
            }
        }

        private void button_Accept_Click(object sender, EventArgs e)
        {
            // when ok is clicked, set the timeouts on the device
            // and open the device
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_SetTimeouts(10000, 10000);
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Open(comboBox_Device.SelectedIndex, ref SLUSBXpressDLL.hUSBDevice);

            if (SLUSBXpressDLL.Status != SLUSBXpressDLL.SI_SUCCESS)
            {
                MessageBox.Show("Error opening device: " + comboBox_Device.Text +
                                ". Application is aborting. Reset hardware and try again.");
                Application.Exit();
            }

            var TPForm = new TestPanel();
            ActiveForm.Hide();
            TPForm.Show();
        }
    }
}