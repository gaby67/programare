using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace WindowsFormsApplication2
{
    public class Program
    {
            /// <summary>
            /// The main entry point for the application.
            /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            //Application.SetCompatibleTextRenderingDefault(false);
            Form2 f2 = new Form2();
            if (f2.ShowDialog() == DialogResult.OK)
            {
                Form1 f = new Form1();
                DateTime data = DateTime.Now;
                f.ConA(f2.datausr, data.ToString());
                //Application.Run(f);
                f.ShowDialog();
                f2.DialogResult = DialogResult.None;
            
            }
        }
        public void dinnou(Form1 f)
        {
            f.Visible = false;
            f.Close();
            Main();
        }
    }
}
