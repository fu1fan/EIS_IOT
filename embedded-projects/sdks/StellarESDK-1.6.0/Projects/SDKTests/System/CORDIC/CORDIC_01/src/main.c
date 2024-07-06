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
#include <cordic.h>
#include <uart.h>
#include <io.h>
#include <stdio.h>

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

/*===========================================================================*/
/* CORDIC-related.                                                           */
/*===========================================================================*/

/* Number of CORDIC calculations.*/
#define CORDIC_CALC_NUM                     9U
/* Number of CORDIC arguments.*/
#define CORDIC_ARG_NUM                      (2 * CORDIC_CALC_NUM)
/* Number of CORDIC results.*/
#define CORDIC_RES_NUM                      (2 * CORDIC_CALC_NUM)

/* This callback is invoked when CORDIC calculation is completed.*/
static void cordic_calc_complete_cb(cordic_driver_t *cpd) {

    (void)cpd;

    /* Blink USER_LED_A.*/
    gpio_toggle_pin(USER_LED_A);
}

/* This function configures the CORDIC.*/
static void cordic_config(cordic_driver_t *cdp) {

    cordic_dma_conf_t cordic_dma_conf;

    /* Configure CORDIC DMA.*/
    cordic_dma_conf.dma_stream_wr_id = DMA1_STREAM0_ID;
    cordic_dma_conf.dma_stream_rd_id = DMA1_STREAM1_ID;
    cordic_dma_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    cordic_dma_conf.dma_stream_irq_prio = IRQ_PRIORITY_10;

    /* Configure CORDIC.*/
    (void)cordic_set_drv_mode(cdp, CORDIC_DRV_MODE_DMA_SYNC);
    cordic_set_dma_conf(cdp, &cordic_dma_conf);
    (void)cordic_set_function(cdp, CORDIC_FUNC_SINE);
    (void)cordic_set_precision(cdp, CORDIC_PREC_6CYCLES);
    (void)cordic_set_cb(cdp, cordic_calc_complete_cb);
    cordic_set_res(cdp, CORDIC_NUM_2_32BIT, CORDIC_WIDTH_32BIT);
    cordic_set_arg(cdp, CORDIC_NUM_2_32BIT, CORDIC_WIDTH_32BIT);
}

/* This function converts a float in Q31 format.*/
static int32_t convert_float_to_q31(float f) {

    return (int32_t)(f * 0x80000000UL);
}

/* This function converts a Q31 format in a float.*/
static float convert_q31_to_float(int32_t q31) {

    return (q31 / ((float)(0x80000000UL)));
}

int main(void)
{
    const int32_t angles[CORDIC_CALC_NUM] = {-180,
                                             -135,
                                              -90,
                                              -45,
                                                0,
                                               45,
                                               90,
                                              135,
                                              180};

    int32_t input[CORDIC_ARG_NUM]  = {0x0UL};
    int32_t output[CORDIC_RES_NUM] = {0x0UL};

    uint8_t i;

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

    /* Initialize UART driver instance used for IO redirection.*/
    uart_init(&UARTD);

    /* Configure UART driver instance used for IO redirection.*/
    uart_config(&UARTD);

    /* Initialize Runtime IO module.*/
    io_init(&UARTD);

    /* Start Runtime IO module.*/
    io_start();

    /* Initialize CORDIC driver instance.*/
    cordic_init(&CORDICD);

    /* Configure CORDIC driver instance.*/
    cordic_config(&CORDICD);

    /* Apply CORDIC instance configuration.*/
    cordic_start(&CORDICD);

    /* Calculate CORDIC arguments.*/
    for (i = 0; i < CORDIC_ARG_NUM; i = i + 2U) {
        /* CORDIC first argument is the angle in radians divided for pi ([-1, 1]).*/
        input[i] = convert_float_to_q31(angles[i/2] / (float)180);
        /* CORDIC second argument is the module ([0, 1]), fixed to 1.*/
        input[i + 1] = convert_float_to_q31(1.0f);
    }

    /* Application main loop */
    for ( ; ; ) {

        printf("START CORDIC CALCULATION...\n\r");

        /* Run CORDIC calculations.*/
        cordic_calculate(&CORDICD, input, output, CORDIC_CALC_NUM);

        /* Print results of CORDIC calculation.*/
        for (i = 0; i < CORDIC_RES_NUM; i = i + 2U) {
            printf("sin(%ld)=%.3f - cos(%ld)=%.3f\n\r", angles[i/2], convert_q31_to_float(output[i]), 
                                                        angles[i/2], convert_q31_to_float(output[i+1]));
        }
        printf("CALCULATION COMPLETED.\n\r\n\r");

        osal_delay_millisec(250U);
    }
}
