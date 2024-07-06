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
#include <mpu.h>

/* User led toggling function, executed from ITCM.*/
static void __attribute__((section(".itcm"))) user_led_toggling(void) {

    gpio_toggle_pin(USER_LED_A);
}

/* Switch core into privileged mode via control register.*/
static void move_in_privileged_mode(void) {

    uint32_t control;

    control = __get_CONTROL();
    control &= ~1U;
    __set_CONTROL(control);
}

/* Switch core into unprivileged mode via control register.*/
static void move_in_unprivileged_mode(void) {

    uint32_t control;

    control = __get_CONTROL();
    control |= 1U;
    __set_CONTROL(control);
}

/* Memory Management Unit fault handler.*/
IRQ_HANDLER(MemManage_Handler) {

    /* Switch-on the user LED B to report the fault.*/
    gpio_toggle_pin(USER_LED_B);

    /* Disable MPU (MPU can be disabled here since in interrrupt handlers the
       core is in privileged mode).*/
    mpu_disable();
}

/* Supervisor call handler.*/
IRQ_HANDLER(SVC_Handler) {

    /* Switch core into privileged mode.*/
    move_in_privileged_mode();
}

/*===========================================================================*/
/* MPU-related.                                                              */
/*===========================================================================*/

/* MPU Region0 = RAM, 128k, R/W */
static mpu_region_config_t mpu_reg0_cfg  = {
    .enable = MPU_REG_ENABLE,
    .number = MPU_REG_NUMBER0,
    .base_address = 0x24000000UL,
    .size = MPU_REG_SIZE_128KB,
    .subregion_disable = MPU_REG_SUBREG_ENABLE_ALL,
    .type_ext = MPU_REG_TEX_LEVEL0,
    .access_permission = MPU_REG_FULL_ACCESS,
    .instruction_disable = MPU_REG_INSTRUCTION_ACCESS_ENABLE,
    .shareable = MPU_REG_SHAREABLE,
    .cacheable = MPU_REG_CACHEABLE,
    .bufferable = MPU_REG_NOT_BUFFERABLE,
};

/* MPU Region1 = Flash, 512k, R/W */
static mpu_region_config_t mpu_reg1_cfg  = {
    .enable = MPU_REG_ENABLE,
    .number = MPU_REG_NUMBER1,
    .base_address = 0x08000000UL,
    .size = MPU_REG_SIZE_512KB,
    .subregion_disable = MPU_REG_SUBREG_ENABLE_ALL,
    .type_ext = MPU_REG_TEX_LEVEL0,
    .access_permission = MPU_REG_FULL_ACCESS,
    .instruction_disable = MPU_REG_INSTRUCTION_ACCESS_ENABLE,
    .shareable = MPU_REG_NOT_SHAREABLE,
    .cacheable = MPU_REG_CACHEABLE,
    .bufferable = MPU_REG_NOT_BUFFERABLE,
};

/* MPU Region2 = ITCM, 32k, Access from privileged software only */
static mpu_region_config_t mpu_reg2_cfg  = {
    .enable = MPU_REG_ENABLE,
    .number = MPU_REG_NUMBER2,
    .base_address = 0x00000000,
    .size = MPU_REG_SIZE_32KB,
    .subregion_disable = MPU_REG_SUBREG_ENABLE_ALL,
    .type_ext = MPU_REG_TEX_LEVEL0,
    .access_permission = MPU_REG_PRIV_RW,
    .instruction_disable = MPU_REG_INSTRUCTION_ACCESS_ENABLE,
    .shareable = MPU_REG_NOT_SHAREABLE,
    .cacheable = MPU_REG_NOT_CACHEABLE,
    .bufferable = MPU_REG_NOT_BUFFERABLE,
};

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

    /* Configure MPU regions.*/
    mpu_set_region(&mpu_reg0_cfg);
    mpu_set_region(&mpu_reg1_cfg);
    mpu_set_region(&mpu_reg2_cfg);

    /* Enable MPU.*/
    mpu_enable(MPU_CTRL_HFNMI_PRIVDEF_ENABLED);

    /* Access to ITCM (currently allowed since the core is in privileged mode).*/
    user_led_toggling();

    osal_delay_millisec(500U);

    /* Switch core into unprivileged mode.*/
    move_in_unprivileged_mode();

    /* Access to ITCM (currently not allowed since the core is in unprivileged
       mode => a memory management fault will be generated).*/
    user_led_toggling();

    /* Move back to privileged mode using a supervisor call instruction.*/
    __asm("SVC #0");

    /* Re-enable the MPU (MPU can be re-enabled here since the core is again in
       privileged mode.*/
    mpu_enable(MPU_CTRL_HFNMI_PRIVDEF_ENABLED);

    osal_delay_millisec(500U);

    /* Application main loop */
    for ( ; ; ) {
        /* Access to ITCM (currently allowed since the core is again in
           privileged mode).*/
        user_led_toggling();
        osal_delay_millisec(250U);
    }
}
