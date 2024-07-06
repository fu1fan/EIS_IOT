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
#include <fccu.h>
#include <cmu.h>

/*===========================================================================*/
/* ADC-related.                                                              */
/*===========================================================================*/

#define ADC_REG_NCHANNELS         1U
#define ADC_DATA_COUNT            5U

static adc_sample_t adc_reg_buffer[ADC_REG_NCHANNELS];
volatile uint8_t reg_conv_completed = 1U;

adc_dma_conf_t adc_dma_conf;
uint8_t reg_seq[ADC_REG_NCHANNELS] = {ADC_CH_IN1};

/* This callback is not used, it is here as an example, it can be replaced
   by a NULL in the structure.*/
static void adc_error_cb(adc_driver_t *adp, adc_err_t err) {

    (void)adp;
    (void)err;
}

/* Regular conversion data callback.*/
static void adc_reg_data_cb(adc_driver_t *adp) {

    (void)adp;
}

static void adc_config(adc_driver_t *adp) {

/* Configure ADC resolution.*/
    (void)adc_set_data_res(adp, ADC_DATA_RES_8);
    /* Configure ADC DMA for regular conversions.*/
    adc_dma_conf.dma_stream_id = DMA_STREAM_ID_ANY;
    adc_dma_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    adc_dma_conf.dma_stream_irq_prio = IRQ_PRIORITY_10;
    adc_set_dma_conf(adp, &adc_dma_conf);
    /* Configure ADC regular conversions (continuos).*/
    (void)adc_set_reg_mode(adp, ADC_REG_CONTINUOUS);
    adc_set_seq(adp, ADC_REG_CONV, ADC_REG_NCHANNELS, reg_seq, adc_reg_buffer);
    /* Configure ADC callbacks.*/
    (void)adc_set_op_cb(adp, ADC_OP_REG_CONV, adc_reg_data_cb);
    (void)adc_set_err_cb(adp, ADC_ERR_DMA, adc_error_cb);
}

/*===========================================================================*/
/* FCCU-related.                                                             */
/*===========================================================================*/

/* This callback is invoked when FCCU config timeout occurs.*/
static void fccu_to_cb(fccu_driver_t *fdp) {

    (void)(fdp);

    for ( ; ; ) {
        osal_delay_millisec(250U);
    }
}

/* This callback is invoked when FCCU alarm timeout for CMU_0_OSC occurs.*/
static void fccu_ch14_alarm_to_cb(fccu_driver_t *fdp) {

    (void)cmu_get_event(&CMU4D);

    fccu_clear_fault(fdp, 14U);
}

/* This callback is invoked when FCCU alarm timeout for CMU_0_PLL0 occurs.*/
static void fccu_ch15_alarm_to_cb(fccu_driver_t *fdp) {

    (void)cmu_get_event(&CMU4D);

    fccu_clear_fault(fdp, 15U);
}

/* This callback is invoked when FCCU alarm timeout for CMU_Sysclk occurs.*/
static void fccu_ch16_alarm_to_cb(fccu_driver_t *fdp) {

    (void)cmu_get_event(&CMU4D);

    fccu_clear_fault(fdp, 16U);
}

/* This callback is invoked when FCCU alarm timeout for CMU_other occurs.*/
static void fccu_ch17_alarm_to_cb(fccu_driver_t *fdp) {

    cmu_int_t event;

    event = cmu_get_event(&CMU4D);

    cmu_stop(&CMU4D);

    cmu_clear_event(&CMU4D, event);

    fccu_clear_fault(fdp, 17U);

    /* Blink USER_LED_B.*/
    gpio_toggle_pin(USER_LED_B);
}

/* This function sets the FCCU driver parameters.*/
static void fccu_config(fccu_driver_t *fdp) {

    (void)fccu_set_config_to(fdp, FCCU_CFG_TO_8192US);
    (void)fccu_set_is_config_to_irq_en(fdp, true);
    (void)fccu_set_config_to_irq_cb(fdp, fccu_to_cb);
    (void)fccu_set_alarm_to(fdp, 10000000U);
    (void)fccu_set_delta_t(fdp, 5000U);

    /* Configuration for XOSC less than IRC CMU fault.*/
    fccu_set_ch_cfg(fdp, 14U, true, FCCU_RF_CONFIG_SW, true, true, fccu_ch14_alarm_to_cb, FCCU_REACTION_NO_RESET, false, false);
    /* Configuration for PLL0 out of frequency CMU fault.*/
    fccu_set_ch_cfg(fdp, 15U, true, FCCU_RF_CONFIG_SW, true, true, fccu_ch15_alarm_to_cb, FCCU_REACTION_NO_RESET, false, false);
    /* Configuration for Sysclk frequency out of range CMU fault.*/
    fccu_set_ch_cfg(fdp, 16U, true, FCCU_RF_CONFIG_SW, true, true, fccu_ch16_alarm_to_cb, FCCU_REACTION_NO_RESET, false, false);
    /* Configuration for other internal clock CMU faults.*/
    fccu_set_ch_cfg(fdp, 17U, true, FCCU_RF_CONFIG_SW, true, true, fccu_ch17_alarm_to_cb, FCCU_REACTION_NO_RESET, false, false);
}

/*===========================================================================*/
/* CMU-related.                                                              */
/*===========================================================================*/

/* This function sets the CMU driver parameters.*/
static void cmu_config(cmu_driver_t *cdp) {

    cmu_set_high_freq_ref(cdp, 0);
    cmu_set_low_freq_ref(cdp, 0);
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

    /* Initialize FCCU driver instance.*/
    fccu_init(&FCCUD);

    /* Clear FCCU configuration.*/
    fccu_clear(&FCCUD);

    /* Set FCCU configuration.*/
    fccu_config(&FCCUD);

    /* Apply FCCU configuration.*/
    fccu_start(&FCCUD);

    /* Set Global Bias Enable to use the SARADC.*/
    SYSCFG->ADC_BIASEN = SYSCFG_ADC_BIASEN_GLOBAL_BIAS_EN;

    /* Initialize ADC driver instance.*/
    adc_init(&ADCD);
    
    /* Set ADC configuration.*/
    adc_config(&ADCD);

    /* Apply ADC configuration.*/
    adc_start(&ADCD);

    /* Initialize CMU instance.*/
    cmu_init(&CMU4D);

    /* Set CMU configuration.*/
    cmu_config(&CMU4D);

    /* Apply CMU configuration.*/
    cmu_start(&CMU4D);

    /* Application main loop */
    for ( ; ; ) {
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(500U);
    }
}
