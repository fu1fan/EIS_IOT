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
#include <eed.h>
#include <uart.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

#include <flash.h>

/*===========================================================================*/
/* EED-related.                                                              */
/*===========================================================================*/

/* Number of records used for the testing.*/
#define EED_ID_NUM                          150U

/* Max record size [byte].*/
#define EED_MAX_ID_SIZE                     100U

/* Number of test cycles.*/
#define TEST_CYCLE_NUM                      2000UL

/* Seed for randomic generation.*/
#define TEST_RAND_SEED                      1254UL

/*===========================================================================*/
/* UART-related.                                                             */
/*===========================================================================*/

static void uart_config(uart_driver_t *udp) {

    (void)uart_set_prio(udp, IRQ_PRIORITY_5);
    (void)uart_set_baud(udp, UART_BAUDRATE_38400);
    (void)uart_set_presc(udp, UART_PRESCALER_DIV1);
    (void)uart_set_parity(udp, UART_PARITY_NONE);
    (void)uart_set_over(udp, UART_OVERSAMPLING_16);
    (void)uart_set_sbit(udp, UART_STOPBIT_1);
}

/* This function is invoked if an error occurred.*/
static void error_trap(void) {

    for ( ; ; ) {
        /* Blink USER_LED_B.*/
        gpio_toggle_pin(USER_LED_B);
        osal_delay_millisec(250U);
    }
}

