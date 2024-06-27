using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SOLUTION_OF_THE_DIRICHLET_PROBLEM_FOR_THE_POISSON_EQUATION
{
    public partial class Template
    {
        public void v_to_v2()
        {
            N *= 2;
            M *= 2;
            h = (right_border_x - left_border_x) / N;
            k = (upper_border_y - down_border_y) / M;

        }

        public void v2_to_v()
        {
            N /= 2;
            M /= 2;
            h = (right_border_x - left_border_x) / N;
            k = (upper_border_y - down_border_y) / M;

        }

        public void init_main()
        {
            init_main_net();
            init_main_v();

            init_check_net();
            init_v_for_check_net();

            v2_to_v();
        }

        private void init_v_for_check_net()
        {

            for (int i = 0; i < N + 1; ++i)
            {
                for (int j = 0; j < M + 1; ++j)
                {
                    if (i == 0)
                    {
                        v2[i][j] = Functions.m1_main(y2[j]);
                    }
                    else if (j == 0)
                    {
                        v2[i][j] = Functions.m3_main(x2[i]);
                    }
                    else if (i == N)
                    {
                        v2[i][j] = Functions.m2_main(y2[j]);
                    }
                    else if (j == M)
                    {
                        v2[i][j] = Functions.m4_main(x2[i]);
                    }
                    else
                    {
                        v2[i][j] = 0;
                    }
                    initial_approximation_v2[i][j] = v2[i][j];
                }
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
                        v[i][j] = Functions.m1_main(y[j]);
                    }
                    else if (j == 0)
                    {
                        v[i][j] = Functions.m3_main(x[i]);
                    }
                    else if (i == N)
                    {
                        v[i][j] = Functions.m2_main(y[j]);
                    }
                    else if (j == M)
                    {
                        v[i][j] = Functions.m4_main(x[i]);
                    }
                    else
                    {
                        v[i][j] = 0;
                    }
                    initial_approximation[i][j] = v[i][j];
                }
            }
        }
    }
}
