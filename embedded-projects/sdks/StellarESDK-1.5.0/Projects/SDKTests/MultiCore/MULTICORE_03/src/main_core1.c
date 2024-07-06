/********************************* *******************************************
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

extern const char core2_bin_start[];

/*===========================================================================*/
/* SEV-related.                                                              */
/*===========================================================================*/

volatile bool core2_sev = false;

/* Core 2 SEV interrupt handler.*/
IRQ_HANDLER(IRQ_CORE2_SEV_HANDLER)
{
    IRQ_PROLOGUE();

    core2_sev = true;

    IRQ_EPILOGUE();
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

    /* Enable core 2 SEV interrupt.*/
    irq_set_priority(IRQ_CORE2_SEV_VECTOR, IRQ_PRIORITY_5);
    irq_enable(IRQ_CORE2_SEV_VECTOR);

    /* Configure core2 vector table address to the start of core2 binary code.*/
    RCC->C2_VTOR_INIT_REG  = (uint32_t)(core2_bin_start);

    /* Run core2.*/
    RCC->C2_BOOT_CTRL_REG |= (RCC_C2_BOOT_CTRL_REG_C2_RES_RELEASE);
    RCC->C2_BOOT_CTRL_REG |= (RCC_C2_BOOT_CTRL_REG_C2_CPU_WAIT_RELEASE);

    /* Wait an event from core2.*/
    while (core2_sev == false);

    /* Application main loop */
    for ( ; ; ) {
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(250U);
    }
}
