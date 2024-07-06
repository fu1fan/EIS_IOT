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
#include <hrtim.h>

/*===========================================================================*/
/* HRTIM-related.                                                            */
/*===========================================================================*/

#define HRTIM_PERIOD_TICK_NUM               37500U
#define HRTIM_TIMER_F_DTR_TICK_NUM          469U
#define HRTIM_TIMER_F_DTF_TICK_NUM          469U

/* HRTIM Master callback.*/
void hrtim_master_callback(hrtim_driver_t *hdp, uint32_t events)
{
    (void)(hdp);
    (void)(events);

    /* Blink USER_LED_A.*/
    gpio_toggle_pin(USER_LED_A);
}

/* HRTIM TimerF callback.*/
void hrtim_timer_callback(hrtim_driver_t *hdp, uint32_t events)
{
    (void)(hdp);
    (void)(events);

    /* Blink USER_LED_B.*/
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

    /* Initialize HRTIM driver instance.*/
    hrtim_init(&HRTIMD);

    /* Configure HRTIM Master: mode = continuos, fHRTIM = 300MHz, prescaler =
       = div4 => fHRCK = 75MHz => period = HRTIM_PERIOD_TICK_NUM/fHRCK = 500us,
       cmp1 = period/2 = 250us.*/
    (void)hrtim_set_timer_mode(&HRTIMD, HRTIM_TIMER_MASTER, HRTIM_MODE_CONTINUOS);
    (void)hrtim_timer_set_prescaler(&HRTIMD, HRTIM_TIMER_MASTER, HRTIM_PRSC_DIV4);
    (void)hrtim_timer_set_period(&HRTIMD, HRTIM_TIMER_MASTER, HRTIM_PERIOD_TICK_NUM);
    (void)hrtim_timer_set_cmp(&HRTIMD, HRTIM_TIMER_MASTER, HRTIM_CMP1, HRTIM_PERIOD_TICK_NUM / 2U);

    /* Configure HRTIM TimerF: mode = continuos, fHRTIM = 300MHz, prescaler =
       = div4 => fHRCK = 75MHz => period = HRTIM_PERIOD_TICK_NUM/fHRCK = 500us,
       cmp1 = period/2 = 250us.*/
    (void)hrtim_set_timer_mode(&HRTIMD, HRTIM_TIMER_F, HRTIM_MODE_CONTINUOS);
    (void)hrtim_timer_set_prescaler(&HRTIMD, HRTIM_TIMER_F, HRTIM_PRSC_DIV4);
    (void)hrtim_timer_set_period(&HRTIMD, HRTIM_TIMER_F, HRTIM_PERIOD_TICK_NUM);
    (void)hrtim_timer_set_cmp(&HRTIMD, HRTIM_TIMER_F, HRTIM_CMP1, HRTIM_PERIOD_TICK_NUM / 2U);

    /* Configure HRTIM TimerF deadtime: fHRTIM = 300MHz, deadtime prescaler =
       = mul16 => tDTG = tHRTIM * 16 = 53.33ns => dead time rising edge =
       HRTIM_TIMER_F_DTR_TICK_NUM * tDTG = 25us, dead time falling edge =
       HRTIM_TIMER_F_DTF_TICK_NUM * tDTG = 25us.*/
    hrtim_timer_set_deadtime(&HRTIMD, HRTIM_TIMER_F, HRTIM_DT_PRSC_MUL16,
                             HRTIM_TIMER_F_DTR_TICK_NUM, HRTIM_DT_SIGN_POSITIVE,
                             HRTIM_TIMER_F_DTF_TICK_NUM, HRTIM_DT_SIGN_POSITIVE);
    hrtim_timer_enable_deadtime(&HRTIMD, HRTIM_TIMER_F);

    /* Configure HRTIM TimerF outputs: rising edge on cmp1, falling edge on
       period.*/
    (void)hrtim_timer_set_outputset(&HRTIMD, HRTIM_TIMER_F, HRTIM_TIMER_OUTPUT_1, HRTIM_OUTPUT_TIMCMP1);
    (void)hrtim_timer_set_outputrst(&HRTIMD, HRTIM_TIMER_F, HRTIM_TIMER_OUTPUT_1, HRTIM_OUTPUT_TIMPER);

    /* Apply HRTIM driver instance configuration.*/
    hrtim_start(&HRTIMD);

    /* Configure and enable HRTIM Master and TimerF repetition interrupt
       callbacks.*/
    (void)hrtim_set_cb(&HRTIMD, HRTIM_CB_MASTER, hrtim_master_callback);
    (void)hrtim_set_cb(&HRTIMD, HRTIM_CB_TIMER_F, hrtim_timer_callback);
    hrtim_enable_cb(&HRTIMD, HRTIM_CB_MASTER, HRTIM_CB_EVENT_REP);
    hrtim_enable_cb(&HRTIMD, HRTIM_CB_TIMER_F, HRTIM_CB_EVENT_REP);

    /* Configure HRTIM Master and TimerF repetition values to 200. This triggers
       an interrupt each 200 * period = 100ms.*/
    hrtim_timer_set_repetition(&HRTIMD, HRTIM_TIMER_MASTER, 199U);
    hrtim_timer_set_repetition(&HRTIMD, HRTIM_TIMER_F, 199U);

    /* Enable HRTIM Master and TimerF.*/
    (void)hrtim_timer_enable_output(&HRTIMD, HRTIM_TIMER_F_OUT1_MASK_ID | HRTIM_TIMER_F_OUT2_MASK_ID);
    (void)hrtim_timer_enable(&HRTIMD, HRTIM_TIMER_F_MASK_ID | HRTIM_TIMER_MASTER_MASK_ID);

    /* Application main loop */
    for ( ; ; ) {
        osal_delay_millisec(250U);
    }
}
