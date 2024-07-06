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
#include <sent.h>
#include <sent_emulation.h>
#include <gpio.h>
#include <dma.h>
#include <uart.h>
#include <io.h>
#include <stdio.h>

/* SENT transmission line definition.*/
#define SENT_TX gpio_iopack(GPIO_PORT_F, GPIO_PIN_3)
#define SENT_TX_CFG                              \
    (GPIO_MODE_MODER_OUTPUT |                    \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_HIGH |                    \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(0U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_DISABLED |             \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

volatile bool dma_transfer_complete = false;
static sent_dma_conf_t dma_conf;

/* This callback is invoked when new SENT frames are received.*/
static void rx_callback(sent_driver_t *sdp, sent_channel_t ch) {

    sent_frame_t frame;

    /* Get all the available frames.*/
    while(sent_is_frame_available(sdp, ch)){
        sent_receive(sdp, ch, &frame);
        printf("0x%.6lX\n\r", frame.data);
    }
}

/* DMA transfer completed callback.*/
void dma_callback(tim_driver_t *tdp) {

    (void)(tdp);

    dma_transfer_complete = true;
}

/* Configuration for SENT driver.*/
static void sent_config(sent_driver_t *sdp) {

    /* Initialize SENT driver instance.*/
    sent_init(sdp);

    /* Set SENT channel DMA configuration.*/
    dma_conf.dma_stream_id       = DMA1_STREAM1_ID;
    dma_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    dma_conf.dma_stream_irq_prio = IRQ_PRIORITY_10;

    /* Initialize TIM driver instance.*/
    tim_init(sent_get_tim(sdp));

    /* Configure the SENT driver.*/
    sent_set_dma_conf(sdp, SENT_CHANNEL_1, &dma_conf);
    (void)sent_enable_dma(sdp, SENT_CHANNEL_1, true);
    (void)sent_set_nibbles(sdp, SENT_CHANNEL_1, SENT_NIBBLES_6);
    (void)sent_set_crc(sdp, SENT_CHANNEL_1, SENT_CRC_DATA);
    (void)sent_set_order(sdp, SENT_CHANNEL_1, SENT_ORDER_MSB);
    (void)sent_set_cb(sdp, SENT_CHANNEL_1, rx_callback);
}

int main(void) {

    /* SENT sensor emulation related.*/
    uint8_t sent_status, sent_nibbles;
    uint32_t sent_data, len;

    /* Enable interrupts.*/
    osal_sys_unlock();

    test_env_init((TestInit_t)
                  (TEST_INIT_CLOCK    |
                   TEST_INIT_GPIO     |
                   TEST_INIT_BOARD    |
                   TEST_INIT_IRQ      |
                   TEST_INIT_OSAL));

    /* Set output PIN configuration.*/
    gpio_set_pin_mode(SENT_TX, SENT_TX_CFG);

    /* Initialize UART driver instance.*/
    uart_init(&UARTD);

    /* Configure UART driver instance used for IO redirection.*/
    (void)uart_set_prio(&UARTD, IRQ_PRIORITY_0);
    (void)uart_set_baud(&UARTD, UART_BAUDRATE_38400);
    (void)uart_set_presc(&UARTD, UART_PRESCALER_DIV1);
    (void)uart_set_parity(&UARTD, UART_PARITY_NONE);
    (void)uart_set_over(&UARTD, UART_OVERSAMPLING_16);
    (void)uart_set_sbit(&UARTD, UART_STOPBIT_1);

    /* Initialize Runtime IO module.*/
    io_init(&UARTD);

    /* Start Runtime IO module.*/
    io_start();

    /* Configure emulated SENT sensor.*/
    sent_emul_config(SENT_TX, &ICU_TIMD, DMA1_STREAM0_ID, dma_callback);

    /* Configure the SENT driver.*/
    sent_config(&SENTD);

    /* Apply SENT driver configuration.*/
    sent_start(&SENTD);

    /* Start the SENT channel.*/
    sent_start_channel(&SENTD, SENT_CHANNEL_1);

    sent_nibbles = 6U;
    sent_data = 0UL;
    sent_status = 0U;

    /* Application main loop.*/
    for ( ; ; ) {

        /* Set signal high.*/
        gpio_write_pin(SENT_TX, 1U);

        /* Update status nibble.*/
        sent_status++;
        sent_status &= 0x0FU;

        /* Update data nibble.*/
        sent_data++;
        sent_data &= 0xFFFFFFUL;

        /* Create a new frame*/
        len = sent_emul_new_frame(sent_status, sent_data, sent_nibbles);

        /* Transmit the frame.*/
        sent_emul_transmit(&ICU_TIMD, len);

        /* Wait for DMA transfer completition.*/
        while(dma_transfer_complete == false){}
        dma_transfer_complete = false;

        /* Stop transmission.*/
        sent_emul_stop(&ICU_TIMD);
    }
}
