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
using Kitware.VTK;
namespace MSLGUI
{
    public partial class Form1 : Form
    {
        private MSLCLR mslclr = new MSLCLR();
        int g_is_preview_real_time_diaply = 0;
        int g_exposure_value;
        RenderWindowControl renderWindowControl;

        public Form1()
        {
            InitializeComponent();
            pictureBox1.SizeMode = PictureBoxSizeMode.Zoom;//设置图大小显示模式，可使图像适应pictureBoxCamShow大小，并保持横纵比
            trackBarExposure.Minimum = 0;
            trackBarExposure.Maximum = (1000000 - 9000) / 1000;  // == 991
            trackBarExposure.TickFrequency = 1;
            trackBarExposure.SmallChange = 1;
            trackBarExposure.LargeChange = 1;
            mslclr.SendPointerCLR(); //传递指针
            // 创建点云显示窗口
            renderWindowControl = new RenderWindowControl();
            renderWindowControl.Parent = panelCloudPoint;	/* 指定显示空间为panel1 */
            renderWindowControl.Dock = DockStyle.Fill;	/* 指定显示方式为铺满 */
            // 轮询投影仪是否已经连接
            Task.Run(() =>
            {
                while (true)
                {
                    mslclr.CheckDLPIsConnectCLR();
                    Thread.Sleep(2000);
                }
            });
        }
            
        /// <summary>
        /// 打开相机按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnOpenCamera_Click(object sender, EventArgs e)
        {
            mslclr.OpenCameraCLR();

        }

        /// <summary>
        /// 关闭相机按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            Interlocked.Exchange(ref g_is_preview_real_time_diaply, 0);
            Thread.Sleep(100);
            mslclr.CloseCameraCLR();
        }

        /// <summary>
        /// 预览实时显示线程
        /// </summary>
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

        /// <summary>
        /// 开始预览实时显示按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void button2_Click(object sender, EventArgs e)
        {
            mslclr.StartGrabbingCLR();
            // 使用 Interlocked 设置 g_is_displaying 的值
            Interlocked.Exchange(ref g_is_preview_real_time_diaply, 1);

            // 使用 lambda 表达式创建线程
            Thread thread = new Thread(() => PreViewRealTimeDisplay());
            thread.Start();
        }

        /// <summary>
        /// 设置曝光
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void trackBarExposure_Scroll(object sender, EventArgs e)
        {
            g_exposure_value = 9000 + trackBarExposure.Value * 1000;
            mslclr.SetExposureCLR(g_exposure_value);
            label3.Text = g_exposure_value.ToString() + "us";
        }

        /// <summary>
        /// 关闭窗体事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            Interlocked.Exchange(ref g_is_preview_real_time_diaply, 0);
            Thread.Sleep(1000);
            mslclr.CloseCameraCLR();
        }

        /// <summary>
        /// 在窗口显示点云
        /// </summary>
        /// <param name="points"></param>
        /// <param name="r"></param>
        /// <param name="g"></param>
        /// <param name="b"></param>
        /// <param name="size"></param>
        public void showCloudPoint(vtkPoints points, double r = 1.0, double g = 1.0, double b = 1.0, float size = 1f)
        {
            vtkPolyData polydata = vtkPolyData.New();
            polydata.SetPoints(points);

            vtkVertexGlyphFilter glyphFilter = vtkVertexGlyphFilter.New();
            glyphFilter.SetInputConnection(polydata.GetProducerPort()); ; 
            glyphFilter.Update();

            vtkPolyDataMapper mapper = vtkPolyDataMapper.New();
            mapper.SetInputConnection(glyphFilter.GetOutputPort());

            vtkActor actor = vtkActor.New();
            actor.SetMapper(mapper);
            actor.GetProperty().SetPointSize(size);
            actor.GetProperty().SetColor(r, g, b);

            vtkRenderer render = renderWindowControl.RenderWindow.GetRenderers().GetFirstRenderer();

            render.RemoveAllViewProps();  // 清除旧的 actor（如果你只显示一个云）
            render.AddActor(actor);

            renderWindowControl.RenderWindow.Render();  // 强制刷新
        }

        /// <summary>
        /// 开始投影按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private async void btnStartProjection_Click(object sender, EventArgs e)
        {
            int exposure_time = g_exposure_value;
            int projection_period = exposure_time < 500000 ? 500000 : exposure_time;

            int projection_type = comboBoxProjectionType.SelectedIndex;

            await Task.Run(() =>
            {
                mslclr.StartProjectionCLR(projection_type, exposure_time, projection_period);
            });
            Console.WriteLine("project done\n");

            if (projection_type == 0)
            {
                // 等待投影完成后显示点云
                List<mslclrimpoort.Point3f> points = mslclr.ShowCloudPointCLR();
                vtkPoints vtkpoints = vtkPoints.New();
                foreach (mslclrimpoort.Point3f pt in points)
                {
                    vtkpoints.InsertNextPoint(pt.x, pt.y, pt.z);
                }
                showCloudPoint(vtkpoints);
            }
           
        }


        /// <summary>
        /// 停止投影按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnStopProjection_Click(object sender, EventArgs e)
        {
            mslclr.StopProjectionCLR();
        }
        private void tabPage1_Click(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }

}
