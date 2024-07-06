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
#include <iwdg.h>

/*===========================================================================*/
/* IWDG-related.                                                             */
/*===========================================================================*/

/* This function sets the IWDG driver parameters.*/
static void iwdg_config(iwdg_driver_t *idp) {

    (void)iwdg_set_prescaler(idp, IWDG_PRESCALER_DIV256);
    (void)iwdg_set_window(idp, 0x7FFU);
    (void)iwdg_set_counter(idp, 0xFFFU);
}

/*===========================================================================*/
/* FCCU-related.                                                             */
/*===========================================================================*/

/* This function sets the FCCU driver parameters.*/
static void fccu_config(fccu_driver_t *fdp) {

	/* Enable functional reset request on IWDG1 (FCCU ch8).*/
    fccu_set_ch_cfg(fdp, 8U, true, FCCU_RF_CONFIG_SW, false, false, NULL, FCCU_REACTION_LONG_FUNCTIONAL, false, false);	
}

int main(void)
{

    bool is_short_func_reset;
    bool is_long_func_reset;

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

    /* Initialize FCCU driver instance.*/
    fccu_init(&FCCUD);

    /* Initialize WWDG driver instance.*/
    iwdg_init(&IWDGD);

    /* If a power-on reset occurs:
       - clear FCCU
       - configure FCCU
       - configure IWDG
       - enable IWDG
       - refresh IWDG counter within WWDG window to avoid the functional reset.*/
    if (is_short_func_reset == false &&
        is_long_func_reset  == false) {

        /* Clear FCCU configuration.*/
        fccu_clear(&FCCUD);

        /* Set IWDG configuration.*/
        iwdg_config(&IWDGD);

        /* Set FCCU configuration.*/
        fccu_config(&FCCUD);

        /* Apply FCCU configuration.*/
        fccu_start(&FCCUD);

        /* Apply IWDG configuration and enable IWDG.*/
        iwdg_start(&IWDGD);

        for ( ; ; ) {
            /* Blink USER_LED_A.*/
            gpio_toggle_pin(USER_LED_A);
            osal_delay_millisec(35U);

            /* Refresh IWDG counter whin WWDG window.*/
            iwdg_refresh(&IWDGD);
        }
    /* If functional reset arising by FCCU occurs:
       - reset FCCU.*/
    } else {
        /* Reset FCCU.*/
        fccu_reset(&FCCUD);

        /* Clears functional reset flags.*/
        RCC->FESR |= RCC_FESR_F_FCCU_SHORT;
        RCC->FESR |= RCC_FESR_F_FCCU_LONG;

        for ( ; ; ) {
            /* Blink USER_LED_B.*/
            gpio_toggle_pin(USER_LED_B);
            osal_delay_millisec(250U);
        }
    }
}
