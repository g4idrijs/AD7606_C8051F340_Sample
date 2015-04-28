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
            this.button_Exit = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.checkBox_LED1 = new System.Windows.Forms.CheckBox();
            this.checkBox_LED2 = new System.Windows.Forms.CheckBox();
            this.groupBoxLEDs = new System.Windows.Forms.GroupBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.groupBoxLEDs.SuspendLayout();
            this.SuspendLayout();
            // 
            // button_Exit
            // 
            this.button_Exit.Location = new System.Drawing.Point(545, 350);
            this.button_Exit.Name = "button_Exit";
            this.button_Exit.Size = new System.Drawing.Size(75, 21);
            this.button_Exit.TabIndex = 8;
            this.button_Exit.Text = "Exit";
            this.button_Exit.UseVisualStyleBackColor = true;
            this.button_Exit.Click += new System.EventHandler(this.button_Exit_Click);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // checkBox_LED1
            // 
            this.checkBox_LED1.AutoSize = true;
            this.checkBox_LED1.Location = new System.Drawing.Point(26, 18);
            this.checkBox_LED1.Name = "checkBox_LED1";
            this.checkBox_LED1.Size = new System.Drawing.Size(48, 16);
            this.checkBox_LED1.TabIndex = 0;
            this.checkBox_LED1.Text = "LED1";
            this.checkBox_LED1.UseVisualStyleBackColor = true;
            // 
            // checkBox_LED2
            // 
            this.checkBox_LED2.AutoSize = true;
            this.checkBox_LED2.Location = new System.Drawing.Point(26, 39);
            this.checkBox_LED2.Name = "checkBox_LED2";
            this.checkBox_LED2.Size = new System.Drawing.Size(48, 16);
            this.checkBox_LED2.TabIndex = 1;
            this.checkBox_LED2.Text = "LED2";
            this.checkBox_LED2.UseVisualStyleBackColor = true;
            // 
            // groupBoxLEDs
            // 
            this.groupBoxLEDs.Controls.Add(this.checkBox_LED2);
            this.groupBoxLEDs.Controls.Add(this.checkBox_LED1);
            this.groupBoxLEDs.Location = new System.Drawing.Point(522, 5);
            this.groupBoxLEDs.Name = "groupBoxLEDs";
            this.groupBoxLEDs.Size = new System.Drawing.Size(108, 67);
            this.groupBoxLEDs.TabIndex = 2;
            this.groupBoxLEDs.TabStop = false;
            this.groupBoxLEDs.Text = "LED States";
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
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(545, 310);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 10;
            this.button1.Text = "Stop";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(545, 270);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 11;
            this.button2.Text = "Start";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // TestPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(632, 383);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.button_Exit);
            this.Controls.Add(this.groupBoxLEDs);
            this.Name = "TestPanel";
            this.Text = "TestPanel";
            this.groupBoxLEDs.ResumeLayout(false);
            this.groupBoxLEDs.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button_Exit;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.CheckBox checkBox_LED1;
        private System.Windows.Forms.CheckBox checkBox_LED2;
        private System.Windows.Forms.GroupBox groupBoxLEDs;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
    }
}