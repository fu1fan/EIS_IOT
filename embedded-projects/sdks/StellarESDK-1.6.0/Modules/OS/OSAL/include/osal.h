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
 * @file    osal.h
 * @brief   Operating System Abstraction Layer module header file.
 *
 * @addtogroup OS
 * @addtogroup OSAL
 * @ingroup OS
 * @{
 */

#ifndef _OSAL_H_
#define _OSAL_H_

#include <platform.h>
#include <typedefs.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of OSAL timeout.
 */
typedef struct {
    /**
     * @brief   Timeout start time in ticks.
     */
    uint64_t                 start;
    /**
     * @brief   Timeout value in ticks.
     * @note    Value set to '0' means no timeout.
     */
    uint64_t                 value;
} osal_to_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Debug macros
 * @{
 */

/**
 * @brief   Dummy condition assertion
 *
 * @param[in] c         condition to be verified
 * @param[in] r         remark string
 *
 * @api
 */
#define osal_dbg_assert(c, s) (void)(c)

/**
 * @brief   Condition assertion
 * @details If the condition check fails, the system halts. The remark string
 *          currently is not used.
 *
 * @param[in] c         condition to be verified
 * @param[in] r         remark string
 *
 * @api
 */
#if !defined(osal_dbg_assert)
#define osal_dbg_assert(c, r) do {                                          \
    if (!(c)) {                                                             \
        osal_sys_halt();                                                    \
    }                                                                       \
} while (0)
#endif /* !defined(osal_dbg_assert) */

/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Initializes OSAL layer
 *
 * @api
 */
void osal_init(void);

/**
 * @brief   Returns the current number of ticks
 *
 * @return              current number of ticks
 *
 * @api
 */
uint64_t osal_get_tick(void);

/**
 * @brief   Converts number of ticks into seconds
 *
 * @param[in] ticks     number of ticks
 * @return              number of seconds
 *
 * @api
 */
uint32_t osal_tk2s(uint64_t ticks);

/**
 * @brief   Converts number of ticks into milliseconds
 *
 * @param[in] ticks     number of ticks
 * @return              number of milliseconds
 *
 * @api
 */
uint32_t osal_tk2ms(uint64_t ticks);

/**
 * @brief   Converts number of ticks into microseconds
 *
 * @param[in] ticks     number of ticks
 * @return              number of microseconds
 *
 * @api
 */
uint32_t osal_tk2us(uint64_t ticks);

/**
 * @brief   Converts number of ticks into nanoseconds
 *
 * @param[in] ticks     number of ticks
 * @return              number of nanoseconds
 *
 * @api
 */
uint32_t osal_tk2ns(uint64_t ticks);

/**
 * @brief   Converts seconds in number of ticks
 *
 * @param[in] sec       number of seconds
 * @return              number of ticks
 *
 * @api
 */
uint64_t osal_s2tk(uint32_t sec);

/**
 * @brief   Converts milliseconds in number of ticks
 *
 * @param[in] msec      number of milliseconds
 * @return              number of ticks
 *
 * @api
 */
uint64_t osal_ms2tk(uint32_t msec);

/**
 * @brief   Converts microseconds in number of ticks
 *
 * @param[in] usec      number of microseconds
 * @return              number of ticks
 *
 * @api
 */
uint64_t osal_us2tk(uint32_t usec);

/**
 * @brief   Delays execution by specified amount of seconds
 * @details This function sets a delay (busy wait) for the specified amount of
 *          seconds.
 *
 * @param[in] sec       delay time (in seconds)
 *
 * @api
 */
void osal_delay_sec(uint32_t sec);

/**
 * @brief   Delays execution by specified amount of milliseconds
 * @details This function sets a delay (busy wait) for the specified amount of
 *          milliseconds.
 *
 * @param[in] msec      delay time (in milliseconds)
 *
 * @api
 */
void osal_delay_millisec(uint32_t msec);

/**
 * @brief   Delays execution by specified amount of microseconds
 * @details This function sets a delay (busy wait) for the specified amount of
 *          microseconds.
 *
 * @param[in] usec      delay time (in microseconds)
 *
 * @api
 */
void osal_delay_microsec(uint32_t usec);

/**
 * @brief   Starts the timeout countdown
 *
 * @param[in] timeout   timeout structure
 *
 * @api
 */
__STATIC_INLINE void osal_start_timeout(osal_to_t *timeout) {

    timeout->start = osal_get_tick();
}

/**
 * @brief   Checks if a timeout occurs
 * @details This function must be invoked only after an osal_start_timeout.
 *
 * @param[in] timeout   timeout structure
 * @return              timeout flag ('true' if a timeout occurs, 'false'
 *                      otherwise).
 *
 * @api
 */
__STATIC_INLINE bool osal_check_timeout(osal_to_t timeout) {

    if (timeout.value != 0UL) {
        return ((osal_get_tick() - timeout.start) > timeout.value);
    } else {
        return false;
    }
}

/**
 * @brief   Enters into a critical zone
 *
 * @api
 */
void osal_sys_lock(void);

/**
 * @brief   Leaves a critical zone
 *
 * @api
 */
void osal_sys_unlock(void);

/**
 * @brief   Enters into a critical zone from ISR
 *
 * @api
 */
void osal_sys_lock_from_isr(void);

/**
 * @brief   Leaves a critical zone from ISR
 *
 * @api
 */
void osal_sys_unlock_from_isr(void);

/**
 * @brief   System halt 
 *
 * @api
 */
void osal_sys_halt(void);

#ifdef __cplusplus
}
#endif

#endif /* _OSAL_H_ */
