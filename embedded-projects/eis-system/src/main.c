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

#include "math.h"
#include "stdio.h"
#include "stdarg.h"

#define HTTP_RETRY 3
#define MEASURE_RETRY 3

const char API_ADDRESS[] = "https://eis.zzzing.cn/api/";
char content[2048];	// http_content
char task_content[128];
char task_id[32];
int post_size;
http_response_t response;

void wpostf(const char *fmt, ...) {
	// 将数据写入content, 并计算post_size
	va_list args;
	va_start(args, fmt);
	post_size = vsnprintf(content, sizeof(content), fmt, args);
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
    system_rgb_pwm_init();                                                      // 初始化 RGB 灯配置
    can_remote_init();

    printf("Running.\r\n");

    response.content = content;
	response.max_size = 2048;

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
	uint8_t eb_count = 1;
	float voltage = 0;
	ui_console_printf("ExBoard found: %d", eb_count);
	for (uint8_t i = 0; i < eb_count; i++) {
		for (uint8_t j = 0; j < 4; j++) {
			eb_set(i, j);
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

	ui_console_printf("");

    ui_console_printf("connecting...");

	for (;;) {
		status = lte_http_get(API_ADDRESS, &response, 5000, 1);
		if (status.is_success) {
			response_purify(&response);
			ui_console_printf("server: %s", response.content);
			break;
		}
		ui_console_printf("error_code: %d", status.error_code);
		ui_console_printf("retrying...");
	}
	ui_console_printf("connected!");

	ui_console_printf("");
	ui_console_printf("init service...");
	wpostf("%d", eb_count*4);
	status = lte_http_post("https://eis.zzzing.cn/api/h/init", (const uint8_t *)content, post_size, &response, 5000, HTTP_RETRY);
	if (!status.is_success) {
		ui_console_printf("init error!");
		ui_console_printf("error_code: %d", status.error_code);
		return 0;
	}
	response_purify(&response);
	if (0 == strcmp((const char *)response.content, "0")) {
		ui_console_printf("service error!");
		return 0;
	}
	ui_console_printf("service inited!");

	char *p, *q;
    for( ; ; )                                                                  // 主循环的代码就是控制各个 LED 呼吸亮灭
        {
    		status = lte_http_get("https://eis.zzzing.cn/api/h/get_task", &response, 5000, HTTP_RETRY);
    		if(!status.is_success){
				ui_console_printf("get task error!");
				ui_console_printf("error_code: %d", status.error_code);
				continue;
			}
    		response_purify(&response);
    		if(0 == strcmp((const char *)response.content, "null")){
    			continue;
    		}
			else {
				memccpy(task_content, response.content, 0, sizeof(task_content));
				p = task_content;
				q = strchr(p, '|');
				if (q == NULL) {
					ui_console_printf("invaild response (0)");
					continue;
				}
				*q = '\0';
				strcpy(task_id, p);
				ui_console_printf("get new task,");
				ui_console_printf("id: %s", task_id);

				wpostf(task_id);
				status = lte_http_post("https://eis.zzzing.cn/api/h/confirm_task", (const uint8_t *)content, post_size, &response, 20000, 1);
				if (!status.is_success) {
					ui_console_printf("confirm task failed!");
					ui_console_printf("error_code: %d", status.error_code);
				}
				response_purify(&response);
				ui_console_printf("task_confirmed: %s", response.content);

				p = ++q;
				q = strchr(p, '|');
				if (q == NULL) {
					ui_console_printf("invaild response (1)");
					continue;
				}
				*q = '\0';
				if(0 == strcmp(p, "single")){	// 单点测量
					p = ++q;
					q = strchr(p, '|');
					if (q == NULL) {
						ui_console_printf("invaild response (2)");
						continue;
					}
					*q = '\0';
					uint8_t cell_id = atoi(p);
					if (cell_id >= eb_count*4) {
						ui_console_printf("invaild cell id");
						continue;
					}
					p = ++q;
					q = strchr(p, '|');
					if (q == NULL) {
						ui_console_printf("invaild response (3)");
						continue;
					}
					*q = '\0';
					uint32_t freq = atoi(p);
					eis_battery_select(cell_id);
					if (freq == 0){	// 欧姆阻抗测量
						float ohmage, voltage;
						uint8_t retry;
						eis_get_voltage(&voltage);
						for (retry = 0; retry < MEASURE_RETRY; retry++) {
							status = eis_ohmage_measure(&ohmage, &freq);
							if (status.is_success) {
								break;
							}
						}
						if (retry == MEASURE_RETRY) {
							ui_console_printf("measure failed!");
							wpostf("%s|failed|%d", task_id, status.error_code);
						}
						else{
							wpostf("%s|single|%d|%.4f|%f|%f",
							task_id, freq, voltage, eis_result.tail->real, eis_result.tail->imag);
						}
						status = lte_http_post("https://eis.zzzing.cn/api/h/submit_result", (const uint8_t *)content, post_size, &response, 5000, HTTP_RETRY);
						if (!status.is_success) {
							ui_console_printf("submit result failed!");
							ui_console_printf("error_code: %d", status.error_code);
						}
					}else{    // 指定频率测量
						float voltage;
						uint8_t retry;
						eis_get_voltage(&voltage);
						eis_single_init();
						for (retry = 0; retry < MEASURE_RETRY; retry++) {
							status = eis_single_measure(freq, 1);
							if (status.is_success) {
								break;
							}
						}
						if (retry == MEASURE_RETRY) {
							ui_console_printf("measure failed!");
							wpostf("%s|failed|%d", task_id, status.error_code);
						}
						else{
							wpostf("%s|single|%d|%.4f|%f|%f",
							task_id, freq, voltage, eis_result.tail->real, eis_result.tail->imag);
						}
						status = lte_http_post("https://eis.zzzing.cn/api/h/submit_result", (const uint8_t *)content, post_size, &response, 5000, HTTP_RETRY);
						if (!status.is_success) {
							ui_console_printf("submit result failed!");
							ui_console_printf("error_code: %d", status.error_code);
						}
						eis_single_end();
					}
				}else if(0 == strcmp(p, "eis")){
					p = ++q;
					q = strchr(p, '|');
					if (q == NULL) {
						ui_console_printf("invaild response (4)");
						continue;
					}
					*q = '\0';
					uint8_t cell_id = atoi(p);
					if (cell_id >= eb_count*4) {
						ui_console_printf("invaild cell id");
						continue;
					}
					eis_battery_select(cell_id);
					uint32_t retry;
					for (retry = 0; retry < MEASURE_RETRY; retry++) {
						status = eis_measure();
						if (status.is_success) {
							break;
						}
					}
					if (retry == MEASURE_RETRY) {
						ui_console_printf("measure failed!");
						wpostf("%s|failed|%d", task_id, status.error_code);
					} else {
						// 示例数据"{task_id}|eis|freq1,freq2...|imag1,imag2...|real1,real2..."
						wpostf("%s|eis|", task_id);
						// 由于每次调用wpostf都会覆盖上一次的内容，所以需要直接对content进行操作
						struct eis_node_t *p = eis_result.head;
						while (p != NULL) {
							sprintf(content + strlen(content), "%d,", (int)p->freq);
							p = p->next;
						}
						content[strlen(content) - 1] = '|';
						content[strlen(content)] = '\0';

						p = eis_result.head;
						while (p != NULL) {
							sprintf(content + strlen(content), "%f,", p->real);
							p = p->next;
						}
						content[strlen(content) - 1] = '|';
						content[strlen(content)] = '\0';

						p = eis_result.head;
						while (p != NULL) {
							sprintf(content + strlen(content), "%f,", p->imag);
							p = p->next;
						}
						content[strlen(content) - 1] = '\0';
						post_size = strlen(content);
					}
					status = lte_http_post("https://eis.zzzing.cn/api/h/submit_result", (const uint8_t *)content, post_size, &response, 5000, HTTP_RETRY);
					if (!status.is_success) {
						ui_console_printf("submit result failed!");
						ui_console_printf("error_code: %d", status.error_code);
					}
				} else if (0 == strcmp(p, "all")) {
					p = ++q;
					q = strchr(p, '|');
					if (q == NULL) {
						ui_console_printf("invaild response (5)");
						continue;
					}
					*q = '\0';
					// 参数：mode
					// mode=0 只测量电压，返回每节电池电压
					// mode=1 测量电压及每节电池100Hz阻抗，反应电池动力特性
					// mode=2 测量电压及每节电池70Hz阻抗，反应电池充电特性
					uint8_t mode = atoi(p);
					if (mode > 2) {
						ui_console_printf("invaild mode");
						continue;
					}
					float *voltages = malloc(eb_count * 4 * sizeof(float));
					float *ohmages = malloc(eb_count * 4 * sizeof(float));
					eis_single_init();
					uint8_t flag = 1;
					uint8_t retry = 0;
					for (uint8_t i = 0; i < eb_count * 4; i++) {
						eis_battery_select(i);
						eis_get_voltage(&voltages[i]);
						switch (mode)
						{
						case 1:
							retry = 0;
							for (retry = 0; retry < MEASURE_RETRY; retry++) {
								status = eis_single_measure(100, 1);
								if (status.is_success) {
									ohmages[i] = sqrt(pow(eis_result.tail->real, 2) + pow(eis_result.tail->imag, 2));
									break;
								}
								if (status.error_code == 0x31){
									ohmages[i] = 0;
									break;
								}
							}
							if (retry == MEASURE_RETRY) {
								ui_console_printf("measure failed!");
								wpostf("%s|failed|%d", task_id, status.error_code);
								flag = 0;
							}
							break;
						
						case 2:
							retry = 0;
							for (retry = 0; retry < MEASURE_RETRY; retry++) {
								status = eis_single_measure(70, 1);
								if (status.is_success) {
									ohmages[i] = sqrt(pow(eis_result.tail->real, 2) + pow(eis_result.tail->imag, 2));
									break;
								}
								if (status.error_code == 0x31) {
									ohmages[i] = 0;
									break;
								}
							}
							if (retry == MEASURE_RETRY) {
								ui_console_printf("measure failed!");
								wpostf("%s|failed|%d", task_id, status.error_code);
								flag = 0;
							}
							break;
						
						default:
							break;
						}
						if (flag == 0) {
							break;
						}
					}
					eis_single_end();
					if (flag == 1) {
						// 示例数据"{task_id}|all|volt1,volt2...|ohm1,ohm2..."
						wpostf("%s|all|", task_id);
						// 由于每次调用wpostf都会覆盖上一次的内容，所以需要直接对content进行操作
						for (uint8_t i = 0; i < eb_count * 4; i++) {
							sprintf(content + strlen(content), "%.4f,", voltages[i]);
						}
						content[strlen(content) - 1] = '\0';
						if (mode != 0) {
							content[strlen(content) - 1] = '|';
							content[strlen(content)] = '\0';
							for (uint8_t i = 0; i < eb_count * 4; i++) {
								sprintf(content + strlen(content), "%.4f,", ohmages[i]);
							}
							content[strlen(content) - 1] = '\0';
						}
						post_size = strlen(content);
						status = lte_http_post("https://eis.zzzing.cn/api/h/submit_result", (const uint8_t *)content, post_size, &response, 5000, HTTP_RETRY);
						if (!status.is_success) {
							ui_console_printf("submit result failed!");
							ui_console_printf("error_code: %d", status.error_code);
						}
					}else{
						ui_console_printf("measure failed!");
						status = lte_http_post("https://eis.zzzing.cn/api/h/submit_result", (const uint8_t *)content, post_size, &response, 5000, HTTP_RETRY);
						if (!status.is_success) {
							ui_console_printf("submit result failed!");
							ui_console_printf("error_code: %d", status.error_code);
						}
					}
				} else {
					ui_console_printf("invaild task type");
				}
			}
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
