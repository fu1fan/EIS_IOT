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
#include <dac.h>
#include <dma.h>
#include <tim.h>

static uint32_t dac_wave[100] = {0x0800, 0x0881, 0x0901, 0x0980, 0x09FD, 0x0A79, 0x0AF2, 0x0B68, 0x0BDA, 0x0C49,
                                 0x0CB3, 0x0D19, 0x0D79, 0x0DD4, 0x0E29, 0x0E78, 0x0EC0, 0x0F02, 0x0F3C, 0x0F6F,
                                 0x0F9B, 0x0FBF, 0x0FDB, 0x0FEF, 0x0FFB, 0x0FFF, 0x0FFB, 0x0FEF, 0x0FDB, 0x0FBF,
                                 0x0F9B, 0x0F6F, 0x0F3C, 0x0F02, 0x0EC0, 0x0E78, 0x0E29, 0x0DD4, 0x0D79, 0x0D19,
                                 0x0CB3, 0x0C49, 0x0BDA, 0x0B68, 0x0AF2, 0x0A79, 0x09FD, 0x0980, 0x0901, 0x0881,
                                 0x0800, 0x077F, 0x06FF, 0x0680, 0x0603, 0x0587, 0x050E, 0x0498, 0x0426, 0x03B7,
                                 0x034D, 0x02E7, 0x0287, 0x022C, 0x01D7, 0x0188, 0x0140, 0x00FE, 0x00C4, 0x0091,
                                 0x0065, 0x0041, 0x0025, 0x0011, 0x0005, 0x0001, 0x0005, 0x0011, 0x0025, 0x0041,
                                 0x0065, 0x0091, 0x00C4, 0x00FE, 0x0140, 0x0188, 0x01D7, 0x022C, 0x0287, 0x02E7,
                                 0x034D, 0x03B7, 0x0426, 0x0498, 0x050E, 0x0587, 0x0603, 0x0680, 0x06FF, 0x077F};
int main(void) {

    dac_dma_conf_t dac_dma_ch2_conf;

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

    /* Configure DAC DMA.*/
    dac_dma_ch2_conf.dma_stream_id = DMA1_STREAM0_ID;
    dac_dma_ch2_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    dac_dma_ch2_conf.dma_stream_irq_prio = IRQ_PRIORITY_10;

    /* Initialize DAC driver instance.*/
    dac_init(&BDACD);

    /* Enable and operate the CH1 and CH2 calibration*/
    (void)dac_enable_calibration(&BDACD, DAC_CHANNEL_1, TRUE);
    (void)dac_user_trimming(&BDACD, DAC_CHANNEL_1);
    (void)dac_enable_calibration(&BDACD, DAC_CHANNEL_2, TRUE);
    (void)dac_user_trimming(&BDACD, DAC_CHANNEL_2);

    /* Configure DAC.*/
    (void)dac_set_mode(&BDACD, DAC_DVR_MODE_SINGLE);
    (void)dac_set_highfreq_mode(&BDACD, DAC_HF_MODE_OVER_160MHZ);
    (void)dac_set_dma_mode(&BDACD, DAC_CHANNEL_1, DAC_DMA_OFF);
    (void)dac_set_dma_mode(&BDACD, DAC_CHANNEL_2, DAC_DMA_ON);
    dac_set_dma_conf(&BDACD, DAC_CHANNEL_2, &dac_dma_ch2_conf);
    (void)dac_set_format(&BDACD, DAC_CHANNEL_1, DAC_DATA_12BIT_RIGHT_ALIGN);
    (void)dac_set_format(&BDACD, DAC_CHANNEL_2, DAC_DATA_12BIT_RIGHT_ALIGN);
    (void)dac_set_trigger_src(&BDACD, DAC_CHANNEL_1, DAC_SWTRIG);
    (void)dac_set_trigger_src(&BDACD, DAC_CHANNEL_2, DAC_CHx_TRIG2);
    (void)dac_enable_trigger(&BDACD, DAC_CHANNEL_2, TRUE);
    (void)dac_set_output_connection(&BDACD, DAC_CHANNEL_1, DAC_OUT_CON_MODE_NORM_PIN_PER_BUF_EN);
    (void)dac_set_output_connection(&BDACD, DAC_CHANNEL_2, DAC_OUT_CON_MODE_NORM_PIN_PER_BUF_EN);

    /* Enable DAC instance channels.*/
    (void)dac_enable_channel(&BDACD, DAC_CHANNEL_1, true);
    (void)dac_enable_channel(&BDACD, DAC_CHANNEL_2, true);

    /* Apply DAC instance configuration.*/
    dac_start(&BDACD);

    /* Initialize TIM driver instance.*/
    tim_init(&DRV_TIM7);

    /* Configure TIM.*/
    (void)tim_set_presc(&DRV_TIM7, TIM_DIVIDE_BY(tim_get_clock(&DRV_TIM7) / 100000U));
    (void)tim_set_trgo(&DRV_TIM7, TIM_TRGO_UPDATE);
    (void)tim_start(&DRV_TIM7);
    (void)tim_enable_channel_callback(&DRV_TIM7, 1U);
    (void)tim_set_interval(&DRV_TIM7, 10000U);      /* 100kHz/4000 = 10Hz. */
    (void)tim_set_comparator(&DRV_TIM7, 1U, 5000U); /* Duty cycle 50%.     */
    (void)tim_enable(&DRV_TIM7);

    /* Set channel 2 digital values through DMA.*/
    dac_set_dma_value(&BDACD, DAC_CHANNEL_2, dac_wave, sizeof(dac_wave)/sizeof(dac_wave[0]));

    /* Application main loop */
    for ( ; ; ) {
        /* Set new channel 1 digital value each 100ms.*/
        dac_set_value(&BDACD, DAC_CHANNEL_1, 0x300U, 0x0);
        osal_delay_millisec(100U); 

        dac_set_value(&BDACD, DAC_CHANNEL_1, 0x500U, 0x0);
        osal_delay_millisec(100U); 

        dac_set_value(&BDACD, DAC_CHANNEL_1, 0xA00U, 0x0);
        osal_delay_millisec(100U); 

        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
    }
}
