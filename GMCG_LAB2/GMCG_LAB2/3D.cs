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
        private class Coord3D
        {
            public double x { get; set; }
            public double y { get; set; }
            public double z { get; set; }
            public double s { get; set; }

            public Coord3D(double x, double y, double z)
            {
                this.x = x;
                this.y = y;
                this.z = z;
                this.s = 1;
            }
        }

        List<Coord3D> coords3D = new List<Coord3D>();

        double x0_for_3D_OXY, y0_for_3D_OXY, z0_for_3D_OXY;
        double x0_for_3D_OXZ, y0_for_3D_OXZ, z0_for_3D_OXZ;
        double x0_for_3D_OYZ, y0_for_3D_OYZ, z0_for_3D_OYZ;

        Pen penFor3DTab;

        Bitmap canvasFor3DTabOXY;
        Bitmap canvasFor3DTabOXZ;
        Bitmap canvasFor3DTabOYZ;

        Graphics user_GraphicsForOXYTab;
        Graphics user_GraphicsForOXZTab;
        Graphics user_GraphicsForOYZTab;

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
                coords3D.Add(new Coord3D(temp_x, temp_y, temp_z));
            }
            else
            {
                MessageBox.Show("Ввод данных произведен неверно!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void button_for_draw_3D_points_Click(object sender, EventArgs e)
        {
            user_GraphicsForOXYTab.FillRectangle(Brushes.White, 0, 0, pictureBoxOXY.Width, pictureBoxOXY.Height);

            if (coords3D.Count < 3)
                foreach (var point in coords3D)
                    user_GraphicsForOXYTab.FillEllipse(Brushes.Blue, (float)point.x + (float)x0_for_3D_OXY - 2, -(float)point.y + (float)y0_for_3D_OXY - 2, 4, 4);
            else
                user_GraphicsForOXYTab.DrawPolygon(penFor3DTab, coords3D.Select(p => new PointF((float)(p.x + x0_for_3D_OXY), (float)(-p.y + y0_for_3D_OXY))).ToArray());

            pictureBoxOXY.Image = canvasFor3DTabOXY;

            foreach (var point in coords3D)
                user_GraphicsForOXYTab.DrawString(string.Format("p{0:d}", coords3D.IndexOf(point)), drawFont, drawBrush, (float)(point.x + x0_for_3D_OXY + 3), (float)(-point.y + y0_for_3D_OXY + 3), drawFormat);

            /////

            user_GraphicsForOXZTab.FillRectangle(Brushes.White, 0, 0, pictureBoxOXZ.Width, pictureBoxOXZ.Height);

            if (coords3D.Count < 3)
                foreach (var point in coords3D)
                    user_GraphicsForOXZTab.FillEllipse(Brushes.Blue, (float)point.x + (float)x0_for_3D_OXZ - 2, -(float)point.z + (float)z0_for_3D_OXZ - 2, 4, 4);
            else
                user_GraphicsForOXZTab.DrawPolygon(penFor3DTab, coords3D.Select(p => new PointF((float)(p.x + x0_for_3D_OXZ), -(float)p.z + (float)z0_for_3D_OXZ)).ToArray());

            pictureBoxOXZ.Image = canvasFor3DTabOXZ;

            foreach (var point in coords3D)
                user_GraphicsForOXZTab.DrawString(string.Format("p{0:d}", coords3D.IndexOf(point)), drawFont, drawBrush, (float)(point.x + x0_for_3D_OXZ + 3), (float)(-point.z + z0_for_3D_OXZ + 3), drawFormat);

            ////

            user_GraphicsForOYZTab.FillRectangle(Brushes.White, 0, 0, pictureBoxOYZ.Width, pictureBoxOYZ.Height);

            if (coords3D.Count < 3)
                foreach (var point in coords3D)
                    user_GraphicsForOYZTab.FillEllipse(Brushes.Blue, (float)(point.y + y0_for_3D_OYZ) - 2, (float)(-point.z + z0_for_3D_OYZ) - 2, 4, 4);
            else
                user_GraphicsForOYZTab.DrawPolygon(penFor3DTab, coords3D.Select(p => new PointF((float)(p.y + y0_for_3D_OYZ), (float)(-p.z + z0_for_3D_OYZ))).ToArray());

            pictureBoxOYZ.Image = canvasFor3DTabOYZ;

            foreach (var point in coords3D)
                user_GraphicsForOYZTab.DrawString(string.Format("p{0:d}", coords3D.IndexOf(point)), drawFont, drawBrush, (float)((point.y + y0_for_3D_OYZ) + 3), (float)((-point.z + z0_for_3D_OYZ) + 3), drawFormat);

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


    }
}
