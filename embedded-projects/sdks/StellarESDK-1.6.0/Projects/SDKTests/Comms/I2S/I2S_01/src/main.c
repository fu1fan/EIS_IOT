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
#include <i2s.h>

#ifdef EVBE3000P
#define MASTER_I2S_MOSI gpio_iopack(GPIO_PORT_F, GPIO_PIN_3)
#define MASTER_I2S_MOSI_CFG                      \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(5U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

#define SLAVE_I2S_SCK gpio_iopack(GPIO_PORT_D, GPIO_PIN_10)
#define SLAVE_I2S_SCK_CFG                        \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(5U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

#define SLAVE_I2S_NSS gpio_iopack(GPIO_PORT_D, GPIO_PIN_13)
#define SLAVE_I2S_NSS_CFG                        \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(5U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

#define SLAVE_I2S_MOSI gpio_iopack(GPIO_PORT_D, GPIO_PIN_12)
#define SLAVE_I2S_MOSI_CFG                       \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(5U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

__STATIC_INLINE void board_reinit(void) {

    gpio_set_pin_mode(MASTER_I2S_MOSI, MASTER_I2S_MOSI_CFG);
    gpio_set_pin_mode(SLAVE_I2S_SCK, SLAVE_I2S_SCK_CFG);
    gpio_set_pin_mode(SLAVE_I2S_NSS, SLAVE_I2S_NSS_CFG);
    gpio_set_pin_mode(SLAVE_I2S_MOSI, SLAVE_I2S_MOSI_CFG);
}
#else
#define SLAVE_I2S_SCK gpio_iopack(GPIO_PORT_D, GPIO_PIN_10)
#define SLAVE_I2S_SCK_CFG                        \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(5U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

#define SLAVE_I2S_NSS gpio_iopack(GPIO_PORT_D, GPIO_PIN_13)
#define SLAVE_I2S_NSS_CFG                        \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(5U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

#define SLAVE_I2S_MOSI gpio_iopack(GPIO_PORT_D, GPIO_PIN_12)
#define SLAVE_I2S_MOSI_CFG                       \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(5U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

__STATIC_INLINE void board_reinit(void) {

    gpio_set_pin_mode(SLAVE_I2S_SCK, SLAVE_I2S_SCK_CFG);
    gpio_set_pin_mode(SLAVE_I2S_NSS, SLAVE_I2S_NSS_CFG);
    gpio_set_pin_mode(SLAVE_I2S_MOSI, SLAVE_I2S_MOSI_CFG);
}
#endif

/*===========================================================================*/
/* I2S-related.                                                              */
/*===========================================================================*/
#define I2S_BUFFER_SIZE      4U

volatile uint8_t i2s_master_completed = 0U;
volatile uint8_t i2s_slave_completed  = 0U;

volatile uint32_t i2s_master_tx_buffer[I2S_BUFFER_SIZE];
volatile uint32_t i2s_slave_rx_buffer[I2S_BUFFER_SIZE];

/* This callback is invoked when I2S Master data trasferring is completed.*/
static void i2s_master_data_cb(i2s_driver_t *idp) {

    (void)idp;

    /* Set i2s_master_completed to indicate I2S Master data transferring is
       completed.*/
    i2s_master_completed = 1;
}

/* This callback is invoked when I2S Slave data receiving is completed.*/
static void i2s_slave_data_cb(i2s_driver_t *idp) {

    (void)idp;

    /* Set i2s_slave_completed to indicate I2S Slave data receiving is
       completed.*/
    i2s_slave_completed = 1;
}

