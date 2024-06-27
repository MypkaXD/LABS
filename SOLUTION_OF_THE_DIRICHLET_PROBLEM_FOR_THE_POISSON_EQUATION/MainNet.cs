using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SOLUTION_OF_THE_DIRICHLET_PROBLEM_FOR_THE_POISSON_EQUATION
{
    public partial class Template
    {
        private void init_main_net()
        {
            create_initial_approximation();
            create_x();
            create_y();
            create_v();
        }

        private void create_initial_approximation()
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
        private void create_x()
        {
            x.Clear();
            x = new List<double>(N + 1);
            for (int i = 0; i < N + 1; i++)
            {
                x.Add(left_border_x + i * h);
            }
        }
        private void create_y()
        {
            y.Clear();
            y = new List<double>(M + 1);
            for (int i = 0; i < M + 1; i++)
            {
                y.Add(down_border_y + i * k);
            }
        }
        private void create_v()
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

    }
}