int main(void)
{
    uint16_t size;
    uint8_t  data[EED_MAX_ID_SIZE];
    uint32_t i, j, h;

    uint16_t id;
    uint8_t  idsize[EED_ID_NUM];
    uint8_t  idfirstvalue[EED_ID_NUM];

    uint8_t  firstvalue;
    uint32_t freespace;

    uint32_t retval;

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

    /* Initialize random generation.*/
    srand(TEST_RAND_SEED);

    /* Initialize UART driver instance used for IO redirection.*/
    uart_init(&UARTD);

    /* Configure UART driver instance used for IO redirection.*/
    uart_config(&UARTD);

    /* Initialize Runtime IO module.*/
    io_init(&UARTD);

    /* Start Runtime IO module.*/
    io_start();

    /* Init EED.*/
    (void)eed_init(&EEDD);

    /* Apply EDD configuration and start EED.*/
    retval = eed_start(&EEDD);
    if (retval != EED_OK && retval != EED_FIRST_INIT_OK) {
        error_trap();
    }

    /***************/
    /* Remove test.*/
    /***************/

    printf("Remove EED and verify the re-start...\n\r");

    /* Remove EDD erasing all EED data.*/
    retval = eed_remove(&EEDD);
    if (retval != EED_OK) {
        error_trap();
    }

    /* Re-apply EDD configuration and re-start EED.*/
    retval = eed_start(&EEDD);
    if (retval != EED_FIRST_INIT_OK) {
        error_trap();
    }

    printf("done!!!\n\r");

    /**************/
    /* Write test.*/
    /**************/

    printf("Write all records for the first time...\n\r");

    /* Write all records for the first time.*/
    for (i = 0; i < EED_ID_NUM; i++) {
        id = (uint16_t)i;
        idsize[id] = (uint8_t)rand() % EED_MAX_ID_SIZE;
        idfirstvalue[id] = (uint8_t)rand() % 0xFFU;
        for (j = 0; j < idsize[id]; j++) {
            data[j] = (idfirstvalue[id] + (uint8_t)j) % 0xFFU;
        }
        retval = eed_write_id(&EEDD, id, (uint16_t)idsize[id], (uint32_t)data);
        if (retval != EED_OK) {
            error_trap();
        }
    }

    printf("done!!!\n\r");

    /***************/
    /* Update test.*/
    /***************/

    /* Then randomically a record is selected and updated in terms of size and
       data.*/
    printf("Update randomically the records and verify them...\n\r");
    for (i = 0; i < TEST_CYCLE_NUM; i++) {
        id = (uint16_t)rand() % EED_ID_NUM;
        idsize[id] = (uint8_t)rand() % EED_MAX_ID_SIZE;
        idfirstvalue[id] = (uint8_t)rand() % 0xFFU;
        for (j = 0; j < idsize[id]; j++) {
            data[j] = (idfirstvalue[id] + (uint8_t)j) % 0xFFU;
        }
        retval = eed_write_id(&EEDD, id, (uint16_t)idsize[id], (uint32_t)data);
        if (retval != EED_OK) {
            error_trap();
        }

        /* Each 100 test cycles the EED is re-started and all records are read
           back and verified.*/
        if (i > 1UL && i % 100UL == 0UL) {
            retval = eed_start(&EEDD);
            if (retval != EED_OK) {
                error_trap();
            }
	    
            /* Read back all records and verify them.*/
            for (j = 0; j < EED_ID_NUM; j++) {
                /* Read back the record.*/
                retval = eed_read_id(&EEDD, (uint16_t)j, &size, (uint32_t)data);
                if (retval != EED_OK) {
                    error_trap();
                }
                /* Verify record size.*/
                if (size != (uint16_t)idsize[j]) {
                    error_trap();
                }
                /* Verify record data.*/
                firstvalue = idfirstvalue[j];
                for (h = 0; h < idsize[j]; h++) {
                    if (data[h] != (firstvalue + (uint8_t)h) % 0xFFU) {
                        error_trap();
                    }
                }
            }
        }
    }

    printf("done!!!\n\r");

    /******************/
    /* Read back test.*/
    /******************/

    printf("Read back all records and verify them...\n\r");

    /* Clean data buffer.*/
    for (i = 0; i < EED_MAX_ID_SIZE; i++) {
        data[i] = 0xFFU;
    }

    /* Read back and verify all records.*/
    for (i = 0; i < EED_ID_NUM; i++) {
        /*Read record.*/
        retval = eed_read_id(&EEDD, (uint16_t)i, &size, (uint32_t)data);
        if (retval != EED_OK) {
            error_trap();
        }
        /* Verify record size.*/
        if (size != idsize[i]) {
            error_trap();
        }
        /* Verify record data.*/
        firstvalue = idfirstvalue[i];
        for (j = 0; j < idsize[i]; j++) {
            if (data[j] != (firstvalue + (uint8_t)j) % 255U) {
                error_trap();
            }
        }
    }

    printf("done!!!\n\r");

    /******************/
    /* Freespace test.*/
    /******************/

    printf("Check free space...\n\r");

    /* Get current free space.*/
    freespace = eed_get_freespace(&EEDD);

    /* Re-start EED.*/
    retval = eed_start(&EEDD);
    if (retval != EED_OK) {
        error_trap();
    }

    /* Check freespace after EED re-start is not changed.*/
    if (eed_get_freespace(&EEDD) != freespace) {
        error_trap();
    }

    printf("done!!!\n\r");

    /***************/
    /* Delete test.*/
    /***************/

    printf("Delete records and check they are not present anymore...\n\r");

    /* Remove EDD erasing all EED data.*/
    retval = eed_remove(&EEDD);
    if (retval != EED_OK) {
        error_trap();
    }

    /* Re-apply EDD configuration and re-start EED.*/
    retval = eed_start(&EEDD);
    if (retval != EED_FIRST_INIT_OK) {
        error_trap();
    }

    /* Write all records.*/
    for (i = 0; i < EED_ID_NUM; i++) {
        retval = eed_write_id(&EEDD, (uint16_t)i, 10U, (uint32_t)data);
        if (retval != EED_OK) {
            error_trap();
        }
    }

    /* Delete first 5 records.*/
    for (i = 0; i < 5U; i++) {
        retval = eed_delete_id(&EEDD, (uint16_t)i);
        if (retval != EED_OK) {
            error_trap();
        }
    }

    /* Check first 5 records have been deleted.*/
    for (i = 0; i < 5U; i++) {
        retval = eed_read_id(&EEDD, (uint16_t)i, &size, (uint32_t)data);
        if (retval != EED_ERROR_ID_NOT_FOUND) {
            error_trap();
        }
    }

    printf("done!!!\n\r");

    /* Remove EDD after test completion.*/
    retval = eed_remove(&EEDD);
    if (retval != EED_OK) {
        error_trap();
    }

    /* Application main loop */
    for ( ; ; ) {
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(250U);
    }
}

