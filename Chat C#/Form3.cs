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
    public partial class Form3 : Form
    {
        public Form3()
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
        public void conectare(string user)
        {
            string connect = "Server=127.0.0.1;port=3306;Database=phplogin;Uid = root;password = amber";
            MySqlConnection com = new MySqlConnection(connect);
            MySqlCommand Comanda = com.CreateCommand();
            Comanda.CommandText = "Select * from users where username = '" + user + "'";
            com.Open();
            MySqlDataReader row = Comanda.ExecuteReader();
            row.Read();
            label1.Text = "Name:           " + row["name"] +"\nAccount:       "+ row["username"] +"\nEmail:            "+ row["idmess"];
            string poza = "http://127.0.0.1/" + row["avatar"].ToString();
            pictureBox1.ImageLocation = poza;
            com.Close();
        }

    }
}
