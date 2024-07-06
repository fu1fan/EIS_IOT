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
#include <tim_ts.h>
#include <tim.h>
#include <dma.h>
#include <uart.h>
#include <io.h>
#include <stdio.h>

/*===========================================================================*/
/* UART-related.                                                             */
/*===========================================================================*/

void uart_config(uart_driver_t *udp) {

    (void)uart_set_prio(udp, IRQ_PRIORITY_5);
    (void)uart_set_baud(udp, UART_BAUDRATE_38400);
    (void)uart_set_presc(udp, UART_PRESCALER_DIV1);
    (void)uart_set_parity(udp, UART_PARITY_NONE);
    (void)uart_set_over(udp, UART_OVERSAMPLING_16);
    (void)uart_set_sbit(udp, UART_STOPBIT_1);
}

/*===========================================================================*/
/* TIM_TS-related.                                                           */
/*===========================================================================*/

void tim_ts_config(tim_ts_driver_t *tdp) {

    (void)tim_ts_enable_autoreload_preload(tdp, true);
    (void)tim_ts_enable_update_event_generation(tdp, true);
    (void)tim_ts_set_prescaler(tdp, TIM_DIVIDE_BY(CLOCK_TIMTSCLK / 100000U));
    (void)tim_ts_set_autoreload(tdp, 0xFFFFFFU);
}

/*===========================================================================*/
/* SDADC-related.                                                            */
/*===========================================================================*/

static void fifo_data_cb(sdadc_driver_t *sdp) {

    uint32_t timestamp;
    float value;

    (void)sdp;
    for(uint8_t i = 0U; i < 16U; i ++){
        (void)sdadc_read(sdp, &value);
        timestamp = sdadc_read_timestamp(sdp);

        value = (value / (float)32768U) * (sdadc_get_voltage(sdp) - sdadc_get_ground(sdp));

        printf("SDADC AN[%u]: %f TIM_TS:0x%.6lX\n\r", SDADC_ANCHSEL_0, value, timestamp);
    }
}

void sdadc_config(sdadc_driver_t *sdp) {

    (void)sdadc_set_ground(sdp, 0.0);
    (void)sdadc_set_voltage(sdp, 3.3);
    (void)sdadc_set_calibration(sdp, SDADC_CALIBRATION_BOTH);
    (void)sdadc_set_mode(sdp, SDADC_MODE_SINGLE_ENDED_INPUT);
    (void)sdadc_set_common_bias(sdp, SDADC_VCOMSEL_VREFN);
    (void)sdadc_set_channel(sdp, SDADC_ANCHSEL_0);
    (void)sdadc_set_pdr(sdp, SDADC_PDR_48);
    (void)sdadc_set_pgain(sdp, SDADC_PGAIN_1);
    (void)sdadc_set_osd(sdp, 255U);
    (void)sdadc_enable_fifo_full_event(sdp, 16U, fifo_data_cb);
    (void)sdadc_enable_timestamp(sdp);
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

    /* Set Global Bias Enable to use the SDADC.*/
    SYSCFG->ADC_BIASEN = SYSCFG_ADC_BIASEN_GLOBAL_BIAS_EN;

    /* Initialize UART driver instance.*/
    uart_init(&UARTD);

    /* Configure UART driver instance used for IO redirection.*/
    (void)uart_config(&UARTD);

    /* Initialize Runtime IO module.*/
    io_init(&UARTD);

    /* Start Runtime IO module.*/
    io_start();

    /* Initialize TIM_TS driver instance.*/
    tim_ts_init(&TIM_TSD);

    /* Configure TIM_TS driver instance.*/
    (void)tim_ts_config(&TIM_TSD);

    /* Initialize SDADC driver instance.*/
    sdadc_init(&SDADCD);

    /* Configure SDADC.*/
    sdadc_config(&SDADCD);

    /* Apply SDADC configurations.*/
    sdadc_start(&SDADCD);

    /* Apply TIM_TS configurations.*/
    tim_ts_start(&TIM_TSD);

    /* Start SDADC conversions.*/
    sdadc_start_conversion(&SDADCD);

    /* Application main loop */
    for ( ; ; ) {
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(500U);
    }
}
