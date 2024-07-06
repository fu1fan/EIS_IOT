/****************************************************************************
 *
 * Copyright (c) 2023 STMicroelectronics - All Rights Reserved
 *
 * License terms: STMicroelectronics Proprietary in accordance with licensing
 * terms SLA0098 at www.st.com.
 *
 * THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
 * INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *****************************************************************************/

#include <test_env.h>
#include <fccu.h>
#include <cmu.h>

/*===========================================================================*/
/* FCCU-related.                                                             */
/*===========================================================================*/

/* This callback is invoked when FCCU config timeout occurs.*/
static void fccu_to_cb(fccu_driver_t *fdp) {

    (void)(fdp);

    for ( ; ; ) {
        osal_delay_millisec(250U);
    }
}

/* This callback is invoked when FCCU alarm timeout for CMU_0_OSC occurs.*/
static void fccu_ch14_alarm_to_cb(fccu_driver_t *fdp) {

    (void)cmu_get_event(&CMU1D);

    fccu_clear_fault(fdp, 14U);
}

/* This callback is invoked when FCCU alarm timeout for CMU_0_PLL0 occurs.*/
static void fccu_ch15_alarm_to_cb(fccu_driver_t *fdp) {

    (void)cmu_get_event(&CMU1D);

    fccu_clear_fault(fdp, 15U);
}

/* This callback is invoked when FCCU alarm timeout for CMU_Sysclk occurs.*/
static void fccu_ch16_alarm_to_cb(fccu_driver_t *fdp) {

    cmu_int_t event;

    event = cmu_get_event(&CMU1D);

    cmu_stop(&CMU1D);

    cmu_clear_event(&CMU1D, event);

    fccu_clear_fault(fdp, 16U);

    /* Blink USER_LED_B.*/
    gpio_toggle_pin(USER_LED_B);
}

/* This callback is invoked when FCCU alarm timeout for CMU_other occurs.*/
static void fccu_ch17_alarm_to_cb(fccu_driver_t *fdp) {

    (void)cmu_get_event(&CMU1D);

    fccu_clear_fault(fdp, 17U);
}

/* This function sets the FCCU driver parameters.*/
static void fccu_config(fccu_driver_t *fdp) {

    (void)fccu_set_config_to(fdp, FCCU_CFG_TO_8192US);
    (void)fccu_set_is_config_to_irq_en(fdp, true);
    (void)fccu_set_config_to_irq_cb(fdp, fccu_to_cb);
    (void)fccu_set_alarm_to(fdp, 10000000U);
    (void)fccu_set_delta_t(fdp, 5000U);

    /* Configuration for XOSC less than IRC CMU fault.*/
    fccu_set_ch_cfg(fdp, 14U, true, FCCU_RF_CONFIG_SW, true, true, fccu_ch14_alarm_to_cb, FCCU_REACTION_NO_RESET, false, false);
    /* Configuration for PLL0 out of frequency CMU fault.*/
    fccu_set_ch_cfg(fdp, 15U, true, FCCU_RF_CONFIG_SW, true, true, fccu_ch15_alarm_to_cb, FCCU_REACTION_NO_RESET, false, false);
    /* Configuration for Sysclk frequency out of range CMU fault.*/
    fccu_set_ch_cfg(fdp, 16U, true, FCCU_RF_CONFIG_SW, true, true, fccu_ch16_alarm_to_cb, FCCU_REACTION_NO_RESET, false, false);
    /* Configuration for other internal clock CMU faults.*/
    fccu_set_ch_cfg(fdp, 17U, true, FCCU_RF_CONFIG_SW, true, true, fccu_ch17_alarm_to_cb, FCCU_REACTION_NO_RESET, false, false);
}

/*===========================================================================*/
/* CMU-related.                                                              */
/*===========================================================================*/

/* This function sets the CMU driver parameters.*/
static void cmu_config(cmu_driver_t *cdp) {

    cmu_set_high_freq_ref(cdp, 0);
    cmu_set_low_freq_ref(cdp, 0);
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

    /* Initialize FCCU driver instance.*/
    fccu_init(&FCCUD);

    /* Clear FCCU configuration.*/
    fccu_clear(&FCCUD);

    /* Set FCCU configuration.*/
    fccu_config(&FCCUD);

    /* Apply FCCU configuration.*/
    fccu_start(&FCCUD);

    /* Initialize CMU instance.*/
    cmu_init(&CMU1D);

    /* Set CMU configuration.*/
    cmu_config(&CMU1D);

    /* Apply CMU configuration.*/
    cmu_start(&CMU1D);

    /* Application main loop */
    for ( ; ; ) {
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(500U);
    }
}
