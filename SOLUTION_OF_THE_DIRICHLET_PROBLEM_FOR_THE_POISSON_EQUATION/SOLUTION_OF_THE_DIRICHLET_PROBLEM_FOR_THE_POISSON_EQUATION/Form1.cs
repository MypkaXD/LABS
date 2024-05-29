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

namespace SOLUTION_OF_THE_DIRICHLET_PROBLEM_FOR_THE_POISSON_EQUATION
{
    public partial class Form1 : Form
    {

        public List<double> x = new List<double>();
        public List<double> y = new List<double>();

        public List<double> x2 = new List<double>();
        public List<double> y2 = new List<double>();

        public List<List<double>> v = new List<List<double>>();
        public List<List<double>> v2 = new List<List<double>>();
        public List<List<double>> u = new List<List<double>>();
        public List<List<double>> difference_clear_and_dirty = new List<List<double>>();
        public List<List<double>> difference_v2_and_v = new List<List<double>>();
        public List<List<double>> initial_approximation = new List<List<double>>();
        public List<List<double>> initial_approximation_v2 = new List<List<double>>();
        public List<double> func = new List<double>();
        public List<List<double>> func2 = new List<List<double>>();

        public double h;
        public double k;

        public double left_border_x;
        public double right_border_x;

        public double left_border_y;
        public double right_border_y;

        public int N;
        public int M;

        public int task_number = 0;
        public int draw_graph_number = 0; // 0 - числ; 1 - истинное; 2 - начальное приближение; 3 - разность точного и численного

        public double w1 = 1.9; // [0;2]
        public double w2 = 1.9; // [0;2]

        public int N_Max1; // число итераций
        public int N_Max2; // число итераций

        int S1 = 0;
        int S2 = 0;

        double eps1; // минимально допустимый прирост
        double eps2; // минимально допустимый прирост
        double eps_max1 = 0; // текущее значение прироста
        double eps_max2 = 0; // текущее значение прироста
        double eps_cur = 0; // для подсчета текущего значения прироста
        double a2, k2, h2; // ненулевые элементы матрицы (–A)

        double residual1 = 0;
        double residual2 = 0;

        bool isUser = false;
        bool isCalc = false;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.task_number_box.Items.Add("Тестовая задача");
            this.task_number_box.Items.Add("Основная задача");

            this.task_number_box.SelectedIndex = 0;

            graph3D1.AssignTrackBars(trackRho, trackTheta, trackPhi);
        }

        private void button1_Click(object sender, EventArgs e) // при нажатии на кнопку Start
        {
            first_init_data();
        }

        // Функции m1 - m4

        private double m1_main(double y)
        {
            return -y * (y - 1);
        }
        private double m2_main(double y)
        {
            return y * (y - 1);
        }
        private double m3_main(double x)
        {
            return Math.Abs(Math.Sin(Math.PI * x));
        }
        private double m4_main(double x)
        {
            return Math.Abs(Math.Sin(Math.PI * x)) * Math.Exp(x);
        }

        private double m1_test(double y)
        {
            return Math.Exp(Math.Sin(Math.PI * left_border_x * y) * Math.Sin(Math.PI * left_border_x * y));
        }
        private double m2_test(double y)
        {
            return Math.Exp(Math.Sin(Math.PI * right_border_x * y) * Math.Sin(Math.PI * right_border_x * y));
        }
        private double m3_test(double x)
        {
            return Math.Exp(Math.Sin(Math.PI * x * left_border_y) * Math.Sin(Math.PI * x * left_border_y));
        }
        private double m4_test(double x)
        {
            return Math.Exp(Math.Sin(Math.PI * x * right_border_y) * Math.Sin(Math.PI * x * right_border_y));
        }

        ///////////////////

