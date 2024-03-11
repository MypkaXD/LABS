using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GMCG_LAB2
{
    public partial class Form1 : Form
    {
        public int counter = 0;

        private class Coord3D
        {
            public double x { get; set; }
            public double y { get; set; }
            public double z { get; set; }
            public double s { get; set; }

            public int index;

            public Coord3D(double x, double y, double z, int index)
            {
                this.x = x;
                this.y = y;
                this.z = z;
                this.s = 1;

                this.index = index;
            }
        }

        List<Coord3D> coords3D = new List<Coord3D>();
        List<Tuple<Coord3D, Coord3D>> pairs = new List<Tuple<Coord3D, Coord3D>>();

        double x0_for_3D_OXY, y0_for_3D_OXY, z0_for_3D_OXY;
        double x0_for_3D_OXZ, y0_for_3D_OXZ, z0_for_3D_OXZ;
        double x0_for_3D_OYZ, y0_for_3D_OYZ, z0_for_3D_OYZ;

        double x0_for_Trimetric_Projection, y0_for_Trimetric_Projection, z0_for_Trimetric_Projection;

        Pen penFor3DTab;

        Pen myPen = new Pen(Color.Blue, 2.0f);

        Bitmap canvasFor3DTabOXY;
        Bitmap canvasFor3DTabOXZ;
        Bitmap canvasFor3DTabOYZ;
        Bitmap canvasfor_Trimetric_Projection;

        Graphics user_GraphicsForOXYTab;
        Graphics user_GraphicsForOXZTab;
        Graphics user_GraphicsForOYZTab;
        Graphics user_Graphicsfor_Trimetric_Projection;

        private void button_for_adding_point_in_3d_Click(object sender, EventArgs e)
        {
            double temp_x = 0;
            double temp_y = 0;
            double temp_z = 0;

            if (coords3D == null)
                return;

            if (Double.TryParse(this.textBox_for_add_X_3D.Text, out temp_x) &&
                Double.TryParse(this.textBox_for_add_Y_3D.Text, out temp_y) &&
                Double.TryParse(this.textBox_for_add_Z_3D.Text, out temp_z))
            {
                coords3D.Add(new Coord3D(temp_x, temp_y, temp_z, counter++));
            }
            else
            {
                MessageBox.Show("Ввод данных произведен неверно!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void button_for_draw_3D_points_Click(object sender, EventArgs e)
        {
            user_GraphicsForOXYTab.FillRectangle(Brushes.White, 0, 0, pictureBoxOXY.Width, pictureBoxOXY.Height);

            foreach (var pair in pairs)
            {
                user_GraphicsForOXYTab.DrawLine(myPen, (float)(pair.Item1.x + x0_for_3D_OXY), (float)(-pair.Item1.y + y0_for_3D_OXY), (float)(pair.Item2.x + x0_for_3D_OXY), (float)(-pair.Item2.y + y0_for_3D_OXY));
            }

            pictureBoxOXY.Image = canvasFor3DTabOXY;

            foreach (var point in coords3D)
                user_GraphicsForOXYTab.DrawString(string.Format("p{0:d}", coords3D.IndexOf(point)), drawFont, drawBrush, (float)(point.x + x0_for_3D_OXY + 3), (float)(-point.y + y0_for_3D_OXY + 3), drawFormat);

            /////

            user_GraphicsForOXZTab.FillRectangle(Brushes.White, 0, 0, pictureBoxOXZ.Width, pictureBoxOXZ.Height);

            foreach (var pair in pairs)
            {
                user_GraphicsForOXZTab.DrawLine(myPen, (float)(pair.Item1.x + x0_for_3D_OXZ), (float)(-pair.Item1.z + z0_for_3D_OXZ), (float)(pair.Item2.x + x0_for_3D_OXZ), (float)(-pair.Item2.z + z0_for_3D_OXZ));
            }

            pictureBoxOXZ.Image = canvasFor3DTabOXZ;

            foreach (var point in coords3D)
                user_GraphicsForOXZTab.DrawString(string.Format("p{0:d}", coords3D.IndexOf(point)), drawFont, drawBrush, (float)(point.x + x0_for_3D_OXZ + 3), (float)(-point.z + z0_for_3D_OXZ + 3), drawFormat);

            ////

            user_GraphicsForOYZTab.FillRectangle(Brushes.White, 0, 0, pictureBoxOYZ.Width, pictureBoxOYZ.Height);

            foreach (var pair in pairs)
            {
                user_GraphicsForOYZTab.DrawLine(myPen, (float)(pair.Item1.y + y0_for_3D_OYZ), (float)(-pair.Item1.z + z0_for_3D_OYZ), (float)(pair.Item2.y + y0_for_3D_OYZ), (float)(-pair.Item2.z + z0_for_3D_OYZ));
            }

            pictureBoxOYZ.Image = canvasFor3DTabOYZ;

            foreach (var point in coords3D)
                user_GraphicsForOYZTab.DrawString(string.Format("p{0:d}", coords3D.IndexOf(point)), drawFont, drawBrush, (float)((point.y + y0_for_3D_OYZ) + 3), (float)((-point.z + z0_for_3D_OYZ) + 3), drawFormat);

            /////

            user_Graphicsfor_Trimetric_Projection.FillRectangle(Brushes.White, 0, 0, pictureBoxTrimetricProjection.Width, pictureBoxTrimetricProjection.Height);

            foreach(var pair in pairs)
            {
                user_Graphicsfor_Trimetric_Projection.DrawLine(myPen, (float)(pair.Item1.x + x0_for_Trimetric_Projection), (float)(-pair.Item1.y + y0_for_Trimetric_Projection), (float)(pair.Item2.x + x0_for_Trimetric_Projection), (float)(-pair.Item2.y + y0_for_Trimetric_Projection));
            }

            pictureBoxTrimetricProjection.Image = canvasfor_Trimetric_Projection;

            foreach (var point in coords3D)
                user_Graphicsfor_Trimetric_Projection.DrawString(string.Format("p{0:d}", coords3D.IndexOf(point)), drawFont, drawBrush, (float)(point.x + x0_for_Trimetric_Projection + 3), (float)(-point.y + y0_for_Trimetric_Projection + 3), drawFormat);


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

            user_Graphicsfor_Trimetric_Projection.DrawLine(Pens.Gray, (float)x0_for_Trimetric_Projection, 2, (float)x0_for_Trimetric_Projection, pictureBoxTrimetricProjection.Height - 2);
            user_Graphicsfor_Trimetric_Projection.DrawLine(Pens.Gray, 2, (float)y0_for_Trimetric_Projection, pictureBoxTrimetricProjection.Width - 2, (float)y0_for_Trimetric_Projection);
            user_Graphicsfor_Trimetric_Projection.DrawString("x", axFont, axBrush, (float)(pictureBoxTrimetricProjection.Width - 10), (float)(y0_for_Trimetric_Projection + 3), drawFormat);
            user_Graphicsfor_Trimetric_Projection.DrawString("y", axFont, axBrush, (float)(x0_for_Trimetric_Projection - 10), (float)(3.0), drawFormat);
        }
        private void ButtonResize3D_Click(object sender, EventArgs e)
        {
            double[][] resize3D = new double[4][];

            for (int count = 0; count < 4; ++count)
                resize3D[count] = new double[4];

            double alpha = System.Convert.ToDouble(this.textBoxResizeX3D.Text);
            double betta = System.Convert.ToDouble(this.textBoxResizeY3D.Text);
            double gamma = System.Convert.ToDouble(this.textBoxResizeZ3D.Text);

            resize3D[0][0] = alpha;
            resize3D[0][1] = 0;
            resize3D[0][2] = 0;
            resize3D[0][3] = 0;

            resize3D[1][0] = 0;
            resize3D[1][1] = betta;
            resize3D[1][2] = 0;
            resize3D[1][3] = 0;

            resize3D[2][0] = 0;
            resize3D[2][1] = 0;
            resize3D[2][2] = gamma;
            resize3D[2][3] = 0;

            resize3D[3][0] = 0;
            resize3D[3][1] = 0;
            resize3D[3][2] = 0;
            resize3D[3][3] = 1;

            for (int count = 0; count < coords3D.Count; ++count)
            {
                Coord3D temp = coords3D[count];

                double deltaX = temp.x;
                double deltaY = temp.y;
                double deltaZ = temp.z;
                double deltaS = temp.s;

                temp.x = (deltaX * resize3D[0][0] + deltaY * resize3D[1][0] +
                    deltaZ * resize3D[2][0] + deltaS * resize3D[3][0]);
                temp.y = (deltaX * resize3D[0][1] + deltaY * resize3D[1][1] + 
                    deltaZ * resize3D[2][1] + deltaS * resize3D[3][1]);
                temp.z = (deltaX * resize3D[0][2] + deltaY * resize3D[1][2] + 
                    deltaZ * resize3D[2][2] + deltaS * resize3D[3][2]);
                temp.s = deltaX * resize3D[0][3] + deltaY * resize3D[1][3] +
                    deltaZ * resize3D[2][3] + deltaS * resize3D[3][3];

            coords3D[count] = temp;
            }
        }

        private void buttonMirror3DOXY_Click(object sender, EventArgs e)
        {
            double[][] mirror3DOXY = new double[4][];

            for (int count = 0; count < 4; ++count)
                mirror3DOXY[count] = new double[4];

            mirror3DOXY[0][0] = 1;
            mirror3DOXY[0][1] = 0;
            mirror3DOXY[0][2] = 0;
            mirror3DOXY[0][3] = 0;

            mirror3DOXY[1][0] = 0;
            mirror3DOXY[1][1] = 1;
            mirror3DOXY[1][2] = 0;
            mirror3DOXY[1][3] = 0;

            mirror3DOXY[2][0] = 0;
            mirror3DOXY[2][1] = 0;
            mirror3DOXY[2][2] = -1;
            mirror3DOXY[2][3] = 0;

            mirror3DOXY[3][0] = 0;
            mirror3DOXY[3][1] = 0;
            mirror3DOXY[3][2] = 0;
            mirror3DOXY[3][3] = 1;

            for (int count = 0; count < coords3D.Count; ++count)
            {
                Coord3D temp = coords3D[count];

                double deltaX = temp.x;
                double deltaY = temp.y;
                double deltaZ = temp.z;
                double deltaS = temp.s;

                temp.x = (deltaX * mirror3DOXY[0][0] + deltaY * mirror3DOXY[1][0] +
                    deltaZ * mirror3DOXY[2][0] + deltaS * mirror3DOXY[3][0]);
                temp.y = (deltaX * mirror3DOXY[0][1] + deltaY * mirror3DOXY[1][1] +
                    deltaZ * mirror3DOXY[2][1] + deltaS * mirror3DOXY[3][1]);
                temp.z = (deltaX * mirror3DOXY[0][2] + deltaY * mirror3DOXY[1][2] +
                    deltaZ * mirror3DOXY[2][2] + deltaS * mirror3DOXY[3][2]);
                temp.s = deltaX * mirror3DOXY[0][3] + deltaY * mirror3DOXY[1][3] +
                    deltaZ * mirror3DOXY[2][3] + deltaS * mirror3DOXY[3][3];

                coords3D[count] = temp;
            }
        }

        private void buttonMirror3DOXZ_Click(object sender, EventArgs e)
        {
            double[][] mirror3DOXZ = new double[4][];

            for (int count = 0; count < 4; ++count)
                mirror3DOXZ[count] = new double[4];

            mirror3DOXZ[0][0] = 1;
            mirror3DOXZ[0][1] = 0;
            mirror3DOXZ[0][2] = 0;
            mirror3DOXZ[0][3] = 0;

            mirror3DOXZ[1][0] = 0;
            mirror3DOXZ[1][1] = -1;
            mirror3DOXZ[1][2] = 0;
            mirror3DOXZ[1][3] = 0;

            mirror3DOXZ[2][0] = 0;
            mirror3DOXZ[2][1] = 0;
            mirror3DOXZ[2][2] = 1;
            mirror3DOXZ[2][3] = 0;

            mirror3DOXZ[3][0] = 0;
            mirror3DOXZ[3][1] = 0;
            mirror3DOXZ[3][2] = 0;
            mirror3DOXZ[3][3] = 1;

            for (int count = 0; count < coords3D.Count; ++count)
            {
                Coord3D temp = coords3D[count];

                double deltaX = temp.x;
                double deltaY = temp.y;
                double deltaZ = temp.z;
                double deltaS = temp.s;

                temp.x = (deltaX * mirror3DOXZ[0][0] + deltaY * mirror3DOXZ[1][0] +
                    deltaZ * mirror3DOXZ[2][0] + deltaS * mirror3DOXZ[3][0]);
                temp.y = (deltaX * mirror3DOXZ[0][1] + deltaY * mirror3DOXZ[1][1] +
                    deltaZ * mirror3DOXZ[2][1] + deltaS * mirror3DOXZ[3][1]);
                temp.z = (deltaX * mirror3DOXZ[0][2] + deltaY * mirror3DOXZ[1][2] +
                    deltaZ * mirror3DOXZ[2][2] + deltaS * mirror3DOXZ[3][2]);
                temp.s = deltaX * mirror3DOXZ[0][3] + deltaY * mirror3DOXZ[1][3] +
                    deltaZ * mirror3DOXZ[2][3] + deltaS * mirror3DOXZ[3][3];

                coords3D[count] = temp;
            }
        }

        private void buttonMirror3DOYZ_Click(object sender, EventArgs e)
        {
            double[][] mirror3DOYZ = new double[4][];

            for (int count = 0; count < 4; ++count)
                mirror3DOYZ[count] = new double[4];

            mirror3DOYZ[0][0] = 1;
            mirror3DOYZ[0][1] = 0;
            mirror3DOYZ[0][2] = 0;
            mirror3DOYZ[0][3] = 0;

            mirror3DOYZ[1][0] = 0;
            mirror3DOYZ[1][1] = -1;
            mirror3DOYZ[1][2] = 0;
            mirror3DOYZ[1][3] = 0;

            mirror3DOYZ[2][0] = 0;
            mirror3DOYZ[2][1] = 0;
            mirror3DOYZ[2][2] = 1;
            mirror3DOYZ[2][3] = 0;

            mirror3DOYZ[3][0] = 0;
            mirror3DOYZ[3][1] = 0;
            mirror3DOYZ[3][2] = 0;
            mirror3DOYZ[3][3] = 1;

            for (int count = 0; count < coords3D.Count; ++count)
            {
                Coord3D temp = coords3D[count];

                double deltaX = temp.x;
                double deltaY = temp.y;
                double deltaZ = temp.z;
                double deltaS = temp.s;

                temp.x = (deltaX * mirror3DOYZ[0][0] + deltaY * mirror3DOYZ[1][0] +
                    deltaZ * mirror3DOYZ[2][0] + deltaS * mirror3DOYZ[3][0]);
                temp.y = (deltaX * mirror3DOYZ[0][1] + deltaY * mirror3DOYZ[1][1] +
                    deltaZ * mirror3DOYZ[2][1] + deltaS * mirror3DOYZ[3][1]);
                temp.z = (deltaX * mirror3DOYZ[0][2] + deltaY * mirror3DOYZ[1][2] +
                    deltaZ * mirror3DOYZ[2][2] + deltaS * mirror3DOYZ[3][2]);
                temp.s = deltaX * mirror3DOYZ[0][3] + deltaY * mirror3DOYZ[1][3] +
                    deltaZ * mirror3DOYZ[2][3] + deltaS * mirror3DOYZ[3][3];

                coords3D[count] = temp;
            }
        }
        private void button_rotation_3D_OX_Click(object sender, EventArgs e)
        {
            double[][] rotate3DOX = new double[4][];

            for (int count = 0; count < 4; ++count)
                rotate3DOX[count] = new double[4];

            double phi = System.Convert.ToDouble(this.textBox_angle_for_rotation_3D.Text) * Math.PI / 180.0;

            rotate3DOX[0][0] = 1;
            rotate3DOX[0][1] = 0;
            rotate3DOX[0][2] = 0;
            rotate3DOX[0][3] = 0;

            rotate3DOX[1][0] = 0;
            rotate3DOX[1][1] = Math.Cos(phi);
            rotate3DOX[1][2] = Math.Sin(phi);
            rotate3DOX[1][3] = 0;

            rotate3DOX[2][0] = 0;
            rotate3DOX[2][1] = -Math.Sin(phi);
            rotate3DOX[2][2] = Math.Cos(phi);
            rotate3DOX[2][3] = 0;

            rotate3DOX[3][0] = 0;
            rotate3DOX[3][1] = 0;
            rotate3DOX[3][2] = 0;
            rotate3DOX[3][3] = 1;

            for (int count = 0; count < coords3D.Count; ++count)
            {
                Coord3D temp = coords3D[count];

                double deltaX = temp.x;
                double deltaY = temp.y;
                double deltaZ = temp.z;
                double deltaS = temp.s;

                temp.x = (deltaX * rotate3DOX[0][0] + deltaY * rotate3DOX[1][0] +
                    deltaZ * rotate3DOX[2][0] + deltaS * rotate3DOX[3][0]);
                temp.y = (deltaX * rotate3DOX[0][1] + deltaY * rotate3DOX[1][1] +
                    deltaZ * rotate3DOX[2][1] + deltaS * rotate3DOX[3][1]);
                temp.z = (deltaX * rotate3DOX[0][2] + deltaY * rotate3DOX[1][2] +
                    deltaZ * rotate3DOX [2][2] + deltaS * rotate3DOX[3][2]);
                temp.s = deltaX * rotate3DOX[0][3] + deltaY * rotate3DOX[1][3] +
                    deltaZ * rotate3DOX[2][3] + deltaS * rotate3DOX[3][3];

                coords3D[count] = temp;
            }
        }

        private void button_rotation_3D_OY_Click(object sender, EventArgs e)
        {
            double[][] rotate3DOY = new double[4][];

            for (int count = 0; count < 4; ++count)
                rotate3DOY[count] = new double[4];

            double phi = System.Convert.ToDouble(this.textBox_angle_for_rotation_3D.Text) * Math.PI / 180.0;

            rotate3DOY[0][0] = Math.Cos(phi);
            rotate3DOY[0][1] = 0;
            rotate3DOY[0][2] = -Math.Sin(phi);
            rotate3DOY[0][3] = 0;

            rotate3DOY[1][0] = 0;
            rotate3DOY[1][1] = 1;
            rotate3DOY[1][2] = 0;
            rotate3DOY[1][3] = 0;

            rotate3DOY[2][0] = Math.Sin(phi);
            rotate3DOY[2][1] = 0;
            rotate3DOY[2][2] = Math.Cos(phi);
            rotate3DOY[2][3] = 0;

            rotate3DOY[3][0] = 0;
            rotate3DOY[3][1] = 0;
            rotate3DOY[3][2] = 0;
            rotate3DOY[3][3] = 1;

            for (int count = 0; count < coords3D.Count; ++count)
            {
                Coord3D temp = coords3D[count];

                double deltaX = temp.x;
                double deltaY = temp.y;
                double deltaZ = temp.z;
                double deltaS = temp.s;

                temp.x = (deltaX * rotate3DOY[0][0] + deltaY * rotate3DOY[1][0] +
                    deltaZ * rotate3DOY[2][0] + deltaS * rotate3DOY[3][0]);
                temp.y = (deltaX * rotate3DOY[0][1] + deltaY * rotate3DOY[1][1] +
                    deltaZ * rotate3DOY[2][1] + deltaS * rotate3DOY[3][1]);
                temp.z = (deltaX * rotate3DOY[0][2] + deltaY * rotate3DOY[1][2] +
                    deltaZ * rotate3DOY[2][2] + deltaS * rotate3DOY[3][2]);
                temp.s = deltaX * rotate3DOY[0][3] + deltaY * rotate3DOY[1][3] +
                    deltaZ * rotate3DOY[2][3] + deltaS * rotate3DOY[3][3];

                coords3D[count] = temp;
            }
        }

        private void button_rotation_3D_OZ_Click(object sender, EventArgs e)
        {
            double[][] rotate3DOZ = new double[4][];

            for (int count = 0; count < 4; ++count)
                rotate3DOZ[count] = new double[4];

            double phi = System.Convert.ToDouble(this.textBox_angle_for_rotation_3D.Text) * Math.PI / 180.0;

            rotate3DOZ[0][0] = Math.Cos(phi);
            rotate3DOZ[0][1] = Math.Sin(phi);
            rotate3DOZ[0][2] = 0;
            rotate3DOZ[0][3] = 0;

            rotate3DOZ[1][0] = -Math.Sin(phi);
            rotate3DOZ[1][1] = Math.Cos(phi);
            rotate3DOZ[1][2] = 0;
            rotate3DOZ[1][3] = 0;

            rotate3DOZ[2][0] = 0;
            rotate3DOZ[2][1] = 0;
            rotate3DOZ[2][2] = 1;
            rotate3DOZ[2][3] = 0;

            rotate3DOZ[3][0] = 0;
            rotate3DOZ[3][1] = 0;
            rotate3DOZ[3][2] = 0;
            rotate3DOZ[3][3] = 1;

            for (int count = 0; count < coords3D.Count; ++count)
            {
                Coord3D temp = coords3D[count];

                double deltaX = temp.x;
                double deltaY = temp.y;
                double deltaZ = temp.z;
                double deltaS = temp.s;

                temp.x = (deltaX * rotate3DOZ[0][0] + deltaY * rotate3DOZ[1][0] +
                    deltaZ * rotate3DOZ[2][0] + deltaS * rotate3DOZ[3][0]);
                temp.y = (deltaX * rotate3DOZ[0][1] + deltaY * rotate3DOZ[1][1] +
                    deltaZ * rotate3DOZ[2][1] + deltaS * rotate3DOZ[3][1]);
                temp.z = (deltaX * rotate3DOZ[0][2] + deltaY * rotate3DOZ[1][2] +
                    deltaZ * rotate3DOZ[2][2] + deltaS * rotate3DOZ[3][2]);
                temp.s = deltaX * rotate3DOZ[0][3] + deltaY * rotate3DOZ[1][3] +
                    deltaZ * rotate3DOZ[2][3] + deltaS * rotate3DOZ[3][3];

                coords3D[count] = temp;
            }
        }
        private void buttonMove3D_Click(object sender, EventArgs e)
        {
            double[][] move3D = new double[4][];

            for (int count = 0; count < 4; ++count)
                move3D[count] = new double[4];

            move3D[0][0] = 1;
            move3D[0][1] = 0;
            move3D[0][2] = 0;
            move3D[0][3] = 0;

            move3D[1][0] = 0;
            move3D[1][1] = 1;
            move3D[1][2] = 0;
            move3D[1][3] = 0;

            move3D[2][0] = 0;
            move3D[2][1] = 0;
            move3D[2][2] = 1;
            move3D[2][3] = 0;

            move3D[3][0] = System.Convert.ToDouble(this.textBoxLambdaFor3D.Text);
            move3D[3][1] = System.Convert.ToDouble(this.textBoxMuFor3D.Text);
            move3D[3][2] = System.Convert.ToDouble(this.textBoxNuFor3D.Text);
            move3D[3][3] = 1;

            for (int count = 0; count < coords3D.Count; ++count)
            {
                Coord3D temp = coords3D[count];

                double deltaX = temp.x;
                double deltaY = temp.y;
                double deltaZ = temp.z;
                double deltaS = temp.s;

                temp.x = (deltaX * move3D[0][0] + deltaY * move3D[1][0] +
                    deltaZ * move3D[2][0] + deltaS * move3D[3][0]);
                temp.y = (deltaX * move3D[0][1] + deltaY * move3D[1][1] +
                    deltaZ * move3D[2][1] + deltaS * move3D[3][1]);
                temp.z = (deltaX * move3D[0][2] + deltaY * move3D[1][2] +
                    deltaZ * move3D[2][2] + deltaS * move3D[3][2]);
                temp.s = deltaX * move3D[0][3] + deltaY * move3D[1][3] +
                    deltaZ * move3D[2][3] + deltaS * move3D[3][3];

                coords3D[count] = temp;
            }
        }

        private void buttonTrimetricProjection_Click(object sender, EventArgs e)
        {
            double[][] TrimetricProjection = new double[4][];

            for (int count = 0; count < 4; ++count)
                TrimetricProjection[count] = new double[4];

            double phi = System.Convert.ToDouble(this.textBoxTrimetricProjectionPhi.Text);
            double tetta = System.Convert.ToDouble(this.textBoxTrimetricProjectionTetta.Text);

            TrimetricProjection[0][0] = Math.Cos(phi);
            TrimetricProjection[0][1] = Math.Sin(phi)*Math.Sin(tetta);
            TrimetricProjection[0][2] = 0;
            TrimetricProjection[0][3] = 0;

            TrimetricProjection[1][0] = 0;
            TrimetricProjection[1][1] = Math.Sin(tetta);
            TrimetricProjection[1][2] = 0;
            TrimetricProjection[1][3] = 0;

            TrimetricProjection[2][0] = Math.Sin(phi);
            TrimetricProjection[2][1] = -Math.Cos(phi) * Math.Sin(tetta);
            TrimetricProjection[2][2] = 0;
            TrimetricProjection[2][3] = 0;

            TrimetricProjection[3][0] = 0;
            TrimetricProjection[3][1] = 0;
            TrimetricProjection[3][2] = 0;
            TrimetricProjection[3][3] = 1;

            for (int count = 0; count < coords3D.Count; ++count)
            {
                Coord3D temp = coords3D[count];

                double deltaX = temp.x;
                double deltaY = temp.y;
                double deltaZ = temp.z;
                double deltaS = temp.s;

                temp.x = (deltaX * TrimetricProjection[0][0] + deltaY * TrimetricProjection[1][0] +
                    deltaZ * TrimetricProjection[2][0] + deltaS * TrimetricProjection[3][0]);
                temp.y = (deltaX * TrimetricProjection[0][1] + deltaY * TrimetricProjection[1][1] +
                    deltaZ * TrimetricProjection[2][1] + deltaS * TrimetricProjection[3][1]);
                temp.z = (deltaX * TrimetricProjection[0][2] + deltaY * TrimetricProjection[1][2] +
                    deltaZ * TrimetricProjection[2][2] + deltaS * TrimetricProjection[3][2]);
                temp.s = deltaX * TrimetricProjection[0][3] + deltaY * TrimetricProjection[1][3] +
                    deltaZ * TrimetricProjection[2][3] + deltaS * TrimetricProjection[3][3];

                coords3D[count] = temp;
            }
        }
        private void button8_Click(object sender, EventArgs e)
        {
            string name_first = this.textBox8.Text;
            string name_second = this.textBox9.Text;

            Coord3D first = null;
            Coord3D second = null;

            if (name_first == name_second)
            {
                this.textBox8.Text = "";
                this.textBox9.Text = "";
                MessageBox.Show("Ввод данных произведен неверно!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            bool isContainFirst = false;
            bool isContainSecond = false;

            foreach(var point in coords3D)
            {
                if (point.index == System.Convert.ToInt32(name_first))
                {
                    first = point;
                    isContainFirst = true;
                }
                else if (point.index == System.Convert.ToInt32(name_second))
                {
                    second = point;
                    isContainSecond = true;
                }
            }

            if (isContainFirst && isContainSecond) 
            {
                pairs.Add(Tuple.Create(first, second));
            }
            else
            {
                MessageBox.Show("Ввод данных произведен неверно!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

    }
}
