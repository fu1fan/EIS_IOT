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
#include <exti.h>
#include <tim.h>

/*===========================================================================*/
/* EXTI-related.                                                             */
/*===========================================================================*/

/* GPIO pin configuration used as EXTI.*/
#define EXTI_IN gpio_iopack(GPIO_PORT_B, GPIO_PIN_1)
#define EXTI_IN_CFG                              \
    (GPIO_MODER_INPUT |                          \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_LOW |                     \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(0U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_DISABLED |             \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

void exti_line1_callback(void) {

    /* Toggle USER_LED_B.*/
    gpio_toggle_pin(USER_LED_B);
}

int main(void)
{
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

    /* Apply TIM configuration.*/
    tim_start(&TIMD);

    /* Start PWM on TIM1 channel 1 (frequency 2Hz, duty cycle 50%).*/
    tim_set_interval(&TIMD, 50000U);        /* 100kHz/50000 = 2Hz. */
    tim_set_comparator(&TIMD, 1U, 25000U);  /* Duty cycle 50%.     */

    /* Configure EXTI pin.*/
    gpio_set_pin_mode(EXTI_IN, EXTI_IN_CFG);

    /* Configure EXTI.*/
    exti_set_line(EXTI_LINE_1, EXTI_MODE_INTERRUPT, EXTI_TRIGGER_BOTH);
    exti_set_port(EXTI_LINE_1, EXTI_PORT_B);
    exti_set_irq(EXTI_LINE_1, IRQ_MIN_PRIORITY, exti_line1_callback);

    /* Enable TIM.*/
    tim_enable(&TIMD);

    /* Application main loop */
    for ( ; ; ) {
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(250U);
    }
}

