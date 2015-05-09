using System;

namespace USBXpress_TestPanel
{
    /// <summary>
    ///     复数类
    /// </summary>
    internal class Complex
    {
        /// <summary>
        ///     默认构造函数
        /// </summary>
        public Complex()
            : this(0, 0)
        {
        }

        /// <summary>
        ///     只有实部的构造函数
        /// </summary>
        /// <param name="real">实部</param>
        public Complex(double real)
            : this(real, 0)
        {
        }

        /// <summary>
        ///     由实部和虚部构造
        /// </summary>
        /// <param name="real">实部</param>
        /// <param name="image">虚部</param>
        public Complex(double real, double image)
        {
            this.Real = real;
            this.Image = image;
        }

        /// <summary>
        ///     复数的实部
        /// </summary>
        public double Real { get; set; }

        /// <summary>
        ///     复数的虚部
        /// </summary>
        public double Image { get; set; }

        /// 重载加法
        public static Complex operator +(Complex c1, Complex c2)
        {
            return new Complex(c1.Real + c2.Real, c1.Image + c2.Image);
        }

        /// 重载减法
        public static Complex operator -(Complex c1, Complex c2)
        {
            return new Complex(c1.Real - c2.Real, c1.Image - c2.Image);
        }

        /// 重载乘法
        public static Complex operator *(Complex c1, Complex c2)
        {
            return new Complex(c1.Real*c2.Real - c1.Image*c2.Image, c1.Image*c2.Real + c1.Real*c2.Image);
        }

        /// <summary>
        ///     求复数的模
        /// </summary>
        /// <returns>模</returns>
        public double ToModul()
        {
            return Math.Sqrt(Real*Real + Image*Image);
        }

        /// <summary>
        ///     重载ToString方法
        /// </summary>
        /// <returns>打印字符串</returns>
        public override string ToString()
        {
            if (Real == 0 && Image == 0)
            {
                return string.Format("{0}", 0);
            }
            if (Real == 0 && (Image != 1 && Image != -1))
            {
                return string.Format("{0} i", Image);
            }
            if (Image == 0)
            {
                return string.Format("{0}", Real);
            }
            if (Image == 1)
            {
                return string.Format("i");
            }
            if (Image == -1)
            {
                return string.Format("- i");
            }
            if (Image < 0)
            {
                return string.Format("{0} - {1} i", Real, -Image);
            }
            return string.Format("{0} + {1} i", Real, Image);
        }
    }
}