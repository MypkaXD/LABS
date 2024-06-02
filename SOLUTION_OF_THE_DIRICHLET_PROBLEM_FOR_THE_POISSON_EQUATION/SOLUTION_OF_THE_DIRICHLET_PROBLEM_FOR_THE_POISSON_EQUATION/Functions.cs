using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SOLUTION_OF_THE_DIRICHLET_PROBLEM_FOR_THE_POISSON_EQUATION
{
    internal class Functions
    {
        // Функции m1 - m4

        static public double m1_main(double y)
        {
            return -y * (y - 1);
        }
        static public double m2_main(double y)
        {
            return y * (1 - y);
        }
        static public double m3_main(double x)
        {
            return Math.Abs(Math.Sin(Math.PI * x));
        }
        static public double m4_main(double x)
        {
            return Math.Abs(Math.Sin(Math.PI * x)) * Math.Exp(x);
        }

        static public double m1_test(double y)
        {
            return 1;
        }
        static public double m2_test(double y)
        {
            return Math.Exp(Math.Sin(Math.PI * 2 * y) * Math.Sin(Math.PI * 2 * y));
        }
        static public double m3_test(double x)
        {
            return 1;
        }
        static public double m4_test(double x)
        {
            return Math.Exp(Math.Sin(Math.PI * x) * Math.Sin(Math.PI * x));
        }

        ///////////////////

        static public double test_func(double x, double y)
        {
            return Math.Exp(Math.Sin(Math.PI * x * y) * Math.Sin(Math.PI * x * y));
        }
        static public double test_func_f(double x, double y)
        {

            double test = 2 * Math.Exp(Math.Pow(Math.Sin(Math.PI * x * y), 2)) *
                Math.Pow(Math.PI, 2) * (y * y + x * x) *
                (Math.Pow(Math.Cos(Math.PI * x * y), 2) - Math.Pow(Math.Sin(Math.PI * x * y), 2) +
                2 * Math.Pow(Math.Sin(Math.PI * x * y), 2) * Math.Pow(Math.Cos(Math.PI * x * y), 2));

            return test;
        }
        static public double test_func_xx(double x, double y)
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
        static public double test_func_yy(double x, double y)
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

        static public double func_main(double x, double y)
        {
            return Math.Pow(Math.Abs(x - y), 4);
        }
    }
}
