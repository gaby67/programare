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
    public partial class Form1 : Form
    {
        Panel[] panel = new Panel[256];
        private string a;// create a property
        int i = 0;
        DateTime data;
        Form3 f = new Form3();
        public void ConA(string v , string dat)
        {
            a = v;
            data = Convert.ToDateTime(dat);
        }

        public Form1()
        {
            InitializeComponent();
            PictureBox[] poze = new PictureBox[256];
            Label[] label = new Label[256];
            Label[] label2 = new Label[256];
            string connect = "Server=127.0.0.1;port=3306;Database=phplogin;Uid = root;password = amber";
            MySqlConnection com = new MySqlConnection(connect);
            MySqlCommand Comanda = com.CreateCommand();
            Comanda.CommandText = "Select * from users order by Id asc";
            com.Open();
            MySqlDataReader row = Comanda.ExecuteReader();
            string poza;
            while (row.Read())
            {
                label[i] = new Label();
                string str = row["username"].ToString();
                str = char.ToUpper(str[0]) + str.Substring(1);
                label[i].Text = str;
                label[i].AutoSize = true;
                label[i].Location = new Point(1,1);
                label[i].Size = new Size(35, 13);
                label[i].Visible = true;

                label2[i] = new Label();
                label2[i].AutoSize = true;
                label2[i].Location = new Point(1,25);
                label2[i].Size = new Size(35, 13);
                label2[i].Visible = true;
                label2[i].Font = new Font("Comic Sans MS", 10F, FontStyle.Bold);

                panel[i] = new Panel();
                panel[i].Location = new Point(2, 84*i);
                panel[i].Size = new Size(250,80);
                panel[i].AutoSize = false;
                panel[i].Name = "Panel" + i.ToString();
                label2[i].Visible = true;
                
                string connect2 = "Server=127.0.0.1;port=3306;Database=phplogin;Uid = root;password = amber";
                MySqlConnection com2 = new MySqlConnection(connect2);
                MySqlCommand Comanda2 = com2.CreateCommand();
                Comanda2.CommandText = "Select * from plus_login where userid ='" + row["username"].ToString() + "'";
                com2.Open();
                MySqlDataReader row2 = Comanda2.ExecuteReader();
                row2.Read();
                if (row2["status"].ToString() == "OFF")
                {
                    label[i].ForeColor = Color.DarkGray;
                    label2[i].ForeColor = Color.DarkRed;
                    label2[i].Text = "Offline";
                    label[i].Font = new Font("Comic Sans MS", 12F, FontStyle.Italic);

                }
                else
                {
                    label[i].ForeColor = Color.Black;
                    label2[i].ForeColor = Color.DarkGreen;
                    label2[i].Text = "Online";
                    label[i].Font = new Font("Comic Sans MS", 12F, FontStyle.Regular);
                }
                com2.Close();
                
                poza = "http://127.0.0.1/" + row["avatar"].ToString();
                poze[i] = new PictureBox();
                poze[i].Height = 80;
                poze[i].Width =  80;
                poze[i].Location = new Point(170, 0);
                poze[i].ImageLocation = poza;
                poze[i].Visible = true;
                poze[i].SizeMode = PictureBoxSizeMode.StretchImage;
                panel[i].Controls.Add(poze[i]);
                panel[i].Controls.Add(label[i]);
                panel[i].Controls.Add(label2[i]);
                panel1.Controls.Add(panel[i]);
                Panel cacat = panel[i];
                string user = row["username"].ToString();
                panel[i].MouseEnter += delegate(object sender, EventArgs e) 
                { panel_Enter(cacat,user); };
                panel[i].MouseLeave += delegate(object sender, EventArgs e)
                { panel_Leave(cacat); };

                panel[i].MouseDoubleClick += delegate(object sender, MouseEventArgs e)
                { panel_DClick(a, user); };
                i++;
            }
        }

        private void panel_Enter(Panel nume, string user)
        {
            nume.BackColor = Color.LightGray;
            timer1.Enabled = true;
            this.timer1.Tick += delegate(object sender, EventArgs e)
            { timer1_Tick(nume,user); };
        }

        private void panel_Leave(Panel nume)
        {
            nume.BackColor = Color.Transparent;
            f.Hide();
            timer1.Enabled = false;
       }

        private void panel_DClick(string user, string receiver)
        {
            bool l = false;
            foreach (Form OpenForm in Application.OpenForms)
            {
                if (OpenForm.Text == receiver)
                {
                    l = true;
                    Form4 b = (Form4)OpenForm;
                    b.Focus();
                }
            }
            if (l == false)
            {

                Form4 f2 = new Form4();
                f2.conectare(user, receiver , "");
                f2.datatext = "";
                f2.Show();

            }

            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            logout();
        }

        private void logout()
        {
            Program pr = new Program();
            pr.dinnou(this);
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void tableLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            
        }

        private void timer1_Tick(Panel nume , string user)
        {
            timer1.Enabled = false;
            f.Show();
            this.TopMost = false;
            f.Left = this.Left - 300;
            f.Top = nume.Top + this.Top + 26;
            f.conectare(user);
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            string connect = "Server=127.0.0.1;port=3306;Database=phplogin;Uid = root;password = amber";
            MySqlConnection com = new MySqlConnection(connect);
            MySqlCommand Comanda = com.CreateCommand();
            Comanda.CommandText = "Select * from comments where user ='" + a + "' order by id ASC";
            com.Open();
            bool k = false;
            bool l = false;
            MySqlDataReader row = Comanda.ExecuteReader();
            while (row.Read())
            {
                sender = row["sender"].ToString();
                DateTime dat = Convert.ToDateTime(row["date"].ToString());
                if (DateTime.Compare(dat, data) > 0)
                {
                        foreach (Form OpenForm in Application.OpenForms)
                        {
                            if (OpenForm.Text == row["sender"].ToString())
                            {
                                l = true;
                                Form4 b = (Form4)OpenForm;
                                b.timer1.Enabled = true;
                                b.datatext = "aa";
                            }
                        }
                        if (l == false)
                        {

                            Form4 f3 = new Form4();
                            f3.conectare(a, row["sender"].ToString(), row["text"].ToString());
                            f3.Show();

                        }
                        k = true;
                        
                }
            }
            com.Close();
            if (k == true) 
            { 
                data = DateTime.Now;
                
            }
        }
       
    }
}
