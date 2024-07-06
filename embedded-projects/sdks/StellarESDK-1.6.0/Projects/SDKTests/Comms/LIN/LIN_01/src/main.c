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
#include <lin.h>

#define UART1_RX gpio_iopack(GPIO_PORT_F, GPIO_PIN_2)
#define UART1_RX_CFG                             \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_LOW |                     \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(7U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

#define UART1_TX gpio_iopack(GPIO_PORT_F, GPIO_PIN_3)
#define UART1_TX_CFG                             \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_LOW |                     \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(7U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_DISABLED |             \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

/*===========================================================================*/
/* LIN-related.                                                              */
/*===========================================================================*/
#define LIN_BUFFER_SIZE      4U

/* For ID = 0x5 the slave will receive 4bytes from the master, while for the ID
   = 0xA the slave will transmit 4bytes to the master.*/
#define LIN_ID1              0x5U
#define LIN_ID2              0xAU

volatile bool lin_slave_rx_pid_completed = false;
volatile bool lin_slave_completed = false;
volatile bool lin_master_completed = false;

uint8_t lin_master_tx_buffer[LIN_BUFFER_SIZE];
uint8_t lin_slave_rx_buffer[LIN_BUFFER_SIZE];
uint8_t lin_master_rx_buffer[LIN_BUFFER_SIZE];
uint8_t lin_slave_tx_buffer[LIN_BUFFER_SIZE];

static void lin_rx_pid_callback(lin_driver_t *ldp) {

    (void)ldp;
    lin_slave_rx_pid_completed = true;
}

static void lin_tx_mst_callback(lin_driver_t *ldp) {

    (void)ldp;
}

static void lin_rx_slv_callback(lin_driver_t *ldp) {

    (void)ldp;
    lin_slave_completed = true;
}

static void lin_rx_mst_callback(lin_driver_t *ldp) {

    (void)ldp;
    lin_master_completed = true;
}

static void lin_tx_slv_callback(lin_driver_t *ldp) {

    (void)ldp;
}

