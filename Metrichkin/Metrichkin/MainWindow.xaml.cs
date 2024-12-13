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

namespace Metrichkin
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public class Coord
    {
        public double m_t { get; set; }
        public double m_x { get; set; }
        public double m_y { get; set; }

        public Coord(double t, double x, double y)
        {
            m_t = t;
            m_x = x;
            m_y = y;
        }
    }
    public class GridRowModel
    {
        public Coord m_coord_for_RK { get; set; } // Привязка объекта координат
        public Coord m_coord_for_VDP { get; set; } // Привязка объекта координат
        public int m_index { get; set; }   // Индекс элемента
        public double m_step { get; set; } // Шаг
    }

    public partial class MainWindow : Window
    {
        public double h = 0;
        public double k = 0;
        public double m = 0;
        public double k_ = 0;
        public double f0 = 0;
        public double omega = 0;


        public double step = 1;
        public double time = 0;
        public double mu = 0.01;

        public int maxSteps = 1000;
        public int p = 4;

        public List<Coord> coordsForRKIV = new List<Coord>();
        public List<Coord> coordsForVanDerPol = new List<Coord>();
        public List<double> steps = new List<double>();

        public void SetInitialConditions()
        {
            double t0 = 0;
            double x_0 = System.Convert.ToDouble(this.StartConditionsForX.Text);
            double y_0 = System.Convert.ToDouble(this.StartConditionsForY.Text);

            coordsForRKIV.Add(new Coord(t0, x_0, y_0));
            coordsForVanDerPol.Add(new Coord(t0, x_0, y_0));

            steps.Add(0);
        }

        private void SetInitialParams()
        {
            m = System.Convert.ToDouble(this.mParam.Text);
            h = System.Convert.ToDouble(this.hParam.Text);
            k = System.Convert.ToDouble(this.kParam.Text);
            k_ = System.Convert.ToDouble(this.k_Param.Text);
            f0 = System.Convert.ToDouble(this.f0Param.Text);
            omega = System.Convert.ToDouble(this.omegaParam.Text);

            mu = System.Convert.ToDouble(this.muParam.Text);
        }

        private void SetParams()
        {
            maxSteps = System.Convert.ToInt32(this.ConditionsForMaxSteps.Text);
            step = System.Convert.ToDouble(this.ConditionsForStartStep.Text);
        }
        public double func2VDP(double t, double x, double y) // dk/dt
        {
            return 1;
        }

        public double func1VDP(double t, double x, double y) // du/dt
        {
            return 0;
        }

        public double func1RK(double t, double x, double y) // du/dt
        {
            return y;
        }
        public double func2RK(double t, double x, double y) // dk/dt
        {
            return (-h / m) * y - (k / m) * x - (k_ / m) * x * x * x + f0 / m * Math.Cos(omega * t);
        }

        public Coord getNewCoordRK(Coord currentCoord, double currentStep)
        {
            double k1 = func1RK(currentCoord.m_t, currentCoord.m_x, currentCoord.m_y);
            double l1 = func2RK(currentCoord.m_t, currentCoord.m_x, currentCoord.m_y);

            double k2 = func1RK(currentCoord.m_t + currentStep / 2, currentCoord.m_x + currentStep / 2 * k1, currentCoord.m_y + currentStep / 2 * l1);
            double l2 = func2RK(currentCoord.m_t + currentStep / 2, currentCoord.m_x + currentStep / 2 * k1, currentCoord.m_y + currentStep / 2 * l1);

            double k3 = func1RK(currentCoord.m_t + currentStep / 2, currentCoord.m_x + currentStep / 2 * k2, currentCoord.m_y + currentStep / 2 * l2);
            double l3 = func2RK(currentCoord.m_t + currentStep / 2, currentCoord.m_x + currentStep / 2 * k2, currentCoord.m_y + currentStep / 2 * l2);

            double k4 = func1RK(currentCoord.m_t + currentStep, currentCoord.m_x + currentStep * k3, currentCoord.m_y + currentStep * l3);
            double l4 = func2RK(currentCoord.m_t + currentStep, currentCoord.m_x + currentStep * k3, currentCoord.m_y + currentStep * l3);

            double tNext = currentCoord.m_t + currentStep;
            double xNext = currentCoord.m_x + currentStep / 6 * (k1 + 2 * k2 + 2 * k3 + k4);
            double yNext = currentCoord.m_y + currentStep / 6 * (l1 + 2 * l2 + 2 * l3 + l4);

            return (new Coord(tNext, xNext, yNext));

        }

        public Coord getNewCoordVDP(Coord currentCoord, double currentStep)
        {
            double k1 = func1VDP(currentCoord.m_t, currentCoord.m_x, currentCoord.m_y);
            double l1 = func2VDP(currentCoord.m_t, currentCoord.m_x, currentCoord.m_y);

            double k2 = func1VDP(currentCoord.m_t + currentStep / 2, currentCoord.m_x + currentStep / 2 * k1, currentCoord.m_y + currentStep / 2 * l1);
            double l2 = func2VDP(currentCoord.m_t + currentStep / 2, currentCoord.m_x + currentStep / 2 * k1, currentCoord.m_y + currentStep / 2 * l1);

            double k3 = func1VDP(currentCoord.m_t + currentStep / 2, currentCoord.m_x + currentStep / 2 * k2, currentCoord.m_y + currentStep / 2 * l2);
            double l3 = func2VDP(currentCoord.m_t + currentStep / 2, currentCoord.m_x + currentStep / 2 * k2, currentCoord.m_y + currentStep / 2 * l2);

            double k4 = func1VDP(currentCoord.m_t + currentStep, currentCoord.m_x + currentStep * k3, currentCoord.m_y + currentStep * l3);
            double l4 = func2VDP(currentCoord.m_t + currentStep, currentCoord.m_x + currentStep * k3, currentCoord.m_y + currentStep * l3);

            double tNext = currentCoord.m_t + currentStep;
            double xNext = currentCoord.m_x + currentStep / 6 * (k1 + 2 * k2 + 2 * k3 + k4);
            double yNext = currentCoord.m_y + currentStep / 6 * (l1 + 2 * l2 + 2 * l3 + l4);

            return (new Coord(tNext, xNext, yNext));

        }

        public void RK_IV()
        {

            for (int i = 0; i < maxSteps; ++i)
            {
                Coord currentCoordRK = coordsForRKIV[i];
                Coord coordWithStepRK = getNewCoordRK(currentCoordRK, step);

                coordsForRKIV.Add(coordWithStepRK);

                steps.Add(step);
            }
        }

        public MainWindow()
        {
            InitializeComponent();

            SetInitialParams();
            SetParams();

            createEquiationString();
        }

        private void createEquiationString()
        {
            if (this.LabelEquations != null)
            {
                this.LabelEquations.Content = "Уравнение Дуффинга: m·ẍ + h·ẋ + k·x + k'·x³ = f₀·cos(ω·t)\n";
                this.LabelEquations.Content += $"{m}*ẍ + {h}*ẋ' + {k}*x + {k_}*x^3 = {f0}*cos({omega}*t)";
                this.LabelEquations.FontSize = 20;
            }
        }

        private void ClearPrevSolution()
        {
            coordsForRKIV = new List<Coord>();
            coordsForVanDerPol = new List<Coord>();
            steps = new List<double>();
        }

        private void StartCalcultaing_Click(object sender, RoutedEventArgs e)
        {
            ClearPrevSolution();
            SetInitialConditions();
            SetInitialParams();
            SetParams();

            RK_IV();
            UpdateGraph();
            UpdateDataGrid();
        }

        private void UpdateDataGrid()
        {
            List<GridRowModel> gridRowModels = new List<GridRowModel>();

            for (int i = 0; i < coordsForRKIV.Count && i < steps.Count; ++i)
            {
                GridRowModel gridRowModel = new GridRowModel();
                gridRowModel.m_step = steps[i];
                gridRowModel.m_coord_for_RK = coordsForRKIV[i];
                //gridRowModel.m_coord_for_VDP = coordsForVanDerPol[i];
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

                List<double> tCoordsRK = new List<double>();
                List<double> u1CoordsRK = new List<double>();
                List<double> u2CoordsRK = new List<double>();

                List<double> tCoordsVDP = new List<double>();
                List<double> u1CoordsVDP = new List<double>();
                List<double> u2CoordsVDP = new List<double>();

                for (int i = 0; i < coordsForRKIV.Count; ++i)
                {
                    tCoordsRK.Add(coordsForRKIV[i].m_t);
                    u1CoordsRK.Add(coordsForRKIV[i].m_x);
                    u2CoordsRK.Add(coordsForRKIV[i].m_y);
                }

                for (int i = 0; i < coordsForVanDerPol.Count; ++i)
                {
                    tCoordsVDP.Add(coordsForVanDerPol[i].m_t);
                    u1CoordsVDP.Add(coordsForVanDerPol[i].m_x);
                    u2CoordsVDP.Add(coordsForVanDerPol[i].m_y);
                }

                WpfPlotMain.Plot.Clear();
                if (header == "Graph (t, x)")
                {
                    //WpfPlotMain.Plot.Add.Scatter(tCoordsVDP.ToArray(), u1CoordsVDP.ToArray(), ScottPlot.Colors.Blue);
                    WpfPlotMain.Plot.Add.Scatter(tCoordsRK.ToArray(), u1CoordsRK.ToArray(), ScottPlot.Colors.Green);
                }
                else if (header == "Graph (t, y)")
                {
                    //WpfPlotMain.Plot.Add.Scatter(tCoordsVDP.ToArray(), u2CoordsVDP.ToArray(), ScottPlot.Colors.Blue);
                    WpfPlotMain.Plot.Add.Scatter(tCoordsRK.ToArray(), u2CoordsRK.ToArray(), ScottPlot.Colors.Green);
                }
                else if (header == "Graph (x, y)")
                {
                    //WpfPlotMain.Plot.Add.Scatter(u1CoordsVDP.ToArray(), u2CoordsVDP.ToArray(), ScottPlot.Colors.Blue);
                    WpfPlotMain.Plot.Add.Scatter(u1CoordsRK.ToArray(), u2CoordsRK.ToArray(), ScottPlot.Colors.Green);
                }
                WpfPlotMain.Plot.Axes.AutoScale();
                WpfPlotMain.Refresh();
            }
        }

        private void TabControlGraphs_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            UpdateGraph();
        }

        private void mParam_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (this.mParam != null)
            {
                m = System.Convert.ToDouble(mParam.Text);
                createEquiationString();
            }
        }

        private void hParam_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (this.hParam != null)
            {
                h = System.Convert.ToDouble(hParam.Text);
                createEquiationString();
            }
        }

        private void kParam_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (this.kParam != null)
            {
                k = System.Convert.ToDouble(kParam.Text);
                createEquiationString();
            }
        }

        private void k_Param_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (this.k_Param != null)
            {
                k_ = System.Convert.ToDouble(k_Param.Text);
                createEquiationString();
            }
        }

        private void f0Param_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (this.f0Param != null)
            {
                f0 = System.Convert.ToDouble(f0Param.Text);
                createEquiationString();
            }
        }

        private void omegaParam_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (this.omegaParam != null)
            {
                omega = System.Convert.ToDouble(omegaParam.Text);
                createEquiationString();
            }
        }
    }
}