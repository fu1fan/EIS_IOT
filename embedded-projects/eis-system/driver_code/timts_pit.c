/*
 * timts_pit.c
 *
 *  Created on: 2024年2月23日
 *      Author: SEEKFREE_Teternal
 */

#include "timts_pit.h"

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TIM PIT 中断回调
// 参数说明     *tdp            传输的 TIM 实例句柄
// 返回参数     void            无返回值
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static void timts_callback (tim_ts_driver_t *tdp)
{
    (void)tdp;
}

static void (*tim_ts_cb) (tim_ts_driver_t *tdp) = timts_callback;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     配置 PIT 回调函数
// 参数说明     *callback       回调函数指针
// 返回参数     void            无返回值
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void timts_set_callback (void (*callback)(tim_ts_driver_t *tdp))
{
    tim_ts_cb = (NULL == callback) ? (timts_callback) : (callback);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 中断模式
// 参数说明     state       中断状态 0-禁用 1-使能
// 返回参数     void
// 使用示例     
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void timts_interrupt_mode (uint8_t state)
{
    if(state)
    {
        tim_ts_start(&DRV_TIM_TS);
    }
    else
    {
        tim_ts_stop(&DRV_TIM_TS);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     配置 PIT 回调函数
// 参数说明     period_ms       定时器周期 单位 ms
// 返回参数     void            无返回值
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void timts_pit_ms_init (uint16_t period_ms)
{
    uint32_t period = period_ms * 150000;
    uint16_t freq_div = (period >> 15);                                         // 计算预分频
    uint16_t period_temp = (period / (freq_div + 1));                           // 计算自动重装载值

    tim_ts_init(&DRV_TIM_TS);                                                   // 初始化 TS 实例参数

    tim_ts_enable_dithering(&DRV_TIM_TS, false);                                // 禁止抖动功能
    tim_ts_enable_uif_remapping(&DRV_TIM_TS, false);                            // 禁止路由
    tim_ts_enable_autoreload_preload(&DRV_TIM_TS, true);                        // 使能自动重装载
    tim_ts_enable_one_pulse_mode(&DRV_TIM_TS, false);                           // 禁止单次脉冲模式
    tim_ts_set_source_ofuf_only(&DRV_TIM_TS, true);                             // 设置更新请求事件
    tim_ts_enable_update_event_generation(&DRV_TIM_TS, true);                   // 是否允许更新事件
    tim_ts_set_master_mode_selection(&DRV_TIM_TS, TIM_TS_MMS_UPDATE);           // 配置模式
    tim_ts_set_prescaler(&DRV_TIM_TS, freq_div);                                // 设置分配
    tim_ts_set_autoreload(&DRV_TIM_TS, period_temp);                            // 设置周期计数值

    tim_ts_set_prio(&DRV_TIM_TS, IRQ_PRIORITY_5);                               // 配置中断优先级
    tim_ts_set_cb(&DRV_TIM_TS, tim_ts_cb);                                      // 设置回调

    tim_ts_start(&DRV_TIM_TS);                                                  // 启动模块
}

void timts_pit_us_init (uint16_t period_ms)
{
	uint32_t period = period_ms * 150;
	uint16_t freq_div = (period >> 15);                                 // 计算预分频
	uint16_t period_temp = (period / (freq_div + 1));                // 计算自动重装载值

	tim_ts_init(&DRV_TIM_TS);                                     // 初始化 TS 实例参数

	tim_ts_enable_dithering(&DRV_TIM_TS, false);                       // 禁止抖动功能
	tim_ts_enable_uif_remapping(&DRV_TIM_TS, false);                     // 禁止路由
	tim_ts_enable_autoreload_preload(&DRV_TIM_TS, true);              // 使能自动重装载
	tim_ts_enable_one_pulse_mode(&DRV_TIM_TS, false);                // 禁止单次脉冲模式
	tim_ts_set_source_ofuf_only(&DRV_TIM_TS, true);                  // 设置更新请求事件
	tim_ts_enable_update_event_generation(&DRV_TIM_TS, true);        // 是否允许更新事件
	tim_ts_set_master_mode_selection(&DRV_TIM_TS, TIM_TS_MMS_UPDATE);    // 配置模式
	tim_ts_set_prescaler(&DRV_TIM_TS, freq_div);                         // 设置分配
	tim_ts_set_autoreload(&DRV_TIM_TS, period_temp);                  // 设置周期计数值

	tim_ts_set_prio(&DRV_TIM_TS, IRQ_PRIORITY_5);                     // 配置中断优先级
	tim_ts_set_cb(&DRV_TIM_TS, tim_ts_cb);                               // 设置回调

	tim_ts_start(&DRV_TIM_TS);                                           // 启动模块
}

void timts_pit_freq_init(uint32_t freq) {
	uint32_t period = 150000000 / freq;
	uint16_t freq_div = (period >> 15);                                 // 计算预分频
	uint16_t period_temp = (period / (freq_div + 1));                // 计算自动重装载值

	tim_ts_init(&DRV_TIM_TS);                                     // 初始化 TS 实例参数

	tim_ts_enable_dithering(&DRV_TIM_TS, false);                       // 禁止抖动功能
	tim_ts_enable_uif_remapping(&DRV_TIM_TS, false);                     // 禁止路由
	tim_ts_enable_autoreload_preload(&DRV_TIM_TS, true);              // 使能自动重装载
	tim_ts_enable_one_pulse_mode(&DRV_TIM_TS, false);                // 禁止单次脉冲模式
	tim_ts_set_source_ofuf_only(&DRV_TIM_TS, true);                  // 设置更新请求事件
	tim_ts_enable_update_event_generation(&DRV_TIM_TS, true);        // 是否允许更新事件
	tim_ts_set_master_mode_selection(&DRV_TIM_TS, TIM_TS_MMS_UPDATE);    // 配置模式
	tim_ts_set_prescaler(&DRV_TIM_TS, freq_div);                         // 设置分配
	tim_ts_set_autoreload(&DRV_TIM_TS, period_temp);                  // 设置周期计数值

	tim_ts_set_prio(&DRV_TIM_TS, IRQ_PRIORITY_5);                     // 配置中断优先级
	tim_ts_set_cb(&DRV_TIM_TS, tim_ts_cb);                               // 设置回调

	tim_ts_start(&DRV_TIM_TS);                                           // 启动模块
}

