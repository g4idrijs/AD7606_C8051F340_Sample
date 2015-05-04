using System;
using System.Drawing;
using System.Windows.Forms;
using ZedGraph;

namespace USBXpress_TestPanel
{
    public partial class TestPanel : Form
    {
        int[] ReceivedValue1 = new int[3000];
        int[] ReceivedValue2 = new int[3000];
        int[] ReceivedValue3 = new int[3000];
        int[] ReceivedValue4 = new int[3000];
        int T = 0;
        private int x1,y1,x2,y2,x3,y3,x4,y4;
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
            
            var InBufSize = 32;
            var OutBufSize = 2;
            var InBuf = new Byte[InBufSize];
            var OutBuf = new Byte[OutBufSize];
            var BytesSucceed = 0;
            var BytesWriteRequest = OutBufSize;
            var BytesReadRequest = InBufSize;

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
            for (int i = 0; i < 4; i++)
            { 
                if (InBuf[8*i]>=128)
                {
                    ReceivedValue1[T++] = (InBuf[8*i] - 128)*256 + InBuf[8*i + 1] - 35000;
                }
                else
                {
                    ReceivedValue1[T++] = (InBuf[8 * i] + 128) * 256 + InBuf[8 * i + 1] -35000; 
                }
                //ReceivedValue2[T] = InBuf[8 * i + 2] * 256 + InBuf[8 * i + 3];
                //ReceivedValue3[T] = InBuf[8 * i + 4] * 256 + InBuf[8 * i + 5];
                //ReceivedValue4[T++] = InBuf[8 * i + 6] * 256 + InBuf[8 * i + 7];
                textBox1.Text += ReceivedValue1[T - 1].ToString() + " ";
                CulveDisplay();
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
            for (int h = 0; h < 3000; h++)
            {
                x1 = h;
                y1 = ReceivedValue1[h];
                culveList1.Add(x1, y1);
                //x2 = h;
                //y2 = ReceivedValue2[h];
                //culveList2.Add(x2, y2);
                //x3 = h;
                //y3 = ReceivedValue3[h];
                //culveList3.Add(x3, y3);
                //x4 = h;
                //y4 = ReceivedValue4[h];
                //culveList4.Add(x4, y4);
            }
            
            LineItem Curve1 = myPane1.AddCurve("", culveList1, Color.Black, SymbolType.Diamond);
            //LineItem Curve2 = myPane1.AddCurve("", culveList2, Color.Blue, SymbolType.Circle);
            //LineItem Curve3 = myPane1.AddCurve("", culveList3, Color.BlueViolet, SymbolType.None);
            //LineItem Curve4 = myPane1.AddCurve("", culveList4, Color.Chartreuse, SymbolType.None);
            myPane1.Title.Text = "";
            myPane1.YAxis.Title.Text = "采样值";
            myPane1.XAxis.Title.IsVisible = false;
            Curve1.Symbol.Fill = new Fill(Color.Red);
            Curve1.Symbol.Size = 8;
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
            Application.Exit(); // Exit program
        }

    }
}