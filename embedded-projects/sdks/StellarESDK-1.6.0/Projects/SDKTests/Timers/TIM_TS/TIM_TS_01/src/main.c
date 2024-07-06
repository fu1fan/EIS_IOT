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
#include <tim_ts.h>

/*===========================================================================*/
/* TIM_TS-related.                                                           */
/*===========================================================================*/

static void tim_ts_cb(tim_ts_driver_t *tdp) {

    (void)tdp;

    /* Blink USER_LED_A.*/
    gpio_toggle_pin(USER_LED_A);
}

void tim_ts_config(tim_ts_driver_t *tdp) {

    (void)tim_ts_enable_dithering(tdp, false);
    (void)tim_ts_enable_uif_remapping(tdp, false);
    (void)tim_ts_enable_autoreload_preload(tdp, true);
    (void)tim_ts_enable_one_pulse_mode(tdp, false);
    (void)tim_ts_set_source_ofuf_only(tdp, true);
    (void)tim_ts_enable_update_event_generation(tdp, true);
    (void)tim_ts_set_master_mode_selection(tdp, TIM_TS_MMS_UPDATE);
    (void)tim_ts_set_prescaler(tdp, 65535U);
    (void)tim_ts_set_autoreload(tdp, 2289U);
    (void)tim_ts_set_cb(tdp, tim_ts_cb);
    (void)tim_ts_set_prio(tdp, IRQ_PRIORITY_5);
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

    /* Switch-off user led.*/
    USER_LED_SWITCH_OFF(USER_LED_A);

    /* Initialize TIM_TS driver instance.*/
    tim_ts_init(&TIM_TSD);

    /* Configure TIM_TS driver instance.*/
    tim_ts_config(&TIM_TSD);

    /* Apply TIM_TS configuration.*/
    tim_ts_start(&TIM_TSD);

    /* Application main loop */
    for ( ; ; ) { }
}
