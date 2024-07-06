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
/* PMU-related.                                                              */
/*===========================================================================*/

/* This callback is invoked when SAR ADC supply < 2.90V.*/
void pmu_lvd290_as(pmu_driver_t *pdp) {

    (void)pdp;

    /* Set SAR ADC supply high.*/
    gpio_set_pin(SAR_ADC_SUPPLY);

    /* Blink USER_LED_B.*/
    gpio_toggle_pin(USER_LED_B);
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

    /* Initialize PMU driver instance.*/
    pmu_init(&PMUD);

    /* Configure PMU driver instance.*/
    (void)pmu_set_reset(&PMUD, PMU_VD_LVD290_AS, PMU_RES_NO_RESET);
    (void)pmu_enable_interrupt(&PMUD, PMU_VD_LVD290_AS, true);
    (void)pmu_enable_fccu(&PMUD, PMU_VD_LVD290_AS, false);
    (void)pmu_set_cb(&PMUD, PMU_VD_LVD290_AS, pmu_lvd290_as);

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
