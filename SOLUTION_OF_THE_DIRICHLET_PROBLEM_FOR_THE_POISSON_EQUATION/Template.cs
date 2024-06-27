using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using delRendererFunction = Plot3D.Graph3D.delRendererFunction;
using cPoint3D = Plot3D.Graph3D.cPoint3D;
using eRaster = Plot3D.Graph3D.eRaster;
using cScatter = Plot3D.Graph3D.cScatter;
using eNormalize = Plot3D.Graph3D.eNormalize;
using eSchema = Plot3D.ColorSchema.eSchema;
using System.Runtime.CompilerServices;
using System.Collections;
using Plot3D;
using System.Runtime.ConstrainedExecution;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.ScrollBar;
using System.Drawing.Printing;

namespace SOLUTION_OF_THE_DIRICHLET_PROBLEM_FOR_THE_POISSON_EQUATION
{
    public partial class Template : Form
    {
        public List<double> x = new List<double>();
        public List<double> y = new List<double>();
        public List<List<double>> v = new List<List<double>>();
        public List<List<double>> difference_clear_and_dirty = new List<List<double>>();
        public List<List<double>> initial_approximation = new List<List<double>>();

        public List<double> x2 = new List<double>();
        public List<double> y2 = new List<double>();
        public List<List<double>> v2 = new List<List<double>>();
        public List<List<double>> difference_v2_and_v = new List<List<double>>();
        public List<List<double>> initial_approximation_v2 = new List<List<double>>();

        public List<List<double>> u = new List<List<double>>();

        public double h = 0.2;
        public double k = 0.1;

        public double left_border_x = 0;
        public double right_border_x = 2;

        public double down_border_y = 0;
        public double upper_border_y = 1;

        public int N = 10;
        public int M = 10;

        public double residual1 = 0;
        public double residual2 = 0;
        public double residual_start_v1 = 0;
        public double residual_start_v2 = 0;

        public double param1 = 0.5;
        public double param2 = 1.9;

        public int S1 = 0;
        public int S2 = 0;

        public int task_number = 0;
        public int draw_graph_number = 0; // 0 - числ; 1 - истинное; 2 - начальное приближение; 3 - разность точного и численного
        public int method = 0;

        public int N_Max1; // число итераций
        public int N_Max2; // число итераций

        double eps1; // минимально допустимый прирост
        double eps2; // минимально допустимый прирост

        double eps_max1 = 0;
        double eps_max2 = 0;

        bool isUser = false;
        bool isCalc = false;

        public Template()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.task_number_box.Items.Add("Тестовая задача");
            this.task_number_box.Items.Add("Основная задача");

            this.task_number_box.SelectedIndex = 0;

            this.combo_methods.Items.Clear();

            this.combo_methods.Items.Add("МВР");
            this.combo_methods.Items.Add("МПИ");
            this.combo_methods.Items.Add("ММН");

            this.combo_methods.SelectedIndex = 0;

            graph3D1.AssignTrackBars(trackRho, trackTheta, trackPhi);
        }

        private void button1_Click(object sender, EventArgs e) // при нажатии на кнопку Start
        {
            first_init_data();
        }

        private void init_getted_data()
        {
            N = System.Convert.ToInt32(this.inputN.Text);
            M = System.Convert.ToInt32(this.inputM.Text);

            h = (right_border_x - left_border_x) / N;
            k = (upper_border_y - down_border_y) / M;

            N_Max1 = System.Convert.ToInt32(this.input_N_max.Text);
            N_Max2 = System.Convert.ToInt32(this.N_Max_V2.Text);

            eps1 = System.Convert.ToDouble(this.input_E_met.Text);
            eps2 = System.Convert.ToDouble(this.eps_v2.Text);

            param1 = System.Convert.ToDouble(this.textBoxw1.Text);
            param2 = System.Convert.ToDouble(this.w2TextBox.Text);
        }

