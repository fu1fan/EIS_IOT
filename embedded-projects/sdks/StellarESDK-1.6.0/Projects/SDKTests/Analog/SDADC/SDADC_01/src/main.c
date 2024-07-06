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
#include <sdadc.h>
#include <dma.h>
#include <uart.h>
#include <io.h>
#include <stdio.h>

/*===========================================================================*/
/* SDADC-related.                                                            */
/*===========================================================================*/

static bool data_out_of_range = false;

/* Watchdog event callback. It is invoked when the converted data is less than
   low threshould or greter than high threshold.*/
static void sdadc_watchdog_cb(sdadc_driver_t *sdp)
{
    (void)sdp;
    data_out_of_range = true;
}

int main(void) {

    bool data_valid = false;
    float data = 0.0;
    float calculated_data = 0.0;

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

    /* Set Global Bias Enable to use the SDADC.*/
    SYSCFG->ADC_BIASEN = SYSCFG_ADC_BIASEN_GLOBAL_BIAS_EN;

    /* Initialize SDADC driver instance.*/
    sdadc_init(&SDADCD);

    /* Configure SDADC.*/
    (void)sdadc_set_ground(&SDADCD, 0.0);
    (void)sdadc_set_voltage(&SDADCD, 3.3);
    (void)sdadc_set_calibration(&SDADCD, SDADC_CALIBRATION_BOTH);
    (void)sdadc_set_mode(&SDADCD, SDADC_MODE_SINGLE_ENDED_INPUT);
    (void)sdadc_set_common_bias(&SDADCD, SDADC_VCOMSEL_VREFN);
    (void)sdadc_set_channel(&SDADCD, SDADC_ANCHSEL_0);
    (void)sdadc_set_pdr(&SDADCD, SDADC_PDR_48);
    (void)sdadc_set_pgain(&SDADCD, SDADC_PGAIN_1);
    (void)sdadc_set_osd(&SDADCD, 255U);
    (void)sdadc_enable_fifo_overwrite(&SDADCD);
    sdadc_enable_watchdog_event(&SDADCD, 1.0, 3.0, sdadc_watchdog_cb);

    /* Apply SDADC configurations.*/
    sdadc_start(&SDADCD);

    /* Initialize UART driver instance used for IO redirection.*/
    uart_init(&UARTD);

    /* Configure UART driver instance used for IO redirection.*/
    (void)uart_set_prio(&UARTD, IRQ_PRIORITY_5);
    (void)uart_set_baud(&UARTD, UART_BAUDRATE_38400);
    (void)uart_set_presc(&UARTD, UART_PRESCALER_DIV1);
    (void)uart_set_parity(&UARTD, UART_PARITY_NONE);
    (void)uart_set_over(&UARTD, UART_OVERSAMPLING_16);
    (void)uart_set_sbit(&UARTD, UART_STOPBIT_1);

    /* Initialize Runtime IO module.*/
    io_init(&UARTD);

    /* Start Runtime IO module.*/
    io_start();

    /* Start SDADC conversions.*/
    sdadc_start_conversion(&SDADCD);

    /* Application main loop */
    for ( ; ; ) {
        /* Read and print converted value and check if it is out of range.*/
        data_valid = sdadc_read(&SDADCD, &data);
        if (data_valid == true) {
            calculated_data = (data / (float)32768U) * (sdadc_get_voltage(&SDADCD) - sdadc_get_ground(&SDADCD));
            printf("SDADC AN[%u]: %f", SDADC_ANCHSEL_0, calculated_data);
            if (data_out_of_range == true) {
                printf(" --> out of range!\n\r");
                data_out_of_range = false;
            } else {
                printf("\n\r");
            }
        }
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(500U);
    }
}

