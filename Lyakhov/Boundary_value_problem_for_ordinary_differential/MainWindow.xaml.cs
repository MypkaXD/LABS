using ScottPlot;
using System.Reflection.PortableExecutable;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Boundary_value_problem_for_ordinary_differential
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 
    public class SweepMethod
    {
        private List<double> v_coords;
        private List<double> t_coords;

        private int N = 10;
        private double step = 0.1;

        private double a = 0;
        private double b = 1;

        private double m1 = 0;
        private double m2 = 0;
        private double X1 = 0;
        private double X2 = 0;

        private List<double> A;
        private List<double> B;
        private List<double> C;

        private List<double> alpha;
        private List<double> betta;

        private List<double> phi;

        public SweepMethod(int N)
        {
            this.N = N;

            step = Math.Abs(b - a) / N;

            v_coords = new List<double>(Enumerable.Repeat(0.0, N + 1));
            t_coords = new List<double>(Enumerable.Repeat(0.0, N + 1));

            A = new List<double>(Enumerable.Repeat(0.0, N + 1));
            B = new List<double>(Enumerable.Repeat(0.0, N + 1));
            C = new List<double>(Enumerable.Repeat(0.0, N + 1));

            alpha = new List<double>(Enumerable.Repeat(0.0, N + 1));
            betta = new List<double>(Enumerable.Repeat(0.0, N + 1));

            phi = new List<double>(Enumerable.Repeat(0.0, N + 1));

            setTSolve();
            setVSolve();
        }

        private void setVSolve()
        {
            phi[0] = (1.0 - 0.5 * step * coef_phi_i_for_main(a + 0.5 * step, 0.0, 0.5 * step)) /
                (-coef_a_i_for_main(a + step, a, step) / step + 3 - 0.5 * step * coef_d_i_for_main(a + step / 2, 0, step / 2));
            phi[N] = (k_func(a + step * N) - 0.5 * step * coef_phi_i_for_main(a + step * N, a + step * N - step / 2.0, step / 2.0)) /
                (2 * k_func((a + step * N) * (a + step * N)) - coef_a_i_for_main(a + step * N, a + step * (N - 1), step) / step
                    - 0.5 * step * coef_d_i_for_main(a + step * N, a + step * ((double)N - 1.0 / 2.0), step / 2));

            m1 = phi[0];
            m2 = phi[N];

            A[0] = 0;
            B[0] = (coef_a_i_for_main(a + step, a, step) / step) /
                (-coef_a_i_for_main(a + step, a, step) / step + 3 - 0.5 * step * coef_d_i_for_main(a + step / 2, 0, step / 2));
            C[0] = 1;

            X1 = -B[0];

            A[N] = (coef_a_i_for_main(a + step * N, a + step * (N - 1), step) / step) /
                (2 * k_func((a + step * N) * (a + step * N)) - coef_a_i_for_main(a + step * N, a + step * (N - 1), step) / step
                    - 0.5 * step * coef_d_i_for_main(a + step * N, a + step * ((double)N - 1.0 / 2.0), step / 2));
            B[N] = 0;
            C[N] = 1;

            X2 = -A[N];

            for (int count = 1; count < N; ++count)
                phi[count] = -coef_phi_i_for_main(a + (count + 1.0 / 2.0) * step, a + (count - 1.0 / 2.0) * step, step);

            for (int count = 1; count < N; ++count)
            { // установка оставшихся коэффов для 3-х диагональной части матрицы. (от 1 до N-1) ВАЖНО
                A[count] = coef_a_i_for_main(a + count * step, a + (count - 1) * step, step) / Math.Pow(step, 2); // согласно ЛЕКЦИЯМ
                B[count] = coef_a_i_for_main(a + (count + 1) * step, a + count * step, step) / Math.Pow(step, 2); // согласно ЛЕКЦИЯМ
                C[count] = (A[count] + B[count]) + coef_d_i_for_main(a + (count + 1.0 / 2.0) * step, a + (count - 1.0 / 2.0) * step, step); // согласно ЛЕКЦИЯМ
            }

            straight_stroke();
            reverse_stroke();
        }

        private double k_func(double x)
        {
            double value = Math.Exp(x * x);
            return value;
        }

        private double q_func(double x)
        {
            double value = (4 - x * x) * Math.Exp(x * x);
            return value;
        }

        private double f_func(double x)
        {
            double value = Math.Exp(x * x) * (-x * x * x * x - x * x * x + 2 * x - 2);
            return value;
        }

        private double coef_a_i_for_main(double x_plus, double x_minus, double h)
        { // расчет коэффа a_i с помощью метода прямоугольников
            double value = Math.Pow(1.0 / h * ((x_plus - x_minus) / k_func((x_plus + x_minus) / 2)), -1);
            return value;
        }

        // АНАЛОГИЧНО ПРИМЕРУ ВЫШЕ
        private double coef_d_i_for_main(double x_plus, double x_minus, double h)
        {
            double value = (1.0 / h * ((x_plus - x_minus) * q_func((x_plus + x_minus) / 2)));
            return value;

        }
        // АНАЛОГИЧНО ПРИМЕРУ ВЫШЕ
        private double coef_phi_i_for_main(double x_plus, double x_minus, double h)
        {
            double value = (1.0 / h * ((x_plus - x_minus) * f_func((x_plus + x_minus) / 2)));
            return value;
        }

        private void straight_stroke()
        { // прямой ход прогонки
            alpha[1] = X1;
            betta[1] = m1;

            for (int count = 1; count < N; ++count)
            {
                alpha[count + 1] = B[count] / (C[count] - A[count] * alpha[count]);
                betta[count + 1] = (-phi[count] + A[count] * betta[count]) / (C[count] - A[count] * alpha[count]);
            }
        }

        private void reverse_stroke()
        {
            v_coords[N] = (-X2 * betta[N] - m2) / (X2 * alpha[N] - 1);

            for (int count = N - 1; count >= 0; --count)
            {
                v_coords[count] = alpha[count + 1] * v_coords[count + 1] + betta[count + 1];
            }
        }

        private void setTSolve()
        {
            for (int i = 0; i <= N; ++i)
            {
                t_coords[i] = a + i * step;
            }
        }

        public List<double> getSolution()
        {
            return v_coords;
        }
    }
    public partial class MainWindow : Window
    {

        private List<double> u_coords;
        private List<double> t_coords;
        private List<double> v_coordsForStep;
        private List<double> v_coordsForHalfStep;
        private List<double> v_Runge;

        private int N = 10;

        public MainWindow()
        {
            InitializeComponent();
        }

        private double getUPoint(double t)
        {
            return t * t + t;
        }
        private void setUSolve()
        {
            u_coords = new List<double>();

            double a = 0;
            double b = 1;
            double step = Math.Abs(b - a) / (2 * N);

            for (int i = 0; i < 2 * N + 1; ++i)
            {
                u_coords.Add(getUPoint(a + i * step));
            }
        }

        private void setTSolve()
        {
            t_coords = new List<double>();

            double a = 0;
            double b = 1;
            double step = Math.Abs(b - a) / (2 * N);

            for (int i = 0; i < 2 * N + 1; ++i)
            {
                t_coords.Add(a + i * step);
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            N = System.Convert.ToInt32(this.NParam.Text);

            SweepMethod sweepMethodForStep = new SweepMethod(N);
            v_coordsForStep = sweepMethodForStep.getSolution();
            SweepMethod sweepMethodForHalfStep = new SweepMethod(2 * N);
            v_coordsForHalfStep = sweepMethodForHalfStep.getSolution();

            setUSolve();
            setTSolve();
            setRungeSolve();

            UpdateDataGrid();
            UpdateGraph();
        }

        private void setRungeSolve()
        {
            v_Runge = new List<double>();

            int p = 2;
            double r = 0.5;

            for (int i = 0; i < 2 * N + 1; ++i)
            {
                if (i % 2 == 0)
                {
                    v_Runge.Add(v_coordsForStep[i / 2] +
                        (v_coordsForStep[i / 2] - v_coordsForHalfStep[i]) / (Math.Pow(r, p) - 1));
                }
            }
        }

        private void UpdateDataGrid()
        {
            List<GridRowModel> gridRowModels = new List<GridRowModel>();

            double maxEps = 0;

            for (int i = 0; i < 2 * N + 1; ++i)
            {
                GridRowModel gridRowModel = new GridRowModel();
                gridRowModel.x_i = t_coords[i];
                if (i % 2 == 0)
                {
                    gridRowModel.y_i_with_step = v_coordsForStep[i / 2];
                    gridRowModel.eps = Math.Abs(v_coordsForStep[i / 2] - v_coordsForHalfStep[i]);
                    gridRowModel.y_i_Runge = v_Runge[i / 2];

                    if (gridRowModel.eps > maxEps)
                    {
                        this.EpsMax.Content = "EpsMax: " + gridRowModel.eps + " in " + gridRowModel.x_i;
                    }
                }
                gridRowModel.y_i_with_half_step = v_coordsForHalfStep[i];
                gridRowModel.u_i = u_coords[i];

                gridRowModels.Add(gridRowModel);
            }

            dgCoords.ItemsSource = gridRowModels;
        }

        private void UpdateGraph()
        {
            List<double> tCoordsWithStep = new List<double>();

            for (int i = 0; i < 2 * N + 1; ++i)
            {
                if (i % 2 == 0)
                    tCoordsWithStep.Add(t_coords[i]);
            }

            WpfPlotMain.Plot.Clear();
            var scatter1 = WpfPlotMain.Plot.Add.Scatter(t_coords.ToArray(), u_coords.ToArray(), ScottPlot.Colors.Red);
            scatter1.LegendText = "u(t)";
            var scatter2 = WpfPlotMain.Plot.Add.Scatter(tCoordsWithStep.ToArray(), v_coordsForStep.ToArray(), ScottPlot.Colors.Blue);
            scatter2.LegendText = "v_N(t)";
            var scatter3 = WpfPlotMain.Plot.Add.Scatter(tCoordsWithStep.ToArray(), v_Runge.ToArray(), ScottPlot.Colors.Purple);
            scatter3.LegendText = "v_Runge(t)";
            var scatter4 = WpfPlotMain.Plot.Add.Scatter(t_coords.ToArray(), v_coordsForHalfStep.ToArray(), ScottPlot.Colors.Green);
            scatter4.LegendText = "v_2N(t)";
            WpfPlotMain.Plot.ShowLegend();
            WpfPlotMain.Plot.Axes.AutoScale();
            WpfPlotMain.Refresh();
        }
    }

    public class GridRowModel
    {
        public double x_i { get; set; }
        public double y_i_with_step { get; set; }
        public double y_i_with_half_step { get; set; }
        public double y_i_Runge { get; set; }
        public double u_i { get; set; }
        public double eps { get; set; }
    }
}