        private void calc_difference_clear_and_dirty()
        {
            difference_clear_and_dirty.Clear();
            difference_clear_and_dirty = new List<List<double>>(N + 1);
            for (int i = 0; i < N + 1; ++i)
            {
                List<double> innerList = new List<double>(M + 1);
                for (int j = 0; j < M + 1; ++j)
                {
                    innerList.Add(v[i][j] - u[i][j]);
                }
                difference_clear_and_dirty.Add(innerList);
            }
        }

        private void init_table()
        {

            dataGridView1.Columns.Clear();
            dataGridView1.Rows.Clear();

            var column1 = new DataGridViewColumn();
            column1.HeaderText = " "; //текст в шапке
            column1.ReadOnly = true; //значение в этой колонке нельзя править
            column1.Name = "yj"; //текстовое имя колонки, его можно использовать вместо обращений по индексу
            column1.Frozen = true; //флаг, что данная колонка всегда отображается на своем месте
            column1.CellTemplate = new DataGridViewTextBoxCell(); //тип нашей колонки

            var column2 = new DataGridViewColumn();
            column2.HeaderText = "xi";
            column2.Name = "xi";
            column2.CellTemplate = new DataGridViewTextBoxCell();
            column2.ReadOnly = true; //значение в этой колонке нельзя править

            dataGridView1.Columns.Add(column1);
            dataGridView1.Columns.Add(column2);

            for (int i = 0; i < (task_number == 1 && (draw_graph_number == 1 || draw_graph_number == 4) ? 2 * N + 1 : N + 1); ++i)
            {
                var column_temp = new DataGridViewColumn();
                column_temp.HeaderText = "x_" + System.Convert.ToString(i);
                column_temp.CellTemplate = new DataGridViewTextBoxCell();
                column_temp.ReadOnly = true; //значение в этой колонке нельзя править
                dataGridView1.Columns.Add(column_temp);
            }

            for (int j = 0; j < ((draw_graph_number == 1 || draw_graph_number == 4) && task_number == 1 ? 2 * M + 2 : M + 2); ++j)
            {
                dataGridView1.Rows.Add();
                if (j != 0)
                    dataGridView1[0, j].Value = "y_" + System.Convert.ToString(j - 1);
                else
                    dataGridView1[0, 0].Value = "yj";
                for (int i = 1; i < ((draw_graph_number == 1 || draw_graph_number == 4) && task_number == 1 ? 2 * N + 3 : N + 3); ++i)
                {
                    if (j == 0)
                    {
                        if (i == 1)
                            dataGridView1[i, j].Value = "j/i";
                        else
                        {
                            dataGridView1[i, j].Value = System.Convert.ToString(i - 2);
                        }
                    }
                    else
                    {
                        if (i == 1)
                            dataGridView1[i, j].Value = System.Convert.ToString(j - 1);
                        else
                        {
                            if (draw_graph_number == 0)
                                dataGridView1[i, j].Value = System.Convert.ToString(v[i -2][j - 1]);
                            if (task_number == 0)
                            {
                                if (draw_graph_number == 1)
                                    dataGridView1[i, j].Value = System.Convert.ToString(u[i - 2][j - 1]);
                                else if (draw_graph_number == 2)
                                    dataGridView1[i, j].Value = System.Convert.ToString(initial_approximation[i - 2][j - 1]);
                                else if (draw_graph_number == 3)
                                    dataGridView1[i, j].Value = System.Convert.ToString(difference_clear_and_dirty[i - 2][j - 1]);
                            }
                            else
                            {
                                if (draw_graph_number == 1)
                                    dataGridView1[i, j].Value = System.Convert.ToString(v2[i - 2][j - 1]);
                                if (draw_graph_number == 2)
                                    dataGridView1[i, j].Value = System.Convert.ToString(initial_approximation[i - 2][j - 1]);
                                if (draw_graph_number == 3)
                                    dataGridView1[i, j].Value = System.Convert.ToString(difference_v2_and_v[i - 2][j - 1]);
                                if (draw_graph_number == 4)
                                    dataGridView1[i, j].Value = System.Convert.ToString(initial_approximation_v2[i - 2][j - 1]);
                            }
                        }
                    }
                }
            }

            dataGridView1.AllowUserToAddRows = false; //запрешаем пользователю самому добавлять строки

        }

