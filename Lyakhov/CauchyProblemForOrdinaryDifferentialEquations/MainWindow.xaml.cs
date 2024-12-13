using System.Reflection.PortableExecutable;
using System.Runtime.InteropServices;
using System.Security.Cryptography.X509Certificates;
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

namespace CauchyProblemForOrdinaryDifferentialEquations
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 

    public class Coord
    {
        public double m_t { get; set; }
        public double m_u1 { get; set; }
        public double m_u2 { get; set; }

        public Coord(double t, double u1, double u2)
        {
            m_t = t;
            m_u1 = u1;
            m_u2 = u2;
        }
    }
    public class GridRowModel
    {
        public Coord m_coord { get; set; } // Привязка объекта координат
        public int m_index { get; set; }   // Индекс элемента
        public double m_step { get; set; } // Шаг
    }

    public partial class MainWindow : Window
    {
        /*
         * Я буду решать задачу 4.3 это соответствует решить задачу 4 методом 3 (метод
         * РК 4-го порядка)
         * 
        */

        public double alpha = 0;
        public double mu = 0;
        public double step = 1;
        public double localEps = 0.0000005;

        public int maxSteps = 1000;
        public int p = 4;

        public List<Coord> coords = new List<Coord>();
        public List<double> steps = new List<double>();

        public void SetInitialConditions()
        {
            double t0 = 0;
            double u1_0 = System.Convert.ToDouble(this.StartConditionsForU1.Text);
            double u2_0 = System.Convert.ToDouble(this.StartConditionsForU2.Text);

            coords.Add(new Coord(t0, u1_0, u2_0));
            steps.Add(0);
        }

        private void SetInitialParams()
        {
            alpha = System.Convert.ToDouble(this.ParamAlpha.Text);
            mu = System.Convert.ToDouble(this.ParamMu.Text);
        }

        private void SetParams()
        {
            maxSteps = System.Convert.ToInt32(this.ConditionsForMaxSteps.Text);
            localEps = System.Convert.ToDouble(this.ConditionsForEps.Text);
            step = System.Convert.ToDouble(this.ConditionsForStartStep.Text);
        }
        public double func2(double t, double u1, double u2) // dk/dt
        {
            return (-mu * (u2 * u2 + alpha) * u2 - u1);
        }

        public double func1(double t, double u1, double u2) // du/dt
        {
            return u2;
        }

        public Coord getNewCoord(Coord currentCoord, double currentStep)
        {
            double k1 = func1(currentCoord.m_t, currentCoord.m_u1, currentCoord.m_u2);
            double l1 = func2(currentCoord.m_t, currentCoord.m_u1, currentCoord.m_u2);

            double k2 = func1(currentCoord.m_t + currentStep / 2, currentCoord.m_u1 + currentStep / 2 * k1, currentCoord.m_u2 + currentStep / 2 * l1);
            double l2 = func2(currentCoord.m_t + currentStep / 2, currentCoord.m_u1 + currentStep / 2 * k1, currentCoord.m_u2 + currentStep / 2 * l1);

            double k3 = func1(currentCoord.m_t + currentStep / 2, currentCoord.m_u1 + currentStep / 2 * k2, currentCoord.m_u2 + currentStep / 2 * l2);
            double l3 = func2(currentCoord.m_t + currentStep / 2, currentCoord.m_u1 + currentStep / 2 * k2, currentCoord.m_u2 + currentStep / 2 * l2);

            double k4 = func1(currentCoord.m_t + currentStep, currentCoord.m_u1 + currentStep * k3, currentCoord.m_u2 + currentStep * l3);
            double l4 = func2(currentCoord.m_t + currentStep, currentCoord.m_u1 + currentStep * k3, currentCoord.m_u2 + currentStep * l3);

            double tNext = currentCoord.m_t + currentStep;
            double u1Next = currentCoord.m_u1 + currentStep / 6 * (k1 + 2 * k2 + 2 * k3 + k4);
            double u2Next = currentCoord.m_u2 + currentStep / 6 * (l1 + 2 * l2 + 2 * l3 + l4);

            return (new Coord(tNext, u1Next, u2Next));

        }

        public void RK_IV(int countOfSteps, bool isUseConstStep)
        {

            for (int i = 0; i < countOfSteps; ++i)
            {
                Coord currentCoord = coords[i];

                Coord coordWithStep = getNewCoord(currentCoord, step);
                Coord coordWithHalfStep = getNewCoord(currentCoord, step / 2);
                Coord coordWithTwiceHalfStep = getNewCoord(coordWithHalfStep, step / 2);

                if (isUseConstStep)
                {
                    Coord nextCoord = coordWithStep;
                    coords.Add(nextCoord);
                    steps.Add(step);
                }
                else
                {
                    double deltaU1 = Math.Abs(coordWithTwiceHalfStep.m_u1 - coordWithStep.m_u1);
                    double deltaU2 = Math.Abs(coordWithTwiceHalfStep.m_u2 - coordWithStep.m_u2);

                    double denominator = Math.Pow(2, p) - 1;

                    if ((deltaU1 / denominator < localEps)
                        || (deltaU2 / denominator < localEps))
                    {
                        Coord nextCoord = coordWithStep;
                        coords.Add(nextCoord);
                        steps.Add(step);
                    }
                    else
                    {
                        Console.WriteLine($"STEP / 2: {step}");
                        step /= 2;
                        i -= 1;
                        continue;
                    }
                }

            }
        }
        public void Hemming(int countOfSteps)
        {
            int accelerationPoints = 4; // количество разгонных точек

            RK_IV(accelerationPoints, isUseConstStep: true); // вычисление разгонных точек

            for (int i = accelerationPoints; i < countOfSteps; ++i)
            {
                // Применение метода Хэмминга для предсказания значений
                double u1_step4 = coords[i - 4].m_u1;
                double f1_step3 = func1(coords[i - 3].m_t, coords[i - 3].m_u1, coords[i - 3].m_u2);
                double f1_step2 = func1(coords[i - 2].m_t, coords[i - 2].m_u1, coords[i - 2].m_u2);
                double f1_step1 = func1(coords[i - 1].m_t, coords[i - 1].m_u1, coords[i - 1].m_u2);

                // Предсказание для u1
                double u1_predict = u1_step4 + 4 * step / 3 * (2 * f1_step1 - f1_step2 + 2 * f1_step3);

                double u2_step4 = coords[i - 4].m_u2;
                double f2_step3 = func2(coords[i - 3].m_t, coords[i - 3].m_u1, coords[i - 3].m_u2);
                double f2_step2 = func2(coords[i - 2].m_t, coords[i - 2].m_u1, coords[i - 2].m_u2);
                double f2_step1 = func2(coords[i - 1].m_t, coords[i - 1].m_u1, coords[i - 1].m_u2);

                // Предсказание для u2
                double u2_predict = u2_step4 + 4 * step / 3 * (2 * f2_step1 - f2_step2 + 2 * f2_step3);

                // Расчет новых значений с использованием метода Хэмминга
                double u1_step1 = coords[i - 1].m_u1;
                double u1_step3 = coords[i - 3].m_u1;
                double f1_predict_step_next = func1(coords[i].m_t, u1_predict, u2_predict);
                double u1_next = (1.0 / 8.0) * (9 * u1_step1 - u1_step3 + 3 * step * (f1_predict_step_next + 2 * f1_step1 - f1_step2));

                double u2_step1 = coords[i - 1].m_u2;
                double u2_step3 = coords[i - 1].m_u2;
                double f2_predict_step_next = func2(coords[i].m_t, u1_predict, u2_predict);
                double u2_next = (1.0 / 8.0) * (9 * u2_step1 - u2_step3 + 3 * step * (f2_predict_step_next + 2 * f2_step1 - f2_step2));

                // Добавляем новое значение в список
                coords.Add(new Coord(coords[i].m_t + step, u1_next, u2_next));
                steps.Add(step);
            }
        }


        public void PrintCoords()
        {
            for (int i = 0; i < coords.Count; ++i)
            {
                Console.WriteLine($"(t_{i}: {coords[i].m_t}; u1_{i} : {coords[i].m_u1}; u2_{i} : {coords[i].m_u2})");
            }
        }

        public MainWindow()
        {
            InitializeComponent();

            this.Method.SelectedIndex = 0;
            this.IsConstStep.IsChecked = true;
        }

        private void ClearPrevSolution()
        {
            coords = new List<Coord>();
            steps = new List<double>();
        }

        private void StartCalcultaing_Click(object sender, RoutedEventArgs e)
        {
            ClearPrevSolution();
            SetInitialConditions();
            SetInitialParams();
            SetParams();

            if (this.Method.SelectedIndex == 0)
            {
                bool isChecked = this.IsConstStep.IsChecked ?? false;
                RK_IV(maxSteps, isChecked);
            }
            else
                Hemming(maxSteps);
            //PrintCoords();
            UpdateGraph();
            UpdateDataGrid();
        }

        private void UpdateDataGrid()
        {
            List<GridRowModel> gridRowModels = new List<GridRowModel>();

            for (int i = 0; i < coords.Count && i < steps.Count; ++i)
            {
                GridRowModel gridRowModel = new GridRowModel();
                gridRowModel.m_step = steps[i];
                gridRowModel.m_coord = coords[i];
                gridRowModel.m_index = i;

                gridRowModels.Add(gridRowModel);
            }

            dgCoords.ItemsSource = gridRowModels;
        }

        private void UpdateGraph()
        {
            if (TabControlGraphs.SelectedItem is TabItem selectedTab)
            {
                string header = selectedTab.Header.ToString();

                List<double> tCoords = new List<double>();
                List<double> u1Coords = new List<double>();
                List<double> u2Coords = new List<double>();

                for (int i = 0; i < coords.Count; ++i)
                {
                    tCoords.Add(coords[i].m_t);
                    u1Coords.Add(coords[i].m_u1);
                    u2Coords.Add(coords[i].m_u2);
                }

                WpfPlotMain.Plot.Clear();
                if (header == "Graph u_1(t)")
                    WpfPlotMain.Plot.Add.Scatter(tCoords.ToArray(), u1Coords.ToArray());
                else if (header == "Graph u_2(t)")
                    WpfPlotMain.Plot.Add.Scatter(tCoords.ToArray(), u2Coords.ToArray());
                else if (header == "Graph (du_1/dt, du_2/dt)")
                    WpfPlotMain.Plot.Add.Scatter(u1Coords.ToArray(), u2Coords.ToArray());
                WpfPlotMain.Plot.Axes.AutoScale();
                WpfPlotMain.Refresh();
            }
        }

        private void TabControlGraphs_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            UpdateGraph();
        }

        private void Method_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (this.Method.SelectedIndex == 0)
            {
                this.IsConstStep.Visibility = Visibility.Visible;
            }
            else
            {
                this.IsConstStep.Visibility = Visibility.Hidden;
            }
        }
    }
}