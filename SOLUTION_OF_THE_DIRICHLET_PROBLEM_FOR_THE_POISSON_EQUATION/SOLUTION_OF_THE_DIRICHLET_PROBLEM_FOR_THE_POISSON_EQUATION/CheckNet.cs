using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace SOLUTION_OF_THE_DIRICHLET_PROBLEM_FOR_THE_POISSON_EQUATION
{
    public partial class Template
    {

        public void init_check_net()
        {
            N *= 2;
            M *= 2;
            h = (right_border_x - left_border_x) / N;
            k = (upper_border_y - down_border_y) / M;

            create_initial_approximation_v2();
            create_x_for_check_net();
            create_y_for_check_net();
            create_v_for_check_net();

            N /= 2;
            M /= 2;
            h = (right_border_x - left_border_x) / N;
            k = (upper_border_y - down_border_y) / M;
        }

        private void create_y_for_check_net()
        {
            y2.Clear();
            y2 = new List<double>(M + 1);
            for (int i = 0; i < M + 1; i++)
            {
                y2.Add(down_border_y + i * k);
            }
        }

        private void create_v_for_check_net()
        {
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
        }

        private void create_x_for_check_net()
        {
            x2.Clear();
            x2 = new List<double>(N + 1);
            for (int i = 0; i < N + 1; i++)
            {
                x2.Add(left_border_x + i * h);
            }
        }

        private void create_initial_approximation_v2()
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
    }
}
