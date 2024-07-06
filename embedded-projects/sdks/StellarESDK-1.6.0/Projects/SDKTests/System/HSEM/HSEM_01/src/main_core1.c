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
#include <hsem.h>

volatile bool hsem_sem30_cleared = false;

void hsem_sem30_callback(hsem_driver_t *hdp) {

    (void)hdp;
    hsem_sem30_cleared = true;
}

int main(void)
{
    uint8_t i = 0U;

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

    /* Initialize HSEM module.*/
    hsem_init(&DRV_HSEM);
    /* Configure core from which this code is executed.*/
    hsem_set_core(&DRV_HSEM, HSEM_CORE_ID_CORE1);
    /* Configure HSEM priority.*/
    (void)hsem_set_prio(&DRV_HSEM, IRQ_PRIORITY_10);

    /* Enable semaphore 30 interrupt.*/
    (void)hsem_en_irq(&DRV_HSEM, HSEM_SEM_ID_30);
    (void)hsem_set_cb(&DRV_HSEM, HSEM_SEM_ID_30, hsem_sem30_callback);

    /* Apply HSEM configuration.*/
    hsem_start(&DRV_HSEM);

    /* Take the semaphore 30.*/
    (void)hsem_take_sem_2s(&DRV_HSEM, HSEM_SEM_ID_30, 0U);

    /* Configure core2 vector table address to the start of core2 flash.*/
    RCC->C2_VTOR_INIT_REG  = 0x080F0000UL;

    RCC->C2_BOOT_CTRL_REG |= (RCC_C2_BOOT_CTRL_REG_C2_RES_RELEASE);
    RCC->C2_BOOT_CTRL_REG |= (RCC_C2_BOOT_CTRL_REG_C2_CPU_WAIT_RELEASE);

    for ( ; ; ) {
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        if (hsem_sem30_cleared == false) {
            /* If the semaphore is locked, USER_LED_A is blinked each 250ms.*/
            osal_delay_millisec(250U);
        } else {
            /* If the semaphore is locked, USER_LED_A is blinked each 500ms.*/
            osal_delay_millisec(500U);
        }
        if (i <= 4U) {
            i++;
        } else if (i == 5U) {
            /* Release the semaphore 30.*/
            hsem_release_sem(&DRV_HSEM, HSEM_SEM_ID_30, 0U);
            i++;
        } else {
            ; /* close "if" as required by MISRA */
        }
    }
}
