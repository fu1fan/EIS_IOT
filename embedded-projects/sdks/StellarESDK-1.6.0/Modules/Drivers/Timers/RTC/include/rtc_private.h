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
 * @file    rtc_private.h
 * @brief   RTC driver private header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup TIMERS
 * @ingroup DRIVERS
 * @addtogroup RTC
 * @ingroup TIMERS
 * @{
 */

#ifndef _RTC_PRIVATE_H_
#define _RTC_PRIVATE_H_

#include <irq.h>
#include <exti.h>
#include <platform.h>

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
 * @brief   RTC driver structure.
 * @note    This structure is internal to the RTC driver.
 */
struct rtc_driver {
    /**
     * @brief   Pointer to RTC registers block.
     */
    RTC_TypeDef              *rtc;
    /**
     * @brief   Clock source.
     */
    rtc_clk_src_t            clk_src;
    /**
     * @brief   Synchronous predivider.
     */
    uint32_t                 prediv_s;
    /**
     * @brief   Asynchronous predivider.
     */
    uint32_t                 prediv_a;
    /**
     * @brief   Pointer to date data structure.
     */
    rtc_date_t               *date;
    /**
     * @brief   Pointer to time data structure.
     */
    rtc_time_t               *time;
    /**
     * @brief   Specifies the daylight saving operation type.
     */
    rtc_dst_op_t             dst_op;
    /**
     * @brief   Specifies if the daylight saving operation has been performed.
     */
    bool                     dst_bkp;
    /**
     * @brief   Specifies the RTC output format (BCD or BIN).
     */
    rtc_output_t             out_format;
    /**
     * @brief   Alarm enabled flag.
     */
    bool                     is_alarm_en;
    /**
     * @brief   Pointer to alarm data structure.
     */
    rtc_alarm_t              *alarm;
    /**
     * @brief   wake-up timer enabled flag.
     */
    bool                     is_wakeup_en;
    /**
     * @brief   Pointer to wake up timer data structure.
     */
    rtc_wakeup_t             *wakeup;
    /**
     * @brief   Pointers to RTC callback functions.
     * @note    Set to @p NULL if not required.
     */
    rtc_cb_t                 cb[RTC_CB_OPS_MAX];
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

IRQ_HANDLER(IRQ_RTC_ALARM_HANDLER);
IRQ_HANDLER(IRQ_RTC_WKPU_HANDLER);

RTC_TypeDef *rtc_dev_get_reg_ptr(rtc_driver_t *rdp);
void rtc_dev_clock_enable(rtc_driver_t *rdp);
void rtc_dev_clock_disable(rtc_driver_t *rdp);
IRQn_Type rtc_dev_get_alarm_vector(rtc_driver_t *rdp);
IRQn_Type rtc_dev_get_wakeup_vector(rtc_driver_t *rdp);
exti_line_t rtc_dev_get_exti_alarm_line(rtc_driver_t *rdp);
exti_line_t rtc_dev_get_exti_wakeup_line(rtc_driver_t *rdp);

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _RTC_PRIVATE_H_ */

/** @} */
