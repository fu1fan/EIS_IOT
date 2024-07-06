/*
 * system_init.h
 *
 *  Created on: 2024年2月20日
 *      Author: SEEKFREE_Teternal
 */

#ifndef DRIVER_CODE_SYSTEM_INIT_H_
#define DRIVER_CODE_SYSTEM_INIT_H_

typedef enum
{
    SYSTEM_LED_RED      = 1,
    SYSTEM_LED_GREEN    = 2,
    SYSTEM_LED_BLUE     = 3,
}system_led_channle_enum;

void system_cordic_calculate_default    (float angles, float *result);
void system_cordic_default_init         (void);

void system_rgb_control                 (system_led_channle_enum channel, uint8_t brightness);
void system_rgb_pwm_init                (void);

#endif /* DRIVER_CODE_SYSTEM_INIT_H_ */
