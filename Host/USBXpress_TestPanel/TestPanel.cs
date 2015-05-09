using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using ZedGraph;

namespace USBXpress_TestPanel
{
    public partial class TestPanel : Form
    {
        private static readonly int N = 16*1024*20;
        private static readonly int InBufSize = 16*1024;
        private static readonly int OutBufSize = 2;
        private static readonly int skip = 2;
        private readonly int BytesReadRequest = InBufSize;
        private readonly int BytesWriteRequest = OutBufSize;
        private readonly Byte[] InBuf = new Byte[InBufSize];
        private readonly Byte[] OutBuf = new Byte[OutBufSize];
        private readonly double[] ReceivedValue1 = new double[N/skip];
        private readonly Stopwatch sw = new Stopwatch();
        private readonly double[] t = new double[InBufSize/skip];
        private readonly double[] ValueToShow = new double[InBufSize];
        private int BytesSucceed;
        private Complex[] output_complex = new Complex[InBufSize/skip];
        private int T;

        public TestPanel()
        {
            InitializeComponent();
            //设置窗体和插图填充颜色
            zedGraphControl1.GraphPane.Fill = new Fill(Color.AliceBlue);
            zedGraphControl1.GraphPane.Chart.Fill = new Fill(Color.Black);
            zedGraphControl2.GraphPane.Fill = new Fill(Color.AliceBlue);
            zedGraphControl2.GraphPane.Chart.Fill = new Fill(Color.Black);
            //设置显示信息
            zedGraphControl1.GraphPane.Title.IsVisible = false;
            zedGraphControl1.GraphPane.XAxis.Title.IsVisible = false;
            zedGraphControl1.GraphPane.YAxis.Title.IsVisible = false;
            zedGraphControl1.GraphPane.XAxis.IsVisible = false;
            zedGraphControl1.GraphPane.YAxis.IsVisible = false;
            zedGraphControl2.GraphPane.Title.IsVisible = false;
            zedGraphControl2.GraphPane.XAxis.Title.IsVisible = false;
            zedGraphControl2.GraphPane.YAxis.Title.IsVisible = false;
            zedGraphControl2.GraphPane.XAxis.IsVisible = false;
            zedGraphControl2.GraphPane.YAxis.IsVisible = false;

            var pattern = "data1.txt";
            File.Delete(pattern);

            if (N/1000000 >= 1)
            {
                label1.Text = "传输的字节数为：" + ((float) N/1000000) + "MB";
            }
            else
            {
                label1.Text = "传输的字节数为：" + ((float) (N/1000)) + "KB";
            }
            label2.Text = "传输速度为：";
            for (var i = 0; i < InBufSize/skip; i++)
            {
                t[i] = i;
            }
            sw.Start();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            // Send output data out to the board
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Write(SLUSBXpressDLL.hUSBDevice, ref OutBuf[0], BytesWriteRequest,
                ref BytesSucceed, 0);

            //if ((BytesSucceed != BytesWriteRequest) || (SLUSBXpressDLL.Status != SLUSBXpressDLL.SI_SUCCESS))
            //{
            //    MessageBox.Show("Error writing to USB. Wrote " + BytesSucceed + " of " + BytesWriteRequest +
            //                    " bytes. Application is aborting. Reset hardware and try again.");
            //    Application.Exit();
            //}

            //clear out bytessucceed for the next read
            BytesSucceed = 0;

            //var sw = new Stopwatch();
            //sw.Start();
            //read data from the board
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Read(SLUSBXpressDLL.hUSBDevice, ref InBuf[0], BytesReadRequest,
                ref BytesSucceed, 0);

            if ((BytesSucceed != BytesReadRequest) || (SLUSBXpressDLL.Status != SLUSBXpressDLL.SI_SUCCESS))
            {
                MessageBox.Show("Error writing to USB. Read " + BytesSucceed + " of " + BytesReadRequest +
                                " bytes. Application is aborting. Reset hardware and try again.");
                Application.Exit();
            }


            //take the newly received array and put it into the for
            for (var i = 0; i < InBufSize/skip; i++)
            {
                if (T == (N/skip))
                {
                    sw.Stop();
                    //timer1.Stop();
                    if (1/(sw.Elapsed.Seconds + (float) sw.Elapsed.Milliseconds/1000)*N >= 1000000)
                    {
                        label2.Text = "传输速度为：" +
                                      (1/(sw.Elapsed.Seconds + (float) sw.Elapsed.Milliseconds/1000)*N)/1000000 +
                                      "MB/s";
                    }
                    else
                    {
                        label2.Text = "传输速度为：" + (1/(sw.Elapsed.Seconds + (float) sw.Elapsed.Milliseconds/1000)*N)/1000 +
                                      "KB/s";
                    }
                    fft();
                    CulveDisplay();
                    textBox1.Text += (sw.Elapsed.Seconds + (float) sw.ElapsedMilliseconds/1000) + "    ";
                    T = 0;
                    SaveReceivedData();
                    break;
                }
                if (InBuf[skip*i] >= 128)
                {
                    ReceivedValue1[T++] = (Double) ((InBuf[skip*i] - 128)*256 + InBuf[skip*i + 1] - 27268 - 5500)/32768;
                }
                else
                {
                    ReceivedValue1[T++] = (Double) ((InBuf[skip*i] + 128)*256 + InBuf[skip*i + 1] - 27268 - 5500)/32768;
                }
                //ReceivedValue1[T++] = InBuf[skip*i];
                ValueToShow[i] = ReceivedValue1[i];
            }
        }

        public void fft()
        {
            output_complex = FFT_IFFT.FFT(ValueToShow, false); //正变换

            Double x1, y1;
            var myPane2 = zedGraphControl2.GraphPane;
            myPane2.CurveList.Clear();
            myPane2.GraphObjList.Clear();
            var culveList1 = new PointPairList();
            for (var i = 0; i < InBufSize/skip; i++)
            {
                x1 = i;
                y1 =
                    Math.Sqrt(output_complex[i].Image*output_complex[i].Image +
                              output_complex[i].Real*output_complex[i].Real)/2;
                culveList1.Add(x1, y1);
            }
            var Culve1 = myPane2.AddCurve("", culveList1, Color.Red, SymbolType.None);
            myPane2.Title.Text = "";
            myPane2.YAxis.Title.Text = "fft";
            myPane2.XAxis.Title.IsVisible = false;
            //Culve1.Line.IsSmooth = true;
            myPane2.YAxis.Scale.Align = AlignP.Inside;
            myPane2.YAxis.Scale.FontSpec.FontColor = Color.Black;
            myPane2.YAxis.MajorGrid.IsZeroLine = false;
            myPane2.YAxis.Scale.Align = AlignP.Inside;
            //myPane1.XAxis.Scale.Max = InBufSize/skip;
            zedGraphControl2.AxisChange();
            zedGraphControl2.Invalidate();
        }

        public void SaveReceivedData()
        {
            var fs = new FileStream("data1.txt", FileMode.Append);
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
            //Culve1.Line.IsSmooth = true;
            myPane1.YAxis.Scale.Align = AlignP.Inside;
            myPane1.YAxis.Scale.FontSpec.FontColor = Color.Black;
            myPane1.YAxis.MajorGrid.IsZeroLine = false;
            myPane1.YAxis.Scale.Align = AlignP.Inside;
            myPane1.XAxis.Scale.Max = InBufSize/skip;
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
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
            //Application.Exit(); // Exit program
        }
    }
}