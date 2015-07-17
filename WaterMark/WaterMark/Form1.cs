using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Windows.Forms;
using System.Drawing.Imaging;

namespace WaterMark
{
    public partial class Form1 : Form
    {
        private Bitmap original, watermark;
        private bool mid = false, mul = false, stret = false;
        string originalName, watermarkName;
        public Form1()
        {
            InitializeComponent();
        }
        private void helpToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("1.Load image file\n2.Load watermark file\n3.Press button\n", "Help");
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("This program was created for the summer practice.\nAuthor: Alitov Vladimir(PLaG)\nGroup: PS-21\n", "About");
        }

        private void openFileToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog1 = new OpenFileDialog();
            openFileDialog1.Filter = "Image files|*.bmp";
            if (openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                originalName = openFileDialog1.FileName;
                original = new Bitmap(originalName);
                if (watermark != null)
                {
                    if (mid)
                    {
                        MiddleProc();
                        mid = false;
                    }
                    else if (mul)
                    {
                        MultProc();
                        mul = false;
                    }
                    else if (stret)
                    {
                        StretProc();
                        stret = false;
                    }
                }
                else
                {
                    pictureBox1.Image = original;
                }
            }
        }

        private void openImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog1 = new OpenFileDialog();
            openFileDialog1.Filter = "Image files (*.bmp)|*.bmp|All files (*.*)|*.*";
            if (openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                watermarkName = openFileDialog1.FileName;
                watermark = new Bitmap(watermarkName);
                if (original != null)
                {
                    if (mid)
                    {
                        MiddleProc();
                        mid = false;
                    }
                    else if (mul)
                    {
                        MultProc();
                        mul = false;
                    }
                    else if (stret)
                    {
                        StretProc();
                        stret = false;
                    }
                }
                else
                {
                    pictureBox1.Image = watermark;
                }
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image == null)
            {
                MessageBox.Show("No picture!");
                return;
            }
            Bitmap bmpSave = new Bitmap(pictureBox1.Image);
            if (bmpSave != null)
            {
                bmpSave.Save(openFileDialog1.FileName, ImageFormat.Bmp);
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image == null)
            {
                MessageBox.Show("No picture!");
                return;
            }
            Bitmap bmpSave = new Bitmap(pictureBox1.Image);
            if (bmpSave != null)
            {
                SaveFileDialog sfd = new SaveFileDialog();
                sfd.DefaultExt = "bmp";
                sfd.Filter = "Image files (*.bmp)|*.bmp|All files (*.*)|*.*";
                if (sfd.ShowDialog() == DialogResult.OK)
                    bmpSave.Save(sfd.FileName, ImageFormat.Bmp);
            }
        }    
        
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        static float SoftLight(int a, int b)
        {
            float newA = a / 255.0f, newB = b / 255.0f;
            return newB < 0.5f ? Math.Min(1, (0.5f - newB) * newA + 2 * newB * newA + newA * newA * 
                (1 - 2 * newB)) : 2 * newA * (1 - newB) + Convert.ToSingle((Math.Sqrt(newA))) * (2 * newB - 1); 
        }

        private void MiddleProc()
        {
            if (original == null && mid)
            {
                MessageBox.Show("No picture!");
                return;
            }
            if (watermark == null && mid)
            {
                MessageBox.Show("No watermark!");
                return;
            }
            mid = true;
            mul = false;
            stret = false;
            if (watermark != null && original != null)
            {
                int oldWidth = watermark.Width, oldHeight = watermark.Height;
                if (watermark.Width > original.Width && watermark.Height > original.Height)
                {
                    watermark = new Bitmap(Image.FromFile(watermarkName), original.Width, original.Height);
                }
                Bitmap newBitmap = new Bitmap(original.Width, original.Height);
                int centerX = Math.Max(0, (original.Width - watermark.Width) / 2);
                int centerY = Math.Max(0, (original.Height - watermark.Height) / 2);
                float kx = 1.0f * original.Width / watermark.Width,
                    ky = 1.0f * original.Height / watermark.Height;

                for (int i = 0; i < original.Width; ++i)
                {
                    for (int j = 0; j < original.Height; ++j)
                    {
                        var inpPixel = original.GetPixel(i, j);
                        if (i >= centerX && j >= centerY && i < centerX + watermark.Width && j < centerY + watermark.Height)
                        {
                            var watermarkPixel = watermark.GetPixel(i - centerX, j - centerY);
                            float bright = 1 - watermarkPixel.A / 255.0f;
                            Color newColor = Color.FromArgb((int)(255 * SoftLight(inpPixel.R, (int)(watermarkPixel.R + (inpPixel.R - watermarkPixel.R) * bright))),
                                (int)(255 * SoftLight(inpPixel.G, (int)(watermarkPixel.G + (inpPixel.G - watermarkPixel.G) * bright))),
                                (int)(255 * SoftLight(inpPixel.B, (int)(watermarkPixel.B + (inpPixel.B - watermarkPixel.B) * bright))));

                            newBitmap.SetPixel(i, j, newColor);
                        }
                        else
                        {
                            newBitmap.SetPixel(i, j, inpPixel);
                        }
                    }
                }
                watermark = new Bitmap(Image.FromFile(watermarkName), oldWidth, oldHeight);
                pictureBox1.Image = newBitmap;
            }
        }

        private void MultProc()
        { 
            if (original == null && mul)
            {
                MessageBox.Show("No picture!");
                return;
            }
            if (watermark == null && mul)
            {
                MessageBox.Show("No watermark!");
                return;
            }
            mul = true;
            mid = false;
            stret = false;
            if (watermark != null && original != null)
            {
                int oldWidth = watermark.Width, oldHeight = watermark.Height;
                if (watermark.Width > original.Width && watermark.Height > original.Height)
                {
                    watermark = new Bitmap(Image.FromFile(watermarkName), original.Width / 2, original.Height / 2);
                }

                Bitmap newBitmap = new Bitmap(original.Width, original.Height);
                int centerX = Math.Max(0, (original.Width - watermark.Width) / 2);
                int centerY = Math.Max(0, (original.Height - watermark.Height) / 2);
                float kx = 1.0f * original.Width / watermark.Width,
                    ky = 1.0f * original.Height / watermark.Height;

                for (int i = 0; i < original.Width; ++i)
                {
                    for (int j = 0; j < original.Height; ++j)
                    {
                        var inpPixel = original.GetPixel(i, j);
                        var watermarkPixel = watermark.GetPixel(i % watermark.Width, j % watermark.Height);
                        float bright = 1 - watermarkPixel.A / 255.0f;
                        Color newColor = Color.FromArgb((int)(255 * SoftLight(inpPixel.R, (int)(watermarkPixel.R + (inpPixel.R - watermarkPixel.R) * bright))),
                            (int)(255 * SoftLight(inpPixel.G, (int)(watermarkPixel.G + (inpPixel.G - watermarkPixel.G) * bright))),
                            (int)(255 * SoftLight(inpPixel.B, (int)(watermarkPixel.B + (inpPixel.B - watermarkPixel.B) * bright))));

                        newBitmap.SetPixel(i, j, newColor);
                    }
                }
                pictureBox1.Image = newBitmap;
                watermark = new Bitmap(Image.FromFile(watermarkName), oldWidth, oldHeight);
            }
        }

        private void StretProc()
        {
            
            if (original == null && stret)
            {
                MessageBox.Show("No picture!");
                return;
            }
            if (watermark == null && stret)
            {
                MessageBox.Show("No watermark!");
                return;
            }
            stret = true;
            mul = false;
            mid = false;
            if (watermark != null && original != null)
            {
                Bitmap newBitmap = new Bitmap(original.Width, original.Height);
                int centerX = Math.Max(0, (original.Width - watermark.Width) / 2);
                int centerY = Math.Max(0, (original.Height - watermark.Height) / 2);
                float kx = 1.0f * original.Width / watermark.Width, ky = 1.0f * original.Height / watermark.Height;

                for (int i = 0; i < original.Width; ++i)
                {
                    for (int j = 0; j < original.Height; ++j)
                    {
                        var inpPixel = original.GetPixel(i, j);
                        var watermarkPixel = watermark.GetPixel((int)(i / kx), (int)(j / ky));
                        float bright = 1 - watermarkPixel.A / 255.0f;
                        Color newColor = Color.FromArgb((int)(255 * SoftLight(inpPixel.R, (int)(watermarkPixel.R + (inpPixel.R - watermarkPixel.R) * bright))),
                            (int)(255 * SoftLight(inpPixel.G, (int)(watermarkPixel.G + (inpPixel.G - watermarkPixel.G) * bright))),
                            (int)(255 * SoftLight(inpPixel.B, (int)(watermarkPixel.B + (inpPixel.B - watermarkPixel.B) * bright))));

                        newBitmap.SetPixel(i, j, newColor);
                    }
                }
                pictureBox1.Image = newBitmap;
            }
        }

        private void Middle_CheckedChanged(object sender, EventArgs e)
        {
            MiddleProc();
        }

        private void Multiply_CheckedChanged(object sender, EventArgs e)
        {
            MultProc();
        }

        private void Stretch_CheckedChanged(object sender, EventArgs e)
        {
            StretProc();
        }

    }
}

