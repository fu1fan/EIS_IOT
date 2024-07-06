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
#include <can.h>

/*===========================================================================*/
/* CAN-related.                                                              */
/*===========================================================================*/

#define CAN_DEFAULT_FILTER_ID               0x7F0UL

#define CAN_DEFAULT_RX_BUFF_NUM             4U
#define CAN_DEFAULT_TX_BUFF_NUM             4U
#define CAN_DEFAULT_RX_FIFO0_NUM            4U
#define CAN_DEFAULT_RX_FIFO1_NUM            4U
#define CAN_DEFAULT_TX_FIFOQ_NUM            4U

#define CAN_DATA0_TO_SEND                   0xF0F0F0F0UL
#define CAN_DATA1_TO_SEND                   0xA5A5A5A5UL
#define CAN_DATA2_TO_SEND                   0x0F0F0F0FUL
#define CAN_MESSAGE_SIZE                    16U

static uint8_t i = 0U;

/* This callback is invoked when a frame is received.*/
static void canrx_receive_cb(can_driver_t *cdp, can_frame_t frame, can_evt_t event) {

    (void)cdp;
    (void)frame;
    (void)event;

    /* Verify the received frame is equal to trasmitted frame.*/
    if ((event == CAN_CB_FIFO1_NEW_MSG)        &&
        (frame.TYPE == CAN_ID_STD)             &&
        (frame.ID == CAN_DEFAULT_FILTER_ID)    &&
        (frame.data32[0] == CAN_DATA0_TO_SEND) &&
        (frame.data32[1] == CAN_DATA1_TO_SEND) &&
        (frame.data32[2] == CAN_DATA2_TO_SEND) &&
        (frame.data32[3] == i)) {
        gpio_toggle_pin(USER_LED_B);
        i++;
    } else {
        while(1);
    }
}

/* This callback is invoked when an error occurs during reception.*/
static void canrx_error_cb(can_driver_t *cdp, uint32_t psr) {

    (void)cdp;
    (void)psr;
    while(1);
}

/* This callback is invoked when an error occurs during transmission.*/
static void cantx_error_cb(can_driver_t *cdp, uint32_t psr) {

    (void)cdp;
    (void)psr;
    while(1);
}

int main(void) {

    bool status;
    can_frame_t frame;

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

    /* Initialize CAN RX/TX driver instances.*/
    can_init(&CANRXD);
    can_init(&CANTXD);

    /* Configure CAN RX/TX.*/
    (void)can_set_irq_line(&CANRXD, CAN_IRQ_LINE0);
    (void)can_set_irq_line(&CANTXD, CAN_IRQ_LINE0);

    (void)can_set_irq_mask(&CANRXD, CAN_IRQ_ALL_ENABLED);
    (void)can_set_irq_mask(&CANTXD, CAN_IRQ_ALL_ENABLED);

    (void)can_set_loopback(&CANRXD, CAN_NO_LOOPBACK);
    (void)can_set_loopback(&CANTXD, CAN_NO_LOOPBACK);

    (void)can_set_fd_mode(&CANRXD, true);
    (void)can_set_fd_mode(&CANTXD, true);

    (void)can_set_baudrate(&CANRXD, CAN_BAUDRATE_1MBIT);
    (void)can_set_baudrate(&CANTXD, CAN_BAUDRATE_1MBIT);

    (void)can_set_datasize(&CANRXD, CAN_DATASIZE_16BYTE);
    (void)can_set_datasize(&CANTXD, CAN_DATASIZE_16BYTE);

    (void)can_set_dmu_mode(&CANRXD, false);
    (void)can_set_dmu_mode(&CANTXD, false);

    (void)can_set_filter(&CANRXD, CAN_DEFAULT_FILTER_ID, 0U,
                         CAN_STD_FILTER_TYPE, CAN_FILTER_CLASSIC,
                         CAN_FEC_FIFO1);
    (void)can_set_filter(&CANTXD, CAN_DEFAULT_FILTER_ID, 0U,
                         CAN_STD_FILTER_TYPE, CAN_FILTER_CLASSIC,
                         CAN_FEC_RX_BUF);

    can_set_buffers(&CANRXD, CAN_DEFAULT_RX_BUFF_NUM, CAN_DEFAULT_TX_BUFF_NUM,
                    CAN_DEFAULT_RX_FIFO0_NUM, CAN_DEFAULT_RX_FIFO1_NUM,
                    CAN_DEFAULT_TX_FIFOQ_NUM);
    can_set_buffers(&CANTXD, CAN_DEFAULT_RX_BUFF_NUM, CAN_DEFAULT_TX_BUFF_NUM,
                    CAN_DEFAULT_RX_FIFO0_NUM, CAN_DEFAULT_RX_FIFO1_NUM,
                    CAN_DEFAULT_TX_FIFOQ_NUM);

    (void)can_set_rx_polling_mode(&CANRXD, false);
    (void)can_set_tx_mode(&CANTXD, CAN_TX_MODE_FIFO);

    (void)can_set_cb(&CANRXD, canrx_receive_cb);
    (void)can_set_err_cb(&CANRXD, canrx_error_cb);
    (void)can_set_err_cb(&CANTXD, cantx_error_cb);

    /* Apply CAN RX/TX configurations.*/
    can_start(&CANRXD);
    can_start(&CANTXD);

    /* Prepare frame to transmit.*/
    frame.TYPE = CAN_ID_STD;
    frame.ID = CAN_DEFAULT_FILTER_ID;
    frame.DLC = CAN_MESSAGE_SIZE;
    frame.data32[0] = CAN_DATA0_TO_SEND;
    frame.data32[1] = CAN_DATA1_TO_SEND;
    frame.data32[2] = CAN_DATA2_TO_SEND;
    frame.OPERATION = CAN_OPERATION_CANFD;

    /* Application main loop */
    for ( ; ; ) {
        frame.data32[3] = i;
        status = can_transmit(&CANTXD, CAN_TX_FIFO, &frame);
        if (status == false) {
            while(1);
        }
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(500U);
    }
}

