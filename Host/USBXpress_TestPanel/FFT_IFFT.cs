using System;

namespace USBXpress_TestPanel
{
    /// <summary>
    ///     做FFT和IFFT的类
    /// </summary>
    internal class FFT_IFFT
    {
        /// <summary>
        ///     求复数complex数组的模modul数组
        /// </summary>
        /// <param name="input">复数数组</param>
        /// <returns>模数组</returns>
        public static double[] Cmp2Mdl(Complex[] input)
        {
            ///有输入数组的长度确定输出数组的长度
            var output = new double[input.Length];

            ///对所有输入复数求模
            for (var i = 0; i < input.Length; i++)
            {
                if (input[i].Real > 0)
                {
                    output[i] = -input[i].ToModul();
                }
                else
                {
                    output[i] = input[i].ToModul();
                }
            }

            ///返回模数组
            return output;
        }

        /// <summary>
        ///     傅立叶变换或反变换，递归实现多级蝶形运算
        ///     作为反变换输出需要再除以序列的长度
        ///     ！注意：输入此类的序列长度必须是2^n
        /// </summary>
        /// <param name="input">输入实序列</param>
        /// <param name="invert">false=正变换，true=反变换</param>
        /// <returns>傅立叶变换或反变换后的序列</returns>
        public static Complex[] FFT(double[] input, bool invert)
        {
            ///由输入序列确定输出序列的长度
            var output = new Complex[input.Length];

            ///将输入的实数转为复数
            for (var i = 0; i < input.Length; i++)
            {
                output[i] = new Complex(input[i]);
            }

            ///返回FFT或IFFT后的序列
            return output = FFT(output, invert);
        }

        /// <summary>
        ///     傅立叶变换或反变换，递归实现多级蝶形运算
        ///     作为反变换输出需要再除以序列的长度
        ///     ！注意：输入此类的序列长度必须是2^n
        /// </summary>
        /// <param name="input">复数输入序列</param>
        /// <param name="invert">false=正变换，true=反变换</param>
        /// <returns>傅立叶变换或反变换后的序列</returns>
        public static Complex[] FFT(Complex[] input, bool invert)
        {
            ///输入序列只有一个元素，输出这个元素并返回
            if (input.Length == 1)
            {
                return new[] {input[0]};
            }

            ///输入序列的长度
            var length = input.Length;

            ///输入序列的长度的一半
            var half = length/2;

            ///有输入序列的长度确定输出序列的长度
            var output = new Complex[length];

            ///正变换旋转因子的基数
            var fac = -2.0*Math.PI/length;

            ///反变换旋转因子的基数是正变换的相反数
            if (invert)
            {
                fac = -fac;
            }

            ///序列中下标为偶数的点
            var evens = new Complex[half];

            for (var i = 0; i < half; i++)
            {
                evens[i] = input[2*i];
            }

            ///求偶数点FFT或IFFT的结果，递归实现多级蝶形运算
            var evenResult = FFT(evens, invert);

            ///序列中下标为奇数的点
            var odds = new Complex[half];

            for (var i = 0; i < half; i++)
            {
                odds[i] = input[2*i + 1];
            }

            ///求偶数点FFT或IFFT的结果，递归实现多级蝶形运算
            var oddResult = FFT(odds, invert);

            for (var k = 0; k < half; k++)
            {
                ///旋转因子
                var fack = fac*k;

                ///进行蝶形运算
                var oddPart = oddResult[k]*new Complex(Math.Cos(fack), Math.Sin(fack));
                output[k] = evenResult[k] + oddPart;
                output[k + half] = evenResult[k] - oddPart;
            }

            ///返回FFT或IFFT的结果
            return output;
        }
    }
}