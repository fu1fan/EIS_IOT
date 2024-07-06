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
#include <crc.h>

/*===========================================================================*/
/* CRC-related.                                                             */
/*===========================================================================*/

#define CRC_BUFF_SIZE                       39U
#define CRC_BUFF_SIZE1                      20U
#define CRC_BUFF_SIZE2                      19U

/* CRC expected value.*/
#define CRC_EXPECTED_VALUE                  0x5C4581C0U

volatile bool crc_completed = false;

static crc_dma_conf_t crc_dma_config;
static uint32_t result = 0;

/* This callback is invoked when the async crc calculation is completed.*/
static void crc_completed_cb(crc_driver_t *cdp) {

    result = crc_get_result(cdp);

    crc_completed = true;
}

/* This function configures CRC.*/
static void crc_config(crc_driver_t *cdp) {

    /* Configure CRC DMA.*/
    crc_dma_config.dma_stream_id = DMA1_STREAM0_ID;
    crc_dma_config.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    crc_dma_config.dma_stream_irq_prio = IRQ_PRIORITY_10;

    (void)crc_set_init(cdp, 0xFFFFFFFF);
    (void)crc_set_rev_in(cdp, CRC_REV_IN_BYTE);
    (void)crc_set_rev_out(cdp, CRC_REV_OUT_BIT);
    (void)crc_set_polysize(cdp, CRC_POLYSIZE_32);
    (void)crc_enable_dma(cdp, true);
    crc_set_dma_conf(cdp, &crc_dma_config);
    (void)crc_set_drv_mode(cdp, CRC_DRV_MODE_ASYNC);
    (void)crc_set_cb(cdp, crc_completed_cb);
}

int main(void) {

    uint32_t res, res_concat;

    /* Data will be digested with 9 32-bit, 1 16-bit and 1 8-bit operations.*/
    uint8_t dataBuffer[CRC_BUFF_SIZE] = {0x21, 0x10, 0x00, 0x00,
                                         0x63, 0x30, 0x42, 0x20,
                                         0xa5, 0x50, 0x84, 0x40,
                                         0xe7, 0x70, 0xc6, 0x60,
                                         0x4a, 0xa1, 0x29, 0x91,
                                         0x8c, 0xc1, 0x6b, 0xb1,
                                         0xce, 0xe1, 0xad, 0xd1,
                                         0x31, 0x12, 0xef, 0xf1,
                                         0x52, 0x22, 0x73, 0x32,
                                         0xa1, 0xb2,
                                         0xc3};

    /* Data split in two different buffers to test the concatenation mode.*/
    uint8_t dataBuffer1[CRC_BUFF_SIZE1] = {0x21, 0x10, 0x00, 0x00,
                                           0x63, 0x30, 0x42, 0x20,
                                           0xa5, 0x50, 0x84, 0x40,
                                           0xe7, 0x70, 0xc6, 0x60,
                                           0x4a, 0xa1, 0x29, 0x91};

    uint8_t dataBuffer2[CRC_BUFF_SIZE2] = {0x8c, 0xc1, 0x6b, 0xb1,
                                           0xce, 0xe1, 0xad, 0xd1,
                                           0x31, 0x12, 0xef, 0xf1,
                                           0x52, 0x22, 0x73, 0x32,
                                           0xa1, 0xb2,
                                           0xc3};

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

    /* Initialize CRC driver instance.*/
    crc_init(&CRCD);

    /* Configure CRC driver instance.*/
    crc_config(&CRCD);

    /* Apply CRC driver instance configuration.*/
    crc_start(&CRCD);

    /* Calculate one-shot CRC.*/
    crc_calculate(&CRCD, CRC_MODE_NEW, dataBuffer, CRC_BUFF_SIZE);

    /* Wait till CRC calculation is completed.*/
    do {
        ;
    } while(crc_completed == false);

    crc_completed = false;
    res = result;

    /* Calculate concatenating CRC.*/
    crc_calculate(&CRCD, CRC_MODE_NEW, dataBuffer1, CRC_BUFF_SIZE1);

    /* Wait till CRC calculation is completed.*/
    do {
        ;
    } while(crc_completed == false);

    crc_completed = false;

    crc_calculate(&CRCD, CRC_MODE_CONCAT, dataBuffer2, CRC_BUFF_SIZE2);

    /* Wait till CRC calculation is completed.*/
    do {
        ;
    } while(crc_completed == false);

    res_concat = result;

    /* Predefined CRC32 calculation includes a XOR of result with 0xFFFFFFFF.*/
    res = res ^ 0xFFFFFFFF;
    res_concat = res_concat ^ 0xFFFFFFFF;

    crc_stop(&CRCD);

    crc_deinit(&CRCD);

    /* Application main loop */
    for ( ; ; ) {
        if((res == CRC_EXPECTED_VALUE) && (res_concat == CRC_EXPECTED_VALUE)) {
            /* Blink USER_LED_A.*/
            gpio_toggle_pin(USER_LED_A);
            osal_delay_millisec(500U);
        }
    }
}
