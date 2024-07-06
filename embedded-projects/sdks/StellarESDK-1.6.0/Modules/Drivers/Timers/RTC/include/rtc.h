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
 * @file    rtc.h
 * @brief   RTC driver header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup TIMERS
 * @ingroup DRIVERS
 * @addtogroup RTC
 * @ingroup TIMERS
 * @{
 */

#ifndef _RTC_H_
#define _RTC_H_

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
 * @brief   Type of an RTC driver.
 */
typedef struct rtc_driver rtc_driver_t;

/**
 * @enum rtc_clk_src_t.
 * RTC supported peripheral clocks.
 */
typedef enum {
    RTC_CLOCK_SOURCE_LSI  = 0U,        /**< Low speed internal clock  */
    RTC_CLOCK_SOURCE_XOSC = 1U         /**< High speed external clock */
} rtc_clk_src_t;

/**
 * @enum rtc_hour_format.
 * RTC supported hour formats.
 */
typedef enum {
    RTC_24H_FORMAT  = 0U,              /**< 24 Hours format            */
    RTC_AMPM_FORMAT = 1U               /**< 12 Hours format with AM/PM */
} rtc_hour_format_t;

/**
 * @enum rtc_am_pm_t.
 * RTC supported AM/PM hour formats.
 */
typedef enum {
    RTC_AM  = 0U,                      /**< AM flag */
    RTC_PM = 1U                        /**< PM flag */
} rtc_am_pm_t;

/**
 * @enum rtc_day_format.
 * RTC supported day formats.
 */
typedef enum {
    RTC_MONDAY    = 1U,                /**< Monday value    */
    RTC_TUESDAY   = 2U,                /**< Tuesday value   */
    RTC_WEDNESDAY = 3U,                /**< Wednesday value */
    RTC_THURSDAY  = 4U,                /**< Thursday value  */
    RTC_FRIDAY    = 5U,                /**< Friday value    */
    RTC_SATURDAY  = 6U,                /**< Saturday value  */
    RTC_SUNDAY    = 7U                 /**< Sunday value    */
} rtc_day_t;

/**
 * @enum rtc_output_format.
 * RTC supported output formats.
 */
typedef enum {
    RTC_BIN_FORMAT = 0U,               /**< Binary format            */
    RTC_BCD_FORMAT = 1U                /**< Binary coded data format */
} rtc_output_t;

/**
 * @enum rtc_dst_op_t.
 * RTC supported daylight saving time formats.
 */
typedef enum {
    RTC_DST_NONE  = 0U,                 /**< No bias           */
    RTC_DST_ADD1H = 1U,                 /**< Add one hour      */
    RTC_DST_SUB1H = 2U                  /**< Subtract one hour */
} rtc_dst_op_t;

/**
 * @enum rtc_alarm_date_sel_t.
 * RTC supported alarm configurations.
 */
typedef enum {
    RTC_ALRM_SEL_DATE    = 0U,         /**< Value is an exact date     */
    RTC_ALRM_SEL_WEEKDAY = 1U          /**< Value is a day of the week */
} rtc_alarm_sel_t;

/**
 * @enum rtc_match_t.
 * RTC alarm field match configuration.
 */
typedef enum {
    RTC_MATCH      = 0U,               /**< Match parameter       */
    RTC_DONT_MATCH = 1U                /**< Don't match parameter */
} rtc_match_t;

/**
 * @enum rtc_ss_match_t.
 * RTC alarm field match configurations.
 */
typedef enum {
    RTC_SS_MATCH_NONE  = 0U,           /**< No match needed on subseconds */
    RTC_SS_MATCH_SS_0  = 1U,           /**< Only SS[0]    is compared     */
    RTC_SS_MATCH_1to0  = 2U,           /**< Only SS[1:0]  is compared     */
    RTC_SS_MATCH_2to0  = 3U,           /**< Only SS[2:0]  is compared     */
    RTC_SS_MATCH_3to0  = 4U,           /**< Only SS[3:0]  is compared     */
    RTC_SS_MATCH_4to0  = 5U,           /**< Only SS[4:0]  is compared     */
    RTC_SS_MATCH_5to0  = 6U,           /**< Only SS[5:0]  is compared     */
    RTC_SS_MATCH_6to0  = 7U,           /**< Only SS[6:0]  is compared     */
    RTC_SS_MATCH_7to0  = 8U,           /**< Only SS[7:0]  is compared     */
    RTC_SS_MATCH_8to0  = 9U,           /**< Only SS[8:0]  is compared     */
    RTC_SS_MATCH_9to0  = 10U,          /**< Only SS[9:0]  is compared     */
    RTC_SS_MATCH_10to0 = 11U,          /**< Only SS[10:0] is compared     */
    RTC_SS_MATCH_11to0 = 12U,          /**< Only SS[11:0] is compared     */
    RTC_SS_MATCH_12to0 = 13U,          /**< Only SS[12:0] is compared     */
    RTC_SS_MATCH_13to0 = 14U,          /**< Only SS[13:0] is compared     */
    RTC_SS_MATCH_ALL   = 15U           /**< Only SS[14:0] is compared     */
} rtc_ss_match_t;

/**
 * @enum rtc_ss_match_t.
 * RTC supported wake-up clock selections.
 */
typedef enum {
    RTC_WUCKSEL_RTC16         = 0U,    /**< Clk = RTC/16         */
    RTC_WUCKSEL_RTC8          = 1U,    /**< Clk = RTC/8          */
    RTC_WUCKSEL_RTC4          = 2U,    /**< Clk = RTC/4          */
    RTC_WUCKSEL_RTC2          = 3U,    /**< Clk = RTC/2          */
    RTC_WUCKSEL_CK_SPRE       = 4U,    /**< Clk = ck_spre        */
    RTC_WUCKSEL_CK_SPRE_17BIT = 6U     /**< Clk = ck_spre + 2^16 */
} rtc_wucksel_t;

/**
 * @enum rtc_cb_ops_t
 * RTC callback operation type.
 */
typedef enum {
    RTC_CB_OPS_TSMP,                   /**< Timestamp interruption      */
    RTC_CB_OPS_ALARM,                  /**< Alarm interruption          */
    RTC_CB_OPS_WKUP,                   /**< Wakeup interruption         */
    RTC_CB_OPS_MAX                     /**< Number of driver operations */
} rtc_cb_ops_t;

/**
 * @brief   Type of RTC time structure.
 */
typedef struct {
    /**
     * @brief   RTC hours.
     */
    uint8_t                  hours;
    /**
     * @brief   RTC minutes.
     */
    uint8_t                  minutes;
    /**
     * @brief   RTC seconds.
     */
    uint8_t                  seconds;
    /**
     * @brief   RTC_SSR (sub seconds register) content.
     */
    uint32_t                 sub_seconds;
    /**
     * @brief   Sub seconds fraction (PREDIV_S).
     */
    uint32_t                 ss_fraction;
    /**
     * @brief   Hour format (AM/PM or 24H).
     */
    rtc_hour_format_t        hour_format;
    /**
     * @brief   Hour format (AM or PM). Used only in AM/PM mode.
     */
    rtc_am_pm_t              am_pm;
} rtc_time_t;

/**
  * @brief   Type of RTC date structure
  */
typedef struct {
    /**
     * @brief   Day of the week
     */
    rtc_day_t                day;
    /**
     * @brief   Day of the month [1:31]
     */
    uint8_t                  date;
    /**
     * @brief   Month [1:12]
     */
    uint8_t                  month;
    /**
     * @brief   Year [0:99]
     */
    uint8_t                  year;
} rtc_date_t;

/**
 * @brief   Type of RTC alarm structure.
 */
typedef struct {
    /**
     * @brief   Alarm RTC hours.
     */
    uint8_t                  hours;
    /**
     * @brief   Alarm RTC minutes.
     */
    uint8_t                  minutes;
    /**
     * @brief   Alarm RTC seconds.
     */
    uint8_t                  seconds;
    /**
     * @brief   Alarm RTC_SSR (sub seconds register) content.
     */
    uint32_t                 sub_sec;
    /**
     * @brief   Alarm hour format (AM/PM or 24H).
     */
    rtc_hour_format_t        hour_format;
    /**
     * @brief   Alarm hour format (AM or PM). Used only in AM/PM mode.
     */
    rtc_am_pm_t              am_pm;
    /**
     * @brief   Alarm date selector (as date unit or day of the week).
     */
    rtc_alarm_sel_t          date_sel;
    /**
     * @brief   Alarm selected date.
     * @note    It can be a @p rtc_day_t type or a number corresponding to the
     *          date unit selected.
     */
    uint32_t                 date;
    /**
     * @brief   Day match enable flag.
     */
    rtc_match_t              match_day;
    /**
     * @brief   Hour match enable flag.
     */
    rtc_match_t              match_hour;
    /**
     * @brief   Minute match enable flag.
     */
    rtc_match_t              match_min;
    /**
     * @brief   Second match enable flag.
     */
    rtc_match_t              match_sec;
    /**
     * @brief   Sub-second digits enable flag.
     */
    rtc_ss_match_t           match_ss;
    /**
     * @brief   Interrupt enabled flag.
     */
    bool                     is_it_en;
    /**
     * @brief   Interrupt priority.
     */
    uint32_t                 prio;
} rtc_alarm_t;

/**
 * @brief   Type of RTC wake-up timer structure.
 */
typedef struct {
    /**
     * @brief   WUT auto-reload value.
     */
    uint16_t                 autoreload;
    /**
     * @brief   WUT clock selection.
     */
    rtc_wucksel_t            clk_source;
    /**
     * @brief   Interrupt priority.
     */
    uint32_t                 prio;
} rtc_wakeup_t;

/**
 * @brief   Type of RTC callbacks.
 *
 * @param[in] rdp       pointer to the @p rtc_driver_t object
 */
typedef void (*rtc_cb_t)(rtc_driver_t *rdp);

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Export driver modules.*/
#include <rtc_instances.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Initializes RTC instance.
 *
 * @param[out] rdp      pointer to a @p rtc_driver_t structure
 *
 * @api
 */
void rtc_init(rtc_driver_t *rdp);

/**
 * @brief   Sets the RTC clock source.
 * 
 * @note    The resulting frequency has to be 1Hz.
 *
 * @param[out] rdp        pointer to a @p rtc_driver_t structure
 * @param[in]  source     clock source to be configured. It can be one of
 *                        the following values:
 *             @arg @ref  RTC_CLOCK_SOURCE_LSI
 *             @arg @ref  RTC_CLOCK_SOURCE_XOSC
 * @param[in]  prediv_s   synchronous predivision factor, Max = 32,767 + 1
 * @param[in]  prediv_a   asynchronous predivision factor, Max = 127 + 1
 *
 * @api
 */
void rtc_set_clock(rtc_driver_t *rdp, rtc_clk_src_t source,
                   uint32_t prediv_s, uint32_t prediv_a);

/**
 * @brief   Sets the RTC output data format.
 * @note    This configuration is instantly effective and can be changed at 
 *          runtime.
 *
 * @param[out] rdp        pointer to a @p rtc_driver_t structure
 * @param[in]  out_format output data format to be configured. It can be one of
 *                        the following values:
 *             @arg @ref  RTC_BIN_FORMAT
 *             @arg @ref  RTC_BDC_FORMAT
 * @return                previous output data format. It can be one of the
 *                        following values:
 *             @arg @ref  RTC_BIN_FORMAT
 *             @arg @ref  RTC_BDC_FORMAT
 *
 * @api
 */
rtc_output_t rtc_set_output_format(rtc_driver_t *rdp, rtc_output_t out_format);

/**
 * @brief   Sets the initial time of the RTC peripheral.
 *
 * @param[out] rdp      pointer to a @p rtc_driver_t structure
 * @param[in] new_time  pointer to a @p rtc_time_t structure.
 * @return              Previous time.
 * 
 * @api
 */
rtc_time_t *rtc_set_time(rtc_driver_t *rdp, rtc_time_t *new_time);

/**
 * @brief   Sets the initial date of the RTC peripheral.
 * @note    The max year is 99, offset has to be managed in the application.
 *
 * @param[out] rdp      pointer to a @p rtc_driver_t structure
 * @param[in] new_date  pointer to a @p rtc_date_t structure.
 * @return              Previous date.
 * 
 * @api
 */
rtc_date_t *rtc_set_date(rtc_driver_t *rdp, rtc_date_t *new_date);

/**
 * @brief   Subtract or add one hour to the calendar in one single operation 
 *          without going through the initialization procedure.
 * @note    The configuration becomes effective after 1 second.
 *
 * @param[out] rdp      pointer to an @p rtc_driver_t structure
 * @param[in] dst_op    operation to make
 * @param[in] backup    backup operation flag.
 *
 * @api
 */
rtc_dst_op_t rtc_set_dst(rtc_driver_t *rdp, rtc_dst_op_t dst_op, bool backup);

/**
 * @brief   Configures pointers to RTC callback functions.
 *
 * @param[out] rdp      pointer to a @p rtc_driver_t structure
 * @param[in] cb_op     callback operation type. It can be one of the following
 *                      values:
 *            @arg @ref rtc_CB_OPS_TSMP
 *            @arg @ref rtc_CB_OPS_ALARM
 *            @arg @ref rtc_CB_OPS_WKUP
 *            @arg @ref rtc_CB_OPS_MAX
 * @param[in] cb        pointer to RTC callback function
 * @return              pointer to previuos RTC callback function.
 *
 * @api
 */
rtc_cb_t rtc_set_cb(rtc_driver_t *rdp, rtc_cb_ops_t cb_op, rtc_cb_t cb);

/**
 * @brief   Sets the alarm time of the RTC peripheral.
 * @note    This configuration becomes effective after rtc_start.
 *
 * @param[out] rdp      pointer to a @p rtc_driver_t structure
 * @param[in] new_alarm pointer to a @p rtc_alarm_t structure.
 * @return              Previous alarm.
 * 
 * @api
 */
rtc_alarm_t *rtc_set_alarm(rtc_driver_t *rdp, rtc_alarm_t *new_alarm);

/**
 * @brief   Sets the wake-up timer of the RTC peripheral.
 * @note    This configuration becomes effective after rtc_start.
 *
 * @param[out] rdp      pointer to a @p rtc_driver_t structure
 * @param[in] wut       pointer to a @p rtc_wakeup_t structure.
 * @return              Previous wake-up timer.
 * 
 * @api
 */
rtc_wakeup_t *rtc_set_wkpu(rtc_driver_t *rdp, rtc_wakeup_t *wut);

/**
 * @brief   Starts RTC instance.
 *
 * @param[in] rdp       pointer to a @p rtc_driver_t structure
 *
 * @api
 */
void rtc_start(rtc_driver_t *rdp);

/**
 * @brief   Returns an RTC time instance of the current time.
 *
 * @param[in] rdp       pointer to a @p rtc_driver_t structure
 * @return              RTC time instance.
 *
 * @api
 */
rtc_time_t rtc_get_time(rtc_driver_t *rdp);

/**
 * @brief   Returns an RTC date instance of the current date.
 *
 * @param[in] rdp       pointer to a @p rtc_driver_t structure
 * @return              RTC date instance.
 *
 * @api
 */
rtc_date_t rtc_get_date(rtc_driver_t *rdp);

/**
 * @brief   Returns the value of the alarm event interrupt flag.
 * @note    Used to poll for the interrupt event.
 *
 * @param[in] rdp       pointer to a @p rtc_driver_t structure
 * @return              interrupt flag.
 *
 * @api
 */
bool rtc_get_alarm_event(rtc_driver_t *rdp);

/**
 * @brief   Returns the value of the daylight saving time backup value
 * @note    Used to know if a dst operation has been done.
 *
 * @param[in] rdp       pointer to a @p rtc_driver_t structure
 * @return              backup flag.
 *
 * @api
 */
bool rtc_get_backup_flag(rtc_driver_t *rdp);

/**
 * @brief   Stops RTC instance.
 *
 * @param[in] rdp       pointer to a @p rtc_driver_t structure
 *
 * @api
 */
void rtc_stop(rtc_driver_t *rdp);

/**
 * @brief   Deinitializes RTC instance.
 *
 * @param[out] rdp      pointer to a @p rtc_driver_t structure
 *
 * @api
 */
void rtc_deinit(rtc_driver_t *rdp);

/**
 * @brief   Shared interrupt handler.
 * @note    Not an API, used internally.
 *
 * @param[in] rdp       pointer to a @p rtc_driver_t structure
 *
 * @isr
 */
void __rtc_serve_alarm_interrupt(rtc_driver_t *rdp);

/**
 * @brief   Shared interrupt handler.
 * @note    Not an API, used internally.
 *
 * @param[in] rdp       pointer to a @p rtc_driver_t structure
 *
 * @isr
 */
void __rtc_serve_wkpu_interrupt(rtc_driver_t *rdp);

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _RTC_H_ */

/** @} */
