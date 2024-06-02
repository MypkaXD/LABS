using System;
using System.Collections.Generic;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SOLUTION_OF_THE_DIRICHLET_PROBLEM_FOR_THE_POISSON_EQUATION
{
    internal class MethodVerRel
    {

        public double w = 1.9; // [0;2]

        public double h;
        public double k;

        double eps_cur = 0; // для подсчета текущего значения прироста
        double eps;

        int N;
        int M;

        int N_Max;

        double  h2;
        double  k2;
        double  a2;

        public List<double> right_part_v = new List<double>();

        public MethodVerRel(int n_max, int n, int m, double w, double h, double k, double eps, ref List<double> x_current, ref List<double> y_current,
            ref List<List<double>> matrix, int task_number, ref double residual_value, ref int S, ref double eps_max)
        {
            this.N_Max = n_max;

            this.N = n;
            this.M = m;

            this.w = w;

            this.eps = eps;
            this.h = h;
            this.k = k;

            h2 = 1.0 / (h * h);
            k2 = 1.0 / (k * k);
            a2 = -2.0 * (h2 + k2);

            set_right_part(ref x_current, ref y_current, task_number);
            MVR(ref matrix, ref x_current, ref y_current, task_number, ref S, ref eps_max);
            residual_value = residual(ref matrix);
        }

        private void set_right_part(ref List<double> x_current, ref List<double> y_current, int task_number)
        {

            right_part_v.Clear();

            for (int j = 1; j < M; ++j)
            {
                for (int i = 1; i < N; ++i)
                {

                    double right_part_of_eq_increment = 0;

                    if (j == 1)
                    {
                        if (task_number == 0)
                            right_part_of_eq_increment += k2 * Functions.m3_test(x_current[i]);
                        else
                            right_part_of_eq_increment += k2 * Functions.m3_main(x_current[i]);
                    }
                    else
                    {
                        if (j == M - 1)
                        {
                            if (task_number == 0)
                                right_part_of_eq_increment += k2 * Functions.m4_test(x_current[i]);
                            else
                                right_part_of_eq_increment += k2 * Functions.m4_main(x_current[i]);
                        }
                    }

                    if (i == 1)
                    {
                        if (task_number == 0)
                            right_part_of_eq_increment += h2 * Functions.m1_test(y_current[j]);
                        else
                            right_part_of_eq_increment += h2 * Functions.m1_main(y_current[j]);
                    }
                    else
                    {
                        if (i == N - 1)
                        {
                            if (task_number == 0)
                                right_part_of_eq_increment += h2 * Functions.m2_test(y_current[j]);
                            else
                                right_part_of_eq_increment += h2 * Functions.m2_main(y_current[j]);
                        }
                    }

                    if (task_number == 0)
                        right_part_v.Add(Functions.test_func_f(x_current[i], y_current[j]) - right_part_of_eq_increment);
                    else
                        right_part_v.Add(Functions.func_main(x_current[i], y_current[j]) - right_part_of_eq_increment);

                }
            }
        }

        public void MVR(ref List<List<double>> matrix, ref List<double> x_temp, ref List<double> y_temp, int task_number, ref int S, ref double eps_max)
        {

            double v_old; // старое значение преобразуемой компоненты вектора v
            double v_new; // новое значение преобразуемой компоненты вектора v

            bool check = false; // условие остановки

            S = 0;

            while (!check)
            {
                eps_max = 0;

                for (int j = 1; j < M; j++)
                {
                    for (int i = 1; i < N; i++)
                    {
                        v_old = matrix[i][j];
                        v_new = -w * (h2 * (matrix[i + 1][j] + matrix[i - 1][j]) +
                            k2 * (matrix[i][j + 1] + matrix[i][j - 1]));

                        if (task_number == 0)
                            v_new = v_new + (1 - w) * a2 * matrix[i][j] + w * Functions.test_func_f(x_temp[i], y_temp[j]);
                        else
                            v_new = v_new + (1 - w) * a2 * matrix[i][j] + w * Functions.func_main(x_temp[i], y_temp[j]);

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
        private bool isBorder(int i, int j)
        {
            return (i == 0 || j == 0 || j == M || i == N) ? true : false;
        }

        private double residual(ref List<List<double>> matrix)
        {

            double res_loc = 0;
            double res = 0;

            for (int j = 1; j < M; ++j)
            {
                for (int i = 1; i < N; ++i)
                {
                    double left_part_of_eq = 0;

                    left_part_of_eq = a2 * matrix[i][j] +
                        h2 * matrix[i + 1][j] * System.Convert.ToInt32(!isBorder(i + 1, j)) +
                        k2 * matrix[i][j + 1] * System.Convert.ToInt32(!isBorder(i, j + 1)) +
                        h2 * matrix[i - 1][j] * System.Convert.ToInt32(!isBorder(i - 1, j)) +
                        k2 * matrix[i][j - 1] * System.Convert.ToInt32(!isBorder(i, j - 1));

                    res_loc = left_part_of_eq - right_part_v[(i - 1) + (j - 1) * (N - 1)];

                    if (Math.Abs(res_loc) > res)
                        res = Math.Abs(res_loc);
                }
            }

            return res;

        }
    }
}