        private void calc_difference_v2_and_v()
        {
            difference_v2_and_v.Clear();
            difference_v2_and_v = new List<List<double>>(N + 1);
            for (int i = 0; i < N + 1; ++i)
            {
                List<double> innerList = new List<double>(M + 1);
                for (int j = 0; j < M + 1; ++j)
                {
                    innerList.Add(v[i][j] - v2[i*2][j*2]);
                }
                difference_v2_and_v.Add(innerList);
            }
        }
        private void first_init_data()
        {

            init_getted_data();

            if ( task_number == 0)
            {
                init_test();

                if (method == 0)
                {
                    MethodVerRel mvr = new MethodVerRel(N_Max1, N, M, param1, h, k, eps1, ref x, ref y, ref v, task_number, ref residual1, ref S1, ref eps_max1);
                }
                else if (method == 1) { 
                    MethodSimpleIter msi = new MethodSimpleIter(N_Max1, N, M, ref param1, h, k, eps1, ref x, ref y, ref v, task_number, ref residual1, ref S1, ref eps_max1, ref residual_start_v1);
                }
                else if (method == 2)
                {
                    MethodMinNevazok mmn = new MethodMinNevazok(N_Max1, N, M, ref param1, h, k, eps1, ref x, ref y, ref v, task_number, ref residual1, ref S1, ref eps_max1, ref residual_start_v1);
                }

                calc_difference_clear_and_dirty();
            }
            else
            {

                init_main();

                if (method == 0)
                {
                    //Gauss gauss = new Gauss(N, M, h,k,ref x, ref y, ref v, task_number);

                    MethodVerRel mvr_v1 = new MethodVerRel(N_Max1, N, M, param1, h, k, eps1, ref x, ref y, ref v, task_number, ref residual1, ref S1, ref eps_max1);

                    v_to_v2();

                    MethodVerRel mvr_v2 = new MethodVerRel(N_Max2, N, M, param2, h, k, eps2, ref x2, ref y2, ref v2, task_number, ref residual2, ref S2, ref eps_max2);

                    v2_to_v();
                }
                else if (method == 1)
                {
                    MethodSimpleIter msi_v1 = new MethodSimpleIter(N_Max1, N, M, ref param1, h, k, eps1, ref x, ref y, ref v, task_number, ref residual1, ref S1, ref eps_max1, ref residual_start_v1);

                    v_to_v2();

                    MethodSimpleIter msi_v2 = new MethodSimpleIter(N_Max2, N, M, ref param2, h, k, eps2, ref x2, ref y2, ref v2, task_number, ref residual2, ref S2, ref eps_max2, ref residual_start_v2);

                    v2_to_v();
                }
                else if (method == 2)
                {
                    MethodMinNevazok mmn_v1 = new MethodMinNevazok(N_Max1, N, M, ref param1, h, k, eps1, ref x, ref y, ref v, task_number, ref residual1, ref S1, ref eps_max1, ref residual_start_v1);

                    v_to_v2();

                    MethodMinNevazok mmn_v2 = new MethodMinNevazok(N_Max2, N, M, ref param2, h, k, eps2, ref x2, ref y2, ref v2, task_number, ref residual2, ref S2, ref eps_max2, ref residual_start_v2);

                    v2_to_v();
                }

                calc_difference_v2_and_v();
            }

            isCalc = true;

            draw_graph();
            init_table();

            reference();
        }

        private void graph3D1_Load(object sender, EventArgs e)
        {
        }

        private void draw_graph()
        {
            SetSurface();
            graph3D1.Raster = (eRaster)3;
            Color[] c_Colors = Plot3D.ColorSchema.GetSchema(eSchema.Hot);
            graph3D1.SetColorScheme(c_Colors, 3);
        }

        private void task_number_box_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.draw_graph_box.Items.Clear();

            isCalc = false;

