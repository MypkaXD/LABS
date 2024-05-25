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
        public List<List<double>> func = new List<List<double>>();

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

        public double A = 0;

        public double w = 1.5; // [0;2]

        public int N_Max; // число итераций

        int S = 0;
        double eps; // минимально допустимый прирост
        double eps_max = 0; // текущее значение прироста
        double eps_cur = 0; // для подсчета текущего значения прироста
        double a2, k2, h2; // ненулевые элементы матрицы (–A)

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

        private void init_func()
        {
            func.Clear();
            func = new List<List<double>>(N + 1);
            for (int i = 0; i < N + 1; ++i)
            {
                List<double> innerList = new List<double>(M + 1);

                for (int j = 0; j < M + 1; ++j)
                {
                    if (task_number == 0)
                        innerList.Add(test_func(x[i], y[j]));
                }
                func.Add(innerList);
            }
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

        private void init_u()
        {
            u.Clear();
            u = new List<List<double>>(N + 1);
            for (int i = 0; i < N + 1; ++i)
            {
                List<double> innerList = new List<double>(M + 1);
                for (int j = 0; j < M + 1; ++j)
                {
                    innerList.Add(0);
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
                        double value = test_func(x[i], y[j]);
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

        private double m1(double y)
        {
            return -y * (y - 1);
        }
        private double m2(double y)
        {
            return y * (y - 1);
        }
        private double m3(double x)
        {
            return Math.Abs(Math.Sin(Math.PI * x));
        }
        private double m4(double x)
        {
            return Math.Abs(Math.Sin(Math.PI * x)) * Math.Exp(x);
        }
        private void init_main_v()
        {
            for (int i = 0; i < N + 1; ++i)
            {
                for (int j = 0; j < M + 1; ++j)
                {
                    if (i == 0)
                    {
                        v[i][j] = m1(y[j]);
                    }
                    else if (j == 0)
                    {
                        v[i][j] = m3(x[i]);
                    }
                    else if (i == N)
                    {
                        v[i][j] = m2(y[j]);
                    }
                    else if (j == M)
                    {
                        v[i][j] = m4(x[i]);
                    }
                    else
                    {
                        v[i][j] = 0;
                    }
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
                        v2[i][j] = m1(y2[j]);
                    }
                    else if (j == 0)
                    {
                        v2[i][j] = m3(x2[i]);
                    }
                    else if (i == N)
                    {
                        v2[i][j] = m2(y2[j]);
                    }
                    else if (j == M)
                    {
                        v2[i][j] = m4(x2[i]);
                    }
                    else
                    {
                        v2[i][j] = 0;
                    }
                }
            }
        }

        private void init_test_u()
        {
            for (int i = 0; i < N + 1; ++i)
            {
                for (int j = 0; j < M + 1; ++j)
                {
                    u[i][j] = test_func(x[i], y[j]);
                }
            }
        }

        private void init_getted_data()
        {
            N = System.Convert.ToInt32(this.inputN.Text);
            M = System.Convert.ToInt32(this.inputM.Text);
            h = (right_border_x - left_border_x) / N;
            k = (right_border_y - left_border_y) / M;
            N_Max = System.Convert.ToInt32(this.input_N_max.Text);
            eps = System.Convert.ToDouble(this.input_E_met.Text);

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

            for (int i = 0; i < (task_number == 1 && draw_graph_number == 1 ? 2 * N + 1 : N + 1); ++i)
            {
                var column_temp = new DataGridViewColumn();
                column_temp.HeaderText = "x_" + System.Convert.ToString(i);
                column_temp.CellTemplate = new DataGridViewTextBoxCell();
                column_temp.ReadOnly = true; //значение в этой колонке нельзя править
                dataGridView1.Columns.Add(column_temp);
            }

            for (int j = 0; j < (draw_graph_number == 1 && task_number == 1 ? 2 * M + 2 : M + 2); ++j)
            {
                dataGridView1.Rows.Add();
                if (j != 0)
                    dataGridView1[0, j].Value = "y_" + System.Convert.ToString(j - 1);
                else
                    dataGridView1[0, 0].Value = "yj";
                for (int i = 1; i < (draw_graph_number == 1 && task_number == 1 ? 2 * N + 3 : N + 3); ++i)
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
                                    dataGridView1[i, j].Value = System.Convert.ToString(difference_v2_and_v[i - 2][j - 1]);
                            }
                        }
                    }
                }
            }

            //dataGridView1.Columns.Add(column3);

            dataGridView1.AllowUserToAddRows = false; //запрешаем пользователю самому добавлять строки

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

            init_initial_approximation();
            init_x();
            init_y();
            init_v();
            init_u();

            if ( task_number == 0)
            {
                init_func();
                init_test_v();
                init_test_u();
                MVR(v, x, y);
                calc_difference_clear_and_dirty();
            }
            else if (task_number == 1)
            {
                init_main_v();
                MVR(v, x, y);
                init_main_2v();
                MVR(v2, x2, y2);
                
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

        private double func_f(double x, double y)
        {
            return -Math.Abs(x - y);
        }

        private void MVR(List<List<double>> matrix, List<double> x_temp, List<double> y_temp)
        {

            double v_old; // старое значение преобразуемой компоненты вектора v
            double v_new; // новое значение преобразуемой компоненты вектора v
            bool f = false; // условие остановки

            S = 0;
            h2 = 1.0 / (h * h);
            k2 = 1.0 / (k * k);
            a2 = -2.0 * (h2 + k2);

            while (!f)
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
                        {
                            v_new = v_new + (1 - w) * a2 * matrix[i][j] + w * func_f(x_temp[i], y_temp[j]);
                        }
                        v_new = v_new / a2;
                        eps_cur = Math.Abs(v_old - v_new);
                        if (eps_cur > eps_max)
                        {
                            eps_max = eps_cur;
                        }

                        matrix[i][j] = v_new;
                    }
                }
                S = S + 1;
                if ((eps_max < eps) || (S >= N_Max))
                {
                    f = true;
                }
            }
        }

        private double test_func(double x, double y)
        {

            return Math.Exp(Math.Sin(Math.PI * x * y) * Math.Sin(Math.PI * x * y));

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

        private double test_func_f(double x, double y)
        {

            double test = test_func_xx(x, y) + test_func_yy(x, y);

            return test;
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
                this.draw_graph_box.Items.Add("Разность численных решений v(N)(x,y) и v2(N2) - поверхность");
               
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
                cPoint3D[,] i_Points3D = new cPoint3D[x.Count, y.Count];
                for (int X = 0; X < x.Count; X++)
                {
                    for (int Y = 0; Y < y.Count; Y++)
                    {
                        i_Points3D[X, Y] = new cPoint3D(x[X], y[Y], v[X][Y]);
                    }
                }
                graph3D1.SetSurfacePoints(i_Points3D, eNormalize.Separate);
            }

            if (task_number == 0)
            {
                if (draw_graph_number == 1)
                {
                    cPoint3D[,] i_Points3D = new cPoint3D[x.Count, y.Count];
                    for (int X = 0; X < x.Count; X++)
                    {
                        for (int Y = 0; Y < y.Count; Y++)
                        {
                            i_Points3D[X, Y] = new cPoint3D(x[X], y[Y], u[X][Y]);
                        }
                    }
                    graph3D1.SetSurfacePoints(i_Points3D, eNormalize.Separate);
                }
                if (draw_graph_number == 2)
                {
                    cPoint3D[,] i_Points3D = new cPoint3D[x.Count, y.Count];
                    for (int X = 0; X < x.Count; X++)
                    {
                        for (int Y = 0; Y < y.Count; Y++)
                        {
                            i_Points3D[X, Y] = new cPoint3D(x[X], y[Y], initial_approximation[X][Y]);
                        }
                    }
                    graph3D1.SetSurfacePoints(i_Points3D, eNormalize.Separate);
                }
                if (draw_graph_number == 3)
                {
                    cPoint3D[,] i_Points3D = new cPoint3D[x.Count, y.Count];
                    for (int X = 0; X < x.Count; X++)
                    {
                        for (int Y = 0; Y < y.Count; Y++)
                        {
                            i_Points3D[X, Y] = new cPoint3D(x[X], y[Y], difference_clear_and_dirty[X][Y]);
                        }
                    }
                    graph3D1.SetSurfacePoints(i_Points3D, eNormalize.Separate);
                }

            }
            else
            {
                if (draw_graph_number == 1)
                {
                    cPoint3D[,] i_Points3D = new cPoint3D[x2.Count, y2.Count];
                    Console.WriteLine(x.Count);
                    Console.WriteLine(x2.Count);
                    for (int X = 0; X < x2.Count; X++)
                    {
                        for (int Y = 0; Y < y2.Count; Y++)
                        {
                            i_Points3D[X, Y] = new cPoint3D(x2[X], y2[Y], v2[X][Y]);
                        }
                    }
                    graph3D1.SetSurfacePoints(i_Points3D, eNormalize.Separate);
                }
                if (draw_graph_number == 2)
                {
                    cPoint3D[,] i_Points3D = new cPoint3D[x.Count, y.Count];
                    Console.WriteLine(x.Count);
                    Console.WriteLine(x2.Count);
                    for (int X = 0; X < x.Count; X++)
                    {
                        for (int Y = 0; Y < y.Count; Y++)
                        {
                            i_Points3D[X, Y] = new cPoint3D(x[X], y[Y], difference_v2_and_v[X][Y]);
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

        private double residual()
        {
            
            double res = 0;

            for (int i = 1; i < N; ++i)
            {
                for (int j = 1; j < M; ++j)
                {
                    double value = h2 * (v[i - 1][j] + -2 * v[i][j] + v[i + 1][j]) + 
                        k2 * (v[i][j - 1] - 2 * v[i][j] + v[i][j + 1]) + func[i][j];

                    if (value > res)
                        res = value;
                }
            }

            return res;
        }

        private void reference()
        {
            if (task_number == 0)
            {
                Tuple<double, int, int> fault = find_max_deviation();

                this.label3.Text = "Справка: Для решения тестовой задачи использованы сетка с " +
                    "числом разбиений по x n = \"" + System.Convert.ToString(N) + "\" и числом разбиений по " +
                    "y \"" + System.Convert.ToString(M) + "\"," +
                    "метод верхней релаксации с параметром w = " + System.Convert.ToString(w) +
                    ", применены критерии остановки по точности E_мет = " + System.Convert.ToString(eps) +
                    "\nи по числу итерции N_max = " + System.Convert.ToString(N_Max) +
                    " На решение схемы (СЛАУ) затрачено итераций N = " + System.Convert.ToString(S) +
                    " и достигнута точность итерационного метода E_N = " + System.Convert.ToString(eps_max) +
                    "СЛАУ решена с невязкой ||R(N)|| = " + System.Convert.ToString(residual()) + " для невязки исопльзовалась норма \"max\";\n" +
                    "Тестовая задача должа быть решена с погрешностью не более e = 0.5*10^-6; задача решена с погрешностью e1 = " +
                    System.Convert.ToString(fault.Item1) + "\nМаксимальное отклонение точного и численного решений наблюдается в узле " +
                    "x = \"" + System.Convert.ToString(fault.Item2) + "\"; y = \"" + System.Convert.ToString(fault.Item3) + "\"";
            }

        }
    }
}
