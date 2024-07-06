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
#include <dma.h>
#include <saradc.h>
#include <uart.h>
#include <io.h>
#include <stdio.h>

volatile uint8_t reg_conv_completed = 1U;

/*===========================================================================*/
/* ADC-related.                                                              */
/*===========================================================================*/

#define ADC_REG_NCHANNELS         1U
#define ADC_DATA_COUNT            5U

static adc_sample_t adc_reg_buffer[ADC_REG_NCHANNELS];
static uint8_t adc_sample_num = 0U;
static uint32_t adc_data_avg = 0U;

/* This callback is not used, it is here as an example, it can be replaced
   by a NULL in the structure.*/
static void adc_error_cb(adc_driver_t *adp, adc_err_t err) {

    (void)adp;
    (void)err;
}

/* Regular conversion data callback.*/
static void adc_reg_data_cb(adc_driver_t *adp) {

    adc_data_avg += (uint32_t)adc_reg_buffer[0];
    adc_sample_num++;
    if (adc_sample_num == ADC_DATA_COUNT) {
        /* After a number of continuos conversions equal to ADC_DATA_COUNT, the
           continuos conversion is stopped, the average value is calculated and
           transmitted via UART.*/
        adc_stop_regular(adp);
        reg_conv_completed = 1U;
        adc_sample_num = 0U;
        adc_data_avg = (adc_data_avg / ADC_DATA_COUNT);
        printf("ADC(REG)-CH%d = 0x%02X\n\r", ADC_CH_IN1, (adc_sample_t)adc_data_avg);
        adc_data_avg = 0U;
    }

}

int main(void) {

    adc_dma_conf_t adc_dma_conf;
    uint8_t reg_seq[ADC_REG_NCHANNELS] = {ADC_CH_IN1};

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

    /* Set Global Bias Enable to use the SARADC.*/
    SYSCFG->ADC_BIASEN = SYSCFG_ADC_BIASEN_GLOBAL_BIAS_EN;

    /* Initialize ADC driver instance.*/
    adc_init(&ADCD);
    /* Configure ADC resolution.*/
    (void)adc_set_data_res(&ADCD, ADC_DATA_RES_8);
    /* Configure ADC DMA for regular conversions.*/
    adc_dma_conf.dma_stream_id = DMA_STREAM_ID_ANY;
    adc_dma_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    adc_dma_conf.dma_stream_irq_prio = IRQ_PRIORITY_10;
    adc_set_dma_conf(&ADCD, &adc_dma_conf);
    /* Configure ADC regular conversions (continuos).*/
    (void)adc_set_reg_mode(&ADCD, ADC_REG_CONTINUOUS);
    adc_set_seq(&ADCD, ADC_REG_CONV, ADC_REG_NCHANNELS, reg_seq, adc_reg_buffer);
    /* Configure ADC callbacks.*/
    (void)adc_set_op_cb(&ADCD, ADC_OP_REG_CONV, adc_reg_data_cb);
    (void)adc_set_err_cb(&ADCD, ADC_ERR_DMA, adc_error_cb);

    /* Apply ADC configuration.*/
    adc_start(&ADCD);

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

    /* Application main loop */
    for ( ; ; ) {
        if (reg_conv_completed == 1U) {
            reg_conv_completed = 0U;
            /* Start ADC continuous conversion.*/
            adc_start_regular(&ADCD);
        }
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(500U);
    }
}

