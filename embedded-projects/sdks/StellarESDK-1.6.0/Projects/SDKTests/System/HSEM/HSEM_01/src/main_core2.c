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

int main(void)
{
    /* Enable interrupts.*/
    osal_sys_unlock();

    test_env_init((TestInit_t)
                  (TEST_INIT_IRQ      |
                   TEST_INIT_OSAL));

    /* Initialize HSEM module.*/
    hsem_init(&DRV_HSEM);
    /* Configure core from which this code is executed.*/
    hsem_set_core(&DRV_HSEM, HSEM_CORE_ID_CORE2);

    /* Start HSEM module.*/
    hsem_start(&DRV_HSEM);

    /* Wait till the sempaghore 30 is unlocked.*/
    while (hsem_is_sem_taken(&DRV_HSEM, HSEM_SEM_ID_30) == true);

    /* Application main loop */
    for ( ; ; ) {
        /* Blink USER_LED_B.*/
        gpio_toggle_pin(USER_LED_B);
        osal_delay_millisec(500U);
    }
}
