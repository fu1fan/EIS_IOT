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
#include <flash.h>

/*===========================================================================*/
/* FLASH-related.                                                            */
/*===========================================================================*/

/* Size [1KB] of buffer used for program/verify test.*/
#define BUFFER_1K_BYTE_SIZE                 0x400U

/* Block D0_00 size [16KB].*/
#define D0_00_SIZE                          0x4000U

/* Block D0_00 start address.*/
#define D0_00_START_ADDR                    0x08F00000UL

/* Unlock block mask.*/
#define UNLOCK_ALL_BLOCKS                   0x00000000UL

/* This function notifies a flash error.*/
static void flash_error(void) {

    for ( ; ; ) {
        /* Blink USER_LED_B.*/
        gpio_toggle_pin(USER_LED_B);
        osal_delay_millisec(250U);
    }
}

int main(void) {

    uint32_t returnCode;                    /* return code from each SSD function    */

    uint32_t blkLockState = 0x00000000UL;   /* block lock status to be retrieved     */

    uint32_t buffer_0_1K[ BUFFER_1K_BYTE_SIZE / C55_WORD_SIZE ];
    uint32_t i;

    uint32_t lowBlockSelect;
    uint32_t midBlockSelect;
    uint32_t highBlockSelect;
    N256K_BLOCK_SEL n256KBlockSelect;

    uint32_t dest;                          /* destination address                   */
    uint32_t size;                          /* size applicable                       */
    uint32_t source_0;                      /* Source address for program and verify */

    uint32_t failAddress;                   /* save the failed address in flash      */
    uint32_t failData;                      /* save the failed data in flash         */
    uint32_t failSource;                    /* save the failed expected data         */

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

    /* Initialize FLASH driver instance.*/
    flash_init();

    flash_start(&FLASHD0);

    /* Initialize source buffer to 0.*/
    for (i = 0; i < (BUFFER_1K_BYTE_SIZE / C55_WORD_SIZE); i++) {
        buffer_0_1K[i] = 0x00000000UL;
    }

    /* High address space is locked by default.*/
    /* Get the lock status of high address Space.*/ 
    returnCode = flash_getLock(&FLASHD0, C55_BLOCK_HIGH, (unsigned long *)(&blkLockState));

    /* Check if an error occurred.*/
    if (C55_OK != returnCode) {
        flash_error();
    }

    /* Verify all blocks of high address space are locked.*/
    if (((blkLockState & 0x0000000FUL) != 0x0000000FUL)) {
        flash_error();
    }  

    /* Unlock all blocks of high address space.*/
    returnCode = flash_setLock(&FLASHD0, C55_BLOCK_HIGH, UNLOCK_ALL_BLOCKS);

    /* Check if an error occurred.*/
    if (C55_OK != returnCode) {
        flash_error();
    }

    /* Get the lock status of high address space.*/ 
    returnCode = flash_getLock(&FLASHD0, C55_BLOCK_HIGH, (unsigned long *)(&blkLockState));

    /* Check if an error occurred.*/
    if (C55_OK != returnCode) {
        flash_error();
    }

    /* Verify all blocks of high address space are unlocked.*/
    if (((blkLockState & 0x0000000FUL) != 0x00000000UL)) {
        flash_error();
    }

    /* Select block D0_00.*/
    lowBlockSelect = 0UL;
    midBlockSelect = 0UL;
    highBlockSelect = 0x00000001UL;
    n256KBlockSelect.first256KBlockSelect = 0UL;
    n256KBlockSelect.second256KBlockSelect = 0UL;

    /* Erase block D0_00.*/
    returnCode = flash_flashErase(&FLASHD0, C55_ERASE_MAIN, lowBlockSelect, midBlockSelect, highBlockSelect, n256KBlockSelect);

    /* Check if an error occurred.*/
    if (C55_OK != returnCode) {
        flash_error();
    }

    dest = D0_00_START_ADDR;
    size = D0_00_SIZE;

    /* Blank check block D0_00.*/
    returnCode = flash_blankCheck(&FLASHD0, dest, size, (unsigned long *)(&failAddress), (unsigned long *)(&failData));

    /* Check if an error occurred.*/
    if (C55_OK != returnCode) {
        flash_error();
    }

    source_0 = (uint32_t)buffer_0_1K;
    size = BUFFER_1K_BYTE_SIZE;
    dest = D0_00_START_ADDR;

    /* Program main array of block D0_00.*/
    returnCode = flash_flashProgram(&FLASHD0, FALSE, dest, size, source_0);    

    /* Check if an error occurred.*/
    if (C55_OK != returnCode) {
        flash_error();
    }

    /* Verify main array space that was written previously.*/
    returnCode = flash_programVerify(&FLASHD0, dest, size, source_0, (unsigned long *)(&failAddress), (unsigned long *)(&failData), (unsigned long *)(&failSource));

    /* Check if an error occurred.*/
    if (C55_OK != returnCode) {
        flash_error();
    }

    flash_stop(&FLASHD0);

    /* Application main loop */
    for ( ; ; ) {
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(250U);
    }
}
