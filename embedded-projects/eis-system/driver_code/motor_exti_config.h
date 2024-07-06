/*
 * exti_config.h
 *
 *  Created on: 2024年2月28日
 *      Author: SEEKFREE_Teternal
 */

#ifndef DRIVER_CODE_MOTOR_EXTI_CONFIG_H_
#define DRIVER_CODE_MOTOR_EXTI_CONFIG_H_

#include <platform.h>
#include <exti.h>

#include "Config.h"

void motor_exti_set_callback (void (*callback1)(void), void (callback2)(void));
void motor_exti_interrupt_mode (uint8_t state);
void motor_exti_init (void);

#endif /* DRIVER_CODE_MOTOR_EXTI_CONFIG_H_ */
