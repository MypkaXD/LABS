namespace SOLUTION_OF_THE_DIRICHLET_PROBLEM_FOR_THE_POISSON_EQUATION
{
    partial class Template
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
            this.button_for_start = new System.Windows.Forms.Button();
            this.inputN = new System.Windows.Forms.TextBox();
            this.inputM = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.input_N_max = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.input_E_met = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.draw_graph_box = new System.Windows.Forms.ComboBox();
            this.task_number_box = new System.Windows.Forms.ComboBox();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.trackRho = new System.Windows.Forms.TrackBar();
            this.trackTheta = new System.Windows.Forms.TrackBar();
            this.trackPhi = new System.Windows.Forms.TrackBar();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.N_Max_V2 = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.eps_v2 = new System.Windows.Forms.TextBox();
            this.w2TextBox = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.textBoxw1 = new System.Windows.Forms.TextBox();
            this.w_opt = new System.Windows.Forms.Label();
            this.button_calc_w_opt = new System.Windows.Forms.Button();
            this.combo_methods = new System.Windows.Forms.ComboBox();
            this.graph3D1 = new Plot3D.Graph3D();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackRho)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackTheta)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackPhi)).BeginInit();
            this.SuspendLayout();
            // 
            // button_for_start
            // 
            this.button_for_start.Location = new System.Drawing.Point(12, 12);
            this.button_for_start.Name = "button_for_start";
            this.button_for_start.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.button_for_start.Size = new System.Drawing.Size(75, 23);
            this.button_for_start.TabIndex = 1;
            this.button_for_start.Text = "Start";
            this.button_for_start.UseVisualStyleBackColor = true;
            this.button_for_start.Click += new System.EventHandler(this.button1_Click);
            // 
            // inputN
            // 
            this.inputN.Location = new System.Drawing.Point(161, 41);
            this.inputN.Name = "inputN";
            this.inputN.Size = new System.Drawing.Size(100, 20);
            this.inputN.TabIndex = 2;
            this.inputN.Text = "10";
            // 
            // inputM
            // 
            this.inputM.Location = new System.Drawing.Point(161, 67);
            this.inputM.Name = "inputM";
            this.inputM.Size = new System.Drawing.Size(100, 20);
            this.inputM.TabIndex = 3;
            this.inputM.Text = "10";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 44);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(146, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Ввод числа разбиений по X";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(9, 70);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(146, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Ввод числа разбиений по Y";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(577, 9);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(56, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Справка: ";
            // 
            // input_N_max
            // 
            this.input_N_max.Location = new System.Drawing.Point(64, 143);
            this.input_N_max.Name = "input_N_max";
            this.input_N_max.Size = new System.Drawing.Size(100, 20);
            this.input_N_max.TabIndex = 7;
            this.input_N_max.Text = "1000";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 146);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(40, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "N_max";
            // 
            // input_E_met
            // 
            this.input_E_met.Location = new System.Drawing.Point(372, 143);
            this.input_E_met.Name = "input_E_met";
            this.input_E_met.Size = new System.Drawing.Size(100, 20);
            this.input_E_met.TabIndex = 10;
            this.input_E_met.Text = "0,0000001";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(303, 146);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(57, 13);
            this.label5.TabIndex = 11;
            this.label5.Text = "E_метода";
            // 
            // draw_graph_box
            // 
            this.draw_graph_box.FormattingEnabled = true;
            this.draw_graph_box.Location = new System.Drawing.Point(93, 14);
            this.draw_graph_box.Name = "draw_graph_box";
            this.draw_graph_box.Size = new System.Drawing.Size(218, 21);
            this.draw_graph_box.TabIndex = 12;
            this.draw_graph_box.SelectedIndexChanged += new System.EventHandler(this.draw_graph_box_SelectedIndexChanged);
            // 
            // task_number_box
            // 
            this.task_number_box.FormattingEnabled = true;
            this.task_number_box.Location = new System.Drawing.Point(317, 14);
            this.task_number_box.Name = "task_number_box";
            this.task_number_box.Size = new System.Drawing.Size(218, 21);
            this.task_number_box.TabIndex = 13;
            this.task_number_box.SelectedIndexChanged += new System.EventHandler(this.task_number_box_SelectedIndexChanged);
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Location = new System.Drawing.Point(12, 187);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(897, 783);
            this.dataGridView1.TabIndex = 14;
            // 
            // trackRho
            // 
            this.trackRho.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.trackRho.Location = new System.Drawing.Point(926, 187);
            this.trackRho.Name = "trackRho";
            this.trackRho.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackRho.Size = new System.Drawing.Size(45, 679);
            this.trackRho.TabIndex = 15;
            this.trackRho.TickStyle = System.Windows.Forms.TickStyle.None;
            // 
            // trackTheta
            // 
            this.trackTheta.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.trackTheta.Location = new System.Drawing.Point(959, 187);
            this.trackTheta.Name = "trackTheta";
            this.trackTheta.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackTheta.Size = new System.Drawing.Size(45, 679);
            this.trackTheta.TabIndex = 16;
            this.trackTheta.TickStyle = System.Windows.Forms.TickStyle.None;
            // 
            // trackPhi
            // 
            this.trackPhi.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.trackPhi.Location = new System.Drawing.Point(1000, 187);
            this.trackPhi.Name = "trackPhi";
            this.trackPhi.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackPhi.Size = new System.Drawing.Size(45, 679);
            this.trackPhi.TabIndex = 17;
            this.trackPhi.TickStyle = System.Windows.Forms.TickStyle.None;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label6.Location = new System.Drawing.Point(923, 853);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(27, 13);
            this.label6.TabIndex = 18;
            this.label6.Text = "Rho";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label7.Location = new System.Drawing.Point(997, 853);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(22, 13);
            this.label7.TabIndex = 19;
            this.label7.Text = "Phi";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.label8.Location = new System.Drawing.Point(956, 853);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(35, 13);
            this.label8.TabIndex = 20;
            this.label8.Text = "Theta";
            // 
            // N_Max_V2
            // 
            this.N_Max_V2.Location = new System.Drawing.Point(64, 117);
            this.N_Max_V2.Name = "N_Max_V2";
            this.N_Max_V2.Size = new System.Drawing.Size(100, 20);
            this.N_Max_V2.TabIndex = 21;
            this.N_Max_V2.Text = "1000";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(12, 120);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(46, 13);
            this.label9.TabIndex = 22;
            this.label9.Text = "N2_max";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(303, 120);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(63, 13);
            this.label10.TabIndex = 24;
            this.label10.Text = "E2_метода";
            // 
            // eps_v2
            // 
            this.eps_v2.Location = new System.Drawing.Point(372, 117);
            this.eps_v2.Name = "eps_v2";
            this.eps_v2.Size = new System.Drawing.Size(100, 20);
            this.eps_v2.TabIndex = 23;
            this.eps_v2.Text = "0,0000001";
            // 
            // w2TextBox
            // 
            this.w2TextBox.Location = new System.Drawing.Point(197, 117);
            this.w2TextBox.Name = "w2TextBox";
            this.w2TextBox.Size = new System.Drawing.Size(100, 20);
            this.w2TextBox.TabIndex = 25;
            this.w2TextBox.Text = "1,9";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(170, 120);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(21, 13);
            this.label11.TabIndex = 26;
            this.label11.Text = "w2";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(170, 146);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(21, 13);
            this.label12.TabIndex = 28;
            this.label12.Text = "w1";
            // 
            // textBoxw1
            // 
            this.textBoxw1.Location = new System.Drawing.Point(197, 143);
            this.textBoxw1.Name = "textBoxw1";
            this.textBoxw1.Size = new System.Drawing.Size(100, 20);
            this.textBoxw1.TabIndex = 27;
            this.textBoxw1.Text = "1,9";
            // 
            // w_opt
            // 
            this.w_opt.AutoSize = true;
            this.w_opt.Location = new System.Drawing.Point(148, 93);
            this.w_opt.Name = "w_opt";
            this.w_opt.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.w_opt.Size = new System.Drawing.Size(54, 13);
            this.w_opt.TabIndex = 29;
            this.w_opt.Text = "w_opt = 0";
            // 
            // button_calc_w_opt
            // 
            this.button_calc_w_opt.Location = new System.Drawing.Point(12, 88);
            this.button_calc_w_opt.Name = "button_calc_w_opt";
            this.button_calc_w_opt.Size = new System.Drawing.Size(130, 23);
            this.button_calc_w_opt.TabIndex = 30;
            this.button_calc_w_opt.Text = "Рассчитать w_opt";
            this.button_calc_w_opt.UseVisualStyleBackColor = true;
            this.button_calc_w_opt.Click += new System.EventHandler(this.button_calc_w_opt_Click);
            // 
            // combo_methods
            // 
            this.combo_methods.FormattingEnabled = true;
            this.combo_methods.Location = new System.Drawing.Point(267, 40);
            this.combo_methods.Name = "combo_methods";
            this.combo_methods.Size = new System.Drawing.Size(121, 21);
            this.combo_methods.TabIndex = 31;
            this.combo_methods.SelectedIndexChanged += new System.EventHandler(this.combo_methods_SelectedIndexChanged);
            // 
            // graph3D1
            // 
            this.graph3D1.AxisX_Color = System.Drawing.Color.DarkBlue;
            this.graph3D1.AxisX_Legend = null;
            this.graph3D1.AxisY_Color = System.Drawing.Color.DarkGreen;
            this.graph3D1.AxisY_Legend = null;
            this.graph3D1.AxisZ_Color = System.Drawing.Color.DarkRed;
            this.graph3D1.AxisZ_Legend = null;
            this.graph3D1.BackColor = System.Drawing.Color.White;
            this.graph3D1.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(180)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))));
            this.graph3D1.Location = new System.Drawing.Point(915, 135);
            this.graph3D1.Name = "graph3D1";
            this.graph3D1.PolygonLineColor = System.Drawing.Color.Black;
            this.graph3D1.Raster = Plot3D.Graph3D.eRaster.Off;
            this.graph3D1.Size = new System.Drawing.Size(942, 815);
            this.graph3D1.TabIndex = 0;
            this.graph3D1.TopLegendColor = System.Drawing.Color.FromArgb(((int)(((byte)(200)))), ((int)(((byte)(200)))), ((int)(((byte)(150)))));
            this.graph3D1.Load += new System.EventHandler(this.graph3D1_Load);
            // 
            // Template
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1924, 899);
            this.Controls.Add(this.combo_methods);
            this.Controls.Add(this.button_calc_w_opt);
            this.Controls.Add(this.w_opt);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.textBoxw1);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.w2TextBox);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.eps_v2);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.N_Max_V2);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.trackPhi);
            this.Controls.Add(this.trackTheta);
            this.Controls.Add(this.trackRho);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.task_number_box);
            this.Controls.Add(this.draw_graph_box);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.input_E_met);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.input_N_max);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.inputM);
            this.Controls.Add(this.inputN);
            this.Controls.Add(this.button_for_start);
            this.Controls.Add(this.graph3D1);
            this.Name = "Template";
            this.Text = "SOLUTION_OF_THE_DIRICHLET_PROBLEM_FOR_THE_POISSON_EQUATION";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackRho)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackTheta)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackPhi)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Plot3D.Graph3D graph3D1;
        private System.Windows.Forms.Button button_for_start;
        private System.Windows.Forms.TextBox inputN;
        private System.Windows.Forms.TextBox inputM;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox input_N_max;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox input_E_met;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox draw_graph_box;
        private System.Windows.Forms.ComboBox task_number_box;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.TrackBar trackRho;
        private System.Windows.Forms.TrackBar trackTheta;
        private System.Windows.Forms.TrackBar trackPhi;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox N_Max_V2;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox eps_v2;
        private System.Windows.Forms.TextBox w2TextBox;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TextBox textBoxw1;
        private System.Windows.Forms.Label w_opt;
        private System.Windows.Forms.Button button_calc_w_opt;
        private System.Windows.Forms.ComboBox combo_methods;
    }
}

