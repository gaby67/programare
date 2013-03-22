using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MySql.Data.MySqlClient;

namespace WindowsFormsApplication2
{
    public partial class Form2 : Form
    {
        string pass, usr, datapass;
        Point lastclick;
        public string datausr;
        public Form2()
        {
            InitializeComponent();
        }

        private const int CS_DROPSHADOW = 0x00020000;
        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ClassStyle |= CS_DROPSHADOW;
                return cp;
            }
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void conectare(string pass , string usr)
        {
            string connect = "Server=127.0.0.1;port=3306;Database=phplogin;Uid = root;password = amber";
            MySqlConnection com = new MySqlConnection(connect);
            MySqlCommand Comanda = com.CreateCommand();
            Comanda.CommandText = "Select * from users where username = '" + usr + "'";
            try
            {
                com.Open();
            
            MySqlDataReader row = Comanda.ExecuteReader();
            while (row.Read())
            {
                datausr = row["username"].ToString();
                datapass = row["password"].ToString();
            }

            if (usr == datausr)
            {
                if (pass == datapass)
                {
                    MySqlConnection com2 = new MySqlConnection(connect);
                    MySqlCommand Comanda2 = com2.CreateCommand();
                    string ceasu = DateTime.Now.ToString("s");
                    ceasu = ceasu.Replace("T", " ");
                    Comanda2.CommandText = "UPDATE plus_login set status='ON',tm='" + ceasu + "' where userid='" + datausr + "'";
                    com2.Open();
                    Comanda2.ExecuteNonQuery();
                    this.DialogResult = DialogResult.OK;
                    com2.Close();
                }
                else
                {
                    label3.Text = "Parola este incorecta!!";
                }

            }
            else
            {
                label3.Text = "Acest user nu exista!!";
            }

            }
            catch (Exception)
            {
                label3.Text = "Nu se poate conecta la server!!";
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            timer1.Enabled = true;
            pictureBox1.Visible = true;
        }

        private void Form2_MouseDown(object sender, MouseEventArgs e)
        {
            lastclick = e.Location;
            
        }

        private void Form2_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                this.Left += e.X - lastclick.X;
                this.Top += e.Y - lastclick.Y;
                this.Opacity = 0.9;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void Form2_MouseLeave(object sender, EventArgs e)
        {
            this.Opacity = 0.9;
        }

        private void Form2_MouseUp(object sender, MouseEventArgs e)
        {
            this.Opacity = 1;
        }

        private void button2_MouseHover(object sender, EventArgs e)
        {
            button2.BackgroundImage = global::WindowsFormsApplication2.Properties.Resources.on;
        }

        private void button2_MouseLeave(object sender, EventArgs e)
        {
            button2.BackgroundImage = global::WindowsFormsApplication2.Properties.Resources.off;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            
       }

        private void timer1_Tick(object sender, EventArgs e)
        {

        }

        private void timer1_Tick_1(object sender, EventArgs e)
        {
                timer1.Enabled = false;
                pictureBox1.Visible = false;
                pass = textBox2.Text;
                usr = textBox1.Text;
                conectare(pass, usr);
        }

    }
}
