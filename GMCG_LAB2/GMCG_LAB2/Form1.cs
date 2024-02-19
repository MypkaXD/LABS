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

        private class Coords
        {
            public int x { get; set; }
            public int y { get; set; }
        }

        int n = 0;

        private List<Coords> coords = new List<Coords>();

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int x, y;

            if (Int32.TryParse(this.textBox1.Text, out x) && 
                Int32.TryParse(this.textBox2.Text, out y))
            {
                ++n;
                Coords coord = new Coords();
                coord.x = x;
                coord.y = y;
                coords.Add(coord);
            }
            else
            {
                MessageBox.Show("Ввод данных произведен неверно!", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
