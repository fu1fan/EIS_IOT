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
/**
 * @file    FreeRTOS_osal.c
 * @brief   Operating System Abstraction Layer module source file.
 *
 * @addtogroup OS
 * @addtogroup OSAL
 * @ingroup OS
 * @{
 */

#include <osal.h>
#include <FreeRTOS.h>
#include <task.h>

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Converts seconds into number of ticks
 *
 * @param[in] sec       number of seconds
 * @return              number of ticks
 *
 * @notapi
 */
__STATIC_INLINE uint64_t osal_systick_s2tk(uint32_t sec) {

    return ((uint64_t)((sec) * ((uint64_t)configTICK_RATE_HZ)));
}

/**
 * @brief   Converts milliseconds into number of ticks
 *
 * @param[in] msec      number of milliseconds
 * @return              number of ticks
 *
 * @notapi
 */
__STATIC_INLINE uint64_t osal_systick_ms2tk(uint32_t msec) {

    return ((msec == 0U) ? ((uint64_t) 0) :
                           ((uint64_t)((((msec) * ((uint64_t)configTICK_RATE_HZ) - 1UL) / 1000UL) + 1UL)));
}

/**
 * @brief   Converts microseconds into number of ticks
 *
 * @param[in] usec      number of microseconds
 * @return              number of ticks
 *
 * @notapi
 */
__STATIC_INLINE uint64_t osal_systick_us2tk(uint32_t usec) {

    return ((usec == 0U) ? ((uint64_t) 0) :
                           ((uint64_t)((((usec) * ((uint64_t)configTICK_RATE_HZ) - 1UL) / 1000000UL) + 1UL)));
}

/*===========================================================================*/
/* Local function prototypes                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Hook function prototypes                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void osal_init(void) {

}

uint64_t osal_get_tick(void) {

    return (uint64_t)(xTaskGetTickCount());
}

uint32_t osal_tk2s(uint64_t ticks) {

    return (uint32_t)(ticks / ((uint64_t)configTICK_RATE_HZ));
}

uint32_t osal_tk2ms(uint64_t ticks) {

    return (uint32_t)((ticks * 1000ULL) / ((uint64_t)configTICK_RATE_HZ));
}

uint32_t osal_tk2us(uint64_t ticks) {

    return (uint32_t)((ticks * 1000000ULL) / ((uint64_t)configTICK_RATE_HZ));
}

uint32_t osal_tk2ns(uint64_t ticks) {

    return (uint32_t)((ticks * 1000000000ULL) / ((uint64_t)configTICK_RATE_HZ));
}

uint64_t osal_s2tk(uint32_t sec) {

    return osal_systick_s2tk(sec);
}

uint64_t osal_ms2tk(uint32_t msec) {

    return osal_systick_ms2tk(msec);
}

uint64_t osal_us2tk(uint32_t usec) {

    return osal_systick_us2tk(usec);
}

void osal_delay_sec(uint32_t sec) {

    vTaskDelay((uint32_t)(osal_systick_s2tk(sec)));
}

void osal_delay_millisec(uint32_t msec) {

    vTaskDelay((uint32_t)(osal_systick_ms2tk(msec)));
}

void osal_delay_microsec(uint32_t usec) {

    vTaskDelay((uint32_t)(osal_systick_us2tk(usec)));
}

void osal_sys_lock(void) {

    taskENTER_CRITICAL();
}

void osal_sys_unlock(void) {

    taskEXIT_CRITICAL();
}

void osal_sys_lock_from_isr(void) {

    taskENTER_CRITICAL();
}

void osal_sys_unlock_from_isr(void) {

    taskEXIT_CRITICAL();
}

void osal_sys_halt(void) {

    taskENTER_CRITICAL();

    /* Infinite loop.*/
    while (true) {
    }
}

/** @} */
