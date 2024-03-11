using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace GMCG_LAB2
{
    public partial class Form1 : Form
    {
        private class Coords2D
        {
            public double x { get; set; }
            public double y { get; set; }

            public Coords2D(double x, double y)
            {
                this.x = x;
                this.y = y;
            }
        }

        List<Coords2D> coords2Ds = new List<Coords2D>();

        double x0, y0;

        Font drawFont = new Font("Arial", 10);
        Font axFont = new Font("Tahoma", 10);
        Color user_color;
        SolidBrush drawBrush = new SolidBrush(Color.Black);
        SolidBrush axBrush = new SolidBrush(Color.Gray);

        Pen penForFirstTab;
        Pen penForSecondTab;

        Bitmap canvasForFirstTab;
        Bitmap canvasForSecondTab;

        Graphics user_GraphicsForFirstTab;
        Graphics user_GraphicsForSecondTab;

        StringFormat drawFormat = new StringFormat();

        public Form1()
        {
            InitializeComponent();

            user_color = Color.Blue;

            penForFirstTab = new Pen(user_color, 2);

            canvasForFirstTab = new Bitmap(pictureBox_2d.Width, pictureBox_2d.Height);

            user_GraphicsForFirstTab = Graphics.FromImage(canvasForFirstTab);

            x0 = (int)(pictureBox_2d.Width / 2);
            y0 = (int)(pictureBox_2d.Height / 2);

            user_GraphicsForFirstTab.FillRectangle(Brushes.White, 0, 0, pictureBox_2d.Width, pictureBox_2d.Height);
            user_GraphicsForFirstTab.DrawLine(Pens.Gray, (float)x0, 2, (float)x0, pictureBox_2d.Height - 2);
            user_GraphicsForFirstTab.DrawLine(Pens.Gray, 2, (float)y0, pictureBox_2d.Width - 2, (float)y0); 
            user_GraphicsForFirstTab.DrawString("x", axFont, axBrush, (float)(pictureBox_2d.Width - 10), (float)(y0 + 3), drawFormat);
            user_GraphicsForFirstTab.DrawString("y", axFont, axBrush, (float)(x0 - 10), (float)(3.0), drawFormat);
            
            pictureBox_2d.Image = canvasForFirstTab;

            ///////////

            penForSecondTab = new Pen(user_color, 2);

            canvasForSecondTab = new Bitmap(pictureBox_3d.Width, pictureBox_3d.Height);

            user_GraphicsForSecondTab = Graphics.FromImage(canvasForSecondTab);

            x0_for_second = (int)(pictureBox_3d.Width / 2);
            y0_for_second = (int)(pictureBox_3d.Height / 2);

            user_GraphicsForSecondTab.FillRectangle(Brushes.White, 0, 0, pictureBox_3d.Width, pictureBox_3d.Height);
            user_GraphicsForSecondTab.DrawLine(Pens.Gray, (float)x0_for_second, 2, (float)x0_for_second, pictureBox_3d.Height - 2);
            user_GraphicsForSecondTab.DrawLine(Pens.Gray, 2, (float)y0_for_second, pictureBox_3d.Width - 2, (float)y0_for_second);
            user_GraphicsForSecondTab.DrawString("x", axFont, axBrush, (float)(pictureBox_3d.Width - 10), (float)(y0_for_second + 3), drawFormat);
            user_GraphicsForSecondTab.DrawString("y", axFont, axBrush, (float)(x0_for_second - 10), (float)(3.0), drawFormat);

            pictureBox_3d.Image = canvasForSecondTab;

            ///////////

            penFor3DTab = new Pen(user_color, 2);

            canvasFor3DTabOXY = new Bitmap(pictureBoxOXY.Width, pictureBoxOXY.Height);
            canvasFor3DTabOXZ = new Bitmap(pictureBoxOXZ.Width, pictureBoxOXZ.Height);
            canvasFor3DTabOYZ = new Bitmap(pictureBoxOYZ.Width, pictureBoxOYZ.Height);

            user_GraphicsForOXYTab = Graphics.FromImage(canvasFor3DTabOXY);
            user_GraphicsForOXZTab = Graphics.FromImage(canvasFor3DTabOXZ);
            user_GraphicsForOYZTab = Graphics.FromImage(canvasFor3DTabOYZ);

            x0_for_3D_OXY = (int)(pictureBoxOXY.Width / 2);
            y0_for_3D_OXY = (int)(pictureBoxOXY.Height / 2);

            x0_for_3D_OXZ = (int)(pictureBoxOXZ.Width / 2);
            z0_for_3D_OXZ = (int)(pictureBoxOXZ.Height / 2);

            y0_for_3D_OYZ = (int)(pictureBoxOYZ.Width / 2);
            z0_for_3D_OYZ = (int)(pictureBoxOYZ.Height / 2);

            user_GraphicsForOXYTab.FillRectangle(Brushes.White, 0, 0, pictureBoxOXY.Width, pictureBoxOXY.Height);
            user_GraphicsForOXZTab.FillRectangle(Brushes.White, 0, 0, pictureBoxOXZ.Width, pictureBoxOXZ.Height);
            user_GraphicsForOYZTab.FillRectangle(Brushes.White, 0, 0, pictureBoxOYZ.Width, pictureBoxOYZ.Height);

            user_GraphicsForOXYTab.DrawLine(Pens.Gray, (float)x0_for_3D_OXY, 2, (float)x0_for_3D_OXY, pictureBoxOXY.Height - 2);
            user_GraphicsForOXYTab.DrawLine(Pens.Gray, 2, (float)y0_for_3D_OXY, pictureBoxOXY.Width - 2, (float)y0_for_3D_OXY);
            user_GraphicsForOXYTab.DrawString("x", axFont, axBrush, (float)(pictureBoxOXY.Width - 10), (float)(y0_for_3D_OXY + 3), drawFormat);
            user_GraphicsForOXYTab.DrawString("y", axFont, axBrush, (float)(x0_for_3D_OXY - 10), (float)(3.0), drawFormat);

            user_GraphicsForOXZTab.DrawLine(Pens.Gray, (float)x0_for_3D_OXZ, 2, (float)x0_for_3D_OXZ, pictureBoxOXZ.Height - 2);
            user_GraphicsForOXZTab.DrawLine(Pens.Gray, 2, (float)z0_for_3D_OXZ, pictureBoxOXZ.Width - 2, (float)z0_for_3D_OXZ);
            user_GraphicsForOXZTab.DrawString("x", axFont, axBrush, (float)(pictureBoxOXZ.Width - 10), (float)(z0_for_3D_OXZ + 3), drawFormat);
            user_GraphicsForOXZTab.DrawString("z", axFont, axBrush, (float)(x0_for_3D_OXZ - 10), (float)(3.0), drawFormat);

            user_GraphicsForOYZTab.DrawLine(Pens.Gray, (float)y0_for_3D_OYZ, 2, (float)y0_for_3D_OYZ, pictureBoxOYZ.Height - 2);
            user_GraphicsForOYZTab.DrawLine(Pens.Gray, 2, (float)z0_for_3D_OYZ, pictureBoxOYZ.Width - 2, (float)z0_for_3D_OYZ);
            user_GraphicsForOYZTab.DrawString("y", axFont, axBrush, (float)(pictureBoxOYZ.Width - 10), (float)(z0_for_3D_OYZ + 3), drawFormat);
            user_GraphicsForOYZTab.DrawString("z", axFont, axBrush, (float)(y0_for_3D_OYZ - 10), (float)(3.0), drawFormat);

            pictureBoxOXY.Image = canvasFor3DTabOXY;
            pictureBoxOXZ.Image = canvasFor3DTabOXZ;
            pictureBoxOYZ.Image = canvasFor3DTabOYZ;

            ///////////

            penFor3DTab = new Pen(user_color, 2);

            canvasfor_Trimetric_Projection = new Bitmap(pictureBoxTrimetricProjection.Width, pictureBoxTrimetricProjection.Height);

            user_Graphicsfor_Trimetric_Projection = Graphics.FromImage(canvasfor_Trimetric_Projection);

            x0_for_Trimetric_Projection = (int)(pictureBoxTrimetricProjection.Width / 2);
            y0_for_Trimetric_Projection = (int)(pictureBoxTrimetricProjection.Height / 2);

            user_GraphicsForSecondTab.FillRectangle(Brushes.White, 0, 0, pictureBoxTrimetricProjection.Width, pictureBoxTrimetricProjection.Height);
            user_GraphicsForSecondTab.DrawLine(Pens.Gray, (float)x0_for_Trimetric_Projection, 2, (float)x0_for_Trimetric_Projection, pictureBoxTrimetricProjection.Height - 2);
            user_GraphicsForSecondTab.DrawLine(Pens.Gray, 2, (float)y0_for_Trimetric_Projection, pictureBoxTrimetricProjection.Width - 2, (float)y0_for_Trimetric_Projection);
            user_GraphicsForSecondTab.DrawString("x", axFont, axBrush, (float)(pictureBoxTrimetricProjection.Width - 10), (float)(y0_for_Trimetric_Projection + 3), drawFormat);
            user_GraphicsForSecondTab.DrawString("y", axFont, axBrush, (float)(x0_for_Trimetric_Projection - 10), (float)(3.0), drawFormat);

            pictureBoxTrimetricProjection.Image = canvasfor_Trimetric_Projection;

            basis.Add(new Coord3D(0, 0, 0, 0));
            basis.Add(new Coord3D(100, 0, 0, 1));
            basis.Add(new Coord3D(0, 100, 0, 2));
            basis.Add(new Coord3D(0, 0, 100, 3));

            basis_line.Add(Tuple.Create(basis[0], basis[1]));
            basis_line.Add(Tuple.Create(basis[0], basis[2]));
            basis_line.Add(Tuple.Create(basis[0], basis[3]));
        }

        private void button1_Click(object sender, EventArgs e)
        {
            double temp_x = 0;
            double temp_y = 0;

            if (coords2Ds == null)
                return;

            if (Double.TryParse(this.textBox1.Text, out temp_x) &&
                Double.TryParse(this.textBox2.Text, out temp_y))
            {
                coords2Ds.Add(new Coords2D(x0 + temp_x, y0 - temp_y));
            }
            else
            {
                MessageBox.Show("Ввод данных произведен неверно!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            printPoints();     
        }

        private void button2_Click(object sender, EventArgs e)
        {
            double[][] resize2D = new double[2][];

            for (int count = 0; count < 2; ++count)
                resize2D[count] = new double[2];

            double alpha = System.Convert.ToDouble(this.textBox3.Text);
            double betta = System.Convert.ToDouble(this.textBox5.Text);

            resize2D[0][0] = alpha;
            resize2D[0][1] = 0;
            resize2D[1][0] = 0;
            resize2D[1][1] = betta;

            for (int count = 0; count < coords2Ds.Count; ++count)
            {
                Coords2D temp = coords2Ds[count];
                temp.x = ((((double)temp.x - x0) * resize2D[0][0] + ((double)temp.y - y0) * resize2D[1][0]) + x0);
                temp.y = ((((double)temp.x - x0) * resize2D[0][1] + ((double)temp.y - y0) * resize2D[1][1]) + y0);
                coords2Ds[count] = temp;
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            double[][] rotation2D = new double[2][];

            for (int count = 0; count < 2; ++count)
                rotation2D[count] = new double[2];

            double phi = System.Convert.ToDouble(this.textBox4.Text) * Math.PI / 180.0;

            rotation2D[0][0] = Math.Cos(phi);
            rotation2D[0][1] = Math.Sin(phi);
            rotation2D[1][0] = -Math.Sin(phi);
            rotation2D[1][1] = Math.Cos(phi);

            for (int count = 0; count < coords2Ds.Count; ++count)
            {
                Coords2D temp = coords2Ds   [count];

                double deltaX = temp.x - x0;
                double deltaY = temp.y - y0;

                temp.x = ((deltaX * rotation2D[0][0] + deltaY * rotation2D[1][0]) + x0);
                temp.y = ((deltaX * rotation2D[0][1] + deltaY * rotation2D[1][1]) + y0);

                coords2Ds[count] = temp;
            }
        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            user_GraphicsForFirstTab.FillRectangle(Brushes.White, 0, 0, pictureBox_2d.Width, pictureBox_2d.Height);

            if (coords2Ds.Count < 3)
                foreach (var point in coords2Ds)
                    user_GraphicsForFirstTab.FillEllipse(Brushes.Blue, (float)point.x - 2, (float)point.y - 2, 4, 4);
            else
                user_GraphicsForFirstTab.DrawPolygon(penForFirstTab, coords2Ds.Select(p => new PointF((float)p.x, (float)p.y)).ToArray());

            pictureBox_2d.Image = canvasForFirstTab;

            foreach (var point in coords2Ds)
                user_GraphicsForFirstTab.DrawString(string.Format("p{0:d}", coords2Ds.IndexOf(point)), drawFont, drawBrush, (float)(point.x + 3), (float)(point.y + 3), drawFormat);

            printPoints();

        }

        private void button4_Click(object sender, EventArgs e)
        {
            double[][] mirrorOX = new double[2][];

            for (int count = 0; count < 2; ++count)
                mirrorOX[count] = new double[2];

            mirrorOX[0][0] = 1;
            mirrorOX[0][1] = 0;
            mirrorOX[1][0] = 0;
            mirrorOX[1][1] = -1;

            for (int count = 0; count < coords2Ds.Count; ++count)
            {
                Coords2D temp = coords2Ds[count];

                double deltaX = temp.x - x0;
                double deltaY = temp.y - y0;

                temp.x = (int)((deltaX * mirrorOX[0][0] + deltaY * mirrorOX[1][0]) + x0);
                temp.y = (int)((deltaX * mirrorOX[0][1] + deltaY * mirrorOX[1][1]) + y0);

                coords2Ds[count] = temp;
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            double[][] mirrorOY = new double[2][];

            for (int count = 0; count < 2; ++count)
                mirrorOY[count] = new double[2];

            mirrorOY[0][0] = -1;
            mirrorOY[0][1] = 0;
            mirrorOY[1][0] = 0;
            mirrorOY[1][1] = 1;

            for (int count = 0; count < coords2Ds.Count; ++count)
            {
                Coords2D temp = coords2Ds[count];

                double deltaX = temp.x - x0;
                double deltaY = temp.y - y0;

                temp.x = (int)((deltaX * mirrorOY[0][0] + deltaY * mirrorOY[1][0]) + x0);
                temp.y = (int)((deltaX * mirrorOY[0][1] + deltaY * mirrorOY[1][1]) + y0);

                coords2Ds[count] = temp;
            }
        }

        private void button6_Click(object sender, EventArgs e)
        {
            double[][] mirrorOXY = new double[2][];

            for (int count = 0; count < 2; ++count)
                mirrorOXY[count] = new double[2];

            mirrorOXY[0][0] = -1;
            mirrorOXY[0][1] = 0;
            mirrorOXY[1][0] = 0;
            mirrorOXY[1][1] = -1;

            for (int count = 0; count < coords2Ds.Count; ++count)
            {
                Coords2D temp = coords2Ds[count];

                double deltaX = temp.x - x0;
                double deltaY = temp.y - y0;

                temp.x = (int)((deltaX * mirrorOXY[0][0] + deltaY * mirrorOXY[1][0]) + x0);
                temp.y = (int)((deltaX * mirrorOXY[0][1] + deltaY * mirrorOXY[1][1]) + y0);

                coords2Ds[count] = temp;
            }
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void button7_Click(object sender, EventArgs e)
        {
            double[] move = new double[2];

            move[0] = System.Convert.ToDouble(this.textBox6.Text);
            move[1] = System.Convert.ToDouble(this.textBox7.Text);

            for (int count = 0; count < coords2Ds.Count; ++count)
            {
                Coords2D temp = coords2Ds[count];

                temp.x += (int)move[0];
                temp.y -= (int)move[1];

                coords2Ds[count] = temp;
            }
        }

        private void printPoints()
        {
            user_GraphicsForFirstTab.DrawLine(Pens.Gray, (float)x0, 2, (float)x0, pictureBox_2d.Height - 2);
            user_GraphicsForFirstTab.DrawLine(Pens.Gray, 2, (float)y0, pictureBox_2d.Width - 2, (float)y0);
            user_GraphicsForFirstTab.DrawString("x", axFont, axBrush, (float)(pictureBox_2d.Width - 10), (float)(y0 + 3), drawFormat);
            user_GraphicsForFirstTab.DrawString("y", axFont, axBrush, (float)(x0 - 10), (float)(3.0), drawFormat);

            this.label1.Text = "Координаты точек:\n";

            for (int count = 0; count < coords2Ds.Count; count++)
            {
                this.label1.Text += "(" + System.Convert.ToString(coords2Ds[count].x - x0) + "; " + System.Convert.ToString(-coords2Ds[count].y + y0) + ")\n";
            }
        }
    }
}
