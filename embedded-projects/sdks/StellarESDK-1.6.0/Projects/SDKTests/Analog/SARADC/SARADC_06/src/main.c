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
#include <dma.h>
#include <saradc.h>
#include <uart.h>
#include <tim.h>
#include <io.h>
#include <stdio.h>

/*===========================================================================*/
/* ADC-related.                                                              */
/*===========================================================================*/

#define ADC_REG_NCHANNELS         2U
#define ADC_INJ_NCHANNELS         2U

volatile uint8_t reg_conv_completed = 1U;
volatile uint8_t inj_conv_completed = 1U;

static adc_sample_t adc1_reg_buff[ADC_REG_NCHANNELS];
static adc_sample_t adc2_reg_buff[ADC_REG_NCHANNELS];
static adc_sample_t  adc_inj_buff[ADC_INJ_NCHANNELS];
static uint32_t  dual_mode_buffer[ADC_REG_NCHANNELS];

/* This callback is not used, it is here as an example, it can be replaced
   by a NULL in the structure.*/
static void adc_error_cb(adc_driver_t *adp, adc_err_t err) {

    (void)adp;
    (void)err;
}

/* ADC1-2 dual conversion callback.*/
static void dual_conv_cb(adc_driver_t *adp) {

    uint8_t i;
    (void)adp;

    for(i = 0U; i < ADC_REG_NCHANNELS; i++) {
        printf("ADC dual mode [%d] = 0x%08lX\n\r", i, dual_mode_buffer[i]);
    }

    reg_conv_completed = 1U;
}

/* Injectec conversion data callback.*/
static void adc_inj_data_cb(adc_driver_t *adp) {

    uint8_t i = 0U;

    (void)adp;

    /* ADC conversion completed, data transmitting via UART.*/
    for (i = 0U; i < ADC_INJ_NCHANNELS; i++) {
        printf("ADC injected [%d] = 0x%02X\n\r", i + 3U, adc_inj_buff[i]);
    }

    inj_conv_completed = 1U;
}

int main(void) {

    adc_dma_conf_t adc_dma_conf;

    uint8_t reg_seq1[ADC_REG_NCHANNELS] = {ADC_CH_IN1, ADC_CH_IN2};
    uint8_t reg_seq2[ADC_REG_NCHANNELS] = {ADC_CH_IN2, ADC_CH_IN1};
    uint8_t  inj_seq[ADC_INJ_NCHANNELS] = {ADC_CH_IN3, ADC_CH_IN4};

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

    /* Set Global Bias Enable to use the SARADC.*/
    SYSCFG->ADC_BIASEN = SYSCFG_ADC_BIASEN_GLOBAL_BIAS_EN;

    /* Initialize ADC 1 driver instance.*/
    adc_init(&ADCD);
    /* Configure ADC resolution.*/
    (void)adc_set_data_res(&ADCD, ADC_DATA_RES_8);
    (void)adc_set_prio(&ADCD, IRQ_PRIORITY_10);
    /* Configure ADC regular conversions (single).*/
    (void)adc_set_reg_mode(&ADCD, ADC_REG_SINGLE);
    adc_set_seq(&ADCD, ADC_REG_CONV, ADC_REG_NCHANNELS, reg_seq1, adc1_reg_buff);
    /* Configure ADC callbacks.*/
    (void)adc_set_err_cb(&ADCD, ADC_ERR_DMA, adc_error_cb);

    /* Configure ADC 1 injected conversions (triggered).*/
    adc_set_seq(&ADCD, ADC_INJ_CONV, ADC_INJ_NCHANNELS, inj_seq, adc_inj_buff);
    (void)adc_set_smp_time(&ADCD, ADC_CH_IN4, ADC_SMP_2P5);
    (void)adc_set_smp_time(&ADCD, ADC_CH_IN5, ADC_SMP_2P5);
    /* Configure ADC callbacks.*/
    (void)adc_set_op_cb(&ADCD, ADC_OP_INJ_CONV, adc_inj_data_cb);

    /* Set dual mode configuration for ADC1 (master).*/
    (void)adc_set_dual_mode(&ADCD, ADC_REG_REG_MODE);

    /* Configure ADC DMA for dual mode conversions.*/
    adc_dma_conf.dma_stream_id = DMA_STREAM_ID_ANY;
    adc_dma_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    adc_dma_conf.dma_stream_irq_prio = IRQ_PRIORITY_10;

    /* Configure DMA to read the coommon register data.*/
    adc_set_dual_mode_dma_conf(&ADCD, &adc_dma_conf, dual_mode_buffer);
    (void)adc_set_op_cb(&ADCD, ADC_OP_DUAL_MODE_CONV, dual_conv_cb);

    /* Apply ADC1 configuration.*/
    adc_start(&ADCD);

    /* Initialize ADC 2 driver instance.*/
    adc_init(&ADCD_SLAVE);
    /* Configure ADC resolution.*/
    (void)adc_set_data_res(&ADCD_SLAVE, ADC_DATA_RES_8);
    (void)adc_set_prio(&ADCD_SLAVE, IRQ_PRIORITY_10);
    /* Configure ADC DMA for regular conversions.*/
    adc_dma_conf.dma_stream_id = DMA_STREAM_ID_ANY;
    adc_dma_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    adc_dma_conf.dma_stream_irq_prio = IRQ_PRIORITY_10;
    adc_set_dma_conf(&ADCD_SLAVE, &adc_dma_conf);
    /* Configure ADC regular conversions (single).*/
    (void)adc_set_reg_mode(&ADCD_SLAVE, ADC_REG_SINGLE);
    adc_set_seq(&ADCD_SLAVE, ADC_REG_CONV, ADC_REG_NCHANNELS, reg_seq2, adc2_reg_buff);
    /* Configure ADC callbacks.*/
    (void)adc_set_err_cb(&ADCD_SLAVE, ADC_ERR_DMA, adc_error_cb);

    /* Apply ADC2 configuration.*/
    adc_start(&ADCD_SLAVE);


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

    /* Initialize TIM driver instance used as trigger for ADC regular
       conversions.*/
    tim_init(&TIMD);
    (void)tim_set_presc(&TIMD, TIM_DIVIDE_BY(CLOCK_TIMCLK1 / 100000U));
    (void)tim_set_ch_mode(&TIMD, 1U, TIM_MODE_PWM_ACTIVE_HIGH);
    (void)tim_set_trgo(&TIMD, TIM_TRGO_UPDATE);

    /* Apply TIM configuration.*/
    tim_start(&TIMD);

    /* Start PWM on TIM1 channel 1 (frequency 2Hz, duty cycle 50%).*/
    tim_set_interval(&TIMD, 50000U);        /* 100kHz/50000 = 2Hz. */
    tim_set_comparator(&TIMD, 1U, 25000U);  /* Duty cycle 50%.     */

    /* Enable TIM.*/
    tim_enable(&TIMD);

    /* Application main loop */
    for ( ; ; ) {

        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(500U);

        if (reg_conv_completed == 1U) {

            reg_conv_completed = 0U;
            /* Start ADC1 conversion.*/
            adc_start_regular(&ADCD);
            /* Start ADC2 conversion.*/
            adc_start_regular(&ADCD_SLAVE);
        }

        if (inj_conv_completed == 1U) {

            inj_conv_completed = 0U;
            /* Start ADC injected conversions.*/
            adc_start_injected(&ADCD);
        }
    }
}
