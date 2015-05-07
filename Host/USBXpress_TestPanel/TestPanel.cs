using System;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;
using ZedGraph;

namespace USBXpress_TestPanel
{
    public partial class TestPanel : Form
    {
        private static int N = 16*1024*2;
        private static int InBufSize = 16*1024*2;
        private static int OutBufSize = 2;
        Byte[] InBuf = new Byte[InBufSize];
        Byte[] OutBuf = new Byte[OutBufSize];
        int BytesReadRequest = InBufSize;
        int BytesWriteRequest = OutBufSize;
        int BytesSucceed = 0;
        int skip = 16;
     
        
        int[] ReceivedValue1 = new int[N/16];
        int T = 0;
        public TestPanel()
        {
            InitializeComponent();
            //设置窗体和插图填充颜色
            zedGraphControl1.GraphPane.Fill = new Fill(Color.AliceBlue);
            zedGraphControl1.GraphPane.Chart.Fill = new Fill(Color.Black);
            //设置显示信息
            zedGraphControl1.GraphPane.Title.IsVisible = false;
            zedGraphControl1.GraphPane.XAxis.Title.IsVisible = false;
            zedGraphControl1.GraphPane.YAxis.Title.IsVisible = false;

            zedGraphControl1.GraphPane.XAxis.IsVisible = false;
            zedGraphControl1.GraphPane.YAxis.IsVisible = false;
            if (N*8/1000000>=1)
            {
                label1.Text = "传输的比特数为：" + ((float)(N * 8) / 1000000) + "Mb";
            }
            else
            {
                label1.Text = "传输的比特数为：" + ((float)(N * 8 / 1000)) + "Kb";
            }
            
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

            Stopwatch sw = new Stopwatch();
            sw.Start();
            //read data from the board
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Read(SLUSBXpressDLL.hUSBDevice, ref InBuf[0], BytesReadRequest,
                ref BytesSucceed, 0);

            //if ((BytesSucceed != BytesReadRequest) || (SLUSBXpressDLL.Status != SLUSBXpressDLL.SI_SUCCESS))
            //{
            //    MessageBox.Show("Error writing to USB. Read " + BytesSucceed + " of " + BytesReadRequest +
            //                    " bytes. Application is aborting. Reset hardware and try again.");
            //    Application.Exit();
            //}

            
            //take the newly received array and put it into the for
            for (int i = 0; i < InBufSize / 16; i++)
            {
                if (T == (N / 16 - 1))
                {
                    sw.Stop();
                    //timer1.Stop();
                    if (((float)(sw.Elapsed.Seconds + sw.Elapsed.Milliseconds) * N * 8)>=1000000)
                    {
                        label2.Text = "传输速度为：" + ((float)1 / (float)(sw.Elapsed.Seconds + sw.Elapsed.Milliseconds) * N * 8)/(float)1000000 + "Mb/s"; ;
                    }
                    else
                    {
                        label2.Text = "传输速度为：" + ((float)1 / (float)(sw.Elapsed.Seconds + sw.Elapsed.Milliseconds) * N * 8)/(float)1000 + "Kb/s"; ;
                    }
                    break;
                }
                
                if (InBuf[skip * i] >= 128)
                {
                    ReceivedValue1[T++] = (InBuf[skip * i] - 128) * 256 + InBuf[skip * i + 1] - 27268 - 5500;
                }
                else
                {
                    ReceivedValue1[T++] = (InBuf[skip * i] + 128) * 256 + InBuf[skip * i + 1] - 27268 - 5500;
                }
            }
            CulveDisplay();
            
        }

        public void CulveDisplay()
        {
            int x1, y1;
            GraphPane myPane1 = zedGraphControl1.GraphPane;
            myPane1.CurveList.Clear();
            myPane1.GraphObjList.Clear();
            PointPairList culveList1 = new PointPairList();
            for (int i = 0; i < N/16; i++)
            {
                x1 = i;
                y1 = ReceivedValue1[i];
                culveList1.Add(x1, y1);
            }      
            LineItem Culve1 = myPane1.AddCurve("", culveList1, Color.Red, SymbolType.None);
            myPane1.Title.Text = "";
            myPane1.YAxis.Title.Text = "采样值";
            myPane1.XAxis.Title.IsVisible = false;
            Culve1.Line.IsSmooth = true;
            myPane1.YAxis.Scale.Align = AlignP.Inside;
            myPane1.YAxis.Scale.FontSpec.FontColor = Color.Black;
            myPane1.YAxis.MajorGrid.IsZeroLine = false;
            myPane1.YAxis.Scale.Align = AlignP.Inside;
            myPane1.XAxis.Scale.Max = N/16;
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