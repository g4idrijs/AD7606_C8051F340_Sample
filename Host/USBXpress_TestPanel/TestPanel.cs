using System;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Timers;
using ZedGraph;

namespace USBXpress_TestPanel
{
    public partial class TestPanel : Form
    {
        delegate void delegate_process1();
        delegate void delegate_process2();

        private static readonly int N = 1024*64;
        private static readonly int InBufSize = 1024*64;
        private readonly int BytesReadRequest = InBufSize;
        private readonly Byte[] InBuf = new Byte[InBufSize];
        private static readonly int skip = 2;
        private readonly double[] ReceivedValue1 = new double[N/skip];
        private int BytesSucceed;
        private Complex[] output_complex = new Complex[N/skip/2];
        System.Timers.Timer timer1;
        private double time;
        private int Count;
        public TestPanel()
        {
            InitializeComponent();
            timer1 = new System.Timers.Timer();
            //Control.CheckForIllegalCrossThreadCalls = false;

            zedGraphControl1.GraphPane.Fill = new Fill(Color.AliceBlue);
            zedGraphControl1.GraphPane.Chart.Fill = new Fill(Color.Black);
            zedGraphControl2.GraphPane.Fill = new Fill(Color.AliceBlue);
            zedGraphControl2.GraphPane.Chart.Fill = new Fill(Color.Black);
            zedGraphControl1.GraphPane.Title.IsVisible = false;
            zedGraphControl1.GraphPane.XAxis.Title.IsVisible = false;
            zedGraphControl1.GraphPane.YAxis.Title.IsVisible = false;
            zedGraphControl1.GraphPane.XAxis.IsVisible = true;
            zedGraphControl1.GraphPane.YAxis.IsVisible = true;
            zedGraphControl2.GraphPane.Title.IsVisible = false;
            zedGraphControl2.GraphPane.XAxis.Title.IsVisible = false;
            zedGraphControl2.GraphPane.YAxis.Title.IsVisible = false;
            zedGraphControl2.GraphPane.XAxis.IsVisible = true;
            zedGraphControl2.GraphPane.YAxis.IsVisible = true;

            zedGraphControl1.GraphPane.Title.Text = "";
            zedGraphControl1.GraphPane.YAxis.Title.Text = "采样值";
            zedGraphControl1.GraphPane.XAxis.Title.IsVisible = false;
            zedGraphControl1.GraphPane.YAxis.Scale.Align = AlignP.Inside;
            zedGraphControl1.GraphPane.YAxis.Scale.FontSpec.FontColor = Color.Black;
            zedGraphControl1.GraphPane.YAxis.MajorGrid.IsZeroLine = false;
            zedGraphControl1.GraphPane.YAxis.Scale.Align = AlignP.Inside;
            zedGraphControl1.GraphPane.XAxis.IsVisible = false;
            zedGraphControl1.GraphPane.YAxis.IsVisible = false;
            zedGraphControl1.GraphPane.XAxis.Scale.Max = InBufSize / skip / 16;
            zedGraphControl2.GraphPane.Title.Text = "";
            zedGraphControl2.GraphPane.YAxis.Title.Text = "fft";
            zedGraphControl2.GraphPane.XAxis.Title.IsVisible = false;
            zedGraphControl2.GraphPane.YAxis.Scale.Align = AlignP.Inside;
            zedGraphControl2.GraphPane.YAxis.Scale.FontSpec.FontColor = Color.Black;
            zedGraphControl2.GraphPane.YAxis.MajorGrid.IsZeroLine = false;
            zedGraphControl2.GraphPane.YAxis.Scale.Align = AlignP.Inside;
            zedGraphControl2.GraphPane.XAxis.Scale.Max = InBufSize / skip / 2;
            zedGraphControl2.GraphPane.XAxis.IsVisible = false;
            zedGraphControl2.GraphPane.YAxis.IsVisible = false;

            label_ConnectState.Text = "设备状态：未连接";
        }

