using System;
using System.Drawing;
using System.Windows.Forms;
using ZedGraph;

namespace USBXpress_TestPanel
{
    public partial class TestPanel : Form
    {
        private static int N = 2000000;
        int[] ReceivedValue1 = new int[N];
        //int[] ReceivedValue2 = new int[N];
        //int[] ReceivedValue3 = new int[N];
        //int[] ReceivedValue4 = new int[N];
        //int[] ReceivedValue5 = new int[N];
        //int[] ReceivedValue6 = new int[N];
        //int[] ReceivedValue7 = new int[N];
        //int[] ReceivedValue8 = new int[N];
        int T = 0;
        private int x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8;
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
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            var InBufSize = 2048;
            var OutBufSize = 2;
            var InBuf = new Byte[InBufSize];
            var OutBuf = new Byte[OutBufSize];
            var BytesSucceed = 0;
            var BytesWriteRequest = OutBufSize;
            var BytesReadRequest = InBufSize;

            DateTime d = DateTime.Now;
            // Get information from form to write to device
            if (checkBox_LED.Checked)
            {
                OutBuf[0] = 1;
            }
            else
            {
                OutBuf[0] = 0;
            }

            // Send output data out to the board
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Write(SLUSBXpressDLL.hUSBDevice, ref OutBuf[0], BytesWriteRequest,
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
            SLUSBXpressDLL.Status = SLUSBXpressDLL.SI_Read(SLUSBXpressDLL.hUSBDevice, ref InBuf[0], BytesReadRequest,
                ref BytesSucceed, 0);

            if ((BytesSucceed != BytesReadRequest) || (SLUSBXpressDLL.Status != SLUSBXpressDLL.SI_SUCCESS))
            {
                MessageBox.Show("Error writing to USB. Read " + BytesSucceed + " of " + BytesReadRequest +
                                " bytes. Application is aborting. Reset hardware and try again.");
                Application.Exit();
            }

            //take the newly received array and put it into the for
            //for (int i = 0; i < InBufSize / 16; i++)
            //{
            //    int v = 16;
            //    if (InBuf[v * i] >= 128)
            //    {
            //        ReceivedValue1[T++] = (InBuf[v * i] - 128) * 256 + InBuf[v * i + 1] - 27268-5500;
            //    }
            //    else
            //    {
            //        ReceivedValue1[T++] = (InBuf[v * i] + 128) * 256 + InBuf[v * i + 1] - 27268-5500;
            //    }
            //    //ReceivedValue2[T] = InBuf[v * i + 2] * 256 + InBuf[v * i + 3];
            //    //ReceivedValue3[T] = InBuf[v * i + 4] * 256 + InBuf[v * i + 5];
            //    //ReceivedValue4[T] = InBuf[v * i + 6] * 256 + InBuf[v * i + 7];
            //    //ReceivedValue5[T] = InBuf[v * i + 8] * 256 + InBuf[v * i + 9];
            //    //ReceivedValue6[T] = InBuf[v * i + 10] * 256 + InBuf[v * i + 11];
            //    //ReceivedValue7[T] = InBuf[v * i + 12] * 256 + InBuf[v * i + 13];
            //    //ReceivedValue8[T++] = InBuf[v * i + 14] * 256 + InBuf[v * i + 15];
            //    textBox1.Text += ReceivedValue1[T - 1].ToString() + " ";
            //    CulveDisplay();
            //}
            
            for (int i = 0; i < InBufSize; i++)
            {
                if (T == (N - 1))
                {
                    CulveDisplay();
                    textBox1.Text = (DateTime.Now - d).ToString() + " ";
                    break;
                }
                ReceivedValue1[T++] = InBuf[i];
                //CulveDisplay();
            }
            
            
        }

        public void CulveDisplay()
        {
            GraphPane myPane1 = zedGraphControl1.GraphPane;
            myPane1.CurveList.Clear();
            myPane1.GraphObjList.Clear();
            PointPairList culveList1 = new PointPairList();
            //PointPairList culveList2 = new PointPairList();
            //PointPairList culveList3 = new PointPairList();
            //PointPairList culveList4 = new PointPairList();
            //PointPairList culveList5 = new PointPairList();
            //PointPairList culveList6 = new PointPairList();
            //PointPairList culveList7 = new PointPairList();
            //PointPairList culveList8 = new PointPairList();
            for (int i = 0; i < N; i++)
            {
                x1 = i;
                y1 = ReceivedValue1[i];
                culveList1.Add(x1, y1);
                //x2 = i;
                //y2 = ReceivedValue2[i];
                //culveList2.Add(x2, y2);
                //x3 = i;
                //y3 = ReceivedValue3[i];
                //culveList3.Add(x3, y3);
                //x4 = i;
                //y4 = ReceivedValue4[i];
                //culveList4.Add(x4, y4);
                //x5 = i;
                //y5 = ReceivedValue5[i];
                //culveList1.Add(x5, y5);
                //x6 = i;
                //y6 = ReceivedValue6[i];
                //culveList2.Add(x6, y6);
                //x7 = i;
                //y7 = ReceivedValue7[i];
                //culveList3.Add(x7, y7);
                //x8 = i;
                //y8 = ReceivedValue8[i];
                //culveList4.Add(x8, y8);
            }
            
            LineItem Culve1 = myPane1.AddCurve("", culveList1, Color.Red, SymbolType.None);
            //LineItem Culve2 = myPane1.AddCurve("", culveList2, Color.Blue, SymbolType.None);
            //LineItem Culve3 = myPane1.AddCurve("", culveList3, Color.BlueViolet, SymbolType.None);
            //LineItem Culve4 = myPane1.AddCurve("", culveList4, Color.Chartreuse, SymbolType.None);
            //LineItem Culve5 = myPane1.AddCurve("", culveList5, Color.SaddleBrown, SymbolType.None);
            //LineItem Culve6 = myPane1.AddCurve("", culveList6, Color.PaleVioletRed, SymbolType.None);
            //LineItem Culve7 = myPane1.AddCurve("", culveList7, Color.OrangeRed, SymbolType.None);
            //LineItem Culve8 = myPane1.AddCurve("", culveList8, Color.Olive, SymbolType.None);
            myPane1.Title.Text = "";
            myPane1.YAxis.Title.Text = "采样值";
            myPane1.XAxis.Title.IsVisible = false;
            //Culve1.Line.IsSmooth = true;
            //Culve2.Line.IsSmooth = true;
            //Culve3.Line.IsSmooth = true;
            //Culve4.Line.IsSmooth = true;
            //Culve5.Line.IsSmooth = true;
            //Culve6.Line.IsSmooth = true;
            //Culve7.Line.IsSmooth = true;
            //Culve8.Line.IsSmooth = true;

            //Culve1.Symbol.Fill = new Fill(Color.Red);
            //Culve1.Symbol.Size = 8;
            myPane1.YAxis.Scale.Align = AlignP.Inside;
            myPane1.YAxis.Scale.FontSpec.FontColor = Color.Black;
            myPane1.YAxis.MajorGrid.IsZeroLine = false;
            myPane1.YAxis.Scale.Align = AlignP.Inside;
            //myPane1.YAxis.Scale.Min = 11500;
            //myPane1.YAxis.Scale.Max = 11800;
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