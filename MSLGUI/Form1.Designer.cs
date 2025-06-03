namespace MSLGUI
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.panelCloudPoint = new System.Windows.Forms.Panel();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.btnStopProjection = new System.Windows.Forms.Button();
            this.btnOpenCamera = new System.Windows.Forms.Button();
            this.btnStartProjection = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.comboBoxProjectionType = new System.Windows.Forms.ComboBox();
            this.btnStartGrabbing = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.trackBarExposure = new System.Windows.Forms.TrackBar();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.panel1 = new System.Windows.Forms.Panel();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.btnChoseDirRegisration = new System.Windows.Forms.Button();
            this.btnStartRegisrating = new System.Windows.Forms.Button();
            this.labelRegisration = new System.Windows.Forms.Label();
            this.labelMerge = new System.Windows.Forms.Label();
            this.btnChooseDirMerge = new System.Windows.Forms.Button();
            this.btnStartMerging = new System.Windows.Forms.Button();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarExposure)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.tabPage2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.ItemSize = new System.Drawing.Size(58, 25);
            this.tabControl1.Location = new System.Drawing.Point(-3, 0);
            this.tabControl1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tabControl1.Multiline = true;
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1744, 1085);
            this.tabControl1.TabIndex = 1;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.panelCloudPoint);
            this.tabPage1.Controls.Add(this.groupBox1);
            this.tabPage1.Font = new System.Drawing.Font("宋体", 13F);
            this.tabPage1.Location = new System.Drawing.Point(4, 29);
            this.tabPage1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tabPage1.Size = new System.Drawing.Size(1736, 1052);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "扫描";
            this.tabPage1.UseVisualStyleBackColor = true;
            this.tabPage1.Click += new System.EventHandler(this.tabPage1_Click);
            // 
            // panelCloudPoint
            // 
            this.panelCloudPoint.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelCloudPoint.Location = new System.Drawing.Point(3, 4);
            this.panelCloudPoint.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.panelCloudPoint.Name = "panelCloudPoint";
            this.panelCloudPoint.Size = new System.Drawing.Size(1053, 1029);
            this.panelCloudPoint.TabIndex = 2;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.groupBox2);
            this.groupBox1.Controls.Add(this.pictureBox1);
            this.groupBox1.Font = new System.Drawing.Font("微软雅黑", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBox1.Location = new System.Drawing.Point(1063, 2);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox1.Size = new System.Drawing.Size(672, 1039);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "控制部分";
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.btnStopProjection);
            this.groupBox2.Controls.Add(this.btnOpenCamera);
            this.groupBox2.Controls.Add(this.btnStartProjection);
            this.groupBox2.Controls.Add(this.button1);
            this.groupBox2.Controls.Add(this.comboBoxProjectionType);
            this.groupBox2.Controls.Add(this.btnStartGrabbing);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.trackBarExposure);
            this.groupBox2.Location = new System.Drawing.Point(21, 575);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox2.Size = new System.Drawing.Size(639, 455);
            this.groupBox2.TabIndex = 10;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "控制模块";
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(47, 296);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(159, 54);
            this.label1.TabIndex = 3;
            this.label1.Text = "label1";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(223, 115);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(146, 23);
            this.label3.TabIndex = 6;
            this.label3.Text = "请先设置曝光！！";
            // 
            // btnStopProjection
            // 
            this.btnStopProjection.Font = new System.Drawing.Font("宋体", 9F);
            this.btnStopProjection.Location = new System.Drawing.Point(183, 226);
            this.btnStopProjection.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnStopProjection.Name = "btnStopProjection";
            this.btnStopProjection.Size = new System.Drawing.Size(107, 40);
            this.btnStopProjection.TabIndex = 9;
            this.btnStopProjection.Text = "停止投影";
            this.btnStopProjection.UseVisualStyleBackColor = true;
            this.btnStopProjection.Click += new System.EventHandler(this.btnStopProjection_Click);
            // 
            // btnOpenCamera
            // 
            this.btnOpenCamera.Font = new System.Drawing.Font("宋体", 9F);
            this.btnOpenCamera.Location = new System.Drawing.Point(52, 46);
            this.btnOpenCamera.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnOpenCamera.Name = "btnOpenCamera";
            this.btnOpenCamera.Size = new System.Drawing.Size(107, 40);
            this.btnOpenCamera.TabIndex = 0;
            this.btnOpenCamera.Text = "打开相机";
            this.btnOpenCamera.UseVisualStyleBackColor = true;
            this.btnOpenCamera.Click += new System.EventHandler(this.btnOpenCamera_Click);
            // 
            // btnStartProjection
            // 
            this.btnStartProjection.Font = new System.Drawing.Font("宋体", 9F);
            this.btnStartProjection.Location = new System.Drawing.Point(35, 226);
            this.btnStartProjection.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnStartProjection.Name = "btnStartProjection";
            this.btnStartProjection.Size = new System.Drawing.Size(107, 40);
            this.btnStartProjection.TabIndex = 8;
            this.btnStartProjection.Text = "开始投影";
            this.btnStartProjection.UseVisualStyleBackColor = true;
            this.btnStartProjection.Click += new System.EventHandler(this.btnStartProjection_Click);
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button1.Location = new System.Drawing.Point(356, 49);
            this.button1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(107, 36);
            this.button1.TabIndex = 1;
            this.button1.Text = "关闭相机";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // comboBoxProjectionType
            // 
            this.comboBoxProjectionType.Font = new System.Drawing.Font("微软雅黑", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.comboBoxProjectionType.FormattingEnabled = true;
            this.comboBoxProjectionType.Items.AddRange(new object[] {
            "单幅扫描",
            "拼接扫描",
            "全白图案",
            "静态条纹"});
            this.comboBoxProjectionType.Location = new System.Drawing.Point(335, 166);
            this.comboBoxProjectionType.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.comboBoxProjectionType.Name = "comboBoxProjectionType";
            this.comboBoxProjectionType.Size = new System.Drawing.Size(127, 29);
            this.comboBoxProjectionType.TabIndex = 7;
            this.comboBoxProjectionType.Text = "投影类型";
            this.comboBoxProjectionType.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // btnStartGrabbing
            // 
            this.btnStartGrabbing.Font = new System.Drawing.Font("宋体", 9F);
            this.btnStartGrabbing.Location = new System.Drawing.Point(203, 46);
            this.btnStartGrabbing.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnStartGrabbing.Name = "btnStartGrabbing";
            this.btnStartGrabbing.Size = new System.Drawing.Size(107, 40);
            this.btnStartGrabbing.TabIndex = 2;
            this.btnStartGrabbing.Text = "开始采集";
            this.btnStartGrabbing.UseVisualStyleBackColor = true;
            this.btnStartGrabbing.Click += new System.EventHandler(this.button2_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(49, 115);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(163, 23);
            this.label2.TabIndex = 5;
            this.label2.Text = "相机和投影仪曝光：";
            // 
            // trackBarExposure
            // 
            this.trackBarExposure.Location = new System.Drawing.Point(52, 146);
            this.trackBarExposure.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.trackBarExposure.Name = "trackBarExposure";
            this.trackBarExposure.Size = new System.Drawing.Size(257, 56);
            this.trackBarExposure.TabIndex = 4;
            this.trackBarExposure.Scroll += new System.EventHandler(this.trackBarExposure_Scroll);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBox1.Location = new System.Drawing.Point(21, 28);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(628, 541);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.groupBox3);
            this.tabPage2.Controls.Add(this.panel1);
            this.tabPage2.Location = new System.Drawing.Point(4, 29);
            this.tabPage2.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tabPage2.Size = new System.Drawing.Size(1736, 1052);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "点云拼接";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.Location = new System.Drawing.Point(7, 7);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1183, 1002);
            this.panel1.TabIndex = 0;
            // 
            // groupBox3
            // 
            this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox3.Controls.Add(this.richTextBox1);
            this.groupBox3.Controls.Add(this.groupBox5);
            this.groupBox3.Controls.Add(this.groupBox4);
            this.groupBox3.Location = new System.Drawing.Point(1196, 7);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(537, 1002);
            this.groupBox3.TabIndex = 1;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "控制部分";
            // 
            // groupBox4
            // 
            this.groupBox4.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox4.Controls.Add(this.labelRegisration);
            this.groupBox4.Controls.Add(this.btnStartRegisrating);
            this.groupBox4.Controls.Add(this.btnChoseDirRegisration);
            this.groupBox4.Location = new System.Drawing.Point(6, 24);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(531, 268);
            this.groupBox4.TabIndex = 0;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "点云拼接";
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.labelMerge);
            this.groupBox5.Controls.Add(this.btnStartMerging);
            this.groupBox5.Controls.Add(this.btnChooseDirMerge);
            this.groupBox5.Location = new System.Drawing.Point(6, 298);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(531, 293);
            this.groupBox5.TabIndex = 0;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "点云融合";
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(6, 587);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(531, 415);
            this.richTextBox1.TabIndex = 1;
            this.richTextBox1.Text = "";
            // 
            // btnChoseDirRegisration
            // 
            this.btnChoseDirRegisration.Location = new System.Drawing.Point(348, 122);
            this.btnChoseDirRegisration.Name = "btnChoseDirRegisration";
            this.btnChoseDirRegisration.Size = new System.Drawing.Size(92, 37);
            this.btnChoseDirRegisration.TabIndex = 0;
            this.btnChoseDirRegisration.Text = "选择路径";
            this.btnChoseDirRegisration.UseVisualStyleBackColor = true;
            // 
            // btnStartRegisrating
            // 
            this.btnStartRegisrating.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnStartRegisrating.Location = new System.Drawing.Point(348, 179);
            this.btnStartRegisrating.Name = "btnStartRegisrating";
            this.btnStartRegisrating.Size = new System.Drawing.Size(92, 37);
            this.btnStartRegisrating.TabIndex = 0;
            this.btnStartRegisrating.Text = "开始拼接";
            this.btnStartRegisrating.UseVisualStyleBackColor = true;
            // 
            // labelRegisration
            // 
            this.labelRegisration.Location = new System.Drawing.Point(23, 56);
            this.labelRegisration.Name = "labelRegisration";
            this.labelRegisration.Size = new System.Drawing.Size(288, 103);
            this.labelRegisration.TabIndex = 1;
            this.labelRegisration.Text = "label4";
            // 
            // labelMerge
            // 
            this.labelMerge.Location = new System.Drawing.Point(23, 70);
            this.labelMerge.Name = "labelMerge";
            this.labelMerge.Size = new System.Drawing.Size(288, 103);
            this.labelMerge.TabIndex = 1;
            this.labelMerge.Text = "label4";
            // 
            // btnChooseDirMerge
            // 
            this.btnChooseDirMerge.Location = new System.Drawing.Point(348, 95);
            this.btnChooseDirMerge.Name = "btnChooseDirMerge";
            this.btnChooseDirMerge.Size = new System.Drawing.Size(92, 37);
            this.btnChooseDirMerge.TabIndex = 0;
            this.btnChooseDirMerge.Text = "选择路径";
            this.btnChooseDirMerge.UseVisualStyleBackColor = true;
            // 
            // btnStartMerging
            // 
            this.btnStartMerging.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnStartMerging.Location = new System.Drawing.Point(348, 152);
            this.btnStartMerging.Name = "btnStartMerging";
            this.btnStartMerging.Size = new System.Drawing.Size(92, 37);
            this.btnStartMerging.TabIndex = 0;
            this.btnStartMerging.Text = "开始拼接";
            this.btnStartMerging.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1741, 1084);
            this.Controls.Add(this.tabControl1);
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarExposure)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.tabPage2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox5.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Panel panelCloudPoint;
        private System.Windows.Forms.Button btnStopProjection;
        private System.Windows.Forms.Button btnStartProjection;
        private System.Windows.Forms.ComboBox comboBoxProjectionType;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TrackBar trackBarExposure;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnStartGrabbing;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button btnOpenCamera;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Label labelMerge;
        private System.Windows.Forms.Button btnStartMerging;
        private System.Windows.Forms.Button btnChooseDirMerge;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label labelRegisration;
        private System.Windows.Forms.Button btnStartRegisrating;
        private System.Windows.Forms.Button btnChoseDirRegisration;
    }
}

