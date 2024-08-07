/****************************************************************************
*
* Copyright (c) 2023 STMicroelectronics - All Rights Reserved
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms SLA0098 at www.st.com.
*
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
* EVALUATION ONLY - NOT FOR USE IN PRODUCTION
*****************************************************************************/

#include <test_env.h>
#include "screen_uart.h"

#include "driver_headfile.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include "math.h"

#define EIS_ADDRESS 0x7F0

unsigned char uart_buffer[4096];
uint16_t uart_length = 0;
float temp1[256];
float temp2[256];

void buffer_clear(void) {
	uart_length = 0;
}

void buffer_addf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	uart_length += vsnprintf((char*) uart_buffer + uart_length,
			1024 - uart_length, fmt, args);
	va_end(args);
}

void buffer_printf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	uart_length = vsnprintf((char*) uart_buffer, 1024, fmt, args);
	va_end(args);
}

// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭所有打开的文件
// 第二步 project->clean  等待下方进度条走完


// *************************** 例程硬件连接说明 ***************************
//      使用 Type-C 连接开发板供电 再使用 StellarLINK 连接调试接口即可
//      使用 Type-C 连接开发板供电 再使用 StellarLINK 连接调试接口即可
//      使用 Type-C 连接开发板供电 再使用 StellarLINK 连接调试接口即可


