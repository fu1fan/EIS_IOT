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

#include "driver_headfile.h"

#include "AD9959.h"
#include "ExpansionBoard.h"
#include "screen_uart.h"
#include "math.h"

// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭所有打开的文件
// 第二步 project->clean  等待下方进度条走完


// *************************** 例程硬件连接说明 ***************************
//      使用 Type-C 连接开发板供电 再使用 StellarLINK 连接调试接口即可
//      使用 Type-C 连接开发板供电 再使用 StellarLINK 连接调试接口即可
//      使用 Type-C 连接开发板供电 再使用 StellarLINK 连接调试接口即可


// *************************** 例程测试说明 ***************************
// 1.正常连接供电后 编译本示例程序 然后按照调试说明手册步骤进行烧录调试
// 
// 2.同时使用逐飞助手或其他串口助手打开 StellarLINK 的 COM 虚拟串口 波特率设置为 460800
// 
// 3.运行本示例程序 可以在逐飞助手或其他串口助手看到串口传输的消息：
//      CAN test running.
// 
// 4.同时可以看到 RGB 的绿色 LED 呈呼吸灯闪烁
// 
// 5.使用 USB 转 CAN 模块或者其他 ZFEVB-SR5E1E3 开发板烧录本测试例程后
//  连接 CAN 总线接线端子 CAN 通信速率设置为 1Mbps 按下开发板上 C7 按键后
//  ZFEVB-SR5E1E3 开发板的 CAN 模块会发送一组数据 ID 为 0x7F0
//  如果接收到回传的数据则会通过串口输出
//      ID:0x7F0, data:.
//      0x**, 0x**, 0x**, 0x**,
//      0x**, 0x**, 0x**, 0x**,
// 
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

uint32_t freq_cur = 0;
uint16_t channel_cur = 0;
uint8_t buffer[SCREEN_UART_BUFFER_SIZE];

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

    uint16  run_led_duty    = 0;
    // uint16  send_led_duty   = 0;
    uint16  key_count_ms    = 0;
    float real = 0;
    float imag = 0;
    uint32_t len = 0;
    uint32_t temp;

    gpio_write_pin(PIN_GPIO0_9, 1);
    gpio_write_pin(PIN_GPIO3_9, 1);

    debug_uart_init();                                                          // 以默认配置初始化调试输出串口
    system_rgb_pwm_init();                                                      // 初始化 RGB 灯配置
    can_module_init();
    screen_uart_init();
    printf("CAN test running.\r\n");

	AD9959_Init();								//初始化控制AD9959需要用到的IO口,及寄存器
	AD9959_Set_Phase(CH0, 0);			//设置通道0相位控制值0(0度)，范围0~16383
	AD9959_Set_Phase(CH1, 0);	//设置通道1相位控制值4096(90度)，范围0~16383
	AD9959_Set_Phase(CH2, 0);	//设置通道2相位控制值8192(180度)，范围0~16383
	AD9959_Set_Phase(CH3, 0);	//设置通道3相位控制值12288(270度)，范围0~16383

	screen_uart_printf("\xff\xff\xffrest\xff\xff\xff");

	//EB_Select(0, 0);
	uint8_t eb_count = EB_Init();
	printf("eb_count:%d\r\n", eb_count);

	//	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！
