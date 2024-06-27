using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SOLUTION_OF_THE_DIRICHLET_PROBLEM_FOR_THE_POISSON_EQUATION
{
    internal class Gauss
    {
        int N;
        int M;

        double h2;
        double k2;
        double a2;

        public double h;
        public double k;

        public List<double> right_part_v = new List<double>();
        public List<List<double>> A_matrix = new List<List<double>>();

        public Gauss(int n, int m, double h, double k, ref List<double> x_current, ref List<double> y_current,
            ref List<List<double>> matrix, int task_number)
        {

            this.N = n;
            this.M = m;

            this.h = h;
            this.k = k;

            h2 = 1.0 / (h * h);
            k2 = 1.0 / (k * k);
            a2 = -2.0 * (h2 + k2);

            double d, s;

            set_right_part(ref x_current, ref y_current, task_number);

            for (int j = 1; j < (M-1)*(N-1); ++j)
            {
                List<double> A_vec = new List<double>();
                for (int i = 1; i < (M - 1) * (N - 1); ++i)
                {
                    if (i == j)
                        A_vec.Add(a2);
                    else if (i == j + 1)
                    {
                        if (i != N)
                            A_vec.Add(h2);
                    }
                    else if (i != 0)
                    {
                        if (!isBorder(i, j))
                            A_vec.Add(h2);
                    }
                    else
                        A_vec.Add(0);
                }
                A_matrix.Add(A_vec);
            }

            for (int i = 0; i < A_matrix.Count; ++i)
            {
                for (int j = 0; j < A_matrix[i].Count; ++j)
                {
                    Console.Write(A_matrix[i][j] + "\t");
                }
                Console.WriteLine();
            }
        }

        private bool isBorder(int i, int j)
        {
            return (i == 0 || j == 0 || j == M || i == N) ? true : false;
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
    
    }
}
