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
#include "eis_measure.h"
#include "can_remote.h"
#include "tft_ui.h"
#include "lte.h"

#define gain 255

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
    system_rgb_pwm_init();                                                      // 初始化 RGB 灯配置
    can_remote_init();

    printf("Running.\r\n");

    // 界面初始化
    ui_init();
    ui_console_mode();

    // 核心测量模块初始化
    ui_console_printf("init core...");
    eis_status_t status;
    status = eis_measure_init();
	if (!status.is_success) {
		ui_console_printf("error_code: %d", status.error_code);
		return 0;
	}
	ui_console_printf("init done!");
	ui_console_printf("Embedded 2024 by DOL");
	ui_console_printf("");

	// 电池扫描
	ui_console_printf("searching batterys...");
	uint8_t eb_count = eb_query();
	float voltage = 0;
	ui_console_printf("ExBoard found: %d", eb_count);
	for (uint8_t i = 0; i < eb_count; i++) {
		for (uint8_t j = 0; j < 4; j++) {
			eb_set(i, j);
			osal_delay_millisec(5U);
			eis_get_voltage(&voltage);
			ui_console_printf("b%d: %.4fv", i*4+j, voltage);
		}
	}
//    eis_battery_select(0);
//    eis_get_voltage(&voltage);
//    ui_console_printf("b0: %.4fv", voltage);
//    eis_battery_select(1);
//    eis_get_voltage(&voltage);
//    ui_console_printf("b1: %.4fv", voltage);
//    eis_battery_select(2);
//    eis_get_voltage(&voltage);
//    ui_console_printf("b2: %.4fv", voltage);
//    eis_battery_select(3);
//    eis_get_voltage(&voltage);
//    ui_console_printf("b3: %.4fv", voltage);
//    eis_battery_select(4);
//    eis_get_voltage(&voltage);
//    ui_console_printf("b4: %.4fv", voltage);

    ui_console_printf("");
    // LTE模块初始化
    ui_console_printf("init lte module...");
    status = lte_module_init();
	if (!status.is_success) {
		ui_console_printf("lte init failed!");
		ui_console_printf("error_code: %d", status.error_code);
		return 0;
	}
	ui_console_printf("lte module founded!");
	ui_console_printf("init cecullar...");
	status = lte_celluar_init();
	if (!status.is_success) {
		ui_console_printf("cecullar init failed!");
		ui_console_printf("error_code: %d", status.error_code);
		return 0;
	}
	ui_console_printf("IP:%s", ip_address);
	ui_console_printf("Cecullar connected!");

    //tft180_show_uint(0, 0, id, 10);

    // gpio_write_pin(I_S, 1);
    ui_console_mode();
    ui_console_printf("server_test");

    char content[512];

    http_response_t response;
    response.content = content;
    response.max_size = 512;

    for( ; ; )                                                                  // 主循环的代码就是控制各个 LED 呼吸亮灭
        {
    		status = lte_http_get("https://eis.zzzing.cn/api/h/get_task", &response, 60000);
    		if(!status.is_success){
				ui_console_printf("error_code: %d", status.error_code);
			}
    		response_purify(&response);
    		if(0 == strcmp((const char *)response.content, "1")){
    			eis_measure();
    		}
    		*response.content = '0';
            osal_delay_millisec(200U);
            //eis_trw_measure();
//    		can_remote_id = 0x7F0;
//    		for(uint8 i = 0; 8 > i; i ++)
//    		{
//    			can_receive_buffer[i] = 0x30 + i;
//    		}
//    		can_send_frame(can_remote_id, can_receive_buffer);
        }
}
