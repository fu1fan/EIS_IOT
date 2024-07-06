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

#include <main.h>
#include <test_env.h>
#include <fccu.h>
#include <wwdg.h>

/*===========================================================================*/
/* WWDG-related.                                                             */
/*===========================================================================*/

/* This function sets the WWDG driver parameters.*/
static void wwdg_config(wwdg_driver_t *wdp) {

    (void)wwdg_set_prescaler(wdp, WWDG_PRESCALER_DIV128);
    (void)wwdg_set_window(wdp, 0x5FU);
    (void)wwdg_set_counter(wdp, 0x7FU);
}

/*===========================================================================*/
/* FCCU-related.                                                             */
/*===========================================================================*/

/* This function sets the FCCU driver parameters.*/
static void fccu_config(fccu_driver_t *fdp) {

	/* Enable functional reset request on FCCU channel related to WWDG.*/
    fccu_set_ch_cfg(fdp,
                    WWDG_FCCU_CHANNEL,
                    true,
                    FCCU_RF_CONFIG_SW,
                    false,
                    false,
                    NULL,
                    FCCU_REACTION_LONG_FUNCTIONAL,
                    false,
                    false);
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
    USER_LED_SWITCH_OFF(USER_LED_B);;

    /* Get functional reset flags to estabilish if the application is executed
       for the first time (power-on reset) or is re-executed after a functional
       reset arising by FCCU.*/
    is_short_func_reset = (bool)REG_READ_FIELD(RCC->FESR, RCC_FESR_F_FCCU_SHORT);
    is_long_func_reset = (bool)REG_READ_FIELD(RCC->FESR, RCC_FESR_F_FCCU_LONG);

    /* Initialize FCCU driver instance.*/
    fccu_init(&FCCUD);

    /* Initialize WWDG driver instance.*/
    wwdg_init(&WWDGD);

    /* If a power-on reset occurs:
       - clear FCCU
       - configure FCCU
       - configure WWDG
       - enable WWDG
       - refresh WWDG counter within WWDG window to avoid the functional reset.*/
    if (is_short_func_reset == false &&
        is_long_func_reset  == false) {

        /* Clear FCCU configuration.*/
        fccu_clear(&FCCUD);

        /* Set WWDG configuration.*/
        wwdg_config(&WWDGD);

        /* Set FCCU configuration.*/
        fccu_config(&FCCUD);

        /* Apply FCCU configuration.*/
        fccu_start(&FCCUD);

        /* Apply WWDG configuration and enable WWDG.*/
        wwdg_start(&WWDGD);

        for ( ; ; ) {
            /* Blink USER_LED_A.*/
            gpio_toggle_pin(USER_LED_A);
            osal_delay_millisec(230U);

            /* Refresh WWDG counter whin WWDG window.*/
            wwdg_refresh(&WWDGD);
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
