using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GMCG_LAB2
{
    public partial class Form1 : Form
    {
        private class Coords3D_2D
        {
            public double x { get; set; }
            public double y { get; set; }
            public double z { get; set; }

            public Coords3D_2D(double x, double y)
            {
                this.x = x;
                this.y = y;
                this.z = 1;
            }
        }

        List<Coords3D_2D> coords3D_2Ds = new List<Coords3D_2D>();

        double x0_for_second, y0_for_second;

        private void AddPoint3D_Click(object sender, EventArgs e)
        {
            double temp_x = 0;
            double temp_y = 0;

            if (coords3D_2Ds == null)
                return;

            if (Double.TryParse(this.textBox_Coords_X_3D.Text, out temp_x) &&
                Double.TryParse(this.textBox_Coords_Y_3D.Text, out temp_y))
            {
                coords3D_2Ds.Add(new Coords3D_2D(x0_for_second + temp_x, y0_for_second - temp_y));
            }
            else
            {
                MessageBox.Show("Ввод данных произведен неверно!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            this.label19.Text = "Координаты точек:\n";

            for (int count = 0; count < coords3D_2Ds.Count; count++)
            {
                this.label19.Text += "(" + System.Convert.ToString(coords3D_2Ds[count].x - x0_for_second) + "; " + System.Convert.ToString(-coords3D_2Ds[count].y + y0_for_second) + "; " + System.Convert.ToString(coords3D_2Ds[count].z) + ")\n";
            }
        }

        private void button_Draw_Points_3D_Click(object sender, EventArgs e)
        {
            user_GraphicsForSecondTab.FillRectangle(Brushes.White, 0, 0, pictureBox_2d.Width, pictureBox_2d.Height);

            if (coords3D_2Ds.Count < 3)
                foreach (var point in coords3D_2Ds)
                    user_GraphicsForSecondTab.FillEllipse(Brushes.Blue, (float)point.x - 2, (float)point.y - 2, 4, 4);
            else
                user_GraphicsForSecondTab.DrawPolygon(penForSecondTab, coords3D_2Ds.Select(p => new PointF((float)p.x, (float)p.y)).ToArray());

            pictureBox_3d.Image = canvasForSecondTab;

            foreach (var point in coords3D_2Ds)
                user_GraphicsForSecondTab.DrawString(string.Format("p{0:d}", coords3D_2Ds.IndexOf(point)), drawFont, drawBrush, (float)(point.x + 3), (float)(point.y + 3), drawFormat);


            user_GraphicsForSecondTab.DrawLine(Pens.Gray, (float)x0_for_second, 2, (float)x0_for_second, pictureBox_3d.Height - 2);
            user_GraphicsForSecondTab.DrawLine(Pens.Gray, 2, (float)y0_for_second, pictureBox_3d.Width - 2, (float)y0_for_second);
            user_GraphicsForSecondTab.DrawString("x", axFont, axBrush, (float)(pictureBox_3d.Width - 10), (float)(y0_for_second + 3), drawFormat);
            user_GraphicsForSecondTab.DrawString("y", axFont, axBrush, (float)(x0_for_second - 10), (float)(3.0), drawFormat);

            this.label19.Text = "Координаты точек:\n";

            for (int count = 0; count < coords3D_2Ds.Count; count++)
            {
                this.label19.Text += "(" + System.Convert.ToString(coords3D_2Ds[count].x - x0_for_second) + "; " + System.Convert.ToString(-coords3D_2Ds[count].y + y0_for_second) + "; " + System.Convert.ToString(coords3D_2Ds[count].z) + ")\n";
            }
        }

        private void button_Rotation3D_Click(object sender, EventArgs e)
        {
            double[][] rotation3D = new double[3][];

            for (int count = 0; count < 3; ++count)
                rotation3D[count] = new double[3];

            double phi = System.Convert.ToDouble(this.textBox_for_Rotation3D.Text) * Math.PI / 180.0;

            rotation3D[0][0] = Math.Cos(phi);
            rotation3D[0][1] = Math.Sin(phi);
            rotation3D[0][2] = 0;

            rotation3D[1][0] = -Math.Sin(phi);
            rotation3D[1][1] = Math.Cos(phi);
            rotation3D[1][2] = 0;

            rotation3D[2][0] = 0;
            rotation3D[2][1] = 0;
            rotation3D[2][2] = 1;

            for (int count = 0; count < coords3D_2Ds.Count; ++count)
            {
                Coords3D_2D temp = coords3D_2Ds[count];

                double deltaX = temp.x - x0_for_second;
                double deltaY = temp.y - y0_for_second;
                double deltaZ = 1;

                temp.x = ((deltaX * rotation3D[0][0] + deltaY * rotation3D[1][0] + deltaZ * rotation3D[2][0]) + x0_for_second);
                temp.y = ((deltaX * rotation3D[0][1] + deltaY * rotation3D[1][1] + deltaZ * rotation3D[2][1]) + y0_for_second);
                temp.z = ((deltaX * rotation3D[0][2] + deltaY * rotation3D[1][2] + deltaZ * rotation3D[2][2]));

                coords3D_2Ds[count] = temp;
            }
        }

        private void button_Tension_Compression_3D_Click(object sender, EventArgs e)
        {
            double[][] resize3D = new double[3][];

            for (int count = 0; count < 3; ++count)
                resize3D[count] = new double[3];

            double alpha = System.Convert.ToDouble(this.textBox_alpha_for_3d.Text);
            double betta = System.Convert.ToDouble(this.textBox_betta_for_3d.Text);

            resize3D[0][0] = alpha;
            resize3D[0][1] = 0;
            resize3D[0][2] = 0;

            resize3D[1][0] = 0;
            resize3D[1][1] = betta;
            resize3D[1][2] = 0;

            resize3D[2][0] = 0;
            resize3D[2][1] = 0;
            resize3D[2][2] = 1;

            for (int count = 0; count < coords3D_2Ds.Count; ++count)
            {
                Coords3D_2D temp = coords3D_2Ds[count];

                double deltaX = temp.x - x0_for_second;
                double deltaY = temp.y - y0_for_second;
                double deltaZ = 1;

                temp.x = ((deltaX * resize3D[0][0] + deltaY * resize3D[1][0] + deltaZ * resize3D[2][0]) + x0_for_second);
                temp.y = ((deltaX * resize3D[0][1] + deltaY * resize3D[1][1] + deltaZ * resize3D[2][1]) + y0_for_second);
                temp.z = ((deltaX * resize3D[0][2] + deltaY * resize3D[1][2] + deltaZ * resize3D[2][2]));

                coords3D_2Ds[count] = temp;
            }
        }

        private void button_Mirror_OX_3D_Click(object sender, EventArgs e)
        {
            double[][] mirror3DOX = new double[3][];

            for (int count = 0; count < 3; ++count)
                mirror3DOX[count] = new double[3];

            mirror3DOX[0][0] = 1;
            mirror3DOX[0][1] = 0;
            mirror3DOX[0][2] = 0;

            mirror3DOX[1][0] = 0;
            mirror3DOX[1][1] = -1;
            mirror3DOX[1][2] = 0;

            mirror3DOX[2][0] = 0;
            mirror3DOX[2][1] = 0;
            mirror3DOX[2][2] = 1;

            for (int count = 0; count < coords3D_2Ds.Count; ++count)
            {
                Coords3D_2D temp = coords3D_2Ds[count];

                double deltaX = temp.x - x0_for_second;
                double deltaY = temp.y - y0_for_second;
                double deltaZ = 1;

                temp.x = ((deltaX * mirror3DOX[0][0] + deltaY * mirror3DOX[1][0] + deltaZ * mirror3DOX[2][0]) + x0_for_second);
                temp.y = ((deltaX * mirror3DOX[0][1] + deltaY * mirror3DOX[1][1] + deltaZ * mirror3DOX[2][1]) + y0_for_second);
                temp.z = ((deltaX * mirror3DOX[0][2] + deltaY * mirror3DOX[1][2] + deltaZ * mirror3DOX[2][2]));

                coords3D_2Ds[count] = temp;
            }
        }

        private void button_Mirror_OY_3D_Click(object sender, EventArgs e)
        {
            double[][] mirror3DOY = new double[3][];

            for (int count = 0; count < 3; ++count)
                mirror3DOY[count] = new double[3];

            mirror3DOY[0][0] = -1;
            mirror3DOY[0][1] = 0;
            mirror3DOY[0][2] = 0;

            mirror3DOY[1][0] = 0;
            mirror3DOY[1][1] = 1;
            mirror3DOY[1][2] = 0;

            mirror3DOY[2][0] = 0;
            mirror3DOY[2][1] = 0;
            mirror3DOY[2][2] = 1;

            for (int count = 0; count < coords3D_2Ds.Count; ++count)
            {
                Coords3D_2D temp = coords3D_2Ds[count];

                double deltaX = temp.x - x0_for_second;
                double deltaY = temp.y - y0_for_second;
                double deltaZ = 1;

                temp.x = ((deltaX * mirror3DOY[0][0] + deltaY * mirror3DOY[1][0] + deltaZ * mirror3DOY[2][0]) + x0_for_second);
                temp.y = ((deltaX * mirror3DOY[0][1] + deltaY * mirror3DOY[1][1] + deltaZ * mirror3DOY[2][1]) + y0_for_second);
                temp.z = ((deltaX * mirror3DOY[0][2] + deltaY * mirror3DOY[1][2] + deltaZ * mirror3DOY[2][2]));

                coords3D_2Ds[count] = temp;
            }
        }

        private void button_Mirror_OXY_3D_Click(object sender, EventArgs e)
        {
            double[][] mirror3DOXY = new double[3][];

            for (int count = 0; count < 3; ++count)
                mirror3DOXY[count] = new double[3];

            mirror3DOXY[0][0] = -1;
            mirror3DOXY[0][1] = 0;
            mirror3DOXY[0][2] = 0;

            mirror3DOXY[1][0] = 0;
            mirror3DOXY[1][1] = -1;
            mirror3DOXY[1][2] = 0;

            mirror3DOXY[2][0] = 0;
            mirror3DOXY[2][1] = 0;
            mirror3DOXY[2][2] = 1;

            for (int count = 0; count < coords3D_2Ds.Count; ++count)
            {
                Coords3D_2D temp = coords3D_2Ds[count];

                double deltaX = temp.x - x0_for_second;
                double deltaY = temp.y - y0_for_second;
                double deltaZ = 1;

                temp.x = ((deltaX * mirror3DOXY[0][0] + deltaY * mirror3DOXY[1][0] + deltaZ * mirror3DOXY[2][0]) + x0_for_second);
                temp.y = ((deltaX * mirror3DOXY[0][1] + deltaY * mirror3DOXY[1][1] + deltaZ * mirror3DOXY[2][1]) + y0_for_second);
                temp.z = ((deltaX * mirror3DOXY[0][2] + deltaY * mirror3DOXY[1][2] + deltaZ * mirror3DOXY[2][2]));

                coords3D_2Ds[count] = temp;
            }
        }

        private void button_Move_3D_Click(object sender, EventArgs e)
        {
            double[][] move3D = new double[3][];

            for (int count = 0; count < 3; ++count)
                move3D[count] = new double[3];

            move3D[0][0] = 1;
            move3D[0][1] = 0;
            move3D[0][2] = 0;

            move3D[1][0] = 0;
            move3D[1][1] = 1;
            move3D[1][2] = 0;

            move3D[2][0] = System.Convert.ToDouble(this.textBox_lambda_for_3d.Text);
            move3D[2][1] = -System.Convert.ToDouble(this.textBox_mu_for_3d.Text);
            move3D[2][2] = 1;

            for (int count = 0; count < coords3D_2Ds.Count; ++count)
            {
                Coords3D_2D temp = coords3D_2Ds[count];

                double deltaX = temp.x - x0_for_second;
                double deltaY = temp.y - y0_for_second;
                double deltaZ = 1;

                temp.x = ((deltaX * move3D[0][0] + deltaY * move3D[1][0] + deltaZ * move3D[2][0]) + x0_for_second);
                temp.y = ((deltaX * move3D[0][1] + deltaY * move3D[1][1] + deltaZ * move3D[2][1]) + y0_for_second);
                temp.z = ((deltaX * move3D[0][2] + deltaY * move3D[1][2] + deltaZ * move3D[2][2]));

                coords3D_2Ds[count] = temp;
            }
        }

        private void button_rotate_of_point_Click(object sender, EventArgs e)
        {
            double[][] rotate_of_point_3D = new double[3][];

            for (int count = 0; count < 3; ++count)
                rotate_of_point_3D[count] = new double[3];

            double phi = System.Convert.ToDouble(this.textBox_angle_of_rotating_of_point_3d.Text) * Math.PI / 180.0;

            double a = System.Convert.ToDouble(this.textBox_coordX_for_rotating_of_point_3d.Text);
            double b = System.Convert.ToDouble(this.textBox_coordY_for_rotating_of_point_3d.Text);

            rotate_of_point_3D[0][0] = Math.Cos(phi);
            rotate_of_point_3D[0][1] = Math.Sin(phi);
            rotate_of_point_3D[0][2] = 0;

            rotate_of_point_3D[1][0] = -Math.Sin(phi);
            rotate_of_point_3D[1][1] = Math.Cos(phi);
            rotate_of_point_3D[1][2] = 0;

            rotate_of_point_3D[2][0] = -a * Math.Cos(phi) + b * Math.Sin(phi) + a;
            rotate_of_point_3D[2][1] = -a * Math.Sin(phi) + b * Math.Cos(phi) + b;
            rotate_of_point_3D[2][2] = 1;

            for (int count = 0; count < coords3D_2Ds.Count; ++count)
            {
                Coords3D_2D temp = coords3D_2Ds[count];

                double deltaX = temp.x - x0_for_second;
                double deltaY = temp.y - y0_for_second;
                double deltaZ = 1;

                temp.x = ((deltaX * rotate_of_point_3D[0][0] + deltaY * rotate_of_point_3D[1][0] + deltaZ * rotate_of_point_3D[2][0]) + x0_for_second);
                temp.y = ((deltaX * rotate_of_point_3D[0][1] + deltaY * rotate_of_point_3D[1][1] + deltaZ * rotate_of_point_3D[2][1]) + y0_for_second);
                temp.z = ((deltaX * rotate_of_point_3D[0][2] + deltaY * rotate_of_point_3D[1][2] + deltaZ * rotate_of_point_3D[2][2]));

                coords3D_2Ds[count] = temp;
            }
        }
    }
}
