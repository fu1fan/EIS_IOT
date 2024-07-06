/*
 * soft_fft.h
 *
 *  Created on: 2024年6月10日
 *      Author: Whisp
 */

#ifndef USER_CODE_SOFT_FFT_H_
#define USER_CODE_SOFT_FFT_H_

#define FFT_N 2048                                        //定义傅里叶变换的点数
#define PI 3.14159265358979323846264338327950288419717  //定义圆周率值

struct compx { double real, imag; };                    //定义一个复数结构

extern struct compx CompxIO[];							//FFT输入和输出：从Compx[0]开始存放，根据大小自己定义
extern struct compx CompxRE[];
extern double SIN_TAB[];								//正弦信号表

extern void Refresh_Data(struct compx *xin, int id, double wave_data);
extern void create_sin_tab(double *sin_t);
extern void FFT(struct compx *xin);
extern void Get_Result(struct compx *xin, struct compx *xout, double sample_frequency);

#endif /* USER_CODE_SOFT_FFT_H_ */
