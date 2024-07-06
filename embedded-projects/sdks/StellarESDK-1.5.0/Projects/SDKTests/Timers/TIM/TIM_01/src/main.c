/****************************************************************************
 *
 * Copyright (c) 2022 STMicroelectronics - All Rights Reserved
 *
 * License terms: STMicroelectronics Proprietary in accordance with licensing
 * terms SLA0098 at www.st.com.
 *
 * THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
 * INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *****************************************************************************/

#include <test_env.h>
#include <tim.h>

/*===========================================================================*/
/* TIM-related.                                                              */
/*===========================================================================*/

/* TIM1 ch1 (PWM) callback */
void tim1_ch1_callback(tim_driver_t *tdp)
{
    (void)(tdp);
    /* Blink USER_LED_A.*/
    gpio_toggle_pin(USER_LED_A);
}

/* TIM1 ch2 (ICU) callback */
void tim1_ch2_callback(tim_driver_t *tdp)
{
    (void)(tdp);
    /* Blink USER_LED_B.*/
    gpio_toggle_pin(USER_LED_B);
}

int main(void) {

    /* Enable interrupts.*/
    osal_sys_unlock();

    test_env_init((TestInit_t)
                  (TEST_INIT_CLOCK    |
                   TEST_INIT_GPIO     |
                   TEST_INIT_BOARD    |
                   TEST_INIT_IRQ      |
                   TEST_INIT_OSAL));

    /* Switch-off user leds.*/
    USER_LED_SWITCH_OFF(USER_LED_A);
    USER_LED_SWITCH_OFF(USER_LED_B);

    /* Initialize TIM driver instance.*/
    tim_init(&TIMD);

    /* Configure TIM driver.*/
    (void)tim_set_presc(&TIMD, TIM_DIVIDE_BY(tim_get_clock(&TIMD) / 100000U));
    (void)tim_set_ch_mode(&TIMD, 1U, TIM_MODE_PWM_ACTIVE_HIGH);
    (void)tim_set_ch_mode(&TIMD, 2U, TIM_MODE_ICU_FALLING_EDGE);
    (void)tim_set_cb(&TIMD, TIM_CB_OPS_CC1, tim1_ch1_callback);
    (void)tim_set_cb(&TIMD, TIM_CB_OPS_CC2, tim1_ch2_callback);

    /* Apply TIM configuration.*/
    tim_start(&TIMD);

    /* Enable TIM1 channel 1 and TIM1 channel 2 callbacks.*/
    tim_enable_channel_callback(&TIMD, 1U);
    tim_enable_channel_callback(&TIMD, 2U);

    /* Start PWM on TIM1 channel 1 (frequency 2Hz, duty cycle 50%) and ICU on
       TIM1 channel 2.*/
    tim_set_interval(&TIMD, 50000U);        /* 100kHz/50000 = 2Hz. */
    tim_set_comparator(&TIMD, 1U, 25000U);  /* Duty cycle 50%.     */

    /* Enable TIM.*/
    tim_enable(&TIMD);

    /* Application main loop */
    for ( ; ; ) {
        osal_delay_millisec(500U);
    }
}