//
//	osal_delay_millisec(10000);
//	AD9959_Set_Fre(CH1, 500);	//设置通道1频率100000 Hz
//	IO_Update();	//AD9959更新数据,调用此函数后，上述操作生效！！！！

	screen_uart_read_buffer(buffer, SCREEN_UART_BUFFER_SIZE); // 清空缓冲区

    for ( ; ; )
    {
        osal_delay_millisec(2U);
        run_led_duty = (300 > run_led_duty) ? (run_led_duty + 1) : (0);
        system_rgb_control(SYSTEM_LED_GREEN, run_led_duty/3);
        system_rgb_control(SYSTEM_LED_RED, run_led_duty/3);
        system_rgb_control(SYSTEM_LED_BLUE, run_led_duty/3);
        if(can_rx_flag)
        {
        	can_rx_flag = 0;
            printf("ID:0x%x, data:.\r\n", can_remote_id);
            printf("0x%x,0x%x,0x%x,0x%x,\r\n",
                can_receive_buffer[0], can_receive_buffer[1],
                can_receive_buffer[2], can_receive_buffer[3]);
            printf("0x%x,0x%x,0x%x,0x%x,\r\n",
                can_receive_buffer[4], can_receive_buffer[5],
                can_receive_buffer[6], can_receive_buffer[7]);
            // 查看数据帧头
            if (can_receive_buffer[0] == 0x00){
            	can_receive_buffer[0] = 0xFF;
            	can_send_frame(can_remote_id, can_receive_buffer);
            	AD9959_Init();
            	screen_uart_printf("freq.txt=\"stop\"\xff\xff\xff");
            }
            else if(can_receive_buffer[0] == 0x01){
            	uint8_t channel = can_receive_buffer[1];
            	uint16_t amp = (can_receive_buffer[2] << 8) | can_receive_buffer[3];
            	uint32_t fre = (can_receive_buffer[4] << 24) | (can_receive_buffer[5] << 16) | (can_receive_buffer[6] << 8) | can_receive_buffer[7];
            	printf("channel:%d, fre:%d, amp:%d\r\n", channel, (int)fre, amp);
				if (channel == 0x00) {
					AD9959_Init();
					screen_uart_printf("freq.txt=\"stop\"\xff\xff\xff");
					continue;
				}
            	AD9959_Set_Fre(channel, fre);
            	AD9959_Set_Amp(channel, amp);
            	AD9959_Set_Phase(channel, 0);
            	IO_Update();
            	freq_cur = fre;
            	screen_uart_printf("freq.txt=\"%d Hz\"\xff\xff\xff", freq_cur);
            }
            else if(can_receive_buffer[0] == 0x02){
            	uint8_t mode = can_receive_buffer[1];
            	uint8_t board = can_receive_buffer[2];
            	uint8_t index = can_receive_buffer[3];
            	switch(mode){
				case 0x00:
					eb_count = EB_Init();
					can_receive_buffer[0] = 0x02;
					can_receive_buffer[1] = eb_count;
					// screen_uart_printf("battery_count.txt=\"%d\"\xff\xff\xff", eb_count*4);
					can_send_frame(can_remote_id, can_receive_buffer);
					break;
				case 0x01:
					EB_Select(board, index);
					channel_cur = board*4+index;
					screen_uart_printf("channel.txt=\"%d\"\xff\xff\xff", channel_cur);
					break;
				case 0x02:
					EB_Clear();
					screen_uart_printf("channel.txt=\"open\"\xff\xff\xff");
					break;
            	}
            }
            else if(can_receive_buffer[0] == 0x03){
            	switch(can_receive_buffer[1]) {
            	case 0x00:	// 初始化
            		screen_uart_printf("page 2\xff\xff\xff");
            		screen_uart_printf("battery_count.txt=\"%d\"\xff\xff\xff", eb_count*4);
            		EB_PowerOff();
					screen_uart_printf("relay.txt=\"true\"\xff\xff\xff");
            		switch(can_receive_buffer[2]) {
					case 0x00:
						screen_uart_printf("mode.txt=\"Nomral\"\xff\xff\xff");
						break;
					case 0x01:
						screen_uart_printf("mode.txt=\"DEBUG\"""\xff\xff\xff");
						break;
            		}
            		break;
				case 0x01:	// 任务状态
					switch (can_receive_buffer[2]) {
					case 0x00:
						screen_uart_printf("task.txt=\"scan\"\xff\xff\xff");
						break;
					case 0x01:
						screen_uart_printf("task.txt=\"single\"\xff\xff\xff");
						break;
					case 0x02:
						screen_uart_printf("task.txt=\"eis\"\xff\xff\xff");
						break;
					}
					break;
				case 0x02:	// 提交结果
//					read_two_float(&real, &imag);
					temp = (can_receive_buffer[4] << 24) | (can_receive_buffer[5] << 16) | (can_receive_buffer[6] << 8) | can_receive_buffer[7];
					switch(can_receive_buffer[2]) {
					case 0x00:// 传输real can_receive_buffer[4]~can_receive_buffer[7]为float数据
						memcpy(&real, &temp, sizeof(real));
						break;
					case 0x01:// 传输imag can_receive_buffer[4]~can_receive_buffer[7]为float数据
						memcpy(&imag, &temp, sizeof(imag));
                        screen_uart_printf("task.txt=\"null\"\xff\xff\xff");
						screen_uart_printf("last_channel.txt=\"%d\"\xff\xff\xff", channel_cur);
						screen_uart_printf("last_freq.txt=\"%d\"\xff\xff\xff", freq_cur);
						screen_uart_printf("last_real.txt=\"%f mOhm\"\xff\xff\xff", real);
						screen_uart_printf("last_imag.txt=\"%f mOhm\"\xff\xff\xff", imag);
						screen_uart_printf("las_impedance.txt=\"%f mOhm\"\xff\xff\xff", sqrt(real*real+imag*imag));
						screen_uart_printf("last_angel.txt=\"%f rad\"\xff\xff\xff", atan2(imag, real)); // 我承认我拼错了，但是我懒得改了
                        break;
					break;
					}
            	}
			}
			else if(can_receive_buffer[0] == 0x04){
				if(can_receive_buffer[1] == 0x00){	// 打开供电
					EB_PowerOn();
					screen_uart_printf("relay.txt=\"false\"\xff\xff\xff");
				}
				else{
					EB_PowerOff();
					screen_uart_printf("relay.txt=\"true\"\xff\xff\xff");
				}
			}
		}
        len = screen_uart_read_buffer(buffer, SCREEN_UART_BUFFER_SIZE);
        if(len!=0){
        	// 查看数据是否以FF FF FF结尾
			if (buffer[len - 3] == 0xFF && buffer[len - 2] == 0xFF
					&& buffer[len - 1] == 0xFF) {
				printf("recv:%s", buffer);
				if(buffer[0] == 0x55){
					// 执行软重启
					AD9959_Init();
					EB_Clear();
					gpio_write_pin(PIN_GPIO3_9, 0);
					osal_delay_millisec(500U);
					gpio_write_pin(PIN_GPIO3_9, 1);
				}
			}
        }
        if(!gpio_read_pin(USER_KEY_C7))
        {
            if(key_count_ms ++ > 10)
            {
                AD9959_Init();
                key_count_ms = 0;
            }
        }
    }
}

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 
// 问题1：调试报错
//      按照调试说明手册进行问题排查
//      务必确认连接正常 无短路与断路等问题
// 
// 问题2：串口助手无数据
//      确认打开的是正确的 COM 口
// 
// 问题3：串口助手乱码
//      确认使用的是 460800 波特率
//      确认是 8bit 数据位 1bit 停止位 无校验格式
// 
// 问题2：CAN 通信无反应
//      必须使用 Type-C 连接开发板供电 或通过 5V 引脚供电 否则 CAN 收发器不工作
//      ID 仅匹配 0x7F0 其余 ID 不匹配 除非修改 canconfig.h 中的 ID 定义
//      检查是否连接牢固 是否将开关拨动到 120R 端接电阻一侧
// 

