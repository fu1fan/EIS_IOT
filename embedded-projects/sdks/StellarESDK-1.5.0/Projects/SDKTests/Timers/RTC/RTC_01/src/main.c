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
#include <exti.h>
#include <rtc.h>
#include <dma.h>
#include <uart.h>
#include <io.h>
#include <stdio.h>

/*===========================================================================*/
/* RTC-related.                                                             */
/*===========================================================================*/

#define RTC_LSI_PREDIV_A    0x7C    /* 127 + 1*/
#define RTC_LSI_PREDIV_S    0x3FF   /* 1023 + 1*/

rtc_time_t new_time;
rtc_date_t new_date;
rtc_alarm_t alarm;
rtc_wakeup_t wut;

const char* dayNames[] = {"",
                          "Monday",
                          "Tuesday",
                          "Wednesday",
                          "Thursday",
                          "Friday",
                          "Saturday",
                          "Sunday"};

const char* AMPM[] = {"AM",
                      "PM"};

static void alarm_cb(rtc_driver_t *rdp){
    (void)rdp;

    gpio_toggle_pin(USER_LED_B);
    printf("\n\n\r Alarm event occurred.");
}

static void wakeup_cb(rtc_driver_t *rdp){
    (void)rdp;

    printf("\n\n\r Wake-up event occurred.");
}

/* This function configures RTC.*/
static void rtc_config(rtc_driver_t *rdp) {

    /* Set the clock source to obtain 1Hz.*/
    rtc_set_clock(rdp, RTC_CLOCK_SOURCE_LSI, RTC_LSI_PREDIV_S, RTC_LSI_PREDIV_A);

    /* Start time configuration: 11:29:55 AM.*/
    new_time.seconds = 55;
    new_time.minutes = 29;
    new_time.hours = 11;

    new_time.hour_format = RTC_AMPM_FORMAT;
    new_time.am_pm = RTC_AM;

    /* Start date configuration: 2023/04/07, Monday.*/
    new_date.year = 23; /* -2000 years offset.*/
    new_date.month = 4;
    new_date.date = 7;
    new_date.day = RTC_MONDAY;

    (void)rtc_set_time(rdp, &new_time);
    (void)rtc_set_date(rdp, &new_date);
    (void)rtc_set_output_format(rdp, RTC_BIN_FORMAT);

    /* Set the daylight saving time to subtract one hour.*/
    (void)rtc_set_dst(rdp, RTC_DST_SUB1H, true);

    /* alarm related, interrupt configuration.*/
    alarm.is_it_en = true;
    alarm.prio = IRQ_PRIORITY_10;

    /* Alarm configuration: 10:30:00 AM, Monday. Match all except the subseconds*/
    alarm.sub_sec = 0;
    alarm.seconds = 0;
    alarm.minutes = 30;
    alarm.hours = 10;

    alarm.hour_format = RTC_AMPM_FORMAT;
    alarm.am_pm = RTC_AM;

    alarm.date_sel = RTC_ALRM_SEL_WEEKDAY;
    alarm.date = RTC_MONDAY;

    alarm.match_day = RTC_MATCH;
    alarm.match_hour = RTC_MATCH;
    alarm.match_min = RTC_MATCH;
    alarm.match_sec = RTC_MATCH;
    alarm.match_ss = RTC_SS_MATCH_NONE;

    (void)rtc_set_cb(rdp, RTC_CB_OPS_ALARM, alarm_cb);

    (void)rtc_set_alarm(rdp, &alarm);

    /* wake-up timer related.*/
    wut.prio = IRQ_PRIORITY_10;
    wut.clk_source = RTC_WUCKSEL_CK_SPRE;

    /* Configuration to generate an interrupt every 10 seconds.*/
    wut.autoreload = 10;

    (void)rtc_set_cb(rdp, RTC_CB_OPS_WKUP, wakeup_cb);

    (void)rtc_set_wkpu(rdp, &wut);
}

int main(void) {

    rtc_time_t curr_time;
    rtc_date_t curr_date;

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

    /* Initialize UART driver instance.*/
    uart_init(&UARTD);

    /* Configure UART driver instance used for IO redirection.*/
    (void)uart_set_prio(&UARTD, IRQ_PRIORITY_5);
    (void)uart_set_baud(&UARTD, UART_BAUDRATE_38400);
    (void)uart_set_presc(&UARTD, UART_PRESCALER_DIV1);
    (void)uart_set_parity(&UARTD, UART_PARITY_NONE);
    (void)uart_set_over(&UARTD, UART_OVERSAMPLING_16);
    (void)uart_set_sbit(&UARTD, UART_STOPBIT_1);

    /* Initialize Runtime IO module.*/
    io_init(&UARTD);

    /* Start Runtime IO module.*/
    io_start();

    /* Initialize RTC driver instance.*/
    rtc_init(&RTCD);

    /* Configure RTC driver instance.*/
    rtc_config(&RTCD);

    /* Apply RTC driver instance configuration.*/
    rtc_start(&RTCD);

    /* Application main loop */
    for ( ; ; ) {

        /* Get current time and date.*/
        curr_time = rtc_get_time(&RTCD);
        curr_date = rtc_get_date(&RTCD);
        printf("\n\n\n\rCurrent time: %02d:%02d:%02d %s", curr_time.hours, curr_time.minutes, curr_time.seconds, AMPM[curr_time.am_pm]);
        printf("\n\rCurrent date: %04d/%02d/%02d, %s", curr_date.year + 2000, curr_date.month, curr_date.date, dayNames[curr_date.day]);

        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(1000U);
    }
}
