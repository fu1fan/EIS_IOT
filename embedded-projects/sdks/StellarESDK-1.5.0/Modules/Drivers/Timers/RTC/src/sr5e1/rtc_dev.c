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
 * @file    rtc_private.c
 * @brief   RTC driver specific source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup TIMERS
 * @ingroup DRIVERS
 * @addtogroup RTC
 * @ingroup TIMERS
 * @{
 */

#include <rtc.h>
#include <rtc_private.h>
#include <clock.h>
#include <irq.h>
#include <platform.h>

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   Real time clock EXTI lines.
 */
#define RTC_EXTI_LINE_ALARM                 EXTI_LINE_18
#define RTC_EXTI_LINE_WKPU                  EXTI_LINE_20

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Returns pointer to the RTC register block.
 *
 * @param[in] rdp       pointer to a @p rtc_driver_t structure
 * @return              pointer to the RTC register block.
 *
 * @notapi
 */
RTC_TypeDef *rtc_dev_get_reg_ptr(rtc_driver_t *rdp) {

    (void)rdp;

    return RTC;
}

/**
 * @brief   Enables RTC clock.
 *
 * @param[in] rdp       pointer to a @p rtc_driver_t structure
 *
 * @notapi
 */
void rtc_dev_clock_enable(rtc_driver_t *rdp) {

    (void)rdp;

    clock_enable_RTC(true);
}

/**
 * @brief   Disables RTC clock.
 *
 * @param[in] rdp       pointer to a @p rtc_driver_t structure
 *
 * @notapi
 */
void rtc_dev_clock_disable(rtc_driver_t *rdp) {

    (void)rdp;

    clock_disable_RTC();
}

/**
 * @brief   Returns RTC alarm interrupt vector.
 *
 * @param[in] rdp       pointer to a @p rtc_driver_t structure
 * @return              RTC interrupt vector
 *
 * @notapi
 */
IRQn_Type rtc_dev_get_alarm_vector(rtc_driver_t *rdp) {

    IRQn_Type vector;

    (void)rdp;

    vector = IRQ_RTC_ALARM_VECTOR;

    return vector;
}

/**
 * @brief   Returns RTC wake-up timer interrupt vector.
 *
 * @param[in] rdp       pointer to a @p rtc_driver_t structure
 * @return              RTC interrupt vector
 *
 * @notapi
 */
IRQn_Type rtc_dev_get_wakeup_vector(rtc_driver_t *rdp) {

    IRQn_Type vector;

    (void)rdp;

    vector = IRQ_RTC_WKPU_VECTOR;

    return vector;
}

/**
 * @brief   Returns EXTI line associated to RTC alarm.
 *
 * @param[in] rdp       pointer to a @p rtc_driver_t structure
 * @return              EXTI line associated to real time clock alarm.
 *
 * @notapi
 */
exti_line_t rtc_dev_get_exti_alarm_line(rtc_driver_t *rdp) {

    (void)rdp;
    return RTC_EXTI_LINE_ALARM;
}

/**
 * @brief   Returns EXTI line associated to RTC wakeup timer.
 *
 * @param[in] rdp       pointer to a @p rtc_driver_t structure
 * @return              EXTI line associated to real time clock wakeup timer.
 *
 * @notapi
 */
exti_line_t rtc_dev_get_exti_wakeup_line(rtc_driver_t *rdp) {

    (void)rdp;
    return RTC_EXTI_LINE_WKPU;
}

/**
 * @brief   RTC alarm interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(IRQ_RTC_ALARM_HANDLER) {

    IRQ_PROLOGUE();

    __rtc_serve_alarm_interrupt(&DRV_RTC);
    exti_clear_pending(RTC_EXTI_LINE_ALARM);

    IRQ_EPILOGUE();
}

/**
 * @brief   RTC wakeup interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(IRQ_RTC_WKPU_HANDLER) {

    IRQ_PROLOGUE();

    __rtc_serve_wkpu_interrupt(&DRV_RTC);
    exti_clear_pending(RTC_EXTI_LINE_WKPU);

    IRQ_EPILOGUE();
}

/** @} */
