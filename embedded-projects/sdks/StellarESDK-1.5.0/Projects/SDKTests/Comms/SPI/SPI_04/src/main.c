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

#ifdef EVBE3000P
#define SPI1_SCK gpio_iopack(GPIO_PORT_D, GPIO_PIN_15)
#define SPI1_SCK_CFG                             \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(5U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

#define SPI1_MISO gpio_iopack(GPIO_PORT_E, GPIO_PIN_1)
#define SPI1_MISO_CFG                            \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(5U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

#define SPI1_MOSI gpio_iopack(GPIO_PORT_E, GPIO_PIN_2)
#define SPI1_MOSI_CFG                            \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(5U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

#define SPI2_MISO gpio_iopack(GPIO_PORT_F, GPIO_PIN_2)
#define SPI2_MISO_CFG                            \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(5U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

#define SPI2_MOSI gpio_iopack(GPIO_PORT_F, GPIO_PIN_3)
#define SPI2_MOSI_CFG                            \
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

    gpio_set_pin_mode(SPI1_SCK, SPI1_SCK_CFG);
    gpio_set_pin_mode(SPI1_MISO, SPI1_MISO_CFG);
    gpio_set_pin_mode(SPI1_MOSI, SPI1_MOSI_CFG);
    gpio_set_pin_mode(SPI2_MISO, SPI2_MISO_CFG);
    gpio_set_pin_mode(SPI2_MOSI, SPI2_MOSI_CFG);
}
#else
__STATIC_INLINE void board_reinit(void) {

}
#endif

/*===========================================================================*/
/* SPI-related.                                                              */
/*===========================================================================*/
#define SPI_BUFFER_SIZE      4U

volatile uint8_t spi_master_completed = 0U;
volatile uint8_t spi_slave_completed  = 0U;

uint8_t spi_master_tx_buffer[SPI_BUFFER_SIZE];
uint8_t spi_slave_rx_buffer[SPI_BUFFER_SIZE];

/* This callback is invoked when SPI Master data trasferring is completed.*/
static void spi_master_data_cb(spi_driver_t *spd) {

    (void)spd;

    /* Set spi_master_completed to indicate SPI Master data transferring is
       completed.*/
    spi_master_completed = 1;
}

/* This callback is invoked when SPI Slave data receiving is completed.*/
static void spi_slave_data_cb(spi_driver_t *spd) {

    (void)spd;

    /* Set spi_slave_completed to indicate SPI Slave data receiving is
       completed.*/
    spi_slave_completed = 1;
}

/* This callback is not used, it is here as an example.*/
static void spi_master_err_cb(spi_driver_t *spd) {

    (void)spd;
}

/* This callback is not used, it is here as an example.*/
static void spi_slave_err_cb(spi_driver_t *spd) {

    (void)spd;
}

int main(void) {

    uint8_t i;
    uint8_t j = 0;

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

    /* Switch-off user leds.*/
    USER_LED_SWITCH_OFF(USER_LED_A);
    USER_LED_SWITCH_OFF(USER_LED_B);

    /* Initialize SPI Master driver instance.*/
    spi_init(&MASTER_SPID);

    /* Initialize SPI Slave driver instance.*/
    spi_init(&SLAVE_SPID);

    /* Configure SPI Master driver instance.*/
    (void)spi_set_prio(&MASTER_SPID, IRQ_PRIORITY_10);
    (void)spi_set_drv_mode(&MASTER_SPID, SPI_DRV_MODE_INT_ASYNC);
    (void)spi_set_mode(&MASTER_SPID, SPI_MODE_MASTER);
    (void)spi_set_baud(&MASTER_SPID, SPI_BAUDRATE_DIV8);
    (void)spi_set_cpha(&MASTER_SPID, SPI_CPHA_0);
    (void)spi_set_cpol(&MASTER_SPID, SPI_CPOL_0);
    (void)spi_set_fformat(&MASTER_SPID, SPI_FRAMEFORMAT_MSB);
    (void)spi_set_fsize(&MASTER_SPID, SPI_FRAMESIZE_8);
    (void)spi_set_cb(&MASTER_SPID, SPI_CB_OPS_DONE, spi_master_data_cb);
    (void)spi_set_cb(&MASTER_SPID, SPI_CB_OPS_ERR, spi_master_err_cb);

    /* Configure SPI Slave driver instance.*/
    (void)spi_set_prio(&SLAVE_SPID, IRQ_PRIORITY_9);
    (void)spi_set_drv_mode(&SLAVE_SPID, SPI_DRV_MODE_INT_ASYNC);
    (void)spi_set_mode(&SLAVE_SPID, SPI_MODE_SLAVE);
    (void)spi_set_baud(&SLAVE_SPID, SPI_BAUDRATE_DIV4);
    (void)spi_set_cpha(&SLAVE_SPID, SPI_CPHA_0);
    (void)spi_set_cpol(&SLAVE_SPID, SPI_CPOL_0);
    (void)spi_set_fformat(&SLAVE_SPID, SPI_FRAMEFORMAT_MSB);
    (void)spi_set_fsize(&SLAVE_SPID, SPI_FRAMESIZE_8);
    (void)spi_set_cb(&SLAVE_SPID, SPI_CB_OPS_DONE, spi_slave_data_cb);
    (void)spi_set_cb(&SLAVE_SPID, SPI_CB_OPS_ERR, spi_slave_err_cb);

    /* Apply SPI Master driver instance configuration.*/
    spi_start(&MASTER_SPID);

    /* Apply SPI Slave driver instance configuration.*/
    spi_start(&SLAVE_SPID);

    /* Application main loop */
    for ( ; ; ) {

        /* Initialize SPI Master TX buffer and SPI Slave RX buffer.*/
        for (i = 0; i < SPI_BUFFER_SIZE; i++ ) {
            spi_master_tx_buffer[i] = i + j;
            spi_slave_rx_buffer[i] = 0x0;
        }

        j = j + SPI_BUFFER_SIZE;

        /* Start data receiving via SPI Slave.*/
        (void)spi_receive(&SLAVE_SPID, (uint8_t *)spi_slave_rx_buffer, SPI_BUFFER_SIZE);

        /* Start data trasferring via SPI Master.*/
        (void)spi_send(&MASTER_SPID, (uint8_t *)spi_master_tx_buffer, SPI_BUFFER_SIZE);

        /* Wait till SPI Slave data receiving is completed.*/
        do {
            ;
        } while(spi_slave_completed == 0);
    
        spi_slave_completed = 0;

        /* Check data received by SPI Slave.*/
        for (i = 0; i < SPI_BUFFER_SIZE; i++) {
            if (spi_slave_rx_buffer[i] != spi_master_tx_buffer[i]) {
                /* If data received by SPI Slave is different from data
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