        private void TestPanel_Load(object sender, EventArgs e)
        {
            var DevNum = 0;
            var DevStr = new StringBuilder(SLUSBXpressDLL.SI_MAX_DEVICE_STRLEN);

            comboBox_Device.Items.Clear();
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_GetNumDevices(ref DevNum);
            if (SLUSBXpressDLL.Status == SLUSBXpressDLL.SI_SUCCESS)
            {
                for (var i = 0; i < DevNum; i++)
                {
                    SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_GetProductString(i, DevStr,
                        SLUSBXpressDLL.SI_RETURN_SERIAL_NUMBER);
                    comboBox_Device.Items.Insert(i, DevStr);
                }
                comboBox_Device.SelectedIndex = 0;
            }
            else
            {
                MessageBox.Show("Error finding USB device.  Aborting application.");
                Application.Exit();
            }

            double.TryParse(((double)numericUpDown_time.Value).ToString(), out time);
            Count = (int)(time/0.16);
        }

        private void thread1()
        {
            //timer1.Interval = 200;
            //timer1.Elapsed += new ElapsedEventHandler(P);
            //timer1.Enabled = true;

            //var DevNum = 0;
            //var DevStr = new StringBuilder(SLUSBXpressDLL.SI_MAX_DEVICE_STRLEN);

            //SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_GetNumDevices(ref DevNum);
            //if (SLUSBXpressDLL.Status == SLUSBXpressDLL.SI_SUCCESS)
            //{
            //    for (var i = 0; i < DevNum; i++)
            //    {
            //        SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_GetProductString(i, DevStr,
            //            SLUSBXpressDLL.SI_RETURN_SERIAL_NUMBER);
            //    }
            //}
            //else
            //{
            //    MessageBox.Show("Error finding USB device.  Aborting application.");
            //    Application.Exit();
            //}

            //SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_SetTimeouts(-1, -1);
            //SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Open(0, ref SLUSBXpressDLL.hUSBDevice);

            //if (SLUSBXpressDLL.Status != SLUSBXpressDLL.SI_SUCCESS)
            //{
            //    MessageBox.Show("Error opening device: " + comboBox_Device.Text +
            //                    ". Application is aborting. Reset hardware and try again.");
            //    Application.Exit();
            //}

            //for (int k = 0; k < 10; k++)
            //{
            //    SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Read(SLUSBXpressDLL.hUSBDevice, ref InBuf[0], BytesReadRequest,
            //    ref BytesSucceed, 0);

            //    if ((BytesSucceed != BytesReadRequest) || (SLUSBXpressDLL.Status != SLUSBXpressDLL.SI_SUCCESS))
            //    {
            //        MessageBox.Show("Error reading USB. Read " + BytesSucceed + " of " + BytesReadRequest +
            //                        " bytes. Application is aborting. Reset hardware and try again.");
            //        Application.Exit();
            //    }

            //    for (var i = 0; i < InBufSize / skip; i++)
            //    {
            //        if (T == (N / skip))
            //        {
            //            SaveReceivedData();
            //            T = 0;
            //            break;
            //        }
            //        ReceivedValue1[T++] = ((Double)BitConverter.ToInt16(InBuf, skip * i) / 32768 - 0.365) * 1;
            //    }
            //    Thread.Sleep(128);
            //}
        }

        private void P(object sender, ElapsedEventArgs e)
        {
            delegate_process1 process1 = new delegate_process1(CulveDisplay);
            delegate_process2 process2 = new delegate_process2(fft);
            Invoke(process1);
            Invoke(process2);

        }

        private void button_Connect_Click(object sender, EventArgs e)
        {
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_SetTimeouts(-1, -1); 
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Open(comboBox_Device.SelectedIndex, ref SLUSBXpressDLL.hUSBDevice);
            
            if (SLUSBXpressDLL.Status != SLUSBXpressDLL.SI_SUCCESS)
            {
                MessageBox.Show("Error opening device: " + comboBox_Device.Text +
                                ". Application is aborting. Reset hardware and try again.");
                Application.Exit();
            }
            label_ConnectState.Text = "设备状态：连接" + comboBox_Device.SelectedItem;
        }