            if (this.task_number_box.SelectedIndex == 0)
            {
                this.draw_graph_box.Items.Add("Численное решение v(N)(x,y) - поверхность");
                this.draw_graph_box.Items.Add("Точное решение u*(x,y) - поверхность");
                this.draw_graph_box.Items.Add("Начальное приближение v(0)(x,y) - поверхность");
                this.draw_graph_box.Items.Add("Разность точного и численного решения - поверхность");

                draw_graph_number = 0;
                this.draw_graph_box.SelectedIndex = 0;

                task_number = 0;
            }
            else
            {
                this.draw_graph_box.Items.Add("Численное решение v(N)(x,y) - поверхность");
                this.draw_graph_box.Items.Add("Численное решение v2(N2)(x,y), полученное на сетке с половинным шагом - поверхность");
                this.draw_graph_box.Items.Add("Начальное приближение v(0)(x,y) - поверхность");
                this.draw_graph_box.Items.Add("Разность численных решений v(N)(x,y) и v2(N2) - поверхность");
                this.draw_graph_box.Items.Add("Начальное приближение v2(0)(x,y) - поверхность");

                task_number = 1;
                draw_graph_number = 0;
                this.draw_graph_box.SelectedIndex = 0;

            }
        }

        private void draw_graph_box_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (isUser)
            {
                if (task_number == 0)
                {
                    if (this.draw_graph_box.SelectedIndex == 0)
                        draw_graph_number = 0;
                    else if (this.draw_graph_box.SelectedIndex == 1)
                        draw_graph_number = 1;
                    else if (this.draw_graph_box.SelectedIndex == 2)
                        draw_graph_number = 2;
                    else if (this.draw_graph_box.SelectedIndex == 3)
                        draw_graph_number = 3;

                    if (isCalc)
                    {
                        init_table();

                        SetSurface();
                    }

                }
                else
                {
                    if (this.draw_graph_box.SelectedIndex == 0)
                        draw_graph_number = 0;
                    else if (this.draw_graph_box.SelectedIndex == 1)
                    {
                        draw_graph_number = 1;
                    }
                    else if (this.draw_graph_box.SelectedIndex == 2)
                    {
                        draw_graph_number = 2;
                    }
                    else if (this.draw_graph_box.SelectedIndex == 3)
                    {
                        draw_graph_number = 3;
                    }
                    else if (this.draw_graph_box.SelectedIndex == 4)
                    {
                        draw_graph_number = 4;
                    }

                    if (isCalc)
                    {
                        SetSurface();

                        init_table();
                    }

                }

            }

            isUser = true;
        }

        private void SetSurface()
        {
            if (draw_graph_number == 0)
            {
                cPoint3D[,] i_Points3D = new cPoint3D[x.Count > 50 ? 50 + (x.Count % 50) : x.Count , y.Count > 50 ? 50 + (y.Count % 50) : y.Count];
                for (int X = 0, i = 0; X < x.Count; ++i)
                {
                    for (int Y = 0, j = 0; Y < y.Count; ++j)
                    {
                        i_Points3D[i, j] = new cPoint3D(x[X], y[Y], v[X][Y]);

                        if (y.Count > 50 && j < 50)
                        {
                            Y += (y.Count / 50);
                        }
                        else
                        {
                            ++Y;
                        }
                    }

                    if (x.Count > 50 && i < 50)
                    {
                        X += (x.Count / 50);
                    }
                    else
                    {
                        ++X;
                    }
                }
                graph3D1.SetSurfacePoints(i_Points3D, eNormalize.Separate);
            }

            if (task_number == 0)
            {
                if (draw_graph_number == 1)
                {
                    cPoint3D[,] i_Points3D = new cPoint3D[x.Count > 50 ? 50 + (x.Count % 50) : x.Count, y.Count > 50 ? 50 + (y.Count % 50) : y.Count];
                    for (int X = 0, i = 0; X < x.Count; ++i)
                    {
                        for (int Y = 0, j = 0; Y < y.Count; ++j) 
                        {
                            i_Points3D[i, j] = new cPoint3D(x[X], y[Y], u[X][Y]);

                            if (y.Count > 50 && j < 50)
                            {
                                Y += (y.Count / 50);
                            }
                            else
                            {
                                ++Y;
                            }
                        }

                        if (x.Count > 50 && i < 50)
                        {
                            X += (x.Count / 50);
                        }
                        else
                        {
                            ++X;
                        }
                    }
                    graph3D1.SetSurfacePoints(i_Points3D, eNormalize.Separate);
                }
                if (draw_graph_number == 2)
                {
                    cPoint3D[,] i_Points3D = new cPoint3D[x.Count > 50 ? 50 + (x.Count % 50) : x.Count, y.Count > 50 ? 50 + (y.Count % 50) : y.Count];
                    for (int X = 0, i = 0; X < x.Count; ++i)
                    {
                        for (int Y = 0, j = 0; Y < y.Count; ++j)
                        {
                            i_Points3D[i, j] = new cPoint3D(x[X], y[Y], initial_approximation[X][Y]);

                            if (y.Count > 50 && j < 50)
                            {
                                Y += (y.Count / 50);
                            }
                            else
                            {
                                ++Y;
                            }
                        }

                        if (x.Count > 50 && i < 50)
                        {
                            X += (x.Count / 50);
                        }
                        else
                        {
                            ++X;
                        }
                    }
                    graph3D1.SetSurfacePoints(i_Points3D, eNormalize.Separate);
                }
                if (draw_graph_number == 3)
                {
                    cPoint3D[,] i_Points3D = new cPoint3D[x.Count > 50 ? 50 + (x.Count % 50) : x.Count, y.Count > 50 ? 50 + (y.Count % 50) : y.Count];
                    for (int X = 0, i = 0; X < x.Count; ++i)
                    {
                        for (int Y = 0, j = 0; Y < y.Count; ++j)
                        {
                            i_Points3D[i, j] = new cPoint3D(x[X], y[Y], difference_clear_and_dirty[X][Y]);

                            if (y.Count > 50 && j < 50)
                            {
                                Y += (y.Count / 50);
                            }
                            else
                            {
                                ++Y;
                            }
                        }

                        if (x.Count > 50 && i < 50)
                        {
                            X += (x.Count / 50);
                        }
                        else
                        {
                            ++X;
                        }
                    }
                    graph3D1.SetSurfacePoints(i_Points3D, eNormalize.Separate);
                }

            }
            else
            {
                if (draw_graph_number == 1)
                {
                    cPoint3D[,] i_Points3D = new cPoint3D[x2.Count > 50 ? 50 + (x2.Count % 50) : x2.Count, y2.Count > 50 ? 50 + (y2.Count % 50) : y2.Count];
                    for (int X = 0, i = 0; X < x2.Count; ++i)
                    {
                        for (int Y = 0, j = 0; Y < y2.Count; ++j)
                        {
                            i_Points3D[i,j] = new cPoint3D(x2[X], y2[Y], v2[X][Y]);

                            if (y2.Count > 50 && j < 50)
                            {
                                Y += (y2.Count / 50);
                            }
                            else
                            {
                                ++Y;
                            }
                        }

                        if (x2.Count > 50 && i < 50)
                        {
                            X += (x2.Count / 50);
                        }
                        else
                        {
                            ++X;
                        }
                    }
                    graph3D1.SetSurfacePoints(i_Points3D, eNormalize.Separate);
                }
                if (draw_graph_number == 2)
                {
                    cPoint3D[,] i_Points3D = new cPoint3D[x.Count > 50 ? 50 + (x.Count % 50) : x.Count, y.Count > 50 ? 50 + (y.Count % 50) : y.Count];
                    for (int X = 0, i = 0; X < x.Count; ++i)
                    {
                        for (int Y = 0, j = 0; Y < y.Count; ++j)
                        {
                            i_Points3D[i, j] = new cPoint3D(x[X], y[Y], initial_approximation[X][Y]);

                            if (y.Count > 50 && j < 50)
                            {
                                Y += (y.Count / 50);
                            }
                            else
                            {
                                ++Y;
                            }
                        }
                        if (x.Count > 50 && i < 50)
                        {
                            X += (x.Count / 50);
                        }
                        else
                        {
                            ++X;
                        }
                    }
                    graph3D1.SetSurfacePoints(i_Points3D, eNormalize.Separate);
                }
                if (draw_graph_number == 3)
                {
                    cPoint3D[,] i_Points3D = new cPoint3D[x.Count > 50 ? 50 + (x.Count % 50) : x.Count, y.Count > 50 ? 50 + (y.Count % 50) : y.Count];
                    for (int X = 0, i = 0; X < x.Count; ++i)
                    {
                        for (int Y = 0, j = 0; Y < y.Count; ++j)
                        {
                            i_Points3D[i, j] = new cPoint3D(x[X], y[Y], difference_v2_and_v[X][Y]);

                            if (y.Count > 50 && j < 50)
                            {
                                Y += (y.Count / 50);
                            }
                            else
                            {
                                ++Y;
                            }
                        }

                        if (x.Count > 50 && i < 50)
                        {
                            X += (x.Count / 50);
                        }
                        else
                        {
                            ++X;
                        }
                    }
                    graph3D1.SetSurfacePoints(i_Points3D, eNormalize.Separate);
                }
                if (draw_graph_number == 4)
                {
                    cPoint3D[,] i_Points3D = new cPoint3D[x2.Count > 50 ? 50 + (x2.Count % 50) : x2.Count, y2.Count > 50 ? 50 + (y2.Count % 50) : y2.Count];
                    for (int X = 0, i = 0; X < x2.Count; ++i)
                    {
                        for (int Y = 0, j = 0; Y < y2.Count; ++j)
                        {
                            i_Points3D[i , j] = new cPoint3D(x2[X], y2[Y], initial_approximation_v2[X][Y]);

                            if (y2.Count > 50 && j < 50)
                            {
                                Y += (y2.Count / 50);
                            }
                            else
                            {
                                ++Y;
                            }
                        }


                        if (x2.Count > 50 && i < 50)
                        {
                            X += (x2.Count / 50);
                        }
                        else
                        {
                            ++X;
                        }
                    }
                    graph3D1.SetSurfacePoints(i_Points3D, eNormalize.Separate);
                }
            }

            // Setting one of the strings = null results in hiding this legend
            graph3D1.AxisX_Legend = "Blue";
            graph3D1.AxisY_Legend = "Green";
            graph3D1.AxisZ_Legend = "Red";
        }

        private Tuple<double, int, int> find_max_deviation()
        {
            double res = 0;
            int index_x = 0;
            int index_y = 0;

            for (int i = 0; i < N + 1; ++i)
            {
                for(int j = 0; j < M + 1; ++j)
                {
                    double value = Math.Abs(v[i][j] - u[i][j]);
                    if (value > res)
                    {
                        res = value;
                        index_x  = i;
                        index_y = j;
                    }

                }
            }

            return Tuple.Create(res, index_x, index_y);
        }

        private Tuple<double, int, int> find_max_deviation_main()
        {
            double res = 0;
            int index_x = 0;
            int index_y = 0;

            for (int i = 0; i < N + 1; ++i)
            {
                for (int j = 0; j < M + 1; ++j)
                {
                    double value = Math.Abs(v[i][j] - v2[i * 2][j * 2]);
                    if (value > res)
                    {
                        res = value;
                        index_x = i;
                        index_y = j;
                    }

                }
            }
            return Tuple.Create(res, index_x, index_y);
        }

        void calc_w_opt()
        {

            double m = (double)1 / ( (System.Convert.ToInt32(this.inputM.Text)) * (System.Convert.ToInt32(this.inputN.Text)) + 1);

            this.w_opt.Text = "w_opt = " + System.Convert.ToString((double)2 / (1 + Math.Sin(Math.PI * m)));
        }
        void calc_interval_tau()
        {

            init_getted_data();

            double temp1 = (double)4 / (h * h) * Math.Pow(Math.Sin(Math.PI * (N - 1) / (2 * N)), 2) +
                   (double)4 / (k * k) * Math.Pow(Math.Sin(Math.PI * (M - 1) / (2 * M)), 2);

            double param_v1 = (double)2 / temp1;

            v_to_v2();

            temp1 = (double)4 / (h * h) * Math.Pow(Math.Sin(Math.PI * (N - 1) / (2 * N)), 2) +
                   (double)4 / (k * k) * Math.Pow(Math.Sin(Math.PI * (M - 1) / (2 * M)), 2);

            double param_v2 = (double)2 / temp1;

            v2_to_v();

            this.w_opt.Text = "tau1 = (0; " + System.Convert.ToString(param_v1) + ")" + "    "
                + "tau2 = (0; " + System.Convert.ToString(param_v2) + ")";
        }

        private void button_calc_w_opt_Click(object sender, EventArgs e)
        {
            if (method == 0)
                calc_w_opt();
            else
                calc_interval_tau();
        }

        private void reference()
        {
            if (task_number == 0)
            {
                Tuple<double, int, int> fault = find_max_deviation();

                this.label3.Text = "Справка: Для решения тестовой задачи использованы сетка с " +
                    "числом разбиений по x n = \"" + System.Convert.ToString(N) + "\" и числом разбиений по " +
                    "y m = \"" + System.Convert.ToString(M) + "\", " +
                    "метод ";
                if (method == 0)
                {
                    this.label3.Text += "верхней релаксации с параметром w = " + System.Convert.ToString(param1);
                }
                else if (method == 1)
                {
                    this.label3.Text += "простой итерации с параметром tau = " + System.Convert.ToString(param1);
                }
                else if (method == 2)
                {
                    this.label3.Text += "минимальных невязок";
                }
                this.label3.Text +=
                    ", применены критерии остановки по точности E_мет = " + System.Convert.ToString(eps1) +
                    "\nи по числу итерции N_max = " + System.Convert.ToString(N_Max1) +
                    "\nНа решение схемы (СЛАУ) затрачено итераций N = " + System.Convert.ToString(S1) +
                    " и достигнута точность итерационного метода E_N = " + System.Convert.ToString(eps_max1) +
                    "\nСЛАУ решена с невязкой ||R(N)|| = " + System.Convert.ToString(residual1) + ", для невязки исопльзовалась норма \"max\";" +
                    "\nТестовая задача должа быть решена с погрешностью не более e = 0.5*10^-6; задача решена с погрешностью e1 = " +
                    System.Convert.ToString(fault.Item1) + "\nМаксимальное отклонение точного и численного решений наблюдается в узле " +
                    "x = \"" + System.Convert.ToString(fault.Item2) + "\"; y = \"" + System.Convert.ToString(fault.Item3) + "\"" +
                    "\nВ качестве начального приближения использовано \"нулевое\"\n";
                if (method != 0)
                {
                    this.label3.Text += "Невязка СЛАУ на начальном приближениии ||R_0||_max = \"" + System.Convert.ToString(residual_start_v1) + "\"";
                }
            }
            else
            {
                Tuple<double, int, int> fault = find_max_deviation_main();

                this.label3.Text = "Справка: Для решения основной задачи использованы сетка с " +
                   "числом разбиений по x n = \"" + System.Convert.ToString(N) + "\" и числом разбиений по " +
                   "y m = \"" + System.Convert.ToString(M) + "\"," +
                   "метод ";
                if (method == 0)
                {
                    this.label3.Text += "верхней релаксации с параметром w = " + System.Convert.ToString(param1);
                }
                else if (method == 1)
                {
                    this.label3.Text += "простой итерации с параметром tau = " + System.Convert.ToString(param1);
                }
                else if (method == 2)
                {
                    this.label3.Text += "минимальных невязок";
                }
                this.label3.Text +=
                   ", применены критерии остановки по точности E_мет = " + System.Convert.ToString(eps1) +
                   "\nи по числу итерции N_max = " + System.Convert.ToString(N_Max1) +
                   "\nНа решение схемы (СЛАУ) затрачено итераций N = " + System.Convert.ToString(S1) +
                   " и достигнута точность итерационного метода E_N = " + System.Convert.ToString(eps_max1) +
                   "\nСЛАУ решена сневязкой ||R(N)|| = " + System.Convert.ToString(residual1) + ", для невязки исопльзовалась норма \"max\";\n" +
                    "В качестве начального приближения на основной сетке использовано \"нулевое\"";
                if (method != 0)
                {
                    this.label3.Text += "\nНа основное сетке невязка СЛАУ на начальном приближении ||R_0||_max = \"" + System.Convert.ToString(residual_start_v1) + "\"";
                }
                this.label3.Text += "\nДля контроля точности решения использована сетка с половинным шагом, метод ";
                if (method == 0)
                {
                    this.label3.Text += "верхней релаксации с параметром w = " + System.Convert.ToString(param2);
                }
                else if (method == 1)
                {
                    this.label3.Text += "простой итерации с параметром tau = " + System.Convert.ToString(param2);
                }
                else if (method == 2)
                {
                    this.label3.Text += "минимальных невязок";
                }
                this.label3.Text +=
                   "\", применены критерии остановки по точности E_мет-2 = \"" + System.Convert.ToString(eps2) + "\" и по числу итераций N_max_2 = \"" +
                   System.Convert.ToString(N_Max2) + "\"\n" +
                   "На решение задачи (СЛАУ) затрачено итераций N2 = \"" + System.Convert.ToString(S2) + " и достигнута точность итерационного метода " +
                   "E_N2 = \"" + System.Convert.ToString(eps_max2) + "\n" +
                   "Схема (СЛАУ) на сетке с половинным шагом решена с невязкой ||R(N2)|| = \"" + System.Convert.ToString(residual2) + "\", использована" +
                   " норма \"max\"\n" +
                   "Основная задача должа быть решена с точностью не хочу, чем e = 0.5*10^-6; задача решена с точностью e2 = \"" +
                   System.Convert.ToString(fault.Item1) + "\"\n" +
                   "Максимальное отклонение численных решений на основной сетке и сетке с половинным шагом наблюдается в узле " +
                   "x = \"" + System.Convert.ToString(fault.Item2) + "\"; y = \"" + System.Convert.ToString(fault.Item3) + 
                   "\"\n" +
                   "В качестве начального приближения на сетке с половинным шагом использовано \"нулевое\"";
                if (method != 0)
                {
                    this.label3.Text += "\nНа сетке с половинным шагом невязка СЛАУ на начальном приближении ||R_0||_max = \"" + System.Convert.ToString(residual_start_v2) + "\"";
                }
            }
        }

        private void combo_methods_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (combo_methods.SelectedIndex == 0)
            {
                method = 0;

                this.button_calc_w_opt.Visible = true;
                this.button_calc_w_opt.Text = "Рассчитать w_opt";
                this.w_opt.Text = "w_opt = 0";

                calc_w_opt();
            }
            else if (combo_methods.SelectedIndex == 1)
            {
                method = 1;
                this.button_calc_w_opt.Visible = true;
                this.button_calc_w_opt.Text = "Рассчитать интервал tau";
                this.w_opt.Text = "tau = (0, 1)";
            }
            else if (combo_methods.SelectedIndex == 2)
            {
                method = 2;
                this.button_calc_w_opt.Visible = false;
                this.w_opt.Text = "";
            }

            clear_all();
        }

        private void clear_all()
        {
            x.Clear();
            y.Clear();
            v.Clear();
            difference_clear_and_dirty.Clear();
            initial_approximation.Clear();

            x2.Clear();
            y2.Clear();
            v2.Clear();
            difference_v2_and_v.Clear();
            initial_approximation_v2.Clear();
        }
    }
}