        private double test_func(double x, double y)
        {
            return Math.Exp(Math.Sin(Math.PI * x * y) * Math.Sin(Math.PI * x * y));
        }
        private double test_func_f(double x, double y)
        {

            double test = test_func_xx(x, y) + test_func_yy(x, y);

            return test;
        }
        private double test_func_xx(double x, double y)
        {
            return 4 * Math.Pow(Math.PI, 2) * y * y *
                Math.Exp(Math.Pow(Math.Sin(Math.PI * x * y), 2)) *
                Math.Pow(Math.Sin(Math.PI * x * y), 2) *
                Math.Pow(Math.Cos(Math.PI * x * y), 2) -
                2 * Math.Pow(Math.PI, 2) * Math.Pow(y, 2) *
                Math.Exp(Math.Pow(Math.Sin(Math.PI * x * y), 2)) *
                Math.Pow(Math.Sin(Math.PI * x * y), 2) +
                2 * Math.Pow(Math.PI, 2) * Math.Pow(y, 2) *
                Math.Exp(Math.Pow(Math.Sin(Math.PI * x * y), 2)) *
                Math.Pow(Math.Cos(Math.PI * x * y), 2);
        }
        private double test_func_yy(double x, double y)
        {
            return 4 * Math.Pow(Math.PI, 2) * x * x *
                Math.Exp(Math.Pow(Math.Sin(Math.PI * x * y), 2)) *
                Math.Pow(Math.Sin(Math.PI * x * y), 2) *
                Math.Pow(Math.Cos(Math.PI * x * y), 2) -
                2 * Math.Pow(Math.PI, 2) * Math.Pow(x, 2) *
                Math.Exp(Math.Pow(Math.Sin(Math.PI * x * y), 2)) *
                Math.Pow(Math.Sin(Math.PI * x * y), 2) +
                2 * Math.Pow(Math.PI, 2) * Math.Pow(x, 2) *
                Math.Exp(Math.Pow(Math.Sin(Math.PI * x * y), 2)) *
                Math.Pow(Math.Cos(Math.PI * x * y), 2);
        }

