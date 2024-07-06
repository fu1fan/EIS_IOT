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
#include <cem.h>
#include <fccu.h>

/*===========================================================================*/
/* FCCU/CEM-related.                                                         */
/*===========================================================================*/

#define CEM_IN_FAULT_CHANNEL                0U
#define FCCU_FAULT_CHANNEL                  34U

/* This callback is invoked when FCCU config timeout occurs.*/
static void fccu_config_to_cb(fccu_driver_t *fdp) {

    (void)(fdp);

    for ( ; ; ) {
        osal_delay_millisec(250U);
    }
}

/* This function sets the FCCU driver parameters.*/
static void fccu_config(fccu_driver_t *fdp) {

    (void)fccu_set_config_to(fdp, FCCU_CFG_TO_8192US);
    (void)fccu_set_is_config_to_irq_en(fdp, true);
    (void)fccu_set_config_to_irq_cb(fdp, fccu_config_to_cb);
    (void)fccu_set_alarm_to(fdp, 10000000U);
    (void)fccu_set_delta_t(fdp, 5000U);
    fccu_set_ch_cfg(fdp, FCCU_FAULT_CHANNEL, true, FCCU_RF_CONFIG_SW, false, false, NULL, FCCU_REACTION_LONG_FUNCTIONAL, false, false);
}

int main(void)
{
    bool is_short_func_reset;
    bool is_long_func_reset;
    fccu_status_t status;
    uint8_t i;
    uint8_t fccu_ch_reg, fccu_ch_pos;

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

    /* Get functional reset flags to estabilish if the application is executed
       for the first time (power-on reset) or is re-executed after a functional
       reset arising by FCCU.*/
    is_short_func_reset = (bool)REG_READ_FIELD(RCC->FESR, RCC_FESR_F_FCCU_SHORT);
    is_long_func_reset = (bool)REG_READ_FIELD(RCC->FESR, RCC_FESR_F_FCCU_LONG);

    /* Initialize CEM driver instance.*/
    cem_init(&CEMD);

    /* Initialize FCCU driver instance.*/
    fccu_init(&FCCUD);

    /* If no functional reset arising by FCCU occurs:
       - clear FCCU
       - configure CEM
       - configure FCCU
       - inject fault 34 via CEM.*/
    if (is_short_func_reset == false &&
        is_long_func_reset  == false){

        /* Clear FCCU configuration.*/
        fccu_clear(&FCCUD);

        for(i = 0U; i < 6U; i++) {
            /* Blink USER_LED_A.*/
            gpio_toggle_pin(USER_LED_A);
            osal_delay_millisec(250U);
        }

        /* Apply CEM configuration.*/
        cem_start(&CEMD);

        /* Enable CEM input fault channel.*/
        cem_enable_fault(&CEMD, CEM_IN_FAULT_CHANNEL);

        /* Set FCCU configuration.*/
        fccu_config(&FCCUD);

        /* Apply FCCU configuration.*/
        fccu_start(&FCCUD);

        /* Inject fault on CEM input channel.*/
        cem_set_inject(&CEMD, CEM_IN_FAULT_CHANNEL);

    /* If functional reset arising by FCCU occurs:
       - get FCCU status
       - reset FCCU
       - check FCCU occurred faults.*/
    } else {

        /* Get FCCU status.*/
        status = fccu_get_status(&FCCUD);
        
        /* Clear injected fault on CEM input channel.*/
        cem_clear_inject(&CEMD, CEM_IN_FAULT_CHANNEL);

        /* Reset FCCU.*/
        fccu_reset(&FCCUD);

        /* Clears functional reset flags.*/
        RCC->FESR |= RCC_FESR_F_FCCU_SHORT;
        RCC->FESR |= RCC_FESR_F_FCCU_LONG;

        /* Calculate RF_S register related to FCCU channel.*/
        fccu_ch_reg = (FCCU_FAULT_CHANNEL / 32U);
        /* Calculate position in RF_S register related to FCCU channel.*/
        fccu_ch_pos = (FCCU_FAULT_CHANNEL % 32U);

        /* Check the FCCU has detected the correct fault.*/
        if ((status.rf_status_reg[fccu_ch_reg] & (1U << fccu_ch_pos)) != 0U) {
            for ( ; ; ) {
                /* Blink USER_LED_B.*/
                gpio_toggle_pin(USER_LED_B);
                osal_delay_millisec(250U);
            }
        }
    }

    /* Application main loop */
    for ( ; ; ) {
        osal_delay_millisec(250U);
    }
}
