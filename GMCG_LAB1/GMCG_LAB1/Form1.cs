using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GMCG_LAB1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private int GCD(int a, int b)
        {
            while (b != 0)
            {
                int t = b;
                b = a % b;
                a = t;
            }
            return a;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.chart1.Series[0].Points.Clear();

            double A = System.Convert.ToDouble(this.textBox1.Text);
            double B = System.Convert.ToDouble(this.textBox2.Text);
            double D = System.Convert.ToDouble(this.textBox3.Text);

            double x0 = 0;
            double y0 = 0;

            double t_start = 0;

            double n = B / GCD((int)B, (int)A);

            double max_t = n * 2 * Math.PI;

            double step = (max_t - t_start) / 100;

            double phi_start = ((double)A / (double)B) * t_start;

            double x_start = x0 - ((A - B) * Math.Cos(t_start) + D * Math.Cos(phi_start));
            double y_start = y0 - ((A - B) * Math.Sin(t_start) - D * Math.Sin(phi_start));

            this.chart1.Series[0].Points.AddXY(x_start, y_start);

            double t = t_start;
            double phi = phi_start;

            double x = x0 - ((A - B) * Math.Cos(t_start+step) + D * Math.Cos(((double)A / (double)B) * (t_start+step)));
            double y = y0 - ((A - B) * Math.Sin(t_start+step) - D * Math.Sin(((double)A / (double)B) * (t_start+step)));

            this.chart1.Series[0].Points.AddXY(x, y);

            while (x != x_start && y != y_start)
            {
                t = t + step;

                phi = (A / B) * t;

                x = x0 - ((A - B) * Math.Cos(t) + D * Math.Cos(phi));
                y = y0 - ((A - B) * Math.Sin(t) - D * Math.Sin(phi));

                this.chart1.Series[0].Points.AddXY(x, y);

            }
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void chart1_Click(object sender, EventArgs e)
        {

        }
    }
}
