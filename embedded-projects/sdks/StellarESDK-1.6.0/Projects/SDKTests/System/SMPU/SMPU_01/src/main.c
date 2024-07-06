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
#include <smpu.h>

/*===========================================================================*/
/* SMPU-related.                                                             */
/*===========================================================================*/

IRQ_HANDLER(IRQ_HARDFAULT_HANDLER) {

    smpu_error_info_t err_info;

    err_info = smpu_get_error(&SMPUD);

    if(err_info.address == 0x24038050) {
        /* Toggle USER_LED_B.*/
        gpio_toggle_pin(USER_LED_B);
    }

    /* Clear SMPU errors.*/
    smpu_clear_errors(&SMPUD);

    /* Set access rights in order to avoid the violation.*/
    smpu_stop(&SMPUD);
    smpu_set_rights(&SMPUD, SMPU_REGION_1, SMPU_MASTER_CORE_1,   SMPU_READ_WRITE_ACCESS);
    smpu_set_rights(&SMPUD, SMPU_REGION_1, SMPU_MASTER_CORE_2,   SMPU_READ_WRITE_ACCESS);
    smpu_set_rights(&SMPUD, SMPU_REGION_1, SMPU_MASTER_DMA1,     SMPU_READ_WRITE_ACCESS);
    smpu_set_rights(&SMPUD, SMPU_REGION_1, SMPU_MASTER_DMA2,     SMPU_READ_WRITE_ACCESS);
    smpu_set_rights(&SMPUD, SMPU_REGION_1, SMPU_MASTER_CORE_HSM, SMPU_READ_WRITE_ACCESS);
    smpu_set_rights(&SMPUD, SMPU_REGION_1, SMPU_MASTER_DEBUG,    SMPU_READ_WRITE_ACCESS);
    smpu_start(&SMPUD);

    /* Increment the PC to exit the hardfault loop.*/
    asm(
        "ldr     r0, [sp,#24] \n"      /* Load saved PC    */
        "add     r0, #4 \n"            /* Increment the PC */
        "str     r0, [sp,#24] \n"      /* Save modified PC */
    );
}

void smpu_config(smpu_driver_t *sdp) {

    /* Define a region with read/write access.*/
    smpu_enable_region(sdp, SMPU_REGION_0, true);
    smpu_set_start_address(sdp, SMPU_REGION_0, 0x00000000);
    smpu_set_end_address(sdp, SMPU_REGION_0, 0x2403804F);
    smpu_set_rights(sdp, SMPU_REGION_0, SMPU_MASTER_CORE_1,   SMPU_READ_WRITE_ACCESS);
    smpu_set_rights(sdp, SMPU_REGION_0, SMPU_MASTER_CORE_2,   SMPU_READ_WRITE_ACCESS);
    smpu_set_rights(sdp, SMPU_REGION_0, SMPU_MASTER_DMA1,     SMPU_READ_WRITE_ACCESS);
    smpu_set_rights(sdp, SMPU_REGION_0, SMPU_MASTER_DMA2,     SMPU_READ_WRITE_ACCESS);
    smpu_set_rights(sdp, SMPU_REGION_0, SMPU_MASTER_CORE_HSM, SMPU_READ_WRITE_ACCESS);
    smpu_set_rights(sdp, SMPU_REGION_0, SMPU_MASTER_DEBUG,    SMPU_READ_WRITE_ACCESS);
    smpu_set_cacheability(sdp, SMPU_REGION_0, true);
    smpu_set_read_only(sdp, SMPU_REGION_0, false);

    /* Define a region with read access.*/
    smpu_enable_region(sdp, SMPU_REGION_1, true);
    smpu_set_start_address(sdp, SMPU_REGION_1, 0x24038050);
    smpu_set_end_address(sdp, SMPU_REGION_1, 0x2403805F);
    smpu_set_rights(sdp, SMPU_REGION_1, SMPU_MASTER_CORE_1,   SMPU_READ_ACCESS);
    smpu_set_rights(sdp, SMPU_REGION_1, SMPU_MASTER_CORE_2,   SMPU_READ_ACCESS);
    smpu_set_rights(sdp, SMPU_REGION_1, SMPU_MASTER_DMA1,     SMPU_READ_ACCESS);
    smpu_set_rights(sdp, SMPU_REGION_1, SMPU_MASTER_DMA2,     SMPU_READ_ACCESS);
    smpu_set_rights(sdp, SMPU_REGION_1, SMPU_MASTER_CORE_HSM, SMPU_READ_ACCESS);
    smpu_set_rights(sdp, SMPU_REGION_1, SMPU_MASTER_DEBUG,    SMPU_READ_ACCESS);
    smpu_set_cacheability(sdp, SMPU_REGION_1, true);
    smpu_set_read_only(sdp, SMPU_REGION_1, false);

    /* Define a region with read/write access.*/
    smpu_enable_region(sdp, SMPU_REGION_2, true);
    smpu_set_start_address(sdp, SMPU_REGION_2, 0x24038060);
    smpu_set_end_address(sdp, SMPU_REGION_2, 0xFFFFFFFF);
    smpu_set_rights(sdp, SMPU_REGION_2, SMPU_MASTER_CORE_1,   SMPU_READ_WRITE_ACCESS);
    smpu_set_rights(sdp, SMPU_REGION_2, SMPU_MASTER_CORE_2,   SMPU_READ_WRITE_ACCESS);
    smpu_set_rights(sdp, SMPU_REGION_2, SMPU_MASTER_DMA1,     SMPU_READ_WRITE_ACCESS);
    smpu_set_rights(sdp, SMPU_REGION_2, SMPU_MASTER_DMA2,     SMPU_READ_WRITE_ACCESS);
    smpu_set_rights(sdp, SMPU_REGION_2, SMPU_MASTER_CORE_HSM, SMPU_READ_WRITE_ACCESS);
    smpu_set_rights(sdp, SMPU_REGION_2, SMPU_MASTER_DEBUG,    SMPU_READ_WRITE_ACCESS);
    smpu_set_cacheability(sdp, SMPU_REGION_2, true);
    smpu_set_read_only(sdp, SMPU_REGION_2, false);
}

int main(void) {

    /* Address in region 1 has read only access.*/
    uint32_t *read_only_addr = (uint32_t *) 0x24038050;

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

    /* Initialize SMPU driver instance.*/
    smpu_init(&SMPUD);

    /* Configure SMPU driver instance.*/
    smpu_config(&SMPUD);

    /* Apply SMPU configuration.*/
    smpu_start(&SMPUD);

    /* Write access that violates the access rights.*/
    *read_only_addr = 0xFFFFFFFF;

    /* Write access doesn't violate the access rights anymore.*/
    *read_only_addr = 0xFFFFFFFF;

    /* Application main loop */
    for ( ; ; ) {

        /* Toggle USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(500U);
    }
}
