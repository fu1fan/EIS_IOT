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
#include <exti.h>
#include <comp.h>

/*===========================================================================*/
/* COMP-related.                                                             */
/*===========================================================================*/

/* COMP output pin configuration.*/
#define COMP_OUTPUT gpio_iopack(GPIO_PORT_C, GPIO_PIN_5)
#define COMP_OUTPUT_CFG                          \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_LOW |                     \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(8U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_DISABLED |             \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

/* This callback is invoked when on rising or falling edge of COMP output.*/
static void comp_comp_cb(comp_driver_t *cdp) {

    (void)cdp;
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

    /* Enable COMP Bias + select 1/3*(VREH-VREFL) as VREF for self-test.*/
    SYSCFG->ST_VREF_SEL1  = SYSCFG_ST_VREF_SEL1_COMPBIAS_EN |
                            SYSCFG_ST_VREF_SEL1_COMP_TESTVREF_SEL_0;
    /* Enable self-test.*/
    SYSCFG->ST_VREF_SEL1 |= SYSCFG_ST_VREF_SEL1_SELFTEST_EN ;
    
    /* Configure COMP output pin.*/
    gpio_set_pin_mode(COMP_OUTPUT, COMP_OUTPUT_CFG);

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

    /* Enable TIM.*/
    tim_enable(&TIMD);

    /* Initialize COMP driver instance.*/
    comp_init(&COMPD);

    /* Configure COMP.*/
    (void)comp_set_prio(&COMPD, IRQ_PRIORITY_10);
    (void)comp_set_input_plus(&COMPD, COMP_COMP1_INPUT_PLUS_PB2);
    (void)comp_set_input_minus(&COMPD, COMP_COMP1_INPUT_MINUS_TEST_VREF);
    (void)comp_set_hysteresis(&COMPD, COMP_HYSTERESIS_NONE);
    (void)comp_set_output_polarity(&COMPD, COMP_OUTPOL_NOTINVERTED);
    (void)comp_set_blank_sel(&COMPD, COMP_COMP1_BLANKSRC_NONE);
    (void)comp_set_deglitch(&COMPD, false);
    (void)comp_set_lock(&COMPD, false);
    (void)comp_set_triggermode(&COMPD, COMP_TRIGGERMODE_INT_BOTH);
    (void)comp_set_cb(&COMPD, comp_comp_cb);

    /* Apply COMP instance configuration.*/
    comp_start(&COMPD);

    /* Enable COMP.*/
    comp_enable(&COMPD);

    /* Application main loop */
    for ( ; ; ) {
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(250U);
    }
}
