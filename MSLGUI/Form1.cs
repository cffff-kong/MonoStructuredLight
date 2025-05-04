using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;
using System.Xml.Serialization;
using mslclrimpoort;
using System.Reflection;
using System.Reflection.Emit;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;
namespace MSLGUI
{
    public partial class Form1 : Form
    {
        private MSLCLR mslclr = new MSLCLR();
        int g_is_preview_real_time_diaply = 0;
        public Form1()
        {
            InitializeComponent();
            pictureBox1.SizeMode = PictureBoxSizeMode.Zoom;//设置图大小显示模式，可使图像适应pictureBoxCamShow大小，并保持横纵比
            trackBarExposure.Minimum = 8400;
            trackBarExposure.Maximum = 1000000;
            trackBarExposure.TickFrequency = 10;
            trackBarExposure.SmallChange = 100;    // 鼠标箭头点击或键盘方向键移动步长

        }

        private void btnOpenCamera_Click(object sender, EventArgs e)
        {
            mslclr.OpenCameraCLR();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Interlocked.Exchange(ref g_is_preview_real_time_diaply, 0);
            Thread.Sleep(100);
            mslclr.CloseCameraCLR();
        }
        private void tabPage1_Click(object sender, EventArgs e)
        {

        }

        public void PreViewRealTimeDisplay()
        {
            //等待is_preview_real_time_diaply为1，超时则退出
            DateTime start_time = DateTime.Now;
            while (g_is_preview_real_time_diaply < 1)
            {
                DateTime current_time = DateTime.Now;
                TimeSpan time_interval = current_time - start_time;
                if (time_interval.TotalSeconds > 5)
                {
                    Console.WriteLine("相机连接超时\n");
                    return;
                }
                Thread.Sleep(100);
            }
            Bitmap last_image = new Bitmap(1, 1);
            while (g_is_preview_real_time_diaply > 0)
            {
                Bitmap raw_image = mslclr.GetImageCLR();
                if (raw_image == null)
                {
                    Console.WriteLine("从流中抓取图像失败\n");
                    continue;
                }
                // 将图像转换为24位RGB格式，确保可绘图
                Bitmap current_image = new Bitmap(raw_image.Width, raw_image.Height, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
                using (Graphics g = Graphics.FromImage(current_image))
                {
                    g.DrawImage(raw_image, 0, 0);
                }
                raw_image.Dispose();

                //Interlocked.Exchange(ref g_raw_bitmap, current_image);
                pictureBox1.Image = current_image;//显示图像，引用赋值
                last_image.Dispose();//在显示当前抓取的图像后，及时将上一次抓取的图像内存释放
                last_image = current_image;
                Thread.Sleep(10);

            }
        }
        private void button2_Click(object sender, EventArgs e)
        {
            mslclr.StartGrabbingCLR();
            // 使用 Interlocked 设置 g_is_displaying 的值
            Interlocked.Exchange(ref g_is_preview_real_time_diaply, 1);

            // 使用 lambda 表达式创建线程
            Thread thread = new Thread(() => PreViewRealTimeDisplay());
            thread.Start();
        }

        private void trackBarExposure_Scroll(object sender, EventArgs e)
        {
            mslclr.SetExposureCLR(trackBarExposure.Value);
            label3.Text =  trackBarExposure.Value.ToString() + "us";
        }

        /// <summary>
        /// 关闭窗体时，释放相机资源
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            Interlocked.Exchange(ref g_is_preview_real_time_diaply, 0);
            Thread.Sleep(100);
            mslclr.CloseCameraCLR();
        }
    }
}
