using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SOLUTION_OF_THE_DIRICHLET_PROBLEM_FOR_THE_POISSON_EQUATION
{
    public partial class Template
    {
        private void init_test()
        {
            init_main_net();
            init_test_u();
            init_test_v();
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
                    innerList.Add(Functions.test_func(x[i], y[j]));
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

    }
}
