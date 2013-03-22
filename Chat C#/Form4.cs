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
    public partial class Form4 : Form
    {
        string usr , receiv;
        public string datatext, text;
        public Form4()
        {
            InitializeComponent();
            timer1.Tick += delegate(object sender, EventArgs e)
            { timer1_Tick(); };
        }
    
    public void conectare(string user , string receiver , string textus)
   {
       if (textus != null)
           textBox1.Text = textus;
       pictureBox2.ImageLocation = this.avatar(user);
       pictureBox1.ImageLocation = this.avatar(receiver);
       this.Text = receiver;
       receiv = receiver;
       usr = user;
       button1.Click += delegate(object sender, EventArgs e)
        { button1_Click(receiver , user ); };
   }

    public string avatar(string user)
    {
        string connect = "Server=127.0.0.1;port=3306;Database=phplogin;Uid = root;password = amber";
        MySqlConnection com = new MySqlConnection(connect);
        MySqlCommand Comanda = com.CreateCommand();
        Comanda.CommandText = "Select * from users where username = '" + user + "'";
        com.Open();
        MySqlDataReader row = Comanda.ExecuteReader();
        row.Read();
        string av = row["avatar"].ToString();
        com.Close();
        return "http://127.0.0.1/"+ av;
    }

    private void button1_Click(string user , string sender)
    {
        object j = 6;
        string connect = "Server=127.0.0.1;port=3306;Database=phplogin;Uid = root;password = amber";
        MySqlConnection com = new MySqlConnection(connect);
        MySqlCommand Comanda = com.CreateCommand();
        Comanda.CommandText = "Select Count(*) from comments where (user ='"+user+"') AND (sender = '"+sender+"') Limit 0 , 1";
        com.Open();
        j = Comanda.ExecuteScalar();
        com.Close();
        Comanda.CommandText = "Select * from comments where (user ='" + user + "') AND (sender = '" + sender + "') Limit 0 , 1";
        com.Open();
        MySqlDataReader row = Comanda.ExecuteReader();
        row.Read();
        if (j.ToString() != "0")
        {
            if (datatext == "")
            {
                datatext = usr + ": " + textBox2.Text + "\r\n";
                text = "";
            }
            else
            {
                datatext = usr + ": " + textBox2.Text + "\r\n";
                text = row["text"].ToString();
            }
            com.Close();
            com.Open();
            Comanda.CommandText = "Update comments set text = '" + text + datatext +"',date = '" + DateTime.Now.ToString() + "' where (user ='" + user + "') AND (sender = '" + sender + "')";
            Comanda.ExecuteNonQuery();
            Comanda.CommandText = "Update comments set text = '" + text + datatext +"',date = '" + DateTime.Now.ToString() + "' where (user ='" + sender + "') AND (sender = '" + user + "')";
            Comanda.ExecuteNonQuery();
            com.Close();
        }
        else
        {
            textBox1.Text = usr + ": " + textBox2.Text + "\r\n";
            com.Close();
            com.Open();
            Comanda.CommandText = "Insert into comments values('' , '" + user + "','" + sender + "','" + textBox1.Text + "' , '"+DateTime.Now.ToString()+"')";
            Comanda.ExecuteNonQuery();
            Comanda.CommandText = "Insert into comments values('' , '" + sender + "','" + user + "','" + textBox1.Text + "' , '" + DateTime.Now.ToString() + "')";
            Comanda.ExecuteNonQuery();
            com.Close();
        }
        textBox2.Text = "";
    }

    private void textBox2_KeyDown(object sender, KeyEventArgs e)
    {
        if (textBox2.Text == "\r\n")
        {
            textBox2.Text = "";
        
        }
        if (e.KeyCode == Keys.Return)
        {
            button1.PerformClick();
            timer1.Enabled = false;
            timer1.Enabled = true;
            textBox2.Text = "";
        }
        if (e.KeyCode == Keys.Escape)
        {
            this.Close();
        }
    }

    private void timer1_Tick()
    {
        string connect = "Server=127.0.0.1;port=3306;Database=phplogin;Uid = root;password = amber";
        MySqlConnection com = new MySqlConnection(connect);
        MySqlCommand Comanda = com.CreateCommand();
        Comanda.CommandText = "Select  * from comments where (user ='" + usr + "') AND (sender = '" + receiv + "') Limit 0 , 1";
        com.Open();
        MySqlDataReader row = Comanda.ExecuteReader();
        row.Read();
        textBox1.Text = row["text"].ToString();
        com.Close();
    }

    private void Form4_Resize(object sender, EventArgs e)
    {
        textBox1.Width = this.Width - 155;
        textBox1.Height = this.Height - 152;
        textBox2.Width = this.Width - 155;
        button1.Left = textBox2.Width - 70;
    }

   }


}
