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
#include <spi.h>

/*===========================================================================*/
/* SPI-related.                                                              */
/*===========================================================================*/
#define SPI_BUFFER_SIZE      4U

volatile uint8_t spi_master_completed = 0U;
volatile uint8_t spi_slave_completed  = 0U;
volatile uint8_t spi_slave2_completed = 0U;

uint8_t spi_master_tx_buffer[SPI_BUFFER_SIZE];
uint8_t spi_slave_rx_buffer[SPI_BUFFER_SIZE];
uint8_t spi_slave2_rx_buffer[SPI_BUFFER_SIZE];

/* This callback is invoked when SPI Slave1 data receiving is completed.*/
static void spi_slave_data_cb(spi_driver_t *spd) {

    (void)spd;

    /* Set spi_slave_completed to indicate SPI Slave1 data receiving is
       completed.*/
    spi_slave_completed = 1;
}

/* This callback is invoked when SPI Slave2 data receiving is completed.*/
static void spi_slave2_data_cb(spi_driver_t *spd) {

    (void)spd;

    /* Set spi_slave_completed to indicate SPI Slave2 data receiving is
       completed.*/
    spi_slave2_completed = 1;
}

/* Slave select GPIOs.*/
#define SPI_NSS_GPIO_SLAVE1       gpio_iopack(GPIO_PORT_E, GPIO_PIN_9)
#define SPI_NSS_GPIO_SLAVE2       gpio_iopack(GPIO_PORT_E, GPIO_PIN_8)

