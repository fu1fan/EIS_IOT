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
#include <tim.h>

/*===========================================================================*/
/* TIM-related.                                                              */
/*===========================================================================*/

#define TIM_DMA_BUFFER_SIZE                     4U

static bool tim_dma_completed = false;

static uint16_t tim_dma_source[TIM_DMA_BUFFER_SIZE]      = {10000U, 20000U, 30000U, 40000U};
static uint16_t tim_prev_dma_source[TIM_DMA_BUFFER_SIZE] = {10000U, 20000U, 30000U, 40000U};
static uint16_t tim_dma_destination[TIM_DMA_BUFFER_SIZE] = {0U};

static uint8_t dma_count = 0U;

/* TIM ch1 (output compare) callback.*/
void tim_ch1_callback(tim_driver_t *tdp) {

    uint8_t i;

    (void)(tdp);
    /* After DMA source value is used, it is updated for the next DMA transfer.*/
    i = dma_count % TIM_DMA_BUFFER_SIZE;
    tim_dma_source[i]++;
    dma_count++;
}

/* TIM ch2 (input capture) DMA callback.*/
void tim_ch2_dma_callback(tim_driver_t *tdp) {

    (void)(tdp);
    /* Set flag for DMA completition.*/
    tim_dma_completed = true;
}

int main(void) {

    uint8_t i;
    tim_dma_conf_t tim_dma_ch1_conf, tim_dma_ch2_conf;

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

    /* Initialize TIM driver instance.*/
    tim_init(&TIMD);

    /* Configure TIM driver.*/
    (void)tim_set_presc(&TIMD, TIM_DIVIDE_BY(tim_get_clock(&TIMD) / 200000U));
    (void)tim_set_ch_mode(&TIMD, 1U, TIM_MODE_PWM_ACTIVE_HIGH);
    (void)tim_set_ch_mode(&TIMD, 2U, TIM_MODE_ICU_FALLING_EDGE);
    (void)tim_set_cb(&TIMD, TIM_CB_OPS_CC1, tim_ch1_callback);

    /* Apply TIM configuration.*/
    tim_start(&TIMD);

    /* Enable TIM1 channel 1 and TIM1 channel 2 callbacks.*/
    tim_enable_channel_callback(&TIMD, 1U);
    tim_enable_channel_callback(&TIMD, 2U);

    /* Configure channel 1 DMA.*/
    tim_dma_ch1_conf.dma_stream_id = DMA1_STREAM0_ID;
    tim_dma_ch1_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    tim_dma_ch1_conf.dma_stream_irq_prio = IRQ_PRIORITY_10;
    tim_set_dma_conf(&TIMD, TIM_DMA_OPS_CH1, &tim_dma_ch1_conf);

    /* Configure channel 2 DMA.*/
    tim_dma_ch2_conf.dma_stream_id = DMA1_STREAM1_ID;
    tim_dma_ch2_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    tim_dma_ch2_conf.dma_stream_irq_prio = IRQ_PRIORITY_10;
    tim_set_dma_conf(&TIMD, TIM_DMA_OPS_CH2, &tim_dma_ch2_conf);

    /* Start PWM on TIM1 channel 1 (frequency 4Hz) and ICU on TIM1 channel 2.*/
    tim_set_interval(&TIMD, 50000U);        /* 200kHz/50000 = 4Hz. */
    tim_enable_channel_dma_ic(&TIMD, 2U, tim_dma_destination, TIM_DMA_BUFFER_SIZE);
    (void)tim_set_cb(&TIMD, TIM_CB_OPS_DMA_CC2, tim_ch2_dma_callback);
    tim_enable_channel_dma_oc(&TIMD, 1U, tim_dma_source, TIM_DMA_BUFFER_SIZE);

    /* Enable TIM.*/
    tim_enable(&TIMD);

    /* Application main loop */
    for ( ; ; ) {
        if (tim_dma_completed == true) {
            tim_dma_completed = false;
            /* Check input capture DMA buffer is equal to DMA output compare
               buffer.*/
            for (i = 0U; i < TIM_DMA_BUFFER_SIZE; i++) {
                if (tim_dma_destination[i] != tim_prev_dma_source[i]) {
                    while(1);
                }
                /* Re-init DMA source and destination values for the next check
                   on the next DMA cycle.*/
                tim_dma_destination[i] = 0U;
                tim_prev_dma_source[i]++;
            }

            /* Blink USER_LED_A.*/
            gpio_toggle_pin(USER_LED_A);
        }
    }
}