int main(void) {

    uint8_t i;
    uint8_t j = 0;

    i2s_dma_conf_t i2s_master_dma_conf, i2s_slave_dma_conf;

    /* Configure I2S Master DMA.*/
    i2s_master_dma_conf.dma_stream_tx_id = DMA1_STREAM0_ID;
    i2s_master_dma_conf.dma_stream_rx_id = DMA1_STREAM1_ID;
    i2s_master_dma_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    i2s_master_dma_conf.dma_stream_irq_prio = IRQ_PRIORITY_10;

    /* Configure I2S Slave DMA.*/
    i2s_slave_dma_conf.dma_stream_tx_id = DMA1_STREAM2_ID;
    i2s_slave_dma_conf.dma_stream_rx_id = DMA1_STREAM3_ID;
    i2s_slave_dma_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    i2s_slave_dma_conf.dma_stream_irq_prio = IRQ_PRIORITY_10;

    /* Enable interrupts.*/
    osal_sys_unlock();

    test_env_init((TestInit_t)
                  (TEST_INIT_CLOCK    |
                   TEST_INIT_GPIO     |
                   TEST_INIT_BOARD    |
                   TEST_INIT_IRQ      |
                   TEST_INIT_OSAL));

    /* Reinitialize board default initialization (needed only for some boards).*/
    board_reinit();

    /* Switch-off user led.*/
    USER_LED_SWITCH_OFF(USER_LED_A);

    /* Initialize I2S Master driver instance.*/
    i2s_init(&MASTER_I2SD);

    /* Initialize I2S Slave driver instance.*/
    i2s_init(&SLAVE_I2SD);

    /* Configure I2S2 as Master TX.*/
    (void)i2s_set_prio(&MASTER_I2SD, IRQ_PRIORITY_10);
    (void)i2s_set_drv_mode(&MASTER_I2SD, I2S_DRV_MODE_ASYNCHRONOUS);
    (void)i2s_set_dma_mode(&MASTER_I2SD, I2S_DMA_ON);
    i2s_set_dma_conf(&MASTER_I2SD, &i2s_master_dma_conf);
    (void)i2s_set_mode(&MASTER_I2SD, I2S_MODE_MASTER_TX);
    (void)i2s_set_standard(&MASTER_I2SD, I2S_STANDARD_PHILIPS);
    (void)i2s_set_chlen(&MASTER_I2SD, I2S_CHANNEL_LENGTH_32B);
    (void)i2s_set_datalen(&MASTER_I2SD, I2S_DATA_LENGTH_32B);
    (void)i2s_set_ckpol(&MASTER_I2SD, I2S_CLOCK_POLARITY_LOW);
    (void)i2s_set_mckout(&MASTER_I2SD, I2S_MASTER_CLOCK_OUTPUT_DISABLE);
    (void)i2s_set_audiofreq(&MASTER_I2SD, I2S_AUDIO_FREQUENCY_44K);
    (void)i2s_set_cb(&MASTER_I2SD, I2S_CB_OPS_TX, i2s_master_data_cb);

    /* Configure I2S3 as Slave RX.*/
    (void)i2s_set_prio(&SLAVE_I2SD, IRQ_PRIORITY_10);
    (void)i2s_set_drv_mode(&SLAVE_I2SD, I2S_DRV_MODE_ASYNCHRONOUS);
    (void)i2s_set_dma_mode(&SLAVE_I2SD, I2S_DMA_ON);
    i2s_set_dma_conf(&SLAVE_I2SD, &i2s_slave_dma_conf);
    (void)i2s_set_mode(&SLAVE_I2SD, I2S_MODE_SLAVE_RX);
    (void)i2s_set_standard(&SLAVE_I2SD, I2S_STANDARD_PHILIPS);
    (void)i2s_set_chlen(&SLAVE_I2SD, I2S_CHANNEL_LENGTH_32B);
    (void)i2s_set_datalen(&SLAVE_I2SD, I2S_DATA_LENGTH_32B);
    (void)i2s_set_ckpol(&SLAVE_I2SD, I2S_CLOCK_POLARITY_LOW);
    (void)i2s_set_mckout(&SLAVE_I2SD, I2S_MASTER_CLOCK_OUTPUT_DISABLE);
    (void)i2s_set_audiofreq(&SLAVE_I2SD, I2S_AUDIO_FREQUENCY_44K);
    (void)i2s_set_cb(&SLAVE_I2SD, I2S_CB_OPS_RX, i2s_slave_data_cb);

    /* Apply I2S Master driver instance configuration.*/
    i2s_start(&MASTER_I2SD);

    /* Apply I2S Slave driver instance configuration.*/
    i2s_start(&SLAVE_I2SD);

    /* Application main loop */
    for ( ; ; ) {

        /* Initialize I2S Master TX buffer and I2S Slave RX buffer.*/
        for (i = 0; i < I2S_BUFFER_SIZE; i++ ) {
            i2s_master_tx_buffer[i] = i + j;
            i2s_slave_rx_buffer[i] = 0x0;
        }

        j = j + I2S_BUFFER_SIZE;

        /* Start data receiving via I2S Slave.*/
        (void)i2s_receive(&SLAVE_I2SD, (uint16_t *)i2s_slave_rx_buffer, I2S_BUFFER_SIZE);

        /* Start data trasferring via I2S Master.*/
        (void)i2s_send(&MASTER_I2SD, (uint16_t *)i2s_master_tx_buffer, I2S_BUFFER_SIZE);
    
        /* Wait till I2S Slave data receiving is completed.*/
        do {
            ;
        } while(i2s_slave_completed == 0);
    
        i2s_slave_completed = 0;
    
        /* Check data received by I2S Slave.*/
        for (i = 0; i < I2S_BUFFER_SIZE; i++) {
            if (i2s_slave_rx_buffer[i] != i2s_master_tx_buffer[i]) {
                /* If data received by I2S Slave is different from data
                   trasmitted by I2S Master, then the application will stuck in
                   the following while cycle, otherwise the USER_LED_A will be
                   blinked.*/
                do{
                    ;
                } while(1);
            }
        }
    
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(500U);
    }
}
