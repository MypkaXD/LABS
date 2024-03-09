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

        public Form1()
        {
            InitializeComponent();

            user_color = Color.Blue;

            pen1 = new Pen(user_color, 2);
            
            canvas = new Bitmap(pictureBox_2d.Width, pictureBox_2d.Height);
            
            user_Graphics = Graphics.FromImage(canvas);

            x0 = (int)(pictureBox_2d.Width / 2);
            y0 = (int)(pictureBox_2d.Height / 2);

            user_Graphics.FillRectangle(Brushes.White, 0, 0, pictureBox_2d.Width, pictureBox_2d.Height);
            user_Graphics.DrawLine(Pens.Gray, x0, 2, x0, pictureBox_2d.Height - 2);
            user_Graphics.DrawLine(Pens.Gray, 2, y0, pictureBox_2d.Width - 2, y0); 
            user_Graphics.DrawString("x", axFont, axBrush, (float)(pictureBox_2d.Width - 10), (float)(y0 + 3), drawFormat);
            user_Graphics.DrawString("y", axFont, axBrush, (float)(x0 - 10), (float)(3.0), drawFormat);
            
            pictureBox_2d.Image = canvas;
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

            if (MyPolygon == null)
                return;

            if (Int32.TryParse(this.textBox1.Text, out temp_x) && 
                Int32.TryParse(this.textBox2.Text, out temp_y))
            {
                MyPolygon.Add(new Point(x0 + temp_x, y0 - temp_y));
            }
            else
            {
                MessageBox.Show("Ввод данных произведен неверно!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            printPoints();     
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

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

            for (int count = 0; count < MyPolygon.Count; ++count)
            {
                Point temp = MyPolygon[count];
                temp.X = (int)((((double)temp.X - x0) * resize2D[0][0] + ((double)temp.Y - y0) * resize2D[1][0]) + x0);
                temp.Y = (int)((((double)temp.X - x0) * resize2D[0][1] + ((double)temp.Y - y0) * resize2D[1][1]) + y0);
                MyPolygon[count] = temp;
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

            for (int count = 0; count < MyPolygon.Count; ++count)
            {
                Point temp = MyPolygon[count];

                int deltaX = temp.X - x0;
                int deltaY = temp.Y - y0;

                temp.X = (int)((deltaX * rotation2D[0][0] + deltaY * rotation2D[1][0]) + x0);
                temp.Y = (int)((deltaX * rotation2D[0][1] + deltaY * rotation2D[1][1]) + y0);

                MyPolygon[count] = temp;
            }
        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            user_Graphics.FillRectangle(Brushes.White, 0, 0, pictureBox_2d.Width, pictureBox_2d.Height);

            if (MyPolygon.Count < 3)
                foreach (var point in MyPolygon)
                    user_Graphics.FillEllipse(Brushes.Blue, point.X - 2, point.Y - 2, 4, 4);
            else
                user_Graphics.DrawPolygon(pen1, MyPolygon.ToArray());

            pictureBox_2d.Image = canvas;

            foreach (var point in MyPolygon)
                user_Graphics.DrawString(string.Format("p{0:d}", MyPolygon.IndexOf(point)), drawFont, drawBrush, (float)(point.X + 3), (float)(point.Y + 3), drawFormat);

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

            for (int count = 0; count < MyPolygon.Count; ++count)
            {
                Point temp = MyPolygon[count];

                int deltaX = temp.X - x0;
                int deltaY = temp.Y - y0;

                temp.X = (int)((deltaX * mirrorOX[0][0] + deltaY * mirrorOX[1][0]) + x0);
                temp.Y = (int)((deltaX * mirrorOX[0][1] + deltaY * mirrorOX[1][1]) + y0);

                MyPolygon[count] = temp;
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

            for (int count = 0; count < MyPolygon.Count; ++count)
            {
                Point temp = MyPolygon[count];

                int deltaX = temp.X - x0;
                int deltaY = temp.Y - y0;

                temp.X = (int)((deltaX * mirrorOY[0][0] + deltaY * mirrorOY[1][0]) + x0);
                temp.Y = (int)((deltaX * mirrorOY[0][1] + deltaY * mirrorOY[1][1]) + y0);

                MyPolygon[count] = temp;
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

            for (int count = 0; count < MyPolygon.Count; ++count)
            {
                Point temp = MyPolygon[count];

                int deltaX = temp.X - x0;
                int deltaY = temp.Y - y0;

                temp.X = (int)((deltaX * mirrorOXY[0][0] + deltaY * mirrorOXY[1][0]) + x0);
                temp.Y = (int)((deltaX * mirrorOXY[0][1] + deltaY * mirrorOXY[1][1]) + y0);

                MyPolygon[count] = temp;
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

            for (int count = 0; count < MyPolygon.Count; ++count)
            {
                Point temp = MyPolygon[count];

                temp.X += (int)move[0];
                temp.Y -= (int)move[1];

                MyPolygon[count] = temp;
            }
        }

        private void printPoints()
        {
            user_Graphics.DrawLine(Pens.Gray, x0, 2, x0, pictureBox_2d.Height - 2);
            user_Graphics.DrawLine(Pens.Gray, 2, y0, pictureBox_2d.Width - 2, y0);
            user_Graphics.DrawString("x", axFont, axBrush, (float)(pictureBox_2d.Width - 10), (float)(y0 + 3), drawFormat);
            user_Graphics.DrawString("y", axFont, axBrush, (float)(x0 - 10), (float)(3.0), drawFormat);

            this.label1.Text = "Координаты точек:\n";

            for (int count = 0; count < MyPolygon.Count; count++)
            {
                this.label1.Text += "(" + System.Convert.ToString(MyPolygon[count].X - x0) + "; " + System.Convert.ToString(-MyPolygon[count].Y + y0) + ")\n";
            }
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