// *************************** 例程测试说明 ***************************
// 1.这是一个空工程 用于自行编写程序


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     主函数
// 参数说明     void            无参数
// 返回参数     void            无返回值
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
int main (void)
{
    osal_sys_unlock();                                                          // 启用所有中断

    test_env_init((TestInit_t)(                                                 // 这个接口用于开启 SDK 的默认设置
        TEST_INIT_CLOCK    |                                                    // 默认时钟配置 应用 StellarStudio 的 clock_cfg.Sr5e1_clk 文件
        TEST_INIT_GPIO     |                                                    // 这里其实只是开启了 GPIO 的时钟 而不是配置 GPIO 设置
        TEST_INIT_BOARD    |                                                    // 默认引脚配置 应用 StellarStudio 的 Config.gpio 文件
        TEST_INIT_IRQ      |                                                    // 启用中断
        TEST_INIT_OSAL));                                                       // 启用 OSAL 组件 主要是初始化 Systick 的延时与计数

    debug_uart_init();                                                          // 以默认配置初始化调试输出串口
    // system_rgb_pwm_init();                                                      // 初始化 RGB 灯配置
    can_module_init();
    screen_uart_init();

    uint32_t length = 0;
    uint32_t enable_auto = 1;
    uint8_t is_online = 1;
    uint64_t tick = osal_tk2ms(osal_get_tick())+60000;

    uint8_t enable_monit = 0;
    uint8_t monit_ch = 0;
    uint64_t tick2 = osal_tk2ms(osal_get_tick())+60000;

    float mmax, mmin, mmean, mlast, mrate;
    uint64_t mcount, mltime;

    printf("Running.\r\n");
    char *p, *q;
    for( ; ; )                                                                  // 主循环的代码就是控制各个 LED 呼吸亮灭
    {
        osal_delay_millisec(2U);
        length = screen_uart_read_buffer((uint8 *)uart_buffer, 1024);
        if(length){
        	p = strstr((const char *)uart_buffer, "-prints:");
        	if(p==NULL){
        		continue;
        	}

        	if(strstr((const char *)uart_buffer, "online")!=NULL)
        	{
                is_online = 1;
        		can_receive_buffer[0] = 0x05;
        		can_receive_buffer[1] = 0x00;
        		can_send_frame(EIS_ADDRESS, can_receive_buffer);
        	}
        	else if(strstr((const char *)uart_buffer, "offline")!=NULL)
			{
        		tick = osal_tk2ms(osal_get_tick())+60000;
                is_online = 0;
				can_receive_buffer[0] = 0x05;
				can_receive_buffer[1] = 0x01;
				can_send_frame(EIS_ADDRESS, can_receive_buffer);
			}
            else if(strstr((const char *)uart_buffer, "auto:0")!=NULL)
            {
                enable_auto = 0;
            }
            else if(strstr((const char *)uart_buffer, "auto:1")!=NULL)
            {
                enable_auto = 1;
                tick = 0;
            }
            else if((p = strstr((const char *)uart_buffer, "single:"))!=NULL)
            {
            	p += 7;
            	q = strstr(p, ",");
            	int ch = atoi(p);
            	p = q + 1;
            	q = strstr(p, ",");
            	uint32_t freq = (uint32_t)atoi(p);
            	// 开始单点测量
            	can_receive_buffer[0] = 0x13;
            	can_receive_buffer[1] = ch;
            	can_receive_buffer[2] = 1;
            	// 频率写入4-7
            	can_receive_buffer[4] = (freq >> 24) & 0xFF;
            	can_receive_buffer[5] = (freq >> 16) & 0xFF;
            	can_receive_buffer[6] = (freq >> 8) & 0xFF;
            	can_receive_buffer[7] = freq & 0xFF;
            	can_rx_flag = 0;
            	can_send_frame(EIS_ADDRESS, can_receive_buffer);
            	while(can_rx_flag == 0);
            	can_rx_flag = 0;
				if (can_receive_buffer[0] == 0x23
						&& can_receive_buffer[2] == 0x01) {
					while(can_rx_flag == 0);
					float real, imag;
					can_rx_flag = 0;
					memcpy(&real, can_receive_buffer, 4);
					memcpy(&imag, can_receive_buffer + 4, 4);
					float phase = atan2(imag, real);
					float mag = sqrt(real * real + imag * imag);
					buffer_clear();
					buffer_addf("single.txt.str=\"");
					buffer_addf("Channel: %d\n", ch);
					buffer_addf("Magnitude: %.2f mΩ\n", mag);
					buffer_addf("Phase: %.2f rad\n", phase);
					buffer_addf("Real: %.2f mΩ\n", real);
					buffer_addf("Imag: %.2f mΩ\n", imag);
					buffer_addf("\";\r\n");
					screen_uart_write_buffer(uart_buffer, uart_length);
					buffer_printf("bx.txt.str=\"开始测量\";\r\n", can_receive_buffer[3]);
					screen_uart_write_buffer(uart_buffer, uart_length);
				}
				else
				{
					buffer_clear();
					buffer_printf("single.txt.str=\"Waiting @%#02x.\";\r\n", can_receive_buffer[3]);
					screen_uart_write_buffer(uart_buffer, uart_length);
					buffer_printf("bx.txt.str=\"开始测量\";\r\n", can_receive_buffer[3]);
					screen_uart_write_buffer(uart_buffer, uart_length);
				}
				// 结束单点测量
				can_receive_buffer[0] = 0x12;
				can_receive_buffer[1] = 0x00;
				can_send_frame(EIS_ADDRESS, can_receive_buffer);
            }
            else if(strstr((const char *)uart_buffer, "monit:stop")!=NULL)
            {
            	enable_monit = 0;
            	// 结束单点测量
				can_receive_buffer[0] = 0x12;
				can_receive_buffer[1] = 0x00;
				can_send_frame(EIS_ADDRESS, can_receive_buffer);
            }
            else if((p = strstr((const char *)uart_buffer, "monit:"))!=NULL)
            {
            	p+=6;
            	q = strstr(p, ";");
            	*q = '\0';
            	monit_ch = atoi(p);
            	enable_monit = 1;
            	tick2 = 0;

            	mmax = 0;
            	mmin = 9999;
            	mmean = 0;
            	mlast = 0;

            	mcount = 0;
            	mltime = 0;
            }
        }
        if(osal_tk2ms(osal_get_tick())+2000-tick2>2000 && enable_monit && !is_online)
        {
        	tick2 = osal_tk2ms(osal_get_tick())+2000;
        	// 进行单点扫描
        	can_receive_buffer[0] = 0x13;
        	can_receive_buffer[1] = monit_ch;
        	can_receive_buffer[2] = 1;
        	// 频率为70Hz
        	can_receive_buffer[4] = 0x00;
        	can_receive_buffer[5] = 0x00;
        	can_receive_buffer[6] = 0x00;
        	can_receive_buffer[7] = 0x46;
        	can_rx_flag = 0;
        	can_send_frame(EIS_ADDRESS, can_receive_buffer);
        	while(can_rx_flag == 0);
        	can_rx_flag = 0;
        	if (can_receive_buffer[0] == 0x23 &&
        			can_receive_buffer[2] == 0x01) {
        		while(can_rx_flag == 0);
        		float real, imag;
        		can_rx_flag = 0;
        		memcpy(&real, can_receive_buffer, 4);
        		memcpy(&imag, can_receive_buffer + 4, 4);
        		float phase = atan2(imag, real);
        		float mag = sqrt(real * real + imag * imag);
        		// 将mag从0~50的浮点映射为0~255的整数
        		uint8_t val = (uint8_t)(mag/50.0*255);
        		buffer_printf("cadd(c0,0,%d);\r\n", val);
        		screen_uart_write_buffer(uart_buffer, uart_length);



        		if (mmax < mag) {
					mmax = mag;
				}
				if (mmin > mag) {
					mmin = mag;
				}
				mmean = (mmean * mcount + mag) / (mcount + 1);
				mcount++;

				mrate = (mag - mlast) / (osal_tk2ms(osal_get_tick()) - mltime) * 1000;
				mltime = osal_tk2ms(osal_get_tick());
				mlast = mag;

				buffer_clear();
				buffer_addf("manalyse.txt.str=\"");
				buffer_addf("最大值: %.2f mΩ\n", mmax);
				buffer_addf("最小值: %.2f mΩ\n", mmin);
				buffer_addf("平均值: %.2f mΩ\n", mmean);
				buffer_addf("变化率: \n%.2f mΩ/ms\n", mrate);
				buffer_addf("\";\r\n");
				screen_uart_write_buffer(uart_buffer, uart_length);

				buffer_clear();
				buffer_addf("mstatus.txt.str=\"最近测量: ");
//				buffer_addf("real: %.2f mΩ; ", real);
//				buffer_addf("imag: %.2f mΩ; ", imag);
				buffer_addf("mag: %.2f mΩ; ", mag);
				buffer_addf("phase: %.2f rad", phase);
				buffer_addf("\";\r\n");
				screen_uart_write_buffer(uart_buffer, uart_length);

			} else {
				buffer_printf("mstatus.txt.str=\"Waiting @%#02x.\";\r\n",
						can_receive_buffer[3]);
				screen_uart_write_buffer(uart_buffer, uart_length);
        	}
        }
        if(osal_tk2ms(osal_get_tick())+60000-tick>60000 && enable_auto && !is_online)
        {
            tick = osal_tk2ms(osal_get_tick())+60000;
            can_receive_buffer[0] = 0x15;
            can_receive_buffer[1] = 0x00;
            can_receive_buffer[2] = 0x00;
            can_receive_buffer[3] = 0x00;
            can_receive_buffer[4] = 0x00;
            can_receive_buffer[5] = 0x00;
            can_receive_buffer[6] = 0x00;
            can_receive_buffer[7] = 0x64;
            can_rx_flag = 0;
            can_send_frame(EIS_ADDRESS, can_receive_buffer);
            while(can_rx_flag == 0);
            can_rx_flag = 0;
			if (can_receive_buffer[0] == 0x25
					&& can_receive_buffer[2] == 0x01) {
				uint8_t length = can_receive_buffer[7];
				if(length){
                    uint8_t power_flag = 1;
					for (uint8_t i = 0; i < length; i++) {
						while(can_rx_flag == 0);
						can_rx_flag = 0;
						// 前四位存入temp1
						memcpy(temp1+i, can_receive_buffer, 4);
						// 后四位存入temp2
						memcpy(temp2+i, can_receive_buffer + 4, 4);
                        if(power_flag){
                            if(temp1[i] < 3.0 || temp1[i] > 5 || temp2[i] == 0){
                                power_flag = 0;
                            }
                        }
					}
					buffer_clear();
					buffer_addf("overview.txt.str=\"");
					for (uint8_t i = 0; i < length; i++) {
						buffer_addf("Battery %d: %.2f V, %.2f mΩ\n", i, temp1[i], temp2[i]);
					}
					buffer_addf("\";\r\n");
					screen_uart_write_buffer(uart_buffer, uart_length);

                    if(!power_flag)
                    {
                        buffer_printf("beep(1000, 1500);\r\n");
                        screen_uart_write_buffer(uart_buffer, uart_length);
                    }
				}
				else
				{
					buffer_printf("overview.txt.str=\"No battery\";\r\n", can_receive_buffer[3]);
					screen_uart_write_buffer(uart_buffer, uart_length);
				}
			}
			else
			{
				buffer_printf("overview.txt.str=\"Waiting @%#02x.\";\r\n", can_receive_buffer[3]);
				screen_uart_write_buffer(uart_buffer, uart_length);
			}
        }
    }
}
