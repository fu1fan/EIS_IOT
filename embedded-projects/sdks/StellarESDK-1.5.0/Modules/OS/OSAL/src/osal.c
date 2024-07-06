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
/**
 * @file    osal.c
 * @brief   Operating System Abstraction Layer module source file.
 *
 * @addtogroup OS
 * @addtogroup OSAL
 * @ingroup OS
 * @{
 */

#include <osal_cfg.h>
#include <osal.h>
#include <systick.h>
#include <clock.h>
#include <irq.h>

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/** 
 * @name OSAL Interrupts Frequency macros
 * @{
 */
#define OSAL_IRQ_PER_SEC_1000     1000U /* 1000 interrupts per second -> 1 interrupt per ms    */
#define OSAL_IRQ_PER_SEC_100      100U  /* 100  interrupts per second -> 1 interrupt per 10ms  */
#define OSAL_IRQ_PER_SEC_10       10U   /* 10   interrupts per second -> 1 interrupt per 100ms */
#define OSAL_IRQ_PER_SEC_1        1U    /* 1    interrupt  per second -> 1 interrupt per sec   */
/** @} */

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Local function prototypes                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Hook function prototypes                                                  */
/*===========================================================================*/

/**
 * @brief   OSAL Tick hook function prototype
 * @details When OSAL_CFG_USE_IRQ_HOOK is set to TRUE, the application must
 *          provide a hook function with the following prototype that will be
 *          invoked each time an interrupt is generated.
 */
#if (OSAL_CFG_USE_IRQ_HOOK == TRUE)
extern void osal_tick_hook(void);

static void systick_hook(systick_driver_t *sdp) {

    (void)(sdp);
    osal_tick_hook();
}
#endif

/*===========================================================================*/
/* Module interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void osal_init(void) {

    systick_init(&DRV_SYSTICK);
    (void)systick_set_prio(&DRV_SYSTICK, OSAL_CFG_IRQ_PRIO);
    (void)systick_set_relval(&DRV_SYSTICK, (SystemCoreClock / OSAL_CFG_IRQ_PER_SEC));
#if (OSAL_CFG_USE_IRQ_HOOK == TRUE)
    (void)systick_set_cb(&DRV_SYSTICK, systick_hook);
#endif
    systick_start(&DRV_SYSTICK);
}

uint64_t osal_get_tick(void) {

    return systick_get_ticks(&DRV_SYSTICK);
}

uint32_t osal_tk2s(uint64_t ticks) {

    return systick_tk2s(&DRV_SYSTICK, ticks);
}

uint32_t osal_tk2ms(uint64_t ticks) {

    return systick_tk2ms(&DRV_SYSTICK, ticks);
}

uint32_t osal_tk2us(uint64_t ticks) {

    return systick_tk2us(&DRV_SYSTICK, ticks);
}

uint32_t osal_tk2ns(uint64_t ticks) {

    return systick_tk2ns(&DRV_SYSTICK, ticks);
}

uint64_t osal_s2tk(uint32_t sec) {

    return systick_s2tk(&DRV_SYSTICK, sec);
}

uint64_t osal_ms2tk(uint32_t msec) {

    return systick_ms2tk(&DRV_SYSTICK, msec);
}

uint64_t osal_us2tk(uint32_t usec) {

    return systick_us2tk(&DRV_SYSTICK, usec);
}

void osal_delay_sec(uint32_t sec) {

    systick_delay_sec(&DRV_SYSTICK, sec);
}

void osal_delay_millisec(uint32_t msec) {

    systick_delay_millisec(&DRV_SYSTICK, msec);
}

void osal_delay_microsec(uint32_t usec) {

    systick_delay_microsec(&DRV_SYSTICK, usec);
}

void osal_sys_lock(void) {

    irq_disable_all();
}

void osal_sys_unlock(void) {

    irq_enable_all();
}

void osal_sys_lock_from_isr(void) {

    irq_disable_all();
}

void osal_sys_unlock_from_isr(void) {

    irq_enable_all();
}

void osal_sys_halt(void) {

    irq_disable_all();

    /* Infinite loop.*/
    while (true) {
    }
}

/** @} */
