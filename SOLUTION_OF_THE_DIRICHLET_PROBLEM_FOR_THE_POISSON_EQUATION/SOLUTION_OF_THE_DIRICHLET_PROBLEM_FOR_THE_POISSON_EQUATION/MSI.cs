using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Permissions;
using System.Text;
using System.Threading.Tasks;

namespace SOLUTION_OF_THE_DIRICHLET_PROBLEM_FOR_THE_POISSON_EQUATION
{
    internal class MSI
    {

        private int N;
        private int M;

        private int S;

        private double h;
        private double k;

        private double left_border_x;
        private double right_border_x;

        private double down_border_y;
        private double upper_border_y;

        public void set_h(double h)
        {
            this.h = h;
        }
        public void set_k(double k)
        {
            this.k = k;
        }

        public void set_x_borders(double left_border,  double right_border)
        {
            this.left_border_x = left_border;
            this.right_border_x = right_border;
        }
        public void set_y_borders(double down_border, double upper_border)
        {
            this.down_border_y = down_border;
            this.upper_border_y = upper_border;
        }

        public void set_N(int N)
        {
            this.N = N;
        }
        public void set_M(int M)
        {
            this.M = M;
        }


        public void init_MSI()
        {
            S = 0;
        }

        public void MSI_calc(ref List<List<double>> matrix, int N_Max, double eps)
        {
            return;
        }

        private List<double> calc_residual()
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
