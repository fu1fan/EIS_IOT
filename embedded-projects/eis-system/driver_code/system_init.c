/*
 * system_init.c
 *
 *  Created on: 2024年2月20日
 *      Author: SEEKFREE_Teternal
 */

#include "driver_headfile.h"

#include "cordic.h"

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     默认 CORDIC 计算
// 参数说明     angles          角度
// 返回参数     *result         返回结果缓存 sin 和 cos 值
// 使用示例     
//              float result[2] = {0,0};
//              system_cordic_calculate_default(90.0, result);
//              printf("sin(%.3f)=%.3f - cos(%.3f)=%.3f\n\r", angles, result[0], angles, result[1]);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void system_cordic_calculate_default (float angles, float *result)
{
    int32_t input[2]  = {
        (int32_t)((angles / 180.0) * 0x80000000UL),
        (int32_t)(1.0 * 0x80000000UL)};
    int32_t output[2] = {0x0UL};

    cordic_calculate(&DRV_CORDIC, input, output, 1);
    result[0] = (output[0] / ((float)(0x80000000UL)));
    result[1] = (output[1] / ((float)(0x80000000UL)));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     默认 CORDIC 初始化
// 参数说明     void            无参数
// 返回参数     void            无返回值
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void system_cordic_default_init (void)
{
    cordic_init(&DRV_CORDIC);

    cordic_set_drv_mode(&DRV_CORDIC, CORDIC_DRV_MODE_POLLING_SYNC);
    cordic_set_function(&DRV_CORDIC, CORDIC_FUNC_SINE);
    cordic_set_precision(&DRV_CORDIC, CORDIC_PREC_6CYCLES);
    cordic_set_res(&DRV_CORDIC, CORDIC_NUM_2_32BIT, CORDIC_WIDTH_32BIT);
    cordic_set_arg(&DRV_CORDIC, CORDIC_NUM_2_32BIT, CORDIC_WIDTH_32BIT);

    cordic_start(&DRV_CORDIC);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     开发板 RGB 灯亮度控制
// 参数说明     channel         RGB 通道 详情参照 system_init.h 中 system_led_channle_enum 枚举
// 参数说明     brightness      亮度 0-100 可调
// 返回参数     void            无返回值
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void system_rgb_control (system_led_channle_enum channel, uint8_t brightness)
{
    brightness = (100 <= brightness) ? (0) : (100 - brightness);
    tim_set_comparator  (&DRV_TIM3, channel, brightness);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     开发板 RGB 灯初始化 
// 参数说明     void            无参数
// 返回参数     void            无返回值
// 使用示例     
// 备注信息     引脚配置使用 工程内 Config.gpio 设置 默认匹配 ZFEVB-SR5E1E3 开发板
//-------------------------------------------------------------------------------------------------------------------
void system_rgb_pwm_init (void)
{
    tim_init        (&DRV_TIM3);                                                // 选择对应 TIM3
    tim_set_presc   (&DRV_TIM3, TIM_DIVIDE_BY(tim_get_clock(&DRV_TIM3) / 5000000U));    // 计数频率设置到了 5MHz
    tim_set_ch_mode (&DRV_TIM3, 1U, TIM_MODE_PWM_ACTIVE_HIGH);                  // 配置通道 1 的输出模式
    tim_set_ch_mode (&DRV_TIM3, 2U, TIM_MODE_PWM_ACTIVE_HIGH);                  // 配置通道 2 的输出模式
    tim_set_ch_mode (&DRV_TIM3, 3U, TIM_MODE_PWM_ACTIVE_HIGH);                  // 配置通道 3 的输出模式
    tim_start       (&DRV_TIM3);                                                // 启动定时器实例

    tim_set_interval    (&DRV_TIM3, 100U);                                      // 5MHz/100 = 50kHz 调节范围 0-100
    tim_set_comparator  (&DRV_TIM3, 1U, 100);                                   // 先都熄灯
    tim_set_comparator  (&DRV_TIM3, 2U, 100);                                   // 先都熄灯
    tim_set_comparator  (&DRV_TIM3, 3U, 100);                                   // 先都熄灯
    tim_enable          (&DRV_TIM3);                                            // 使能定时器开始工作
}