        private void init_getted_data()
        {
            N = System.Convert.ToInt32(this.inputN.Text);
            M = System.Convert.ToInt32(this.inputM.Text);

            h = (right_border_x - left_border_x) / N;
            k = (right_border_y - left_border_y) / M;

            N_Max1 = System.Convert.ToInt32(this.input_N_max.Text);
            N_Max2 = System.Convert.ToInt32(this.N_Max_V2.Text);

            eps1 = System.Convert.ToDouble(this.input_E_met.Text);
            eps2 = System.Convert.ToDouble(this.eps_v2.Text);

            w1 = System.Convert.ToDouble(this.textBoxw1.Text);
            w2 = System.Convert.ToDouble(this.w2TextBox.Text);
        }
        private void init_containers()
        {
            init_initial_approximation();
            init_x();
            init_y();
            init_v();
        }
        private void init_initial_approximation()
        {
            initial_approximation.Clear();
            initial_approximation = new List<List<double>>(N + 1);
            for (int i = 0; i < N + 1; ++i)
            {
                List<double> innerList = new List<double>(M + 1);

                for (int j = 0; j < M + 1; ++j)
                {
                    innerList.Add(0);
                }
                initial_approximation.Add(innerList);
            }
        }
        private void init_x()
        {
            x.Clear();
            x = new List<double>(N + 1);
            for (int i = 0; i < N + 1; i++)
            {
                x.Add(left_border_x + i * h);
            }
        }
        private void init_y()
        {
            y.Clear();
            y = new List<double>(M + 1);
            for (int i = 0; i < M + 1; i++)
            {
                y.Add(left_border_y + i * k);
            }
        }
        private void init_v()
        {
            v.Clear();
            v = new List<List<double>>(N + 1);
            for (int i = 0; i < N + 1; ++i)
            {
                List<double> innerList = new List<double>(M + 1);

                for (int j = 0; j < M + 1; ++j)
                {
                    innerList.Add(0);
                }
                v.Add(innerList);
            }
        }
        private void init_test_u()
        {
            u.Clear();
            u = new List<List<double>>(N + 1);
            for (int i = 0; i < N + 1; ++i)
            {
                List<double> innerList = new List<double>(M + 1);
                for (int j = 0; j < M + 1; ++j)
                {
                    innerList.Add(test_func(x[i], y[j]));
                }
                u.Add(innerList);
            }
        }
        private void init_test_v()
        {
            for (int i = 0; i < N + 1; ++i)
            {
                for (int j = 0; j < M + 1; ++j)
                {
                    if (i == 0 || j == 0 || i == N || j == M)
                    {
                        double value = u[i][j];
                        v[i][j] = value;
                        initial_approximation[i][j] = value;
                    }
                    else
                    {
                        v[i][j] = 0;
                        initial_approximation[i][j] = 0;
                    }
                }
            }
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

        private void MVR(List<List<double>> matrix, List<double> x_temp, List<double> y_temp, double w, ref int S, ref int N_Max, ref double eps_max, ref double eps)
        {

            double v_old; // старое значение преобразуемой компоненты вектора v
            double v_new; // новое значение преобразуемой компоненты вектора v

            bool check = false; // условие остановки

            S = 0;
            h2 = 1.0 / (h * h);
            k2 = 1.0 / (k * k);
            a2 = -2.0 * (h2 + k2);

            while (!check)
            {
                eps_max = 0;

                for (int j = 1; j < M; j++)
                {
                    for (int i = 1; i < N; i++)
                    {
                        v_old = matrix[i][j];
                        v_new = -w * (h2 * (matrix[i + 1][j] + matrix[i - 1][j]) + k2 * (matrix[i][j + 1] + matrix[i][j - 1]));

                        if (task_number == 0)
                            v_new = v_new + (1 - w) * a2 * matrix[i][j] + w * test_func_f(x_temp[i], y_temp[j]);
                        else
                            v_new = v_new + (1 - w) * a2 * matrix[i][j] + w * func_main(x_temp[i], y_temp[j]);

                        v_new = v_new / a2;

                        eps_cur = Math.Abs(v_old - v_new);

                        if (eps_cur > eps_max)
                            eps_max = eps_cur;

                        matrix[i][j] = v_new;
                    }
                }

                ++S;

                if ((eps_max < eps) || (S >= N_Max))
                    check = true;

            }
        }

        private void init_func2()
        {
            func2.Clear();
            func2 = new List<List<double>>(N + 1);
            for (int i = 0; i < N + 1; ++i)
            {
                List<double> innerList = new List<double>(M + 1);

                for (int j = 0; j < M + 1; ++j)
                {
                    if (task_number == 0)
                        innerList.Add(test_func(x2[i], y2[j]));
                    else
                    {
                        innerList.Add(func_main(x2[i], y2[j]));
                    }
                }
                func2.Add(innerList);
            }
        }

        private void init_initial_approximation_v2()
        {
            initial_approximation_v2.Clear();
            initial_approximation_v2 = new List<List<double>>(2 * N + 1);
            for (int i = 0; i < 2 * N + 1; ++i)
            {
                List<double> innerList = new List<double>(2 * M + 1);

                for (int j = 0; j < 2 * M + 1; ++j)
                {
                    innerList.Add(0);
                }
                initial_approximation_v2.Add(innerList);
            }
        }

        private void init_main_v()
        {
            for (int i = 0; i < N + 1; ++i)
            {
                for (int j = 0; j < M + 1; ++j)
                {
                    if (i == 0)
                    {
                        v[i][j] = m1_main(y[j]);
                    }
                    else if (j == 0)
                    {
                        v[i][j] = m3_main(x[i]);
                    }
                    else if (i == N)
                    {
                        v[i][j] = m2_main(y[j]);
                    }
                    else if (j == M)
                    {
                        v[i][j] = m4_main(x[i]);
                    }
                    else
                    {
                        v[i][j] = 0;
                    }
                    initial_approximation[i][j] = v[i][j];
                }
            }
        }

        private void init_main_2v()
        {
            N *= 2;
            M *= 2;

            h = (right_border_x - left_border_x) / N;
            k = (right_border_y - left_border_y) / M;

            v2.Clear();
            v2 = new List<List<double>>(N + 1);
            for (int i = 0; i < N + 1; ++i)
            {
                List<double> innerList = new List<double>(M + 1);

                for (int j = 0; j < M + 1; ++j)
                {
                    innerList.Add(0);
                }
                v2.Add(innerList);
            }

            x2.Clear();
            x2 = new List<double>(N + 1);
            for (int i = 0; i < N + 1; i++)
            {
                x2.Add(left_border_x + i * h);
            }

            y2.Clear();
            y2 = new List<double>(M + 1);
            for (int i = 0; i < M + 1; i++)
            {
                y2.Add(left_border_y + i * k);
            }

            for (int i = 0; i < N + 1; ++i)
            {
                for (int j = 0; j < M + 1; ++j)
                {
                    if (i == 0)
                    {
                        v2[i][j] = m1_main(y2[j]);
                    }
                    else if (j == 0)
                    {
                        v2[i][j] = m3_main(x2[i]);
                    }
                    else if (i == N)
                    {
                        v2[i][j] = m2_main(y2[j]);
                    }
                    else if (j == M)
                    {
                        v2[i][j] = m4_main(x2[i]);
                    }
                    else
                    {
                        v2[i][j] = 0;
                    }
                    initial_approximation_v2[i][j] = v2[i][j];
                }
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

            //dataGridView1.Columns.Add(column3);

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

            right_border_x = 2;
            left_border_x = 0;

            right_border_y = 1;
            left_border_y = 0;

            init_getted_data();

            init_containers();

            if ( task_number == 0)
            {
                init_test_u();
                init_test_v();

                MVR(v, x, y, w1, ref S1, ref N_Max1, ref eps_max1, ref eps1);
                
                calc_difference_clear_and_dirty();

                residual1 = residual(v);
            }
            else if (task_number == 1)
            {
                init_initial_approximation_v2();
                init_main_v();
                MVR(v, x, y, w1, ref S1, ref N_Max1, ref eps_max1, ref eps1);
                residual1 = residual(v);
                init_main_2v();
                MVR(v2, x2, y2, w2, ref S2, ref N_Max2, ref eps_max2, ref eps2);
                init_func2();
                //residual2 = residual(v2, func2);
                N /= 2;
                M /= 2;

                h = (right_border_x - left_border_x) / N;
                k = (right_border_y - left_border_y) / M;

                calc_difference_v2_and_v();
            }

            isCalc = true;

            draw_graph();
            init_table();

            reference();
        }

        private double func_main(double x, double y)
        {
            return Math.Abs(x - y);
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

        private bool isBorder(int i, int j)
        {
            return (i == 0 || j == 0 || j == M || i == N) ? true : false;
        }

        private double residual(List<List<double>> matrix)
        {

            double res_glob = 0;
            double res_loc = 0;
            double right_part_of_eq = 0;

            for (int j = 1; j < M; ++j) {
                for (int i = 1; i < N; ++i)
                {

                    double right_part_of_eq_increment = 0;

                    if (j == 1)
                    {
                        if (task_number == 0)
                            right_part_of_eq_increment += k2 * m4_test(x[i]);
                        else
                            right_part_of_eq_increment += k2 * m4_main(x[i]);
                    }
                    else
                    {
                        if (j == M - 1)
                        {
                            if (task_number == 0)
                                right_part_of_eq_increment += k2 * m3_test(x[i]);
                            else
                                right_part_of_eq_increment += k2 * m3_main(x[i]);
                        }
                    }

                    if (i == 1)
                    {
                        if (task_number == 0)
                            right_part_of_eq_increment += h2 * m1_test(y[j]);
                        else
                            right_part_of_eq_increment += h2 * m1_main(y[j]);
                    }
                    else
                    {
                        if (i == N - 1)
                        {
                            if (task_number == 0)
                                right_part_of_eq_increment += h2 * m2_test(y[j]);
                            else
                                right_part_of_eq_increment += h2 * m2_main(y[j]);
                        }
                    }

                    if (task_number == 0)
                        right_part_of_eq = -test_func_f(x[i], y[j]) - right_part_of_eq_increment;
                    else
                        right_part_of_eq = -func_main(x[i], y[j]) - right_part_of_eq_increment;

                    
                    
                    double left_part_of_eq = 0;

                    if (j != 1 && j != M - 1)
                    {
                        if (i != 1 && i != N - 1)
                        {
                            left_part_of_eq = k2 * matrix[i][j-1] + h2 * matrix[i - 1][j]
                                + a2 * matrix[i][j] + h2 * matrix[i+1][j] + 
                                k2 * matrix[i][j + 1];

                        }
                        else if (i == 1)
                        {
                            left_part_of_eq = k2 * matrix[i][j-1]+ 
                                a2 * matrix[i][j] +
                                h2 * matrix[i+1][j]+ k2 *
                                matrix[j+1][i];
                        }
                        else if (i == N - 1)
                        {
                            left_part_of_eq = k2 * matrix[i][j-1]+ h2 * 
                                matrix[i-1][j] + a2 * matrix[i][j] 
                                + k2 * matrix[i][j+1];
                        }
                    }
                    else if (j == 1)
                    {
                        if (i == 1) {
                            left_part_of_eq = a2 * matrix[i][j] + 
                                h2 * matrix[i+1][j] + 
                                k2 * matrix[i][j+1]; 
                        }
                        else if (i != N - 1) {
                            left_part_of_eq = h2 * matrix[i-1][j] + 
                                a2 * matrix[i][j] + 
                                h2 * matrix[i+1][j] + 
                                k2 * matrix[i][j+1]; 
                        }
                        else if (i == N - 1) {
                            left_part_of_eq = h2 * matrix[i-1][j] + 
                                a2 * matrix[i][j] + k2 * matrix[i][j+1]; 
                        }
                    }
                    else if (j == M - 1)
                    {
                        if (i == 1) {
                            left_part_of_eq = k2 * matrix[i][j-1] + 
                                a2 * matrix[i][j] + 
                                h2 * matrix[i+1][j]; 
                        }
                        else if (i != N - 1) {
                            left_part_of_eq = k2 * matrix[i][j-1] + 
                                h2 * matrix[i-1][j] + a2 * matrix[i][j] + 
                                h2 * matrix[i+1][j]; 
                        }
                        else if (i == N - 1) {
                            left_part_of_eq = k2 * matrix[i][j-1] + h2 * matrix[i-1][j] + 
                                a2 * matrix[i][j]; 
                        }
                    }

                    res_loc = Math.Abs(left_part_of_eq - right_part_of_eq);

                    if (res_loc > res_glob)
                        res_glob = res_loc;
                }
            }

            return res_glob;

        }

        private void reference()
        {
            if (task_number == 0)
            {
                Tuple<double, int, int> fault = find_max_deviation();

                this.label3.Text = "Справка: Для решения тестовой задачи использованы сетка с " +
                    "числом разбиений по x n = \"" + System.Convert.ToString(N) + "\" и числом разбиений по " +
                    "y \"" + System.Convert.ToString(M) + "\"," +
                    "метод верхней релаксации с параметром w = " + System.Convert.ToString(w1) +
                    ", применены критерии остановки по точности E_мет = " + System.Convert.ToString(eps1) +
                    "\nи по числу итерции N_max = " + System.Convert.ToString(N_Max1) +
                    " На решение схемы (СЛАУ) затрачено итераций N = " + System.Convert.ToString(S1) +
                    " и достигнута точность итерационного метода E_N = " + System.Convert.ToString(eps_max1) +
                    " СЛАУ решена с невязкой ||R(N)|| = " + System.Convert.ToString(residual1) + ", для невязки исопльзовалась норма \"max\";\n" +
                    "Тестовая задача должа быть решена с погрешностью не более e = 0.5*10^-6; задача решена с погрешностью e1 = " +
                    System.Convert.ToString(fault.Item1) + "\nМаксимальное отклонение точного и численного решений наблюдается в узле " +
                    "x = \"" + System.Convert.ToString(fault.Item2) + "\"; y = \"" + System.Convert.ToString(fault.Item3) + "\"";
            }
            else
            {
                Tuple<double, int, int> fault = find_max_deviation_main();

                this.label3.Text = "Справка: Для решения основной задачи использованы сетка с " +
                   "числом разбиений по x n = \"" + System.Convert.ToString(N) + "\" и числом разбиений по " +
                   "y \"" + System.Convert.ToString(M) + "\"," +
                   "метод верхней релаксации с параметром w1 = " + System.Convert.ToString(w1) +
                   ", применены критерии остановки по точности E_мет = " + System.Convert.ToString(eps1) +
                   "\nи по числу итерции N_max = " + System.Convert.ToString(N_Max1) +
                   " На решение схемы (СЛАУ) затрачено итераций N = " + System.Convert.ToString(S1) +
                   " и достигнута точность итерационного метода E_N = " + System.Convert.ToString(eps_max1) +
                   " СЛАУ решена с\nневязкой ||R(N)|| = " + System.Convert.ToString(residual1) + ", для невязки исопльзовалась норма \"max\";\n" +
                   "Для контроля точности решения использована сетка с половинным шагом, метод верхней релаксации с параметром w2 = \"" + System.Convert.ToString(w2) +
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
                   "В качетсве начально приближения на основной сетке использовано нулево, на сетке с половинным шагом использовано нулево.";
            }
        }
    }
}
