using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;
using ZedGraph;

namespace USBXpress_TestPanel
{
    public partial class TestPanel : Form
    {
        private static readonly int N = 16*1024*2;
        private static readonly int InBufSize = 16*1024;
        private static readonly int OutBufSize = 1;
        private static readonly int skip = 2;
        private readonly int BytesReadRequest = InBufSize;
        private readonly int BytesWriteRequest = OutBufSize;
        private readonly Byte[] InBuf = new Byte[InBufSize];
        private readonly Byte[] OutBuf = new Byte[OutBufSize];
        private readonly double[] ReceivedValue1 = new double[N/skip];
        private readonly Stopwatch stopwatch = new Stopwatch();
        private readonly double[] ValueToShow = new double[InBufSize];
        private int BytesSucceed;
        private Complex[] output_complex = new Complex[InBufSize/skip];
        private int T;
        private float time_read;
        private float time_all;
        private Int16 ParseValue;

        public TestPanel()
        {
            InitializeComponent();
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

            //var path = Environment.CurrentDirectory;
            //var pattern = "*.txt";
            //var strFileName = Directory.GetFiles(path, pattern);
            //foreach (var item in strFileName)
            //{
            //    File.Delete(item);
            //}

            //File.Delete("data.txt");

            if ((float) N/1000000 >= 1)
            {
                label1.Text = "总接收字节数：" + ((float) N/1000000) + "MB";
            }
            else
            {
                label1.Text = "总接收字节数：" + ((float) (N/1000)) + "KB";
            }
            label2.Hide();
            label_ConnectState.Text = "设备状态：未连接";
            stopwatch.Start();
        }

        private void TestPanel_Load(object sender, EventArgs e)
        {
            timer1.Stop();
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
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Write(SLUSBXpressDLL.hUSBDevice, ref OutBuf[0], BytesWriteRequest,
                ref BytesSucceed, 0);

            if ((BytesSucceed != BytesWriteRequest) || (SLUSBXpressDLL.Status != SLUSBXpressDLL.SI_SUCCESS))
            {
                MessageBox.Show("Error writing to USB. Wrote " + BytesSucceed + " of " + BytesWriteRequest +
                                " bytes. Application is aborting. Reset hardware and try again.");
                Application.Exit();
            }
            BytesSucceed = 0;
            
            stopwatch.Restart();
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Read(SLUSBXpressDLL.hUSBDevice, ref InBuf[0], BytesReadRequest,
                ref BytesSucceed, 0);

            if ((BytesSucceed != BytesReadRequest) || (SLUSBXpressDLL.Status != SLUSBXpressDLL.SI_SUCCESS))
            {
                MessageBox.Show("Error writing to USB. Read " + BytesSucceed + " of " + BytesReadRequest +
                                " bytes. Application is aborting. Reset hardware and try again.");
                Application.Exit();
            }
            stopwatch.Stop();
            time_read += stopwatch.Elapsed.Seconds + (float) stopwatch.Elapsed.Milliseconds/1000;
            //stopwatch.Start();
            OutBuf[0] = 0;
            for (var i = 0; i < InBufSize / skip; i++)
            {
                OutBuf[0] = 0;
                if (T == (N / skip))
                {
                    OutBuf[0] = 1;
                    label2.Show();
                    //timer1.Stop();
                    if ((1 / time_read) * N / skip >= 1000000)
                    {
                        label2.Text = "有效速度：" +
                                      (1 / time_read) * N / skip / 1000000 +
                                      "MB/s";
                    }
                    else
                    {
                        label2.Text = "有效速度：" + (1 / time_read) * N / skip / 1000 +
                                      "KB/s";
                    } 
                    fft();
                    CulveDisplay();
                    T = 0;
                    SaveReceivedData();
                    
                    textBox1.Text += "读取时间为：" + time_read + System.Environment.NewLine + "总用时为：" + time_all +
                                     System.Environment.NewLine;
                    time_read = 0;
                    time_all = 0;
                    break;
                }
                ReceivedValue1[T++] = (Double)BitConverter.ToInt16(InBuf, skip * i)/32768;
                //ReceivedValue1[T++] = InBuf[skip*i];
                ValueToShow[i] = ReceivedValue1[i];
                time_all += stopwatch.Elapsed.Seconds + (float)stopwatch.Elapsed.Milliseconds / 1000;
            }
        }

