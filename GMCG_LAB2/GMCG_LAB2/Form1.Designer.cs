namespace GMCG_LAB2
{
    partial class Form1
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
            this.button1 = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.pictureBox_2d = new System.Windows.Forms.PictureBox();
            this.ButtonSize = new System.Windows.Forms.Button();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.button3 = new System.Windows.Forms.Button();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.button2 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.button6 = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.button7 = new System.Windows.Forms.Button();
            this.textBox6 = new System.Windows.Forms.TextBox();
            this.textBox7 = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.pictureBox_3d = new System.Windows.Forms.PictureBox();
            this.AddPoint3D = new System.Windows.Forms.Button();
            this.textBox_Coords_X_3D = new System.Windows.Forms.TextBox();
            this.textBox_Coords_Y_3D = new System.Windows.Forms.TextBox();
            this.button_Draw_Points_3D = new System.Windows.Forms.Button();
            this.button_Rotation3D = new System.Windows.Forms.Button();
            this.textBox_for_Rotation3D = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_2d)).BeginInit();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_3d)).BeginInit();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(8, 44);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(213, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "Добавить точку";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(118, 73);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(103, 20);
            this.textBox1.TabIndex = 1;
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(118, 99);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(103, 20);
            this.textBox2.TabIndex = 2;
            this.textBox2.TextChanged += new System.EventHandler(this.textBox2_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 20F);
            this.label1.Location = new System.Drawing.Point(227, 6);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(254, 31);
            this.label1.TabIndex = 3;
            this.label1.Text = "Координаты точек:";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // pictureBox_2d
            // 
            this.pictureBox_2d.Location = new System.Drawing.Point(487, 6);
            this.pictureBox_2d.Name = "pictureBox_2d";
            this.pictureBox_2d.Size = new System.Drawing.Size(1113, 661);
            this.pictureBox_2d.TabIndex = 4;
            this.pictureBox_2d.TabStop = false;
            this.pictureBox_2d.Click += new System.EventHandler(this.pictureBox1_Click);
            // 
            // ButtonSize
            // 
            this.ButtonSize.Location = new System.Drawing.Point(8, 183);
            this.ButtonSize.Name = "ButtonSize";
            this.ButtonSize.Size = new System.Drawing.Size(213, 23);
            this.ButtonSize.TabIndex = 5;
            this.ButtonSize.Text = "Растяжение/Сжатие";
            this.ButtonSize.UseVisualStyleBackColor = true;
            this.ButtonSize.Click += new System.EventHandler(this.button2_Click);
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(118, 212);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(103, 20);
            this.textBox3.TabIndex = 6;
            this.textBox3.Text = "1";
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(118, 157);
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(103, 20);
            this.textBox4.TabIndex = 7;
            this.textBox4.Text = "0";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(8, 128);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(213, 23);
            this.button3.TabIndex = 8;
            this.button3.Text = "Поворт";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // textBox5
            // 
            this.textBox5.Location = new System.Drawing.Point(118, 238);
            this.textBox5.Name = "textBox5";
            this.textBox5.Size = new System.Drawing.Size(103, 20);
            this.textBox5.TabIndex = 9;
            this.textBox5.Text = "1";
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(8, 6);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(213, 23);
            this.button2.TabIndex = 10;
            this.button2.Text = "Выполнить преобразования";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click_1);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(8, 271);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(213, 23);
            this.button4.TabIndex = 11;
            this.button4.Text = "Отражение относительно оси Ox";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(8, 300);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(213, 23);
            this.button5.TabIndex = 12;
            this.button5.Text = "Отражение относительно оси Oy";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(8, 329);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(213, 23);
            this.button6.TabIndex = 13;
            this.button6.Text = "Отражение относительно оси Oxy";
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.3F);
            this.label2.Location = new System.Drawing.Point(5, 217);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(102, 15);
            this.label2.TabIndex = 14;
            this.label2.Text = "Коэффициент α";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.3F);
            this.label3.Location = new System.Drawing.Point(5, 243);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(102, 15);
            this.label3.TabIndex = 15;
            this.label3.Text = "Коэффициент β";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.3F);
            this.label4.Location = new System.Drawing.Point(5, 160);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(46, 15);
            this.label4.TabIndex = 16;
            this.label4.Text = "Угол φ";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.3F);
            this.label5.Location = new System.Drawing.Point(5, 78);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(87, 15);
            this.label5.TabIndex = 17;
            this.label5.Text = "Координата x";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.3F);
            this.label6.Location = new System.Drawing.Point(5, 104);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(86, 15);
            this.label6.TabIndex = 18;
            this.label6.Text = "Координата y";
            // 
            // button7
            // 
            this.button7.Location = new System.Drawing.Point(8, 358);
            this.button7.Name = "button7";
            this.button7.Size = new System.Drawing.Size(213, 23);
            this.button7.TabIndex = 19;
            this.button7.Text = "Перенос (сдвиг)";
            this.button7.UseVisualStyleBackColor = true;
            this.button7.Click += new System.EventHandler(this.button7_Click);
            // 
            // textBox6
            // 
            this.textBox6.Location = new System.Drawing.Point(118, 387);
            this.textBox6.Name = "textBox6";
            this.textBox6.Size = new System.Drawing.Size(100, 20);
            this.textBox6.TabIndex = 20;
            this.textBox6.Text = "0";
            // 
            // textBox7
            // 
            this.textBox7.Location = new System.Drawing.Point(118, 413);
            this.textBox7.Name = "textBox7";
            this.textBox7.Size = new System.Drawing.Size(100, 20);
            this.textBox7.TabIndex = 21;
            this.textBox7.Text = "0";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.3F);
            this.label7.Location = new System.Drawing.Point(5, 418);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(102, 15);
            this.label7.TabIndex = 22;
            this.label7.Text = "Коэффициент μ";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.3F);
            this.label8.Location = new System.Drawing.Point(5, 392);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(101, 15);
            this.label8.TabIndex = 23;
            this.label8.Text = "Коэффициент λ";
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(0, 3);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1614, 700);
            this.tabControl1.TabIndex = 24;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.button2);
            this.tabPage1.Controls.Add(this.pictureBox_2d);
            this.tabPage1.Controls.Add(this.label8);
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Controls.Add(this.button1);
            this.tabPage1.Controls.Add(this.label7);
            this.tabPage1.Controls.Add(this.textBox1);
            this.tabPage1.Controls.Add(this.textBox7);
            this.tabPage1.Controls.Add(this.textBox2);
            this.tabPage1.Controls.Add(this.textBox6);
            this.tabPage1.Controls.Add(this.ButtonSize);
            this.tabPage1.Controls.Add(this.button7);
            this.tabPage1.Controls.Add(this.textBox3);
            this.tabPage1.Controls.Add(this.label6);
            this.tabPage1.Controls.Add(this.textBox4);
            this.tabPage1.Controls.Add(this.label5);
            this.tabPage1.Controls.Add(this.button3);
            this.tabPage1.Controls.Add(this.label4);
            this.tabPage1.Controls.Add(this.textBox5);
            this.tabPage1.Controls.Add(this.label3);
            this.tabPage1.Controls.Add(this.button4);
            this.tabPage1.Controls.Add(this.label2);
            this.tabPage1.Controls.Add(this.button5);
            this.tabPage1.Controls.Add(this.button6);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(1606, 674);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "tabPage1";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.textBox_for_Rotation3D);
            this.tabPage2.Controls.Add(this.button_Rotation3D);
            this.tabPage2.Controls.Add(this.button_Draw_Points_3D);
            this.tabPage2.Controls.Add(this.textBox_Coords_Y_3D);
            this.tabPage2.Controls.Add(this.textBox_Coords_X_3D);
            this.tabPage2.Controls.Add(this.AddPoint3D);
            this.tabPage2.Controls.Add(this.pictureBox_3d);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(1606, 674);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "tabPage2";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // pictureBox_3d
            // 
            this.pictureBox_3d.Location = new System.Drawing.Point(481, 6);
            this.pictureBox_3d.Name = "pictureBox_3d";
            this.pictureBox_3d.Size = new System.Drawing.Size(1119, 662);
            this.pictureBox_3d.TabIndex = 0;
            this.pictureBox_3d.TabStop = false;
            // 
            // AddPoint3D
            // 
            this.AddPoint3D.Location = new System.Drawing.Point(8, 6);
            this.AddPoint3D.Name = "AddPoint3D";
            this.AddPoint3D.Size = new System.Drawing.Size(197, 23);
            this.AddPoint3D.TabIndex = 1;
            this.AddPoint3D.Text = "Добавить точку";
            this.AddPoint3D.UseVisualStyleBackColor = true;
            this.AddPoint3D.Click += new System.EventHandler(this.AddPoint3D_Click);
            // 
            // textBox_Coords_X_3D
            // 
            this.textBox_Coords_X_3D.Location = new System.Drawing.Point(105, 35);
            this.textBox_Coords_X_3D.Name = "textBox_Coords_X_3D";
            this.textBox_Coords_X_3D.Size = new System.Drawing.Size(100, 20);
            this.textBox_Coords_X_3D.TabIndex = 2;
            // 
            // textBox_Coords_Y_3D
            // 
            this.textBox_Coords_Y_3D.Location = new System.Drawing.Point(104, 62);
            this.textBox_Coords_Y_3D.Name = "textBox_Coords_Y_3D";
            this.textBox_Coords_Y_3D.Size = new System.Drawing.Size(100, 20);
            this.textBox_Coords_Y_3D.TabIndex = 3;
            // 
            // button_Draw_Points_3D
            // 
            this.button_Draw_Points_3D.Location = new System.Drawing.Point(8, 88);
            this.button_Draw_Points_3D.Name = "button_Draw_Points_3D";
            this.button_Draw_Points_3D.Size = new System.Drawing.Size(197, 23);
            this.button_Draw_Points_3D.TabIndex = 4;
            this.button_Draw_Points_3D.Text = "Отрисовать";
            this.button_Draw_Points_3D.UseVisualStyleBackColor = true;
            this.button_Draw_Points_3D.Click += new System.EventHandler(this.button_Draw_Points_3D_Click);
            // 
            // button_Rotation3D
            // 
            this.button_Rotation3D.Location = new System.Drawing.Point(129, 149);
            this.button_Rotation3D.Name = "button_Rotation3D";
            this.button_Rotation3D.Size = new System.Drawing.Size(75, 23);
            this.button_Rotation3D.TabIndex = 5;
            this.button_Rotation3D.Text = "Поворот";
            this.button_Rotation3D.UseVisualStyleBackColor = true;
            this.button_Rotation3D.Click += new System.EventHandler(this.button_Rotation3D_Click);
            // 
            // textBox_for_Rotation3D
            // 
            this.textBox_for_Rotation3D.Location = new System.Drawing.Point(23, 149);
            this.textBox_for_Rotation3D.Name = "textBox_for_Rotation3D";
            this.textBox_for_Rotation3D.Size = new System.Drawing.Size(100, 20);
            this.textBox_for_Rotation3D.TabIndex = 6;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1703, 704);
            this.Controls.Add(this.tabControl1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_2d)).EndInit();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_3d)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.PictureBox pictureBox_2d;
        private System.Windows.Forms.Button ButtonSize;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button button7;
        private System.Windows.Forms.TextBox textBox6;
        private System.Windows.Forms.TextBox textBox7;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.PictureBox pictureBox_3d;
        private System.Windows.Forms.Button AddPoint3D;
        private System.Windows.Forms.TextBox textBox_Coords_X_3D;
        private System.Windows.Forms.TextBox textBox_Coords_Y_3D;
        private System.Windows.Forms.Button button_Draw_Points_3D;
        private System.Windows.Forms.TextBox textBox_for_Rotation3D;
        private System.Windows.Forms.Button button_Rotation3D;
    }
}

