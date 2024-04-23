using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Text;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.ProgressBar;

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

            public Coord3D(double x, double y, double z)
            {
                this.x = x;
                this.y = y;
                this.z = z;
                this.s = 1;
            }
        }

        List<Coord3D> basis = new List<Coord3D>();
        List<Tuple<int, int>> basis_line = new List<Tuple<int, int>>();

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

        List<List<Coord3D>> figures = new List<List<Coord3D>>();
        List<List<Tuple<int, int>>> figures_lines = new List<List<Tuple<int, int>>>();

        List<List<Coord3D>> figures_trimetric_projection = new List<List<Coord3D>>();

        private void create_parallelepiped(Coord3D start_point, int x, int y, int z, int size = 25, int size_for_copy = 10)
        {
            List<Coord3D> parallelepiped = new List<Coord3D>();
            List<Tuple<int, int>> parallelepiped_lines = new List<Tuple<int, int>>();

            // Coords for parallelepiped
            parallelepiped.Add(new Coord3D(size * (start_point.x), size * (start_point.y), size * (start_point.z))); // p0
            parallelepiped.Add(new Coord3D(size * (start_point.x), size * (start_point.y), size * (start_point.z + z))); // p1
            parallelepiped.Add(new Coord3D(size * (start_point.x), size * (start_point.y + y), size * (start_point.z + z))); // p2
            parallelepiped.Add(new Coord3D(size * (start_point.x), size * (start_point.y + y), size * (start_point.z))); // p3
            parallelepiped.Add(new Coord3D(size * (start_point.x + x), size * (start_point.y + y), size * (start_point.z))); // p4
            parallelepiped.Add(new Coord3D(size * (start_point.x + x), size * (start_point.y), size * (start_point.z))); // p5
            parallelepiped.Add(new Coord3D(size * (start_point.x + x), size * (start_point.y), size * (start_point.z + z))); // p6
            parallelepiped.Add(new Coord3D(size * (start_point.x + x), size * (start_point.y + y), size * (start_point.z + z))); // p7

            // Lines for parallelepiped
            parallelepiped_lines.Add(Tuple.Create(0, 1));
            parallelepiped_lines.Add(Tuple.Create(0, 3));
            parallelepiped_lines.Add(Tuple.Create(0, 5));

            parallelepiped_lines.Add(Tuple.Create(1, 2));
            parallelepiped_lines.Add(Tuple.Create(1, 6));

            parallelepiped_lines.Add(Tuple.Create(2, 3));
            parallelepiped_lines.Add(Tuple.Create(2, 7));

            parallelepiped_lines.Add(Tuple.Create(4, 3));
            parallelepiped_lines.Add(Tuple.Create(4, 5));
            parallelepiped_lines.Add(Tuple.Create(4, 7));

            parallelepiped_lines.Add(Tuple.Create(5, 6));

            parallelepiped_lines.Add(Tuple.Create(6, 7));

            figures.Add(parallelepiped);
            figures_lines.Add(parallelepiped_lines);

        }

        private void create_frustum(Coord3D start_point, int h, int r1, int r2, List<int> normal, int size = 25, int size_for_copy = 10)
        {
            List<Coord3D> frustum = new List<Coord3D>();

            int count_of_vertex_in_part = 4;

            int count_of_vertex_for_circle = 4 + 4 * count_of_vertex_in_part;

            for (int count = 0; count < count_of_vertex_for_circle; ++count)
            {
                double angle = count * (double)360 / count_of_vertex_for_circle;
                double first_point = r1 * Math.Cos(angle * Math.PI / 180.0);
                double second_point = r1 * Math.Sin(angle * Math.PI / 180.0);

                if (normal[0] == 1)
                    frustum.Add(new Coord3D(size * start_point.x, size * (start_point.y + first_point), size * (start_point.z + second_point)));
                else if (normal[1] == 1)
                    frustum.Add(new Coord3D(size * (start_point.x + first_point), size * start_point.y, size * (start_point.z + second_point)));
                else if (normal[2] == 1)
                    frustum.Add(new Coord3D(size * (start_point.x + first_point), size * (start_point.y + second_point), size * start_point.z));
            }

            for (int count = 0; count < count_of_vertex_for_circle; ++count)
            {
                double angle = count * (double)360 / count_of_vertex_for_circle;
                double first_point = r2 * Math.Cos(angle * Math.PI / 180.0);
                double second_point = r2 * Math.Sin(angle * Math.PI / 180.0);

                if (normal[0] == 1)
                    frustum.Add(new Coord3D(size * start_point.x + h * size, size * (start_point.y + first_point), size * (start_point.z + second_point)));
                else if (normal[1] == 1)
                    frustum.Add(new Coord3D(size * (start_point.x + first_point), size * start_point.y + +h * size, size * (start_point.z + second_point)));
                else if (normal[2] == 1)
                    frustum.Add(new Coord3D(size * (start_point.x + first_point), size * (start_point.y + second_point), size * start_point.z + h * size));
            }

            List<Tuple<int, int>> frustum_lines = new List<Tuple<int, int>>();

            for (int count = 0; count < count_of_vertex_for_circle; ++count)
            {
                frustum_lines.Add(Tuple.Create(count, (count + 1 == count_of_vertex_for_circle) ? 0 : count + 1));
            }

            for (int count = count_of_vertex_for_circle; count < 2 * count_of_vertex_for_circle; ++count)
            {
                frustum_lines.Add(Tuple.Create(count, (count + 1 == 2 * count_of_vertex_for_circle) ? count_of_vertex_for_circle : count + 1));
            }

            for (int count = 0; count < count_of_vertex_for_circle; ++count)
            {
                frustum_lines.Add(Tuple.Create(count, count + count_of_vertex_for_circle));
            }


            figures.Add(frustum);

            figures_lines.Add(frustum_lines);

        }

        private void init_object()
        {
            figures.Clear();
            figures_lines.Clear();

            create_parallelepiped(new Coord3D(0, 0, 0), 8, 3, 3, 25, 10);
            create_parallelepiped(new Coord3D(0, 3, 0), 6, 8, 2, 25, 10);
            create_parallelepiped(new Coord3D(0, 1, 2), 3, 9, 11, 25, 10);
            create_parallelepiped(new Coord3D(3, 4, 9), 2, 5, 1, 25, 10);

            create_frustum(new Coord3D(5, 1.5, 3), 3, 1, 2, new List<int> { 0, 0, 1 }, 25, 10);
        }

        private void printPointsConsole()
        {
            Console.WriteLine("======================");

            foreach (var figura in figures)
            {
                foreach (var point in figura)
                {
                    Console.WriteLine($"x: {point.x};\t y: {point.y};\t z: {point.z}");
                }
            }
        }

        private void draw_oxy_projection()
        {
            user_GraphicsForOXYTab.FillRectangle(Brushes.White, 0, 0, pictureBoxOXY.Width, pictureBoxOXY.Height);

            for (int i = 0; i < figures_lines.Count; ++i)
            {
                for (int j = 0; j < figures_lines[i].Count; ++j)
                {
                    user_GraphicsForOXYTab.DrawLine(myPen, (float)(figures[i][figures_lines[i][j].Item1].x + x0_for_3D_OXY), (float)(-figures[i][figures_lines[i][j].Item1].y + y0_for_3D_OXY),
                        (float)(figures[i][figures_lines[i][j].Item2].x + x0_for_3D_OXY), (float)(-figures[i][figures_lines[i][j].Item2].y + y0_for_3D_OXY));
                }
            }

            pictureBoxOXY.Image = canvasFor3DTabOXY;

            user_GraphicsForOXYTab.DrawLine(Pens.Gray, (float)x0_for_3D_OXY, 2, (float)x0_for_3D_OXY, pictureBoxOXY.Height - 2);
            user_GraphicsForOXYTab.DrawLine(Pens.Gray, 2, (float)y0_for_3D_OXY, pictureBoxOXY.Width - 2, (float)y0_for_3D_OXY);
            user_GraphicsForOXYTab.DrawString("x", axFont, axBrush, (float)(pictureBoxOXY.Width - 10), (float)(y0_for_3D_OXY + 3), drawFormat);
            user_GraphicsForOXYTab.DrawString("y", axFont, axBrush, (float)(x0_for_3D_OXY - 10), (float)(3.0), drawFormat);
        }

        private void draw_oxz_projection()
        {
            user_GraphicsForOXZTab.FillRectangle(Brushes.White, 0, 0, pictureBoxOXZ.Width, pictureBoxOXZ.Height);

            for (int i = 0; i < figures_lines.Count; ++i)
            {
                for (int j = 0; j < figures_lines[i].Count; ++j)
                {
                    user_GraphicsForOXZTab.DrawLine(myPen, (float)(figures[i][figures_lines[i][j].Item1].x + x0_for_3D_OXZ), (float)(-figures[i][figures_lines[i][j].Item1].z + z0_for_3D_OXZ),
                        (float)(figures[i][figures_lines[i][j].Item2].x + x0_for_3D_OXZ), (float)(-figures[i][figures_lines[i][j].Item2].z + z0_for_3D_OXZ));
                }
            }

            pictureBoxOXZ.Image = canvasFor3DTabOXZ;

            user_GraphicsForOXZTab.DrawLine(Pens.Gray, (float)x0_for_3D_OXZ, 2, (float)x0_for_3D_OXZ, pictureBoxOXZ.Height - 2);
            user_GraphicsForOXZTab.DrawLine(Pens.Gray, 2, (float)z0_for_3D_OXZ, pictureBoxOXZ.Width - 2, (float)z0_for_3D_OXZ);
            user_GraphicsForOXZTab.DrawString("x", axFont, axBrush, (float)(pictureBoxOXZ.Width - 10), (float)(z0_for_3D_OXZ + 3), drawFormat);
            user_GraphicsForOXZTab.DrawString("z", axFont, axBrush, (float)(x0_for_3D_OXZ - 10), (float)(3.0), drawFormat);
        }

        private void draw_oyz_projection()
        {

            user_GraphicsForOYZTab.FillRectangle(Brushes.White, 0, 0, pictureBoxOYZ.Width, pictureBoxOYZ.Height);

            for (int i = 0; i<figures_lines.Count; ++i)
            {
                for (int j = 0; j<figures_lines[i].Count; ++j)
                {
                    user_GraphicsForOYZTab.DrawLine(myPen, (float)(figures[i][figures_lines[i][j].Item1].y + y0_for_3D_OYZ), (float) (-figures[i][figures_lines[i][j].Item1].z + z0_for_3D_OYZ),
                        (float) (figures[i][figures_lines[i][j].Item2].y + y0_for_3D_OYZ), (float) (-figures[i][figures_lines[i][j].Item2].z + z0_for_3D_OYZ));
                }
            }

            pictureBoxOYZ.Image = canvasFor3DTabOYZ;

            user_GraphicsForOYZTab.DrawLine(Pens.Gray, (float)y0_for_3D_OYZ, 2, (float)y0_for_3D_OYZ, pictureBoxOYZ.Height - 2);
            user_GraphicsForOYZTab.DrawLine(Pens.Gray, 2, (float)z0_for_3D_OYZ, pictureBoxOYZ.Width - 2, (float)z0_for_3D_OYZ);
            user_GraphicsForOYZTab.DrawString("y", axFont, axBrush, (float)(pictureBoxOYZ.Width - 10), (float)(z0_for_3D_OYZ + 3), drawFormat);
            user_GraphicsForOYZTab.DrawString("z", axFont, axBrush, (float)(y0_for_3D_OYZ - 10), (float)(3.0), drawFormat);

        }

        private void draw_trimmetric_projection()
        {
            user_Graphicsfor_Trimetric_Projection.FillRectangle(Brushes.White, 0, 0, pictureBoxTrimetricProjection.Width, pictureBoxTrimetricProjection.Height);

            if (figures_trimetric_projection.Count != 0)
            {
                for (int i = 0; i < figures_lines.Count; ++i)
                {
                    for (int j = 0; j < figures_lines[i].Count; ++j)
                    {
                        user_Graphicsfor_Trimetric_Projection.DrawLine(myPen, (float)(figures_trimetric_projection[i][figures_lines[i][j].Item1].x + x0_for_Trimetric_Projection), (float)(-figures_trimetric_projection[i][figures_lines[i][j].Item1].y + y0_for_Trimetric_Projection),
                            (float)(figures_trimetric_projection[i][figures_lines[i][j].Item2].x + x0_for_Trimetric_Projection), (float)(-figures_trimetric_projection[i][figures_lines[i][j].Item2].y + y0_for_Trimetric_Projection));
                    }
                }
            }

            for (int i = 0; i < basis_line.Count; ++i)
            {
                user_Graphicsfor_Trimetric_Projection.DrawLine(new Pen(Color.Red, 2), (float)(basis[basis_line[i].Item1].x + x0_for_Trimetric_Projection),
                    (float)(-basis[basis_line[i].Item1].y + y0_for_Trimetric_Projection),
                    (float)(basis[basis_line[i].Item2].x + x0_for_Trimetric_Projection), 
                    (float)(-basis[basis_line[i].Item2].y + y0_for_Trimetric_Projection));
            }
            user_Graphicsfor_Trimetric_Projection.DrawString(string.Format("x"), drawFont, drawBrush, (float)(x0_for_Trimetric_Projection + basis[basis_line[0].Item2].x) + 3, (float)(-basis[basis_line[0].Item2].y + y0_for_Trimetric_Projection) + 3, drawFormat);
            user_Graphicsfor_Trimetric_Projection.DrawString(string.Format("y"), drawFont, drawBrush, (float)(x0_for_Trimetric_Projection + basis[basis_line[1].Item2].x) + 3, (float)(-basis[basis_line[1].Item2].y + y0_for_Trimetric_Projection) + 3, drawFormat);
            user_Graphicsfor_Trimetric_Projection.DrawString(string.Format("z"), drawFont, drawBrush, (float)(x0_for_Trimetric_Projection + basis[basis_line[2].Item2].x) + 3, (float)(-basis[basis_line[2].Item2].y + y0_for_Trimetric_Projection) + 3, drawFormat);

            pictureBoxTrimetricProjection.Image = canvasfor_Trimetric_Projection;
        }

        private void draw_all()
        {
            draw_oxy_projection();
            draw_oxz_projection();
            draw_oyz_projection();
            draw_trimmetric_projection();
        }

        private void button_for_draw_3D_points_Click(object sender, EventArgs e)
        {

           // printPointsConsole();

            draw_all();
            
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

            for (int i = 0; i < figures.Count; ++i)
            {
                for (int j = 0; j < figures[i].Count; ++j)
                {
                    Coord3D temp = figures[i][j];

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

                    figures[i][j] = temp;
                }
            }

            for (int i = 0; i < figures_trimetric_projection.Count; ++i)
            {
                for (int j = 0; j < figures_trimetric_projection[i].Count; ++j)
                {
                    Coord3D temp = figures_trimetric_projection[i][j];

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

                    figures_trimetric_projection[i][j] = temp;
                }
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

            for (int i = 0; i < figures.Count; ++i)
            {
                for (int j = 0; j < figures[i].Count; ++j)
                {
                    Coord3D temp = figures[i][j];

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

                    figures[i][j] = temp;
                }
            }

            for (int i = 0; i < figures_trimetric_projection.Count; ++i)
            {
                for (int j = 0; j < figures_trimetric_projection[i].Count; ++j)
                {
                    Coord3D temp = figures_trimetric_projection[i][j];

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

                    figures_trimetric_projection[i][j] = temp;
                }
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

            for (int i = 0; i < figures.Count; ++i)
            {
                for (int j = 0; j < figures[i].Count; ++j)
                {
                    Coord3D temp = figures[i][j];

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

                    figures[i][j] = temp;
                }
            }

            for (int i = 0; i < figures_trimetric_projection.Count; ++i)
            {
                for (int j = 0; j < figures_trimetric_projection[i].Count; ++j)
                {
                    Coord3D temp = figures_trimetric_projection[i][j];

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

                    figures_trimetric_projection[i][j] = temp;
                }
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

            for (int i = 0; i < figures.Count; ++i)
            {
                for (int j = 0; j < figures[i].Count; ++j)
                {
                    Coord3D temp = figures[i][j];

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

                    figures[i][j] = temp;
                }
            }

            for (int i = 0; i < figures_trimetric_projection.Count; ++i)
            {
                for (int j = 0; j < figures_trimetric_projection[i].Count; ++j)
                {
                    Coord3D temp = figures_trimetric_projection[i][j];

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

                    figures_trimetric_projection[i][j] = temp;
                }
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

            for (int i = 0; i < figures.Count; ++i)
            {
                for (int j = 0; j < figures[i].Count; ++j)
                {
                    Coord3D temp = figures[i][j];

                    double deltaX = temp.x;
                    double deltaY = temp.y;
                    double deltaZ = temp.z;
                    double deltaS = temp.s;

                    temp.x = (deltaX * rotate3DOX[0][0] + deltaY * rotate3DOX[1][0] +
                        deltaZ * rotate3DOX[2][0] + deltaS * rotate3DOX[3][0]);
                    temp.y = (deltaX * rotate3DOX[0][1] + deltaY * rotate3DOX[1][1] +
                        deltaZ * rotate3DOX[2][1] + deltaS * rotate3DOX[3][1]);
                    temp.z = (deltaX * rotate3DOX[0][2] + deltaY * rotate3DOX[1][2] +
                        deltaZ * rotate3DOX[2][2] + deltaS * rotate3DOX[3][2]);
                    temp.s = deltaX * rotate3DOX[0][3] + deltaY * rotate3DOX[1][3] +
                        deltaZ * rotate3DOX[2][3] + deltaS * rotate3DOX[3][3];

                    figures[i][j] = temp;
                }
            }

            for (int i = 0; i < figures_trimetric_projection.Count; ++i)
            {
                for (int j = 0; j < figures_trimetric_projection[i].Count; ++j)
                {
                    Coord3D temp = figures_trimetric_projection[i][j];

                    double deltaX = temp.x;
                    double deltaY = temp.y;
                    double deltaZ = temp.z;
                    double deltaS = temp.s;

                    temp.x = (deltaX * rotate3DOX[0][0] + deltaY * rotate3DOX[1][0] +
                        deltaZ * rotate3DOX[2][0] + deltaS * rotate3DOX[3][0]);
                    temp.y = (deltaX * rotate3DOX[0][1] + deltaY * rotate3DOX[1][1] +
                        deltaZ * rotate3DOX[2][1] + deltaS * rotate3DOX[3][1]);
                    temp.z = (deltaX * rotate3DOX[0][2] + deltaY * rotate3DOX[1][2] +
                        deltaZ * rotate3DOX[2][2] + deltaS * rotate3DOX[3][2]);
                    temp.s = deltaX * rotate3DOX[0][3] + deltaY * rotate3DOX[1][3] +
                        deltaZ * rotate3DOX[2][3] + deltaS * rotate3DOX[3][3];

                    figures_trimetric_projection[i][j] = temp;
                }
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

            for (int i = 0; i < figures.Count; ++i)
            {
                for (int j = 0; j < figures[i].Count; ++j)
                {
                    Coord3D temp = figures[i][j];

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

                    figures[i][j] = temp;
                }
            }

            for (int i = 0; i < figures_trimetric_projection.Count; ++i)
            {
                for (int j = 0; j < figures_trimetric_projection[i].Count; ++j)
                {
                    Coord3D temp = figures_trimetric_projection[i][j];

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

                    figures_trimetric_projection[i][j] = temp;
                }
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

            for (int i = 0; i < figures.Count; ++i)
            {
                for (int j = 0; j < figures[i].Count; ++j)
                {
                    Coord3D temp = figures[i][j];

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

                    figures[i][j] = temp;
                }
            }

            for (int i = 0; i < figures_trimetric_projection.Count; ++i)
            {
                for (int j = 0; j < figures_trimetric_projection[i].Count; ++j)
                {
                    Coord3D temp = figures_trimetric_projection[i][j];

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

                    figures_trimetric_projection[i][j] = temp;
                }
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

            for (int i = 0; i < figures.Count; ++i)
            {
                for (int j = 0; j < figures[i].Count; ++j)
                {
                    Coord3D temp = figures[i][j];

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

                    figures[i][j] = temp;
                }
            }

        }

        private void clear_and_copy_trimetrical_projection()
        {
            // Очистка списков
            figures_trimetric_projection.Clear();

            for (int i = 0; i < figures.Count; ++i)
            {
                List<Coord3D> temp_points = new List<Coord3D>();

                for (int j = 0; j < figures[i].Count; ++j)
                {

                    double x = figures[i][j].x;
                    double y = figures[i][j].y;
                    double z = figures[i][j].z;

                    Coord3D temp_point = new Coord3D(x, y, z);

                    temp_points.Add(temp_point);

                }

                figures_trimetric_projection.Add(temp_points);
            }
        }
        
        private void trimetrical_projection()
        {
            double[][] TrimetricProjection = new double[4][];

            for (int count = 0; count < 4; ++count)
                TrimetricProjection[count] = new double[4];

            double phi = System.Convert.ToDouble(this.trackBar1.Value) * Math.PI / 180.0;
            double tetta = System.Convert.ToDouble(this.trackBar2.Value) * Math.PI / 180.0;

            TrimetricProjection[0][0] = Math.Cos(phi);
            TrimetricProjection[0][1] = Math.Sin(phi) * Math.Sin(tetta);
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


            for (int i = 0; i < figures_trimetric_projection.Count; ++i)
            {
                for (int j = 0; j < figures_trimetric_projection[i].Count; ++j)
                {
                    Coord3D temp = figures_trimetric_projection[i][j];

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

                    figures_trimetric_projection[i][j] = temp;
                }
            }

            basis[0].x = 0;
            basis[0].y = 0;
            basis[0].z = 0;
            basis[0].s = 1;

            basis[1].x = 100;
            basis[1].y = 0;
            basis[1].z = 0;
            basis[1].s = 1;

            basis[2].x = 0;
            basis[2].y = 100;
            basis[2].z = 0;
            basis[2].s = 1;

            basis[3].x = 0;
            basis[3].y = 0;
            basis[3].z = 100;
            basis[3].s = 1;

            for (int count = 0; count < basis.Count; ++count)
            {
                Coord3D temp = basis[count];

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

                basis[count] = temp;
            }
        }

        private void buttonTrimetricProjection_Click(object sender, EventArgs e)
        {
            clear_and_copy_trimetrical_projection();

            trimetrical_projection();

            draw_trimmetric_projection();
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            clear_and_copy_trimetrical_projection();

            trimetrical_projection();

            draw_trimmetric_projection();
        }

        private void trackBar2_Scroll(object sender, EventArgs e)
        {
            clear_and_copy_trimetrical_projection();

            trimetrical_projection();

            draw_trimmetric_projection();
        }

    }
}
