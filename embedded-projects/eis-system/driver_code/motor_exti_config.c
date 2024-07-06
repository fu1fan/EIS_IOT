/*
 * exti_config.c
 *
 *  Created on: 2024年2月28日
 *      Author: SEEKFREE_Teternal
 */

#include "motor_exti_config.h"

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     外部中断回调 对应 MOTOR1-BREAK 信号
// 参数说明     void            无参数
// 返回参数     void            无返回值
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static void motor1_break_callback (void)
{
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     外部中断回调 对应 MOTOR2-BREAK 信号
// 参数说明     void            无参数
// 返回参数     void            无返回值
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static void motor2_break_callback (void)
{
}

static void (*motor_exti_motor1_break_callback) (void) = motor1_break_callback;
static void (*motor_exti_motor2_break_callback) (void) = motor2_break_callback;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     中断设置回调函数
// 参数说明     *callback1      回调函数指针
// 参数说明     *callback2      回调函数指针
// 返回参数     void            无返回值
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void motor_exti_set_callback (void (*callback1)(void), void (callback2)(void))
{
    motor_exti_motor1_break_callback = (NULL == callback1) ? (motor1_break_callback) : (callback1);
    motor_exti_motor2_break_callback = (NULL == callback2) ? (motor2_break_callback) : (callback2);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     EXTI 中断模式
// 参数说明     state       中断状态 0-禁用 1-使能
// 返回参数     void
// 使用示例     
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void motor_exti_interrupt_mode (uint8_t state)
{
    if(state)
    {
        irq_enable(exti_dev_get_vector(EXTI_LINE_3));
        irq_enable(exti_dev_get_vector(EXTI_LINE_5));
    }
    else
    {
        irq_disable(exti_dev_get_vector(EXTI_LINE_3));
        irq_disable(exti_dev_get_vector(EXTI_LINE_5));
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     外部中断初始化
// 参数说明     void            无参数
// 返回参数     void            无返回值
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void motor_exti_init (void)
{
    exti_set_line(EXTI_LINE_3, EXTI_MODE_INTERRUPT, EXTI_TRIGGER_RISING);       // 中断 上升沿
    exti_set_line(EXTI_LINE_5, EXTI_MODE_INTERRUPT, EXTI_TRIGGER_RISING);       // 中断 上升沿

    exti_set_port(EXTI_LINE_3, GPIO_PORT_I);                                    // 指向 I 口
    exti_set_port(EXTI_LINE_5, GPIO_PORT_I);                                    // 指向 I 口

    exti_set_irq(EXTI_LINE_3, IRQ_MIN_PRIORITY, motor_exti_motor1_break_callback);  // 指定引脚和中断
    exti_set_irq(EXTI_LINE_5, IRQ_MIN_PRIORITY, motor_exti_motor2_break_callback);  // 指定引脚和中断
}