/* Slave select gpio configuration.*/
#define SPI_NSS_GPIO_SLAVE_CFG                   \
    (GPIO_MODE_MODER_OUTPUT |                    \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_PULLUP |                    \
     GPIO_MODE_AFR(0U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_DISABLED |             \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

int main(void) {

    uint8_t i;
    uint8_t j = 0;

    spi_dma_conf_t spi_master_dma_conf, spi_slave_dma_conf, spi_slave2_dma_conf;

    /* Configure SPI Master DMA.*/
    spi_master_dma_conf.dma_stream_tx_id = DMA1_STREAM0_ID;
    spi_master_dma_conf.dma_stream_rx_id = DMA1_STREAM1_ID;
    spi_master_dma_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    spi_master_dma_conf.dma_stream_irq_prio = IRQ_PRIORITY_10;

    /* Configure SPI Slave1 DMA.*/
    spi_slave_dma_conf.dma_stream_tx_id = DMA1_STREAM2_ID;
    spi_slave_dma_conf.dma_stream_rx_id = DMA1_STREAM3_ID;
    spi_slave_dma_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    spi_slave_dma_conf.dma_stream_irq_prio = IRQ_PRIORITY_10;

    /* Configure SPI Slave2 DMA.*/
    spi_slave2_dma_conf.dma_stream_tx_id = DMA1_STREAM4_ID;
    spi_slave2_dma_conf.dma_stream_rx_id = DMA1_STREAM5_ID;
    spi_slave2_dma_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    spi_slave2_dma_conf.dma_stream_irq_prio = IRQ_PRIORITY_10;

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

    /* Configure slave select GPIOs.*/
    gpio_set_pin_mode(SPI_NSS_GPIO_SLAVE1, SPI_NSS_GPIO_SLAVE_CFG);
    gpio_set_pin_mode(SPI_NSS_GPIO_SLAVE2, SPI_NSS_GPIO_SLAVE_CFG);

    /* Set GPIOs used for slave selection to guarantee all slaves are initially
       deselected.*/
    gpio_set_pin(SPI_NSS_GPIO_SLAVE1);
    gpio_set_pin(SPI_NSS_GPIO_SLAVE2);

    /* Initialize SPI Master driver instance.*/
    spi_init(&MASTER_SPID);

    /* Initialize SPI Slave1 driver instance.*/
    spi_init(&SLAVE_SPID);

    /* Initialize SPI Slave2 driver instance.*/
    spi_init(&SLAVE2_SPID);

    /* Configure SPI Master driver instance.*/
    (void)spi_set_drv_mode(&MASTER_SPID, SPI_DRV_MODE_DMA_ASYNC);
    spi_set_dma_conf(&MASTER_SPID, &spi_master_dma_conf);
    (void)spi_set_mode(&MASTER_SPID, SPI_MODE_MASTER);
    (void)spi_set_baud(&MASTER_SPID, SPI_BAUDRATE_DIV8);
    (void)spi_set_cpha(&MASTER_SPID, SPI_CPHA_0);
    (void)spi_set_cpol(&MASTER_SPID, SPI_CPOL_0);
    (void)spi_set_fformat(&MASTER_SPID, SPI_FRAMEFORMAT_MSB);
    (void)spi_set_fsize(&MASTER_SPID, SPI_FRAMESIZE_8);
    (void)spi_set_nss_mode(&MASTER_SPID, SPI_NSS_MODE_SOFTWARE);

    /* Configure SPI Slave1 driver instance.*/
    (void)spi_set_drv_mode(&SLAVE_SPID, SPI_DRV_MODE_DMA_ASYNC);
    spi_set_dma_conf(&SLAVE_SPID, &spi_slave_dma_conf);
    (void)spi_set_mode(&SLAVE_SPID, SPI_MODE_SLAVE);
    (void)spi_set_baud(&SLAVE_SPID, SPI_BAUDRATE_DIV4);
    (void)spi_set_cpha(&SLAVE_SPID, SPI_CPHA_0);
    (void)spi_set_cpol(&SLAVE_SPID, SPI_CPOL_0);
    (void)spi_set_fformat(&SLAVE_SPID, SPI_FRAMEFORMAT_MSB);
    (void)spi_set_fsize(&SLAVE_SPID, SPI_FRAMESIZE_8);
    (void)spi_set_cb(&SLAVE_SPID, SPI_CB_OPS_DONE, spi_slave_data_cb);

    /* Configure SPI Slave2 driver instance.*/
    (void)spi_set_drv_mode(&SLAVE2_SPID, SPI_DRV_MODE_DMA_ASYNC);
    spi_set_dma_conf(&SLAVE2_SPID, &spi_slave2_dma_conf);
    (void)spi_set_mode(&SLAVE2_SPID, SPI_MODE_SLAVE);
    (void)spi_set_baud(&SLAVE2_SPID, SPI_BAUDRATE_DIV8);
    (void)spi_set_cpha(&SLAVE2_SPID, SPI_CPHA_0);
    (void)spi_set_cpol(&SLAVE2_SPID, SPI_CPOL_0);
    (void)spi_set_fformat(&SLAVE2_SPID, SPI_FRAMEFORMAT_MSB);
    (void)spi_set_fsize(&SLAVE2_SPID, SPI_FRAMESIZE_8);
    (void)spi_set_cb(&SLAVE2_SPID, SPI_CB_OPS_DONE, spi_slave2_data_cb);

    /* Apply SPI Master driver instance configuration.*/
    spi_start(&MASTER_SPID);

    /* Apply SPI Slave1 driver instance configuration.*/
    spi_start(&SLAVE_SPID);

    /* Apply SPI Slave2 driver instance configuration.*/
    spi_start(&SLAVE2_SPID);

    /* Application main loop */
    for ( ; ; ) {

        /* Initialize SPI Master TX buffer and SPI Slave RX buffers.*/
        for (i = 0; i < SPI_BUFFER_SIZE; i++ ) {
            spi_master_tx_buffer[i] = i + j;
            spi_slave_rx_buffer[i]  = 0x0;
            spi_slave2_rx_buffer[i] = 0x0;
        }

        j = j + SPI_BUFFER_SIZE;

        /* Start data receiving via SPI Slave1.*/
        (void)spi_receive(&SLAVE_SPID, (uint8_t *)spi_slave_rx_buffer, SPI_BUFFER_SIZE);

        /* Select slave select GPIO for Slave1.*/
        (void)spi_set_nss_gpio(&MASTER_SPID, SPI_NSS_GPIO_SLAVE1);
        /* Start data trasferring via SPI Master.*/
        (void)spi_send(&MASTER_SPID, (uint8_t *)spi_master_tx_buffer, SPI_BUFFER_SIZE);

        /* Wait till SPI Slave1 data receiving is completed.*/
        do {
            ;
        } while(spi_slave_completed == 0);

        spi_slave_completed = 0;

        /* Start data receiving via SPI Slave2.*/
        (void)spi_receive(&SLAVE2_SPID, (uint8_t *)spi_slave2_rx_buffer, SPI_BUFFER_SIZE);

        /* Select slave select GPIO for Slave2.*/
        (void)spi_set_nss_gpio(&MASTER_SPID, SPI_NSS_GPIO_SLAVE2);
        /* Start data trasferring via SPI Master.*/
        (void)spi_send(&MASTER_SPID, (uint8_t *)spi_master_tx_buffer, SPI_BUFFER_SIZE);

        /* Wait till SPI Slave2 data receiving is completed.*/
        do {
            ;
        } while(spi_slave2_completed == 0);

        spi_slave2_completed = 0;

        /* Check data received by SPI Slaves.*/
        for (i = 0; i < SPI_BUFFER_SIZE; i++) {
            if (spi_slave_rx_buffer[i] != spi_master_tx_buffer[i] ||
                spi_slave2_rx_buffer[i] != spi_master_tx_buffer[i]) {
                /* If data received by SPI Slave1 or 2 is different from data
                   trasmitted by SPI Master, then the application will stuck in
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
