/*
 * hrtim_pwm.h
 *
 *  Created on: 2024年2月20日
 *      Author: SEEKFREE_Teternal
 */

#ifndef DRIVER_CODE_HRTIM_PWM_H_
#define DRIVER_CODE_HRTIM_PWM_H_

#include <irq.h>
#include <gpio.h>
#include <hrtim.h>

// 基本设置 HRTIM 锚定系统频率 固定经过四倍频获得 1.2GHz 工作频率 此处不要修改
#define HRTIM_BASE_FERQ_HZ                      ( 300 * 1000 * 1000 )
#define HRTIM_MUL4_FERQ_HZ                      ( HRTIM_BASE_FERQ_HZ * 4 )

// 定义设置 HRTIM-PWM 工作参数 频率 最大占空比 死区时长
#define HRTIM_PWM_FERQ_HZ                       ( 20 * 1000 )
#define HRTIM_PWM_COUNT_MAX                     ( HRTIM_MUL4_FERQ_HZ / HRTIM_PWM_FERQ_HZ )
#define HRTIM_PWM_DUTY_MAX                      ( HRTIM_PWM_COUNT_MAX / 2 )
#define HRTIM_PWM_DDT_NS                        ( 10U )

typedef enum
{
    HRTIM_PWM_MOTOR1_A,
    HRTIM_PWM_MOTOR1_B,
    HRTIM_PWM_MOTOR1_C,

    HRTIM_PWM_MOTOR2_A,
    HRTIM_PWM_MOTOR2_B,
    HRTIM_PWM_MOTOR2_C,
}hrtim_pwm_channle_enum;

extern void (*hrtim_interrupt_callback)(void);

void hrtim_pwm_set_duty (hrtim_pwm_channle_enum channel, uint16_t duty);
void hrtim_set_callback (void (*callback) (hrtim_driver_t *hdp, uint32_t events));
void hrtim_interrupt_mode (uint8_t state);
void hrtimer_init (void);

#endif /* DRIVER_CODE_HRTIM_PWM_H_ */
