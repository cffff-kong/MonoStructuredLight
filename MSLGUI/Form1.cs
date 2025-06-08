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
using Microsoft.WindowsAPICodePack.Dialogs;
using Microsoft.WindowsAPICodePack.Dialogs.Controls;
using System.IO;

namespace MSLGUI
{
    public partial class Form1 : Form
    {
        private MSLCLR mslclr = new MSLCLR();
        int g_is_preview_real_time_diaply = 0;
        int g_exposure_value=0;
        RenderWindowControl renderWindowControl;
        RenderWindowControl renderWindowRegistration;

        List<string> m_pcd_regisration_files; // 点云配准文件列表

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
            renderWindowControl.Parent = panelCloudPoint;	
            renderWindowControl.Dock = DockStyle.Fill;	/* 指定显示方式为铺满 */

            renderWindowRegistration= new RenderWindowControl();
            renderWindowRegistration.Parent = panel1;
            renderWindowRegistration.Dock = DockStyle.Fill;
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
            if (g_exposure_value==0)
            {
                MessageBox.Show("请先设置曝光时间！");
                return;
            }
            int exposure_time = g_exposure_value;
            int projection_period = exposure_time < 500000 ? 500000 : exposure_time;

            int projection_type = comboBoxProjectionType.SelectedIndex;

            await Task.Run(() =>
            {
                mslclr.StartProjectionCLR(projection_type, exposure_time, projection_period);
            });
            Console.WriteLine("project done\n");

            if (projection_type == 0|| projection_type == 1)
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
        /// <summary>
        /// 选择需要配准的点云文件夹
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnChoseDirRegisration_Click(object sender, EventArgs e)
        {
            CommonOpenFileDialog dialog = new CommonOpenFileDialog();
            dialog.IsFolderPicker = true;

            if (dialog.ShowDialog() == CommonFileDialogResult.Ok)
            {
                string selectedPath = dialog.FileName;
                labelRegisration.Text = selectedPath;

                // 获取所有 .pcd 文件
                string[] pcdFiles = Directory.GetFiles(selectedPath, "*.pcd");
                // 按文件名解析为 DateTime 进行排序
                m_pcd_regisration_files = pcdFiles
                    .Select(path => new
                    {
                        FilePath = path,
                        FileTime = ParseTimeFromFileName(Path.GetFileNameWithoutExtension(path))
                    })
                    .Where(x => x.FileTime != null) // 过滤解析失败的文件
                    .OrderBy(x => x.FileTime)
                    .Select(x => x.FilePath)
                    .ToList();
                // 示例输出
                foreach (var file in m_pcd_regisration_files)
                {
                    Console.WriteLine(file);
                    richTextBox1.AppendText("读取到点云" + file + "\n");
                }
                int count = m_pcd_regisration_files.Count;
                richTextBox1.AppendText("共读取到" + count + "个点云文件\n");

            }
        }
        // 解析文件名为 DateTime（例如：20250601_165349）
        private DateTime? ParseTimeFromFileName(string fileName)
        {
            if (DateTime.TryParseExact(
                fileName,
                "yyyyMMdd_HHmmss",
                System.Globalization.CultureInfo.InvariantCulture,
                System.Globalization.DateTimeStyles.None,
                out DateTime result))
            {
                return result;
            }
            return null;
        }
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
          
        }
        public void ShowRegisrationCloudPoint(vtkPoints points, int index, float size = 1f)
        {
            // 1. 生成不同颜色 (简单示例：用index生成颜色，或者你自己定义颜色数组)
            double r = (0.3 + 0.7 * ((index * 37) % 100) / 100.0); // 0.3~1.0 变化
            double g = (0.3 + 0.7 * ((index * 53) % 100) / 100.0);
            double b = (0.3 + 0.7 * ((index * 97) % 100) / 100.0);

            vtkPolyData polydata = vtkPolyData.New();
            polydata.SetPoints(points);

            vtkVertexGlyphFilter glyphFilter = vtkVertexGlyphFilter.New();
            glyphFilter.SetInputConnection(polydata.GetProducerPort());
            glyphFilter.Update();

            vtkPolyDataMapper mapper = vtkPolyDataMapper.New();
            mapper.SetInputConnection(glyphFilter.GetOutputPort());

            vtkActor actor = vtkActor.New();
            actor.SetMapper(mapper);
            actor.GetProperty().SetPointSize(size);
            actor.GetProperty().SetColor(r, g, b);

            // 获取注册用的渲染器
            vtkRenderer render = renderWindowRegistration.RenderWindow.GetRenderers().GetFirstRenderer();

            // 不移除旧的Actor，直接添加新Actor
            render.AddActor(actor);
            renderWindowRegistration.RenderWindow.Render(); // 立即刷新，清空显示

        }

        private async void btnStartRegisrating_Click(object sender, EventArgs e)
        {
            if (m_pcd_regisration_files.Count == 0)
            {
                MessageBox.Show("请先选择需要配准的点云文件夹！");
                return;
            }
            // 先在UI线程清除所有点云
            vtkRenderer render = renderWindowRegistration.RenderWindow.GetRenderers().GetFirstRenderer();
            render.RemoveAllViewProps();
            renderWindowRegistration.RenderWindow.Render(); // 立即刷新，清空显示

            mslclr.InitRegisration();

            await Task.Run(() =>
            {
                int num = 1;
                int total_num = m_pcd_regisration_files.Count;

                foreach (var file in m_pcd_regisration_files)
                {
                    this.Invoke((MethodInvoker)delegate
                    {
                        richTextBox1.AppendText("[" + num + "/" + total_num + "] 开始处理点云 " + file + "\n");
                    });

                    List<mslclrimpoort.Point3f> points = mslclr.RegisrationCloudPointCLR(file);

                    this.Invoke((MethodInvoker)delegate
                    {
                        richTextBox1.AppendText("[" + num + "/" + total_num + "] 点云 " + file + " 拼接完成\n");
                    });

                    vtkPoints vtkpoints = vtkPoints.New();
                    foreach (mslclrimpoort.Point3f pt in points)
                    {
                        vtkpoints.InsertNextPoint(pt.x, pt.y, pt.z);
                    }
                    // 显示点云，注意这里调用时仍在非UI线程，ShowRegisrationCloudPoint里调用了UI线程Render
                    this.Invoke((MethodInvoker)(() => ShowRegisrationCloudPoint(vtkpoints, num)));
                    num++;
                }
            });

            this.Invoke((MethodInvoker)(() => richTextBox1.AppendText("所有点云都拼接完成！\n")));
        }



    }

}
