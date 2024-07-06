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

/*===========================================================================*/
/* ADC-related.                                                              */
/*===========================================================================*/

#define ADC_REG_NCHANNELS         2U

static adc_sample_t adc_reg_buffer[ADC_REG_NCHANNELS];
volatile uint8_t reg_conv_completed = 1U;

/* This callback is not used, it is here as an example, it can be replaced
   by a NULL in the structure.*/
static void adc_error_cb(adc_driver_t *adp, adc_err_t err) {

    (void)adp;
    (void)err;
}

/* Analog watchdog 1 callback.*/
static void adc_awd1_cb(adc_driver_t *adp, adc_err_t err) {

    (void)adp;
    (void)err;

    gpio_toggle_pin(USER_LED_A);
    osal_delay_millisec(250U);
}

/* Analog watchdog 2 callback.*/
static void adc_awd2_cb(adc_driver_t *adp, adc_err_t err) {

    (void)adp;
    (void)err;

    gpio_toggle_pin(USER_LED_B);
    osal_delay_millisec(250U);
}

/* Regular conversion data callback.*/
static void adc_reg_data_cb(adc_driver_t *adp) {

    adc_stop_regular(adp);
    reg_conv_completed = 1U;
}

int main(void) {

    adc_dma_conf_t adc_dma_conf;
    uint8_t reg_seq[ADC_REG_NCHANNELS] = {ADC_CH_IN1, ADC_CH_IN2};

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

    /* Configure adc watchdog trigger tresholds range 0.64V - 3.3V on all channels with 8 bit resolution.*/
    (void)adc_set_aw_tr(&ADCD, ADC_AW_1, 50U, 255U, 1U);
    (void)adc_set_aw_ch(&ADCD, ADC_AW_1, ADC_CH_IN1);
    (void)adc_set_err_cb(&ADCD, ADC_ERR_AWD1, adc_awd1_cb);

    /* Configure adc watchdog trigger tresholds range 0.64V - 3.3V on channel 2 with 8 bit resolution.*/
    (void)adc_set_aw_tr(&ADCD, ADC_AW_2, 50U, 255U, 1U);
    (void)adc_set_aw_ch(&ADCD, ADC_AW_2, ADC_CH_IN2);
    (void)adc_set_err_cb(&ADCD, ADC_ERR_AWD2, adc_awd2_cb);

    /* Enable ADC interrupts.*/
    (void)adc_set_interrupt_en(&ADCD, true);

    /* Apply ADC configuration.*/
    adc_start(&ADCD);

    /* Application main loop */
    for ( ; ; ) {
        if (reg_conv_completed == 1U) {
            reg_conv_completed = 0U;
            /* Start ADC continuous conversion.*/
            adc_start_regular(&ADCD);
        }
    }
}
