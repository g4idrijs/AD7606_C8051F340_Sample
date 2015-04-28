namespace USBXpress_TestPanel
{
    partial class SelectScreen
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
            this.comboBox_Device = new System.Windows.Forms.ComboBox();
            this.label_Device = new System.Windows.Forms.Label();
            this.button_Accept = new System.Windows.Forms.Button();
            this.button_Reject = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // comboBox_Device
            // 
            this.comboBox_Device.FormattingEnabled = true;
            this.comboBox_Device.Location = new System.Drawing.Point(104, 28);
            this.comboBox_Device.Name = "comboBox_Device";
            this.comboBox_Device.Size = new System.Drawing.Size(310, 21);
            this.comboBox_Device.TabIndex = 0;
            this.comboBox_Device.Text = "None Available";
            // 
            // label_Device
            // 
            this.label_Device.AutoSize = true;
            this.label_Device.Location = new System.Drawing.Point(12, 31);
            this.label_Device.Name = "label_Device";
            this.label_Device.Size = new System.Drawing.Size(75, 13);
            this.label_Device.TabIndex = 1;
            this.label_Device.Text = "Device Name:";
            // 
            // button_Accept
            // 
            this.button_Accept.Location = new System.Drawing.Point(104, 64);
            this.button_Accept.Name = "button_Accept";
            this.button_Accept.Size = new System.Drawing.Size(75, 23);
            this.button_Accept.TabIndex = 2;
            this.button_Accept.Text = "&OK";
            this.button_Accept.UseVisualStyleBackColor = true;
            this.button_Accept.Click += new System.EventHandler(this.button_Accept_Click);
            // 
            // button_Reject
            // 
            this.button_Reject.Location = new System.Drawing.Point(268, 64);
            this.button_Reject.Name = "button_Reject";
            this.button_Reject.Size = new System.Drawing.Size(75, 23);
            this.button_Reject.TabIndex = 3;
            this.button_Reject.Text = "&Cancel";
            this.button_Reject.UseVisualStyleBackColor = true;
            this.button_Reject.Click += new System.EventHandler(this.button_Reject_Click);
            // 
            // SelectScreen
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(426, 103);
            this.Controls.Add(this.button_Reject);
            this.Controls.Add(this.button_Accept);
            this.Controls.Add(this.label_Device);
            this.Controls.Add(this.comboBox_Device);
            this.Name = "SelectScreen";
            this.Text = "Select Device";
            this.Load += new System.EventHandler(this.SelectScreen_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox comboBox_Device;
        private System.Windows.Forms.Label label_Device;
        private System.Windows.Forms.Button button_Accept;
        private System.Windows.Forms.Button button_Reject;
    }
}

