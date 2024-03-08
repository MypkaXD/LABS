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

        List<Point> MyPolygon = new List<Point>();
        Pen pen1;
        int x0, x, y0, y, n = 0;
        Bitmap canvas;
        Color user_color;
        Graphics user_Graphics;
        Font drawFont = new Font("Arial", 10);
        Font axFont = new Font("Tahoma", 10);
        SolidBrush drawBrush = new SolidBrush(Color.Black);
        SolidBrush axBrush = new SolidBrush(Color.Gray);
        // формат строки
        StringFormat drawFormat = new StringFormat();

        private class Coords
        {
            public int x { get; set; }
            public int y { get; set; }
        }

        int j = 0;

        private List<Coords> coords = new List<Coords>();

        public Form1()
        {
            InitializeComponent();
            user_color = Color.Blue;
            pen1 = new Pen(user_color, 2);
            canvas = new Bitmap(pictureBox_2d.Width,
            pictureBox_2d.Height);
            user_Graphics = Graphics.FromImage(canvas);
            x0 = (int)(pictureBox_2d.Width / 2);
            y0 = (int)(pictureBox_2d.Height / 2);
            user_Graphics.FillRectangle(Brushes.White, 0, 0,
            pictureBox_2d.Width, pictureBox_2d.Height);
            user_Graphics.DrawLine(Pens.Gray, x0, 2, x0,
            pictureBox_2d.Height - 2);
            user_Graphics.DrawLine(Pens.Gray, 2, y0,
            pictureBox_2d.Width - 2, y0);
            pictureBox_2d.Image = canvas;

            user_Graphics.DrawString("x", axFont, axBrush, (float)(pictureBox_2d.Width - 10), (float)(y0 + 3), drawFormat);
            user_Graphics.DrawString("y", axFont, axBrush, (float)(x0 - 10), (float)(3.0), drawFormat);

        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            int temp_x = 0;
            int temp_y = 0;

            if (Int32.TryParse(this.textBox1.Text, out temp_x) && 
                Int32.TryParse(this.textBox2.Text, out temp_y))
            {
                ++j;
                Coords coord = new Coords();
                coord.x = temp_x;
                coord.y = temp_y;
                coords.Add(coord);
            }
            else
            {
                MessageBox.Show("Ввод данных произведен неверно!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            this.label1.Text = "Координаты точек:\n";

            foreach (var coord in coords)
            {
                this.label1.Text += "(" + System.Convert.ToString(coord.x) + "; " + System.Convert.ToString(coord.y) + ")\n";
            }

            if (MyPolygon == null) 
                return;
            MyPolygon.Add(new Point(temp_x, temp_y));
            user_Graphics.FillRectangle(Brushes.White, 0, 0, pictureBox_2d.Width, pictureBox_2d.Height);
            if (MyPolygon.Count < 3)
            {
                foreach (var point in MyPolygon)
                    user_Graphics.FillEllipse(Brushes.Blue, point.X - 2, point.Y - 2, 4, 4);
            }
            else
                user_Graphics.DrawPolygon(pen1, MyPolygon.ToArray());
            pictureBox_2d.Image = canvas;

            foreach (var point in MyPolygon)
                user_Graphics.DrawString(string.Format("p{0:d}",
                MyPolygon.IndexOf(point)), drawFont, drawBrush,
                (float)(point.X + 3), (float)(point.Y + 3), drawFormat);
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}