int main(void)
{
    uint8_t i;
    uint8_t j = 0;
    uint8_t rx_pid;
    uint8_t rx_checksum;

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

    gpio_set_pin_mode(UART1_RX, UART1_RX_CFG);
    gpio_set_pin_mode(UART1_TX, UART1_TX_CFG);

    /* Initialize LIN Master driver instance.*/
    lin_init(&MASTER_LIND);
    (void)lin_set_prio(&MASTER_LIND, IRQ_PRIORITY_10);
    (void)lin_set_baud(&MASTER_LIND, LIN_BAUDRATE_9600);
    (void)lin_set_presc(&MASTER_LIND, LIN_PRESCALER_DIV1);
    (void)lin_set_bdl(&MASTER_LIND, LIN_BREAK_DETECT_LEN_11BIT);
    (void)lin_set_mode(&MASTER_LIND, LIN_MODE_MASTER);
    (void)lin_set_drv_mode(&MASTER_LIND, LIN_DRV_MODE_ASYNC);

    /* Initialize LIN Slave driver instance.*/
    lin_init(&SLAVE_LIND);
    (void)lin_set_prio(&SLAVE_LIND, IRQ_PRIORITY_5);
    (void)lin_set_baud(&SLAVE_LIND, LIN_BAUDRATE_9600);
    (void)lin_set_presc(&SLAVE_LIND, LIN_PRESCALER_DIV1);
    (void)lin_set_bdl(&SLAVE_LIND, LIN_BREAK_DETECT_LEN_11BIT);
    (void)lin_set_mode(&SLAVE_LIND, LIN_MODE_SLAVE);
    (void)lin_set_drv_mode(&SLAVE_LIND, LIN_DRV_MODE_ASYNC);
    (void)lin_set_cb(&SLAVE_LIND, LIN_CB_OPS_RX_PID, lin_rx_pid_callback);

    /* Apply LIN Master driver instance configuration.*/
    lin_start(&MASTER_LIND);

    /* Apply LIN Slave driver instance configuration.*/
    lin_start(&SLAVE_LIND);

    /* Application main loop */
    for ( ; ; ) {
        /* Master transmit - Slave receive.*/

        /* Configure callbacks.*/
        (void)lin_set_cb(&MASTER_LIND, LIN_CB_OPS_TX, lin_tx_mst_callback);
        (void)lin_set_cb(&SLAVE_LIND, LIN_CB_OPS_RX, lin_rx_slv_callback);

        /* Initialize value of protected ID received from the Slave.*/
        rx_pid = 0U;

        /* Initialize LIN Master TX buffer and LIN Slave RX buffer.*/
        for (i = 0; i < LIN_BUFFER_SIZE; i++ ) {
            lin_master_tx_buffer[i] = i + j;
            lin_slave_rx_buffer[i] = 0x0;
        }

        j = j + LIN_BUFFER_SIZE;

        lin_read_id(&SLAVE_LIND, &rx_pid);

        lin_write_id(&MASTER_LIND, LIN_ID1);

        /* Wait till LIN Slave protected ID (ID + parity bits) receiving is
           completed.*/
        do {
            ;
        } while(lin_slave_rx_pid_completed == false);

        lin_slave_rx_pid_completed = false;

        /* Check ID and parity.*/
        if (LIN_GET_ID(rx_pid) != LIN_ID1 || lin_check_parity(rx_pid) == false) {
            /* If ID received by LIN Slave is different from the ID transmitted
               by the LIN Master or parity check fails, then the application
               will stuck in the following while cycle.*/
            do{
                ;
            } while(1);
        }

        /* Start data receiving via LIN Slave.*/
        (void)lin_read(&SLAVE_LIND, (uint8_t *)lin_slave_rx_buffer, &rx_checksum, LIN_BUFFER_SIZE);

        /* Start data trasferring via LIN Master.*/
        (void)lin_write(&MASTER_LIND, (uint8_t *)lin_master_tx_buffer, LIN_BUFFER_SIZE);

        /* Wait till SPI Slave data receiving is completed.*/
        do {
            ;
        } while(lin_slave_completed == false);
    
        lin_slave_completed = false;

        /* Check data received by LIN Slave.*/
        for (i = 0; i < LIN_BUFFER_SIZE; i++) {
            if (lin_slave_rx_buffer[i] != lin_master_tx_buffer[i]) {
                /* If data received by LIN Slave is different from data
                   trasmitted by LIN Master, then the application will stuck in
                   the following while cycle.*/
                do{
                    ;
                } while(1);
            }
        }

        /* Check checksum.*/
        if (lin_check_checksum(lin_slave_rx_buffer, LIN_BUFFER_SIZE, rx_checksum) != true) {
            /* If checksum received by LIN Slave is different from the checksum
               calculated on received data, then the application will stuck in
               the following while cycle.*/
            do{
                ;
            } while(1);
        }

        /* Slave transmit - Master receive.*/

        /* Configure callbacks.*/
        (void)lin_set_cb(&MASTER_LIND, LIN_CB_OPS_RX, lin_rx_mst_callback);
        (void)lin_set_cb(&SLAVE_LIND, LIN_CB_OPS_TX, lin_tx_slv_callback);

        /* Initialize value of protected ID received from the Slave.*/
        rx_pid = 0U;

        /* Initialize LIN Master RX buffer and LIN Slave TX buffer.*/
        for (i = 0; i < LIN_BUFFER_SIZE; i++ ) {
            lin_slave_tx_buffer[i] = i + j;
            lin_master_rx_buffer[i] = 0x0;
        }

        j = j + LIN_BUFFER_SIZE;

        lin_read_id(&SLAVE_LIND, &rx_pid);

        lin_write_id(&MASTER_LIND, LIN_ID2);

        /* Wait till LIN Slave protected ID (ID + parity bits) receiving is
           completed.*/
        do {
            ;
        } while(lin_slave_rx_pid_completed == false);

        lin_slave_rx_pid_completed = false;

        /* Check ID and parity.*/
        if (LIN_GET_ID(rx_pid) != LIN_ID2 || lin_check_parity(rx_pid) == false) {
            /* If ID received by LIN Slave is different from the ID transmitted
               by the LIN Master or parity check fails, then the application
               will stuck in the following while cycle.*/
            do{
                ;
            } while(1);
        }

        /* Start data receiving via LIN Master.*/
        (void)lin_read(&MASTER_LIND, (uint8_t *)lin_master_rx_buffer, &rx_checksum, LIN_BUFFER_SIZE);

        /* Start data trasferring via LIN Slave.*/
        (void)lin_write(&SLAVE_LIND, (uint8_t *)lin_slave_tx_buffer, LIN_BUFFER_SIZE);

        /* Wait till SPI Master data receiving is completed.*/
        do {
            ;
        } while(lin_master_completed == false);
    
        lin_master_completed = false;

        /* Check data received by LIN Master.*/
        for (i = 0; i < LIN_BUFFER_SIZE; i++) {
            if (lin_master_rx_buffer[i] != lin_slave_tx_buffer[i]) {
                /* If data received by LIN Master is different from data
                   trasmitted by LIN Slave, then the application will stuck in
                   the following while cycle.*/
                do{
                    ;
                } while(1);
            }
        }

        /* Check checksum.*/
        if (lin_check_checksum(lin_master_rx_buffer, LIN_BUFFER_SIZE, rx_checksum) != true) {
            /* If checksum received by LIN Master is different from the checksum
               calculated on received data, then the application will stuck in
               the following while cycle.*/
            do{
                ;
            } while(1);
        }

        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(500U);
    }
}

