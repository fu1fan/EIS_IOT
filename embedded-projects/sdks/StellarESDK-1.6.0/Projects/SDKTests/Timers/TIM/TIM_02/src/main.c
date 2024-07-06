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
#include <tim.h>
#include <uart.h>
#include <dma.h>
#include <io.h>
#include <stdio.h>

#ifdef EVBE3000P
#define TIM3_CH1 gpio_iopack(GPIO_PORT_E, GPIO_PIN_4)
#define TIM3_CH1_CFG                             \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_LOW |                     \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(2U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

#define TIM3_CH2 gpio_iopack(GPIO_PORT_E, GPIO_PIN_2)
#define TIM3_CH2_CFG                             \
    (GPIO_MODE_MODER_ALTERNATE |                 \
     GPIO_MODE_OTYPER_PUSHPULL |                 \
     GPIO_MODE_OSPEEDR_LOW |                     \
     GPIO_MODE_PUPDR_FLOATING |                  \
     GPIO_MODE_AFR(2U) |                         \
     GPIO_MODE_IHYSTR_CMOS |                     \
     GPIO_MODE_TRIGENR_IN_ENABLED |              \
     GPIO_MODE_SAFESELR_DISABLED |               \
     GPIO_MODE_SAFEVALR_LOW)

__STATIC_INLINE void board_reinit(void) {

    gpio_set_pin_mode(TIM3_CH1, TIM3_CH1_CFG);
    gpio_set_pin_mode(TIM3_CH2, TIM3_CH2_CFG);
}
#else
__STATIC_INLINE void board_reinit(void) {

}
#endif

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
/* TIM-related.                                                              */
/*===========================================================================*/

/* PWM period.*/
#define TIM_PWM_PERIOD                      40000U

/* PWM duty cycle.*/
#define TIM_PWM_DUTY                        (TIM_PWM_PERIOD / 2U)

/* ICU maxim period (max period measureable by ICU).*/
#define TIM_ICU_MAX_PERIOD                  60000U

/* This callback is invoked on the rising edge of TIM channel 2.*/
void tim_ch2_callback(tim_driver_t *tdp) {

    uint32_t rising_edge_cnt;
    uint32_t falling_edge_cnt;
    float    duty;
    float    frequency;

    rising_edge_cnt = tim_get_comparator(tdp, 2U);
    falling_edge_cnt = tim_get_comparator(tdp, 1U);

    duty = (((float)falling_edge_cnt / rising_edge_cnt) * 100U);
    frequency = (((float)tim_get_clock(tdp)) / TIM_DIVIDE_BY(tim_get_clock(tdp) / 100000U)) / rising_edge_cnt;

    if (rising_edge_cnt != 0U && falling_edge_cnt != 0U) {
        printf("Duty cycle = %.1f%%\n\r", duty);
        printf("Frequency  = %.1fHz\n\r", frequency);
    }
}

int main(void) {

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

    /* Initialize TIM driver instances.*/
    tim_init(&TIMD);
    tim_init(&ICU_TIMD);

    /* Configure TIMD driver as PWM with a clock frequency of 100kHz.*/
    (void)tim_set_presc(&TIMD, TIM_DIVIDE_BY(tim_get_clock(&TIMD) / 100000U));
    (void)tim_set_ch_mode(&TIMD, 1U, TIM_MODE_PWM_ACTIVE_HIGH);

    /* Configure ICU_TIMD driver as ICU with CC1 mapped on channel 2 (using the
       indirect mode) to capture the falling edge and CC2 mapped on channel 2
       to capture the rising edge, and with clock frequency of 100kHz.*/
    (void)tim_set_ch_mode(&ICU_TIMD, 1U, TIM_MODE_ICU_FALLING_EDGE_IND);
    (void)tim_set_ch_mode(&ICU_TIMD, 2U, TIM_MODE_ICU_RISING_EDGE);
    (void)tim_set_presc(&ICU_TIMD, TIM_DIVIDE_BY(tim_get_clock(&ICU_TIMD) / 100000U));

    /* Configure ICU_TIMD to generate a callback on the event related to
       CC2 (i.e. rising edge of channel 2).*/
    (void)tim_set_cb(&ICU_TIMD, TIM_CB_OPS_CC2, tim_ch2_callback);

    /* Configure ICU_TIMD CC2 to be reset on the rising edge of channel 2.*/
    tim_set_sms(&ICU_TIMD, TIM_SMODE_RESET);
    tim_set_trgi(&ICU_TIMD, TIM_TRGI_TI2FP2);

    /* Apply TIM configurations.*/
    tim_start(&TIMD);
    tim_start(&ICU_TIMD);

    /* Enable ICU_TIMD channel 2 callback.*/
    tim_enable_channel_callback(&ICU_TIMD, 2U);

    /* Configure channel 1 of TIMD with a period = 400ms (2.5Hz) and a duty
       cycle = 50%.*/
    tim_set_interval(&TIMD, TIM_PWM_PERIOD);          /* 40000/100kHz = 400ms.*/
    tim_set_comparator(&TIMD, 1U, TIM_PWM_DUTY);      /* Duty cycle 50%.      */

    /* Configure ICU_TIMD input capture maximum period to 600ms.*/
    tim_set_interval(&ICU_TIMD, TIM_ICU_MAX_PERIOD);  /* 60000/100kHz = 600ms.*/

    /* Enable TIMs.*/
    tim_enable(&TIMD);
    tim_enable(&ICU_TIMD);

    /* Application main loop */
    for ( ; ; ) {
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(500U);
    }
}
