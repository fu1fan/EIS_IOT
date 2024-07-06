/*
 * hrtim_pwm.c
 *
 *  Created on: 2024年2月20日
 *      Author: SEEKFREE_Teternal
 */

#include "hrtim_pwm.h"

const hrtim_timer_t channel_list[6] = {
    HRTIM_TIMER_C, HRTIM_TIMER_B, HRTIM_TIMER_A,
    HRTIM_TIMER_F, HRTIM_TIMER_E, HRTIM_TIMER_D,
};

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 主定时器的中断回调
// 参数说明     *hdp            传输的 HRTIM 实例句柄
// 参数说明     events          触发事件
// 返回参数     void            无返回值
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static void hrtim_callback (hrtim_driver_t *hdp, uint32_t events)
{
    (void)hdp;
    (void)events;
}

static void (*hrtim_master_callback) (hrtim_driver_t *hdp, uint32_t events) = hrtim_callback;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 通道占空比设置
// 参数说明     channel         对应通道 详见 hrtim_pwm_channle_enum 定义
// 参数说明     duty            占空比数值 0 - HRTIM_PWM_DUTY_MAX/2
// 返回参数     void
// 使用示例     hrtim_pwm_set_duty(HRTIM_PWM_MOTOR1_A, HRTIM_PWM_DUTY_MAX / 2);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void hrtim_pwm_set_duty (hrtim_pwm_channle_enum channel, uint16_t duty)
{
    duty = (HRTIM_PWM_DUTY_MAX / 2 >= duty) ? (duty) : (HRTIM_PWM_DUTY_MAX / 2);
    hrtim_timer_set_cmp(&DRV_HRTIM1, channel_list[channel], HRTIM_CMP1, HRTIM_PWM_DUTY_MAX - duty);
    hrtim_timer_set_cmp(&DRV_HRTIM1, channel_list[channel], HRTIM_CMP2, HRTIM_PWM_DUTY_MAX + duty);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 中断回调设置
// 参数说明     void
// 返回参数     void
// 使用示例     hrtim_set_callback(test_callback);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void hrtim_set_callback (void (*callback) (hrtim_driver_t *hdp, uint32_t events))
{
    hrtim_master_callback = (NULL == callback) ? (hrtim_callback) : (callback);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 中断模式
// 参数说明     state       中断状态 0-禁用 1-使能
// 返回参数     void
// 使用示例     
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void hrtim_interrupt_mode (uint8_t state)
{
    if(state)
    {
        hrtim_enable_cb(&DRV_HRTIM1, HRTIM_CB_MASTER, HRTIM_CB_EVENT_CMP1);
    }
    else
    {
        hrtim_disable_cb(&DRV_HRTIM1, HRTIM_CB_MASTER, HRTIM_CB_EVENT_CMP1);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     HRTIM 初始化 初始化 PWM 相关设置 以及中断
// 参数说明     void
// 返回参数     void
// 使用示例     hrtimer_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void hrtimer_init (void)
{
    hrtim_init(&DRV_HRTIM1);                                                    // 初始化实例参数

    // 300MHz 基频 那么经四倍频到 1.2GHz 这样的话 20KHZ 的周期值就是 1.2GHz/20K = 60K = 0xEA60
    hrtim_set_timer_mode(&DRV_HRTIM1, HRTIM_TIMER_MASTER, HRTIM_MODE_CONTINUOS);// 主定时器设置重复计数方式
    hrtim_timer_set_prescaler(&DRV_HRTIM1, HRTIM_TIMER_MASTER, HRTIM_PRSC_MUL4);// 设置是 4 倍频
    hrtim_timer_set_period( &DRV_HRTIM1, HRTIM_TIMER_MASTER,                    // 周期值需要计算
        HRTIM_MUL4_FERQ_HZ / HRTIM_PWM_FERQ_HZ);                                // 使用主定时器的 4 倍频除以需要的 PWM 频率得到计数值
    hrtim_timer_set_cmp(&DRV_HRTIM1, HRTIM_TIMER_MASTER,                        // 设置主定时器的比较器
        HRTIM_CMP1, HRTIM_MUL4_FERQ_HZ / HRTIM_PWM_FERQ_HZ);                    // 比较器值跟计数值一样 这是用来触发周期中断的

    for(uint8_t i = 0; i < 6; i ++)
    {
        // 子定时器的配置与主定时器一致
        // 300MHz 基频 那么经四倍频到 1.2GHz 这样的话 20KHZ 的周期就是 50us 对应 1.2GHz/20K = 60K = 0xEA60
        hrtim_set_timer_mode(&DRV_HRTIM1, channel_list[i], HRTIM_MODE_CONTINUOS);
        hrtim_timer_set_prescaler(&DRV_HRTIM1, channel_list[i], HRTIM_PRSC_MUL4);
        hrtim_timer_set_period(&DRV_HRTIM1, channel_list[i], HRTIM_MUL4_FERQ_HZ / HRTIM_PWM_FERQ_HZ);
        hrtim_timer_set_cmp(&DRV_HRTIM1, channel_list[i], HRTIM_CMP1, 0);

        // 300MHz 基频 这样的话每个周期就是 416 ps
        // 不要看它第三个参数的枚举注释 实际是 300MHz 的 3.33 ns / 8 得到 416ps
        // 第三个参数实际作用是进行 2^n 分频 死区的可调粒度变大 范围变大
        // 然后 416ps 乘以第三个参数的 2^n 得到最终的死区时钟粒度
        hrtim_timer_set_deadtime(                                               // 设置死区时间
            &DRV_HRTIM1, channel_list[i], 0,                                    // 默认按照最小粒度调整
            HRTIM_PWM_DDT_NS * 2.5, HRTIM_DT_SIGN_POSITIVE,                     // 上升死区
            HRTIM_PWM_DDT_NS * 2.5, HRTIM_DT_SIGN_POSITIVE);                    // 下降死区
        hrtim_timer_enable_deadtime(&DRV_HRTIM1, channel_list[i]);              // 配置好后使能

        // 配置通道一在周期事件触发时置位 在比较匹配复位
        hrtim_timer_set_outputset(&DRV_HRTIM1, channel_list[i], HRTIM_TIMER_OUTPUT_1, HRTIM_OUTPUT_TIMCMP1);
        hrtim_timer_set_outputrst(&DRV_HRTIM1, channel_list[i], HRTIM_TIMER_OUTPUT_1, HRTIM_OUTPUT_TIMCMP2);
        // 配置通道二在周期事件触发时复位 在比较匹配置位
        hrtim_timer_set_outputset(&DRV_HRTIM1, channel_list[i], HRTIM_TIMER_OUTPUT_2, HRTIM_OUTPUT_TIMCMP2);
        hrtim_timer_set_outputrst(&DRV_HRTIM1, channel_list[i], HRTIM_TIMER_OUTPUT_2, HRTIM_OUTPUT_TIMCMP1);
    }

    hrtim_start(&DRV_HRTIM1);                                                   // 使用配置好的参数启动模块

    hrtim_set_prio(&DRV_HRTIM1, IRQ_PRIORITY_3);
    hrtim_set_cb(&DRV_HRTIM1, HRTIM_CB_MASTER, hrtim_master_callback);          // 设置主定时器的回调
    hrtim_enable_cb(&DRV_HRTIM1, HRTIM_CB_MASTER, HRTIM_CB_EVENT_CMP1);         // 启用主定时器的比较器一的中断触发
//    hrtim_enable_cb(&DRV_HRTIM1, HRTIM_CB_MASTER, HRTIM_CB_EVENT_REP);        // 或者是重复事件中断触发
//    hrtim_timer_set_repetition(&DRV_HRTIM1, HRTIM_TIMER_MASTER, 1);           // 重复一次

    hrtim_timer_enable_output(&DRV_HRTIM1,                                      // 使能所有的 PWM 输出通道
        HRTIM_TIMER_A_OUT1_MASK_ID | HRTIM_TIMER_A_OUT2_MASK_ID |
        HRTIM_TIMER_B_OUT1_MASK_ID | HRTIM_TIMER_B_OUT2_MASK_ID |
        HRTIM_TIMER_C_OUT1_MASK_ID | HRTIM_TIMER_C_OUT2_MASK_ID |
        HRTIM_TIMER_D_OUT1_MASK_ID | HRTIM_TIMER_D_OUT2_MASK_ID |
        HRTIM_TIMER_E_OUT1_MASK_ID | HRTIM_TIMER_E_OUT2_MASK_ID |
        HRTIM_TIMER_F_OUT1_MASK_ID | HRTIM_TIMER_F_OUT2_MASK_ID);
    (void)hrtim_timer_enable(&DRV_HRTIM1, HRTIM_TIMER_MASTER_MASK_ID |          // 使能所有计数器
        HRTIM_TIMER_A_MASK_ID | HRTIM_TIMER_B_MASK_ID |
        HRTIM_TIMER_C_MASK_ID | HRTIM_TIMER_D_MASK_ID |
        HRTIM_TIMER_E_MASK_ID | HRTIM_TIMER_F_MASK_ID);
}

