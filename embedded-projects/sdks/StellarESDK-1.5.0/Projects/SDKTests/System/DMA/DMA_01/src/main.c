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

/*===========================================================================*/
/* DMA-related.                                                              */
/*===========================================================================*/

#define DMA_BUFFER_SIZE                     128U
#define DTCM_CORE1_CONV_D2I(_addr_)         ((uint32_t)(_addr_) + 0x3C000000UL)

volatile bool dma_completed = false;

/* DMA transfer completed callback.*/
static void dma_callback(void *arg, uint32_t sts) {

    (void)(arg);
    (void)sts;

    /* Blink USER_LED_B.*/
    gpio_toggle_pin(USER_LED_B);
    dma_completed = true;
}

/* source buffer is allocated in RAM.*/
uint8_t source[DMA_BUFFER_SIZE];
/* destination buffer is allocated in DTCM.*/
uint8_t destination[DMA_BUFFER_SIZE] __attribute__((section(".dtcm")));

int main(void)
{
    uint8_t i;
    const dma_descriptor_t *dma_descriptor;
    uint32_t dma_mode;

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

    /* Initialize DMA buffers in RAM.
       RAM:    source[i] = i
       DTCM:   destination[i] = 0xFF */
    for (i = 0U; i < DMA_BUFFER_SIZE; i++) {
        source[i] = i;
        destination[i] = 0xFF;
    }

    /* Allocates DMA stream for Memory-to-Memory transmission.*/
    dma_descriptor = dma_stream_take(DMA1_STREAM0_ID,
                                     IRQ_PRIORITY_10,
                                     dma_callback,
                                     NULL);

    /* Configure DMA stream peripheral addresses.*/
    /* In DMA M2M, the peripheral address is the source address.*/
    dma_stream_set_peripheral(dma_descriptor, (uint32_t)(source));

    /* Configure DMA priority level, data transfer direction and enable
       transfer complete interrupt.*/
    dma_mode = DMA_CCR_PL_VALUE(DMA_PRIORITY_MAX) |
                                DMA_CCR_DIR_M2M   |
                                DMA_CCR_TCIE;

    /* Configure DMA memory increment mode.*/
    dma_mode |= DMA_CCR_MINC | DMA_CCR_PINC;

    /* Configure DMA frame size.*/
    dma_mode |= DMA_CCR_PSIZE_BYTE | DMA_CCR_MSIZE_BYTE;

    /* Set up calculated DMA modes.*/
    dma_stream_set_transfer_mode(dma_descriptor, dma_mode);

    /* Configure DMA number of data items.*/
    dma_stream_set_count(dma_descriptor, DMA_BUFFER_SIZE);

    /* Configure DMA stream memory address.*/
    /* DMA can access to the DTCM only through the indirect address.*/
    dma_stream_set_memory(dma_descriptor, (uint32_t)(DTCM_CORE1_CONV_D2I(destination)));

    /* Enable DMA. It will be disabled on the DMA end of transfer.*/
    dma_stream_enable(dma_descriptor);

    /* Wait till DMA transfer is completed. When the DMA transfer is completed,
       the value of variable destination will be updated to the value of
       variable source.
       RAM:    source[i] = i
       DTCM:   destination[i] = i */
    while (dma_completed == false);

    /* Check DMA transfer.*/
    for (i = 0U; i < DMA_BUFFER_SIZE; i++) {
        if (destination[i] != source[i]) {
            while(1);
        }
    }

    /* Application main loop */
    for ( ; ; ) {
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(250U);
    }
}

