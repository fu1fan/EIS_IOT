/*
 * driver_code.h
 *
 *  Created on: 2024年2月21日
 *      Author: SEEKFREE_Teternal
 */

#ifndef DRIVER_CODE_DRIVER_HEADFILE_H_
#define DRIVER_CODE_DRIVER_HEADFILE_H_

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "zf_common_typedef.h"

#include "debug_uart.h"
#include "hrtim_pwm.h"
#include "sradc_sample.h"
#include "timts_pit.h"
#include "system_init.h"
#include "spi_config.h"
#include "can_config.h"
#include "motor_exti_config.h"

#include "zf_device_ips200.h"
#include "zf_device_spi_encoder.h"
// #include "zf_device_wireless_uart.h"

#include "zf_common_fifo.h"

#define UART_DEBUG

typedef struct{
	uint8_t is_success;
	uint8_t error_code;
} eis_status_t;

/*
错误信息对照表：
0x00: 成功
0x1X: 硬件错误
	0x10: 未知错误
	0x11: 未检测到电池选择板
	0x12: AD7606异常
	0x13: ADS1256异常
	0x14: DAC1220异常
	0x15: LTE模块初始化失败

0x2X: 电池错误
	0x20: 未知错误
	0x21: 电池电压异常

0x3X: 测量时错误
	0x30: 未知错误
	0x31: 超出量程
	0x32: 电压偏置调整异常
	0x33: 部分点采样失败
	0x34: 多点采样失败
	0x35: 单点采样失败
	0x36: 欧姆阻抗位置错误(电池质量太差)
	0x37: 欧姆阻抗超出量程(电池质量太差)
	0x38: 欧姆阻抗查找失败(数据仍有参考价值)

0x4X: 通信错误
	0x40: 未知错误
	0x41: 串口接收超时
	0x42: 接收缓冲区溢出

0x5X: 网络错误
	0x50: 未知错误
	0x51: SIM卡初始化失败
	0x52: 数据不可用
	0x53: 无信号
	0x54: 信号弱
	0x55: 网络未注册
	0x56: IP查询失败

0x6X: HTTP错误
	0x60: 其他错误
	0x61: 缓冲区溢出
	0x62: 读取内容失败
	0x63: 传输post数据失败
 */

#endif /* DRIVER_CODE_DRIVER_HEADFILE_H_ */