        public void fft()
        {
            output_complex = FFT_IFFT.FFT(ValueToShow, false); //正变换

            Double x2, y2;
            var myPane2 = zedGraphControl2.GraphPane;
            myPane2.CurveList.Clear();
            myPane2.GraphObjList.Clear();
            var culveList1 = new PointPairList();
            for (var i = 0; i < InBufSize/skip/2; i++)
            {
                x2 = i;
                y2 =
                    Math.Sqrt(output_complex[i].Image*output_complex[i].Image +
                              output_complex[i].Real*output_complex[i].Real)*2;
                culveList1.Add(x2, y2);
            }
            var Culve2 = myPane2.AddCurve("", culveList1, Color.Red, SymbolType.None);
            myPane2.Title.Text = "";
            myPane2.YAxis.Title.Text = "fft";
            myPane2.XAxis.Title.IsVisible = false;
            Culve2.Line.IsSmooth = true;
            myPane2.YAxis.Scale.Align = AlignP.Inside;
            myPane2.YAxis.Scale.FontSpec.FontColor = Color.Black;
            myPane2.YAxis.MajorGrid.IsZeroLine = false;
            myPane2.YAxis.Scale.Align = AlignP.Inside;
            myPane2.XAxis.Scale.Max = N/skip/2;
            zedGraphControl2.AxisChange();
            zedGraphControl2.Invalidate();
        }

        public void SaveReceivedData()
        {
            var fs = new FileStream("data.txt", FileMode.Append);
            var sw = new StreamWriter(fs);
            var i = 1;
            while (i < N/skip)
            {
                sw.WriteLine(ReceivedValue1[i]);
                i = i + 1;
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
            for (var i = 0; i < InBufSize/skip; i++)
            {
                x1 = i;
                y1 = ValueToShow[i];
                culveList1.Add(x1, y1);
            }
            //culveList1.Add(t, ValueToShow);
            //culveList1.SetX(t);
            //culveList1.SetY(ReceivedValue1);
            var Culve1 = myPane1.AddCurve("", culveList1, Color.Blue, SymbolType.None);
            myPane1.Title.Text = "";
            myPane1.YAxis.Title.Text = "采样值";
            myPane1.XAxis.Title.IsVisible = false;
            Culve1.Line.IsSmooth = true;
            myPane1.YAxis.Scale.Align = AlignP.Inside;
            myPane1.YAxis.Scale.FontSpec.FontColor = Color.Black;
            myPane1.YAxis.MajorGrid.IsZeroLine = false;
            myPane1.YAxis.Scale.Align = AlignP.Inside;
            //myPane1.YAxis.Scale.Min = -0.5;
            //myPane1.YAxis.Scale.Max = 0.5;
            myPane1.XAxis.Scale.Max = InBufSize/skip;
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
        }

        private void button_Connect_Click(object sender, EventArgs e)
        {
            // when ok is clicked, set the timeouts on the device
            // and open the device
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_SetTimeouts(360, 360);//10000
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Open(comboBox_Device.SelectedIndex, ref SLUSBXpressDLL.hUSBDevice);

            if (SLUSBXpressDLL.Status != SLUSBXpressDLL.SI_SUCCESS)
            {
                MessageBox.Show("Error opening device: " + comboBox_Device.Text +
                                ". Application is aborting. Reset hardware and try again.");
                Application.Exit();
            }
            label_ConnectState.Text = "设备状态：连接" + comboBox_Device.SelectedItem.ToString();
            timer1.Start();
        }

        private void button_Disconnect_Click(object sender, EventArgs e)
        {
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Close(SLUSBXpressDLL.hUSBDevice);
            timer1.Stop();
            label_ConnectState.Text = "设备状态：断开";
        }

        private void btn_Start_Click(object sender, EventArgs e)
        {
            timer1.Start();
        }

        private void btn_Stop_Click(object sender, EventArgs e)
        {
            timer1.Stop();
        }

        private void btn_Clear_Click(object sender, EventArgs e)
        {
            textBox1.Clear();
        }

        private void btn_Exit_Click(object sender, EventArgs e)
        {
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Close(SLUSBXpressDLL.hUSBDevice);
            timer1.Stop();
            Application.Exit(); 
        }
       
    }
}