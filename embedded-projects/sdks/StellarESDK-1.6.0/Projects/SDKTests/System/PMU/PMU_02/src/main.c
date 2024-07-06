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
#include <pmu.h>
#include <fccu.h>

/* GPIO pin used to driver the SAR ADC supply.*/
#define SAR_ADC_SUPPLY gpio_iopack(GPIO_PORT_A, GPIO_PIN_0)
#define SAR_ADC_SUPPLY_CFG                       \
    (GPIO_MODE_MODER_OUTPUT |                    \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(0U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

/*===========================================================================*/
/* FCCU-related.                                                             */
/*===========================================================================*/

/* This callback is invoked when FCCU alarm timeout for fault 1 occurs.*/
static void fccu_ch1_alarm_to_cb(fccu_driver_t *fdp) {

    /* Set SAR ADC supply high.*/
    gpio_set_pin(SAR_ADC_SUPPLY);

    /* Clear FCCU configuration.*/
    fccu_clear(fdp);

    /* Blink USER_LED_B.*/
    gpio_toggle_pin(USER_LED_B);
}

/* This function sets the FCCU driver parameters.*/
static void fccu_config(fccu_driver_t *fdp) {

    (void)fccu_set_alarm_to(fdp, 10000000U);

	/* Configure fault 1.*/
    fccu_set_ch_cfg(fdp, 1U, true, FCCU_RF_CONFIG_SW, true, true, fccu_ch1_alarm_to_cb, FCCU_REACTION_NO_RESET, false, false);
}

int main(void)
{
    uint32_t i = 1U;

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

    /* Configure GPIO pin used to driver the SAR ADC supply.*/
    gpio_set_pin_mode(SAR_ADC_SUPPLY, SAR_ADC_SUPPLY_CFG);
    gpio_set_pin(SAR_ADC_SUPPLY);

    /* Initialize FCCU driver instance.*/
    fccu_init(&FCCUD);

    /* Clear FCCU configuration.*/
    fccu_clear(&FCCUD);

    /* Set FCCU configuration.*/
    fccu_config(&FCCUD);

    /* Apply FCCU configuration.*/
    fccu_start(&FCCUD);

    /* Initialize PMU driver instance.*/
    pmu_init(&PMUD);

    /* Configure PMU driver instance.*/
    (void)pmu_set_reset(&PMUD, PMU_VD_LVD290_AS, PMU_RES_NO_RESET);
    (void)pmu_enable_interrupt(&PMUD, PMU_VD_LVD290_AS, false);
    (void)pmu_enable_fccu(&PMUD, PMU_VD_LVD290_AS, true);

    /* Apply PMU driver instance configuration.*/
    pmu_start(&PMUD);

    /* Application main loop */
    for ( ; ; ) {
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(250U);

        /* Remove SAR ADC supply each 1s.*/
        if (i % 4U == 0U) {
            /* Set SAR ADC supply low.*/
            gpio_clear_pin(SAR_ADC_SUPPLY);
        }
        i++;
    }
}
