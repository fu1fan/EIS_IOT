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
#include <io.h>
#include <stdio.h>

/*===========================================================================*/
/* ADC-related.                                                              */
/*===========================================================================*/

#define ADC_REG_NCHANNELS         1U
#define ADC_ENABLED               5U
#define ADC_RES                   4096U

#define ADC_CONV_RULES(_val_, _res_) \
    ((3.3F * _val_) / _res_)

volatile uint8_t reg_conv_completed = 1U;
volatile uint8_t adc_sample_num     = 0U;

volatile uint8_t ref = 0U;

static adc_sample_t adc_reg_buffer[ADC_REG_NCHANNELS];

/* This callback is not used, it is here as an example, it can be replaced
   by a NULL in the structure.*/
static void adc_error_cb(adc_driver_t *adp, adc_err_t err) {

    (void)adp;
    (void)err;
}

/* Regular conversion data callback.*/
static void adc_reg_data_cb(adc_driver_t *adp) {

    (void)adp;

    reg_conv_completed = 1U;

    switch (ref) {
    case 0U:
        printf("TEST VREF = VREL \t\t= 0x%03X (%.1f V)\n\r", adc_reg_buffer[0U], ADC_CONV_RULES( adc_reg_buffer[0U], ADC_RES));
        break;
    case 1U:
        printf("TEST VREF = 1/3 * (VREH-VREFL) \t= 0x%03X (%.1f V)\n\r", adc_reg_buffer[0U], ADC_CONV_RULES( adc_reg_buffer[0U], ADC_RES));
        break;
    case 2U:
        printf("TEST VREF = 2/3 * (VREH-VREFL) \t= 0x%03X (%.1f V)\n\r", adc_reg_buffer[0U], ADC_CONV_RULES( adc_reg_buffer[0U], ADC_RES));
        break;
    case 3U:
        printf("TEST VREF = VREH \t\t= 0x%03X (%.1f V)\n\n\r", adc_reg_buffer[0U], ADC_CONV_RULES( adc_reg_buffer[0U], ADC_RES));
        break;
    }

    ref = (ref + 1U) % 4U;

    /* Clear Self-test enable bit.*/
    REG_CLEAR_FIELD(SYSCFG->ST_VREF_SEL2, SYSCFG_ST_VREF_SEL2_SELFTEST_EN);

    /* Update VREF selection.*/
    REG_SET_FIELD(SYSCFG->ST_VREF_SEL2, SYSCFG_ST_VREF_SEL2_SAR_TESTVREF_SEL, ref);

    /* Set Self-test enable bit.*/
    SYSCFG->ST_VREF_SEL2 |= SYSCFG_ST_VREF_SEL2_SELFTEST_EN;
}

int main(void) {

    adc_dma_conf_t adc_dma_conf;
    
    uint8_t reg_seq[ADC_REG_NCHANNELS] = {ADC_CH_IN18};

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

    /* Set Sar Bias to direct Vref as input of SARADC.*/
    SYSCFG->ST_VREF_SEL2 |= SYSCFG_ST_VREF_SEL2_SARBIAS_EN;

    /* Set VREF to test low reference (Vrel).*/
    REG_SET_FIELD(SYSCFG->ST_VREF_SEL2, SYSCFG_ST_VREF_SEL2_SAR_TESTVREF_SEL, ref);

    /* Set Self-test enable bit.*/
    SYSCFG->ST_VREF_SEL2 |= SYSCFG_ST_VREF_SEL2_SELFTEST_EN;

    /* Initialize ADC driver instance.*/
    adc_init(&ADCD);

    /* Configure ADC resolution.*/
    (void)adc_set_data_res(&ADCD, ADC_DATA_RES_12);

    /* Configure ADC DMA for regular conversions.*/
    adc_dma_conf.dma_stream_id = DMA1_STREAM1_ID;
    adc_dma_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    adc_dma_conf.dma_stream_irq_prio = IRQ_PRIORITY_10;

    (void)adc_set_dma_conf(&ADCD, &adc_dma_conf);

    /* Configure ADC regular conversions.*/
    (void)adc_set_reg_mode(&ADCD, ADC_REG_SINGLE);

    adc_set_seq(&ADCD, ADC_REG_CONV, ADC_REG_NCHANNELS, reg_seq, adc_reg_buffer);

    (void)adc_set_smp_time(&ADCD, ADC_CH_IN18, ADC_SMP_640P5);

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
            /* Start ADC single conversion.*/
            adc_start_regular(&ADCD);
        }
        osal_delay_millisec(500U);
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
    }
}

