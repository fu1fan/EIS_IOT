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

#include "FreeRTOS.h"
#include "task.h"

/* Task #1 */
portTASK_FUNCTION(vTaskOne, pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for ( ;; ) {
        gpio_toggle_pin(USER_LED_A);
        vTaskDelayUntil(&xLastWakeTime, 200);
    }
}

/* Task #2 */
portTASK_FUNCTION(vTaskTwo, pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for ( ;; ) {
        vTaskDelayUntil(&xLastWakeTime, 200);
        gpio_toggle_pin(USER_LED_B);
    }
}

int main(void)
{
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

    /* Create first task to blink USER_LED_A */
    xTaskCreate(vTaskOne,
                (const char *)"task #1",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 1,
                NULL);

    /* Create second task to blink USER_LED_B */
    xTaskCreate(vTaskTwo,
                (const char *)"task #2",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 1,
                NULL);

    /* Start FreeRTOS scheduler */
    vTaskStartScheduler();

    /* Application main loop */
    for ( ; ; ) {
        osal_delay_millisec(250U);
    }
}
