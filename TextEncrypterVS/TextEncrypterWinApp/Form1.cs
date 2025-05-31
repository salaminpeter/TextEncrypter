using System.Windows.Forms;
using TextEncrypterCLR;


namespace TextEncrypterWinApp
{
    public partial class Form1 : Form
    {
        TextEncrypterCLR.CTextEncrypterWrapper m_TextEncrypterWrapper;
        public Form1()
        {
            m_TextEncrypterWrapper = new CTextEncrypterWrapper();
            InitializeComponent();
            InitControls();
        }
    }
}