        private void button_Disconnect_Click(object sender, EventArgs e)
        {
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Close(SLUSBXpressDLL.hUSBDevice);
            
            label_ConnectState.Text = "设备状态：断开";
        }

        private void btn_Start_Click(object sender, EventArgs e)
        {
            //Thread thread_process1 = new Thread(new ThreadStart(thread1));
            //thread_process1.Start();
            //delegate_process1 process1 = new delegate_process1(CulveDisplay);
            //delegate_process2 process2 = new delegate_process2(fft);
            
            for (int k = 0; k < Count; k++)
            {
                SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Read(SLUSBXpressDLL.hUSBDevice, ref InBuf[0], BytesReadRequest,
                ref BytesSucceed, 0);

                if ((BytesSucceed != BytesReadRequest) || (SLUSBXpressDLL.Status != SLUSBXpressDLL.SI_SUCCESS))
                {
                    MessageBox.Show("Error reading USB. Read " + BytesSucceed + " of " + BytesReadRequest +
                                    " bytes. Application is aborting. Reset hardware and try again.");
                    Application.Exit();
                }

                for (int T = 0; T < InBufSize / skip; T++)
                {
                    ReceivedValue1[T] = ((Double)BitConverter.ToInt16(InBuf, skip * T) / 32768) * 1;
                }
                //Invoke(process1);
                //Invoke(process2);       
                SaveReceivedData();

                Thread.Sleep(118);
            }
            CulveDisplay();
            fft();
        }

        private void btn_Stop_Click(object sender, EventArgs e)
        {
            
        }

        private void btn_Exit_Click(object sender, EventArgs e)
        {
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Close(SLUSBXpressDLL.hUSBDevice);

            Application.Exit();
        }

        public void SaveReceivedData()
        {
            var fs = new FileStream("data1.txt", FileMode.Append,FileAccess.Write);
            var sw = new StreamWriter(fs);
            //var fs = new FileStream("data", FileMode.Create, FileAccess.Write);
            //var sw = new BinaryWriter(fs);
            for (int j = 0; j < N / skip;j++ )
            {
                sw.WriteLine((float)ReceivedValue1[j]);
                //sw.Write((float)ReceivedValue1[j] + " ");
                //sw.Write((float)ReceivedValue1[j]);
            }
            sw.Close();
            fs.Close();
        }

        public void CulveDisplay()
        {
            Double x1, y1;
            var myPane1 = zedGraphControl1.GraphPane;
            myPane1.CurveList.Clear();
            myPane1.GraphObjList.Clear();
            var culveList1 = new PointPairList();
            for (var i = 0; i < InBufSize / skip; i++)
            {
                x1 = i;
                y1 = ReceivedValue1[i];
                culveList1.Add(x1, y1);
            }
            var Culve1 = myPane1.AddCurve("", culveList1, Color.Blue, SymbolType.None);

            Culve1.Line.IsSmooth = true;
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
        }

        public void fft()
        {
            output_complex = FFT_IFFT.FFT(ReceivedValue1, false); //正变换

            Double x2, y2;
            var myPane2 = zedGraphControl2.GraphPane;
            myPane2.CurveList.Clear();
            myPane2.GraphObjList.Clear();
            var culveList1 = new PointPairList();
            for (var i = 0; i < N / skip / 2; i++)
            {
                x2 = i;
                y2 =
                    Math.Sqrt(output_complex[i].Image * output_complex[i].Image +
                              output_complex[i].Real * output_complex[i].Real) * 2;
                culveList1.Add(x2, y2);
            }
            var Culve2 = myPane2.AddCurve("", culveList1, Color.Red, SymbolType.None);

            Culve2.Line.IsSmooth = true;
            zedGraphControl2.AxisChange();
            zedGraphControl2.Invalidate();
        }

        private void numericUpDown_time_ValueChanged(object sender, EventArgs e)
        {
            double.TryParse(((double)numericUpDown_time.Value).ToString(), out time);
            Count = (int)(time / 0.16);
            System.Console.WriteLine(Count);
        }

    }
}