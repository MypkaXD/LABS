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

        public void set_x_borders(double left_border,  double right_border)
        {
            this.left_border_x = left_border;
            this.right_border_x = right_border;

            this.h = (right_border_x - left_border_x) / N;
        }
        public void set_y_borders(double down_border, double upper_border)
        {
            this.down_border_y = down_border;
            this.upper_border_y = upper_border;

            this.k = (upper_border_y - down_border_y) / M;
        }

        public void set_N(int N)
        {
            this.N = N;
        }
        public void set_M(int M)
        {
            this.M = M;
        }

    }
}
