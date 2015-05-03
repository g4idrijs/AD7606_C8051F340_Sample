namespace USBXpress_TestPanel
{
    partial class TestPanel
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.btn_Exit = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.checkBox_LED = new System.Windows.Forms.CheckBox();
            this.groupBoxLED = new System.Windows.Forms.GroupBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.btn_Stop = new System.Windows.Forms.Button();
            this.btn_Start = new System.Windows.Forms.Button();
            this.btn_Clear = new System.Windows.Forms.Button();
            this.groupBoxLED.SuspendLayout();
            this.SuspendLayout();
            // 
            // btn_Exit
            // 
            this.btn_Exit.Location = new System.Drawing.Point(228, 398);
            this.btn_Exit.Name = "btn_Exit";
            this.btn_Exit.Size = new System.Drawing.Size(75, 21);
            this.btn_Exit.TabIndex = 8;
            this.btn_Exit.Text = "Exit";
            this.btn_Exit.UseVisualStyleBackColor = true;
            this.btn_Exit.Click += new System.EventHandler(this.btn_Exit_Click);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 1;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // checkBox_LED
            // 
            this.checkBox_LED.AutoSize = true;
            this.checkBox_LED.Location = new System.Drawing.Point(26, 18);
            this.checkBox_LED.Name = "checkBox_LED";
            this.checkBox_LED.Size = new System.Drawing.Size(42, 16);
            this.checkBox_LED.TabIndex = 0;
            this.checkBox_LED.Text = "LED";
            this.checkBox_LED.UseVisualStyleBackColor = true;
            // 
            // groupBoxLED
            // 
            this.groupBoxLED.Controls.Add(this.checkBox_LED);
            this.groupBoxLED.Location = new System.Drawing.Point(3, 377);
            this.groupBoxLED.Name = "groupBoxLED";
            this.groupBoxLED.Size = new System.Drawing.Size(108, 42);
            this.groupBoxLED.TabIndex = 2;
            this.groupBoxLED.TabStop = false;
            this.groupBoxLED.Text = "LED States";
            // 
            // textBox1
            // 
            this.textBox1.AllowDrop = true;
            this.textBox1.Location = new System.Drawing.Point(3, 5);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox1.Size = new System.Drawing.Size(513, 366);
            this.textBox1.TabIndex = 9;
            // 
            // btn_Stop
            // 
            this.btn_Stop.Location = new System.Drawing.Point(228, 377);
            this.btn_Stop.Name = "btn_Stop";
            this.btn_Stop.Size = new System.Drawing.Size(75, 23);
            this.btn_Stop.TabIndex = 10;
            this.btn_Stop.Text = "Stop";
            this.btn_Stop.UseVisualStyleBackColor = true;
            this.btn_Stop.Click += new System.EventHandler(this.btn_Stop_Click);
            // 
            // btn_Start
            // 
            this.btn_Start.Location = new System.Drawing.Point(130, 377);
            this.btn_Start.Name = "btn_Start";
            this.btn_Start.Size = new System.Drawing.Size(75, 23);
            this.btn_Start.TabIndex = 11;
            this.btn_Start.Text = "Start";
            this.btn_Start.UseVisualStyleBackColor = true;
            this.btn_Start.Click += new System.EventHandler(this.btn_Start_Click);
            // 
            // btn_Clear
            // 
            this.btn_Clear.Location = new System.Drawing.Point(130, 398);
            this.btn_Clear.Name = "btn_Clear";
            this.btn_Clear.Size = new System.Drawing.Size(75, 23);
            this.btn_Clear.TabIndex = 12;
            this.btn_Clear.Text = "Clear";
            this.btn_Clear.UseVisualStyleBackColor = true;
            this.btn_Clear.Click += new System.EventHandler(this.btn_Clear_Click);
            // 
            // TestPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(525, 433);
            this.Controls.Add(this.btn_Clear);
            this.Controls.Add(this.btn_Start);
            this.Controls.Add(this.btn_Stop);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.btn_Exit);
            this.Controls.Add(this.groupBoxLED);
            this.Name = "TestPanel";
            this.Text = "TestPanel";
            this.groupBoxLED.ResumeLayout(false);
            this.groupBoxLED.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btn_Exit;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.CheckBox checkBox_LED;
        private System.Windows.Forms.GroupBox groupBoxLED;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button btn_Stop;
        private System.Windows.Forms.Button btn_Start;
        private System.Windows.Forms.Button btn_Clear;
    }
}