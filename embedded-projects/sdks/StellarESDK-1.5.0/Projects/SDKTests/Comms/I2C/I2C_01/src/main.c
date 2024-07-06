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
#include <i2c.h>

/*===========================================================================*/
/* I2C-related.                                                              */
/*===========================================================================*/

#define I2C_BUFFER_SIZE                     300U
#define I2C_SLAVE_ADDRESS                   0x48U
#define I2C_TIMING                          0x50330309U

/* I2C pin slave configurations.*/
#define I2C_SLAVE_SDA gpio_iopack(GPIO_PORT_F, GPIO_PIN_7)
#define I2C_SLAVE_SDA_CFG                        \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_OPENDRAIN |                \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_PULLUP |                    \
     GPIO_MODE_AFR(6U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

#define I2C_SLAVE_SCL gpio_iopack(GPIO_PORT_F, GPIO_PIN_8)
#define I2C_SLAVE_SCL_CFG                        \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_OPENDRAIN |                \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_PULLUP |                    \
     GPIO_MODE_AFR(6U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

volatile uint8_t i2c_master_completed = 0U;
volatile uint8_t i2c_slave_completed  = 0U;

uint8_t i2c_master_tx_buffer[I2C_BUFFER_SIZE];
uint8_t i2c_slave_rx_buffer[I2C_BUFFER_SIZE];
uint8_t i2c_slave_tx_buffer[I2C_BUFFER_SIZE];
uint8_t i2c_master_rx_buffer[I2C_BUFFER_SIZE];

/* This callback is invoked when I2C Master data trasferring is completed.*/
static void i2c_master_data_cb(i2c_driver_t *idp) {

    (void)idp;

    /* Set i2c_master_completed to indicate I2C Master data transferring is
       completed.*/
    i2c_master_completed = 1;
}

/* This callback is invoked when I2C Slave data trasferring is completed.*/
static void i2c_slave_data_cb(i2c_driver_t *idp) {

    (void)idp;

    /* Set i2c_slave_completed to indicate I2C Slave data trasferring is
       completed.*/
    i2c_slave_completed = 1;
}

/* This function configures I2C Master.*/
static void i2c_master_config(i2c_driver_t *idp) {

    (void)i2c_set_prio(idp, IRQ_PRIORITY_10);
    (void)i2c_set_drv_mode(idp, I2C_DRV_MODE_ASYNCHRONOUS);
    (void)i2c_set_timing(idp, I2C_TIMING);
    (void)i2c_set_cb(idp, I2C_CB_OPS_DONE, i2c_master_data_cb);
}

/* This function configures I2C Slave.*/
static void i2c_slave_config(i2c_driver_t *idp) {

    (void)i2c_set_prio(idp, IRQ_PRIORITY_10);
    (void)i2c_set_drv_mode(idp, I2C_DRV_MODE_ASYNCHRONOUS);
    (void)i2c_set_timing(idp, I2C_TIMING);
    (void)i2c_set_own_address1_mode(idp, I2C_OA1_MODE_7BIT);
    (void)i2c_set_own_address1(idp, I2C_SLAVE_ADDRESS);
    (void)i2c_set_cb(idp, I2C_CB_OPS_DONE, i2c_slave_data_cb);
}

int main(void) {

    uint16_t i;
    uint8_t j = 0;

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

    /* Configure I2C Slave pins.*/
    gpio_set_pin_mode(I2C_SLAVE_SDA, I2C_SLAVE_SDA_CFG);
    gpio_set_pin_mode(I2C_SLAVE_SCL, I2C_SLAVE_SCL_CFG);

    /* Initialize I2C Master driver instance.*/
    i2c_init(&MASTER_I2CD);

    /* Initialize I2C Slave driver instance.*/
    i2c_init(&SLAVE_I2CD);

    /* Configure I2C Master driver instance.*/
    i2c_master_config(&MASTER_I2CD);

    /* Configure I2C Salve driver instance.*/
    i2c_slave_config(&SLAVE_I2CD);

    /* Apply I2C Master driver instance configuration.*/
    i2c_start(&MASTER_I2CD);

    /* Apply I2C Slave driver instance configuration.*/
    i2c_start(&SLAVE_I2CD);

    /* Application main loop */
    for ( ; ; ) {

        /* Initialize I2C master/slave transmit/receive buffers.*/
        for (i = 0U; i < I2C_BUFFER_SIZE; i++ ) {
            i2c_master_tx_buffer[i] = (i % 255 + j);
            i2c_slave_tx_buffer[i]  = (i % 255 + j);
            i2c_slave_rx_buffer[i]  = 0x0;
            i2c_master_rx_buffer[i] = 0x0;
        }
        j++;

        /* Master transmit - Slave receive.*/
        (void)i2c_slave_receive(&SLAVE_I2CD, i2c_slave_rx_buffer, I2C_BUFFER_SIZE);
        (void)i2c_master_transmit(&MASTER_I2CD, I2C_SLAVE_ADDRESS, i2c_master_tx_buffer, I2C_BUFFER_SIZE);

        /* Wait till I2C transfer is completed.*/
        do {
            ;
        } while(i2c_master_completed == 0 || i2c_slave_completed == 0);

        i2c_master_completed = 0;
        i2c_slave_completed  = 0;

        /* Check data received by I2C Slave.*/
        for (i = 0; i < I2C_BUFFER_SIZE; i++) {
            if (i2c_slave_rx_buffer[i] != i2c_master_tx_buffer[i]) {
                /* If data received by I2C Slave is different from data
                   trasmitted by I2C Master, then the application will stuck in
                   the following while cycle, otherwise the USER_LED_A will be
                   blinked.*/
                do {
                    ;
                } while(1);
            }
        }

        /* Slave transmit - Master receive.*/
        (void)i2c_slave_transmit(&SLAVE_I2CD, i2c_slave_tx_buffer, I2C_BUFFER_SIZE);
        (void)i2c_master_receive(&MASTER_I2CD, I2C_SLAVE_ADDRESS, i2c_master_rx_buffer, I2C_BUFFER_SIZE);

        /* Wait till I2C transfer is completed.*/
        do {
            ;
        } while(i2c_master_completed == 0 || i2c_slave_completed == 0);

        i2c_master_completed = 0;
        i2c_slave_completed  = 0;

        /* Check data received by I2C Master.*/
        for (i = 0; i < I2C_BUFFER_SIZE; i++) {
            if (i2c_master_rx_buffer[i] != i2c_slave_tx_buffer[i]) {
                /* If data received by I2C Master is different from data
                   trasmitted by I2C Slave, then the application will stuck in
                   the following while cycle, otherwise the USER_LED_A will be
                   blinked.*/
                do {
                    ;
                } while(1);
            }
        }

        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(500U);
    }
}
