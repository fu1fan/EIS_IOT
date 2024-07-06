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
#include <tim.h>
#include <io.h>
#include <stdio.h>

volatile uint8_t reg_conv_completed = 1;

/*===========================================================================*/
/* ADC-related.                                                              */
/*===========================================================================*/

#define ADC_REG_NCHANNELS         3U
#define ADC_INJ_NCHANNELS         2U

static adc_sample_t adc_reg_buffer[ADC_REG_NCHANNELS];
static adc_sample_t adc_inj_buffer[ADC_INJ_NCHANNELS];

/* This callback is not used, it is here as an example, it can be replaced
   by a NULL in the structure.*/
static void adc_error_cb(adc_driver_t *adp, adc_err_t err) {

    (void)adp;
    (void)err;
}

/* Regular conversion data callback.*/
static void adc_reg_data_cb(adc_driver_t *adp) {

    uint8_t i = 0U;

    (void)adp;

    /* ADC conversion completed, data transmitting via UART.*/
    for (i = 0U; i < ADC_REG_NCHANNELS; i++) {
        printf("ADC(REG)-CH%d = 0x%02X\n\r", i, adc_reg_buffer[i]);
    }
    reg_conv_completed = 1;
}

/* Injectec conversion data callback.*/
static void adc_inj_data_cb(adc_driver_t *adp) {

    uint8_t i = 0U;

    (void)adp;

    /* ADC conversion completed, data transmitting via UART.*/
    for (i = 0U; i < ADC_INJ_NCHANNELS; i++) {
        printf("ADC(INJ)-CH%d = 0x%02X\n\r", i, adc_inj_buffer[i]);
    }
}

int main(void) {

    adc_dma_conf_t adc_dma_conf;
    uint8_t reg_seq[ADC_REG_NCHANNELS] = {ADC_CH_IN1, ADC_CH_IN2, ADC_CH_IN3};
    uint8_t inj_seq[ADC_INJ_NCHANNELS] = {ADC_CH_IN4, ADC_CH_IN5};

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
    /* Configure ADC regular conversions (single).*/
    (void)adc_set_reg_mode(&ADCD, ADC_REG_SINGLE);
    adc_set_seq(&ADCD, ADC_REG_CONV, ADC_REG_NCHANNELS, reg_seq, adc_reg_buffer);
    /* Configure ADC injected conversions (triggered).*/
    adc_set_seq(&ADCD, ADC_INJ_CONV, ADC_INJ_NCHANNELS, inj_seq, adc_inj_buffer);
    adc_set_ext(&ADCD, ADC_INJ_CONV, 0U, ADC_EXT_POL_RISING);
    /* Configure ADC callbacks.*/
    (void)adc_set_op_cb(&ADCD, ADC_OP_REG_CONV, adc_reg_data_cb);
    (void)adc_set_op_cb(&ADCD, ADC_OP_INJ_CONV, adc_inj_data_cb);
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

    /* Initialize TIM driver instance used as trigger for ADC injected
       conversions.*/
    tim_init(&TIMD);
    (void)tim_set_presc(&TIMD, TIM_DIVIDE_BY(tim_get_clock(&TIMD) / 10000U));
    (void)tim_set_ch_mode(&TIMD, 1U, TIM_MODE_PWM_ACTIVE_HIGH);
    (void)tim_set_trgo(&TIMD, TIM_TRGO_UPDATE);

    /* Apply TIM configuration.*/
    tim_start(&TIMD);

    /* Start PWM on TIM1 channel 1 (frequency 0.2Hz, duty cycle 50%).*/
    tim_set_interval(&TIMD, 50000U);        /* 10kHz/500000 = 0.2Hz. */
    tim_set_comparator(&TIMD, 1U, 25000U);  /* Duty cycle 50%.       */

    /* Enable TIM.*/
    tim_enable(&TIMD);

    /* Start ADC injected conversions (triggered by TIM1 ch1).*/
    adc_start_injected(&ADCD);

    /* Application main loop */
    for ( ; ; ) {
        if (reg_conv_completed == 1) {
            reg_conv_completed = 0;
            /* Start ADC regular conversion (single).*/
            adc_start_regular(&ADCD);
        } else {
            ; /* close "if" as required by MISRA */
        }
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(500U);
    }
}
