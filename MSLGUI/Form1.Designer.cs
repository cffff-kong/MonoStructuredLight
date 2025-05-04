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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnStopProjection = new System.Windows.Forms.Button();
            this.btnStartProjection = new System.Windows.Forms.Button();
            this.comboBoxProjectionType = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.trackBarExposure = new System.Windows.Forms.TrackBar();
            this.label1 = new System.Windows.Forms.Label();
            this.btnStartGrabbing = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.btnOpenCamera = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarExposure)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
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
            this.tabControl1.Margin = new System.Windows.Forms.Padding(4);
            this.tabControl1.Multiline = true;
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1256, 862);
            this.tabControl1.TabIndex = 1;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.groupBox1);
            this.tabPage1.Controls.Add(this.pictureBox1);
            this.tabPage1.Font = new System.Drawing.Font("宋体", 13F);
            this.tabPage1.Location = new System.Drawing.Point(4, 29);
            this.tabPage1.Margin = new System.Windows.Forms.Padding(4);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(4);
            this.tabPage1.Size = new System.Drawing.Size(1248, 829);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "扫描";
            this.tabPage1.UseVisualStyleBackColor = true;
            this.tabPage1.Click += new System.EventHandler(this.tabPage1_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.btnStopProjection);
            this.groupBox1.Controls.Add(this.btnStartProjection);
            this.groupBox1.Controls.Add(this.comboBoxProjectionType);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.trackBarExposure);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.btnStartGrabbing);
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.Controls.Add(this.btnOpenCamera);
            this.groupBox1.Font = new System.Drawing.Font("微软雅黑", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBox1.Location = new System.Drawing.Point(893, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(352, 807);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "控制部分";
            // 
            // btnStopProjection
            // 
            this.btnStopProjection.Font = new System.Drawing.Font("宋体", 9F);
            this.btnStopProjection.Location = new System.Drawing.Point(196, 389);
            this.btnStopProjection.Name = "btnStopProjection";
            this.btnStopProjection.Size = new System.Drawing.Size(120, 48);
            this.btnStopProjection.TabIndex = 9;
            this.btnStopProjection.Text = "停止投影";
            this.btnStopProjection.UseVisualStyleBackColor = true;
            this.btnStopProjection.Click += new System.EventHandler(this.btnStopProjection_Click);
            // 
            // btnStartProjection
            // 
            this.btnStartProjection.Font = new System.Drawing.Font("宋体", 9F);
            this.btnStartProjection.Location = new System.Drawing.Point(46, 389);
            this.btnStartProjection.Name = "btnStartProjection";
            this.btnStartProjection.Size = new System.Drawing.Size(120, 48);
            this.btnStartProjection.TabIndex = 8;
            this.btnStartProjection.Text = "开始投影";
            this.btnStartProjection.UseVisualStyleBackColor = true;
            this.btnStartProjection.Click += new System.EventHandler(this.btnStartProjection_Click);
            // 
            // comboBoxProjectionType
            // 
            this.comboBoxProjectionType.Font = new System.Drawing.Font("微软雅黑", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.comboBoxProjectionType.FormattingEnabled = true;
            this.comboBoxProjectionType.Items.AddRange(new object[] {
            "扫描模式",
            "全白图案",
            "静态条纹"});
            this.comboBoxProjectionType.Location = new System.Drawing.Point(24, 307);
            this.comboBoxProjectionType.Name = "comboBoxProjectionType";
            this.comboBoxProjectionType.Size = new System.Drawing.Size(142, 27);
            this.comboBoxProjectionType.TabIndex = 7;
            this.comboBoxProjectionType.Text = "投影类型";
            this.comboBoxProjectionType.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(145, 177);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(0, 19);
            this.label3.TabIndex = 6;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 177);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(126, 19);
            this.label2.TabIndex = 5;
            this.label2.Text = "相机和投影仪曝光：";
            // 
            // trackBarExposure
            // 
            this.trackBarExposure.Location = new System.Drawing.Point(17, 214);
            this.trackBarExposure.Name = "trackBarExposure";
            this.trackBarExposure.Size = new System.Drawing.Size(289, 45);
            this.trackBarExposure.TabIndex = 4;
            this.trackBarExposure.Scroll += new System.EventHandler(this.trackBarExposure_Scroll);
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(58, 609);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(179, 64);
            this.label1.TabIndex = 3;
            this.label1.Text = "label1";
            // 
            // btnStartGrabbing
            // 
            this.btnStartGrabbing.Font = new System.Drawing.Font("宋体", 9F);
            this.btnStartGrabbing.Location = new System.Drawing.Point(186, 50);
            this.btnStartGrabbing.Name = "btnStartGrabbing";
            this.btnStartGrabbing.Size = new System.Drawing.Size(120, 48);
            this.btnStartGrabbing.TabIndex = 2;
            this.btnStartGrabbing.Text = "开始采集";
            this.btnStartGrabbing.UseVisualStyleBackColor = true;
            this.btnStartGrabbing.Click += new System.EventHandler(this.button2_Click);
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button1.Location = new System.Drawing.Point(46, 114);
            this.button1.Margin = new System.Windows.Forms.Padding(4);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(120, 44);
            this.button1.TabIndex = 1;
            this.button1.Text = "关闭相机";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // btnOpenCamera
            // 
            this.btnOpenCamera.Font = new System.Drawing.Font("宋体", 9F);
            this.btnOpenCamera.Location = new System.Drawing.Point(46, 50);
            this.btnOpenCamera.Name = "btnOpenCamera";
            this.btnOpenCamera.Size = new System.Drawing.Size(120, 48);
            this.btnOpenCamera.TabIndex = 0;
            this.btnOpenCamera.Text = "打开相机";
            this.btnOpenCamera.UseVisualStyleBackColor = true;
            this.btnOpenCamera.Click += new System.EventHandler(this.btnOpenCamera_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBox1.Location = new System.Drawing.Point(3, 3);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(884, 508);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // tabPage2
            // 
            this.tabPage2.Location = new System.Drawing.Point(4, 29);
            this.tabPage2.Margin = new System.Windows.Forms.Padding(4);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(4);
            this.tabPage2.Size = new System.Drawing.Size(1248, 829);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "待定";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1253, 861);
            this.Controls.Add(this.tabControl1);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarExposure)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnOpenCamera;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button btnStartGrabbing;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TrackBar trackBarExposure;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox comboBoxProjectionType;
        private System.Windows.Forms.Button btnStopProjection;
        private System.Windows.Forms.Button btnStartProjection;
    }
}

