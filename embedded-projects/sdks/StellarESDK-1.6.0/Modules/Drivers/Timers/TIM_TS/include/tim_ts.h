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
 * @file    tim_ts.h
 * @brief   TIM_TS driver header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup TIMERS
 * @ingroup DRIVERS
 * @addtogroup TIM_TS
 * @ingroup TIMERS
 * @{
 */

#ifndef _TIM_TS_H_
#define _TIM_TS_H_

#include <dma.h>
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
 * @brief   Type of a TIM_TS driver.
 */
typedef struct tim_ts_driver tim_ts_driver_t;

/**
 * @brief   Type of a TIM_TS callback.
 *
 * @param[in] tdp      pointer to the @p tim_ts_driver_t object
 */
typedef void (*tim_ts_cb_t)(tim_ts_driver_t *tdp);

/**
 * @enum tim_ts_mms_t
 * Supported trigger outputs (tim_trgo).
 */
typedef enum {
    TIM_TS_MMS_RESET  = 0U,  /**< UG bit used as trigger output             */
    TIM_TS_MMS_ENABLE = 1U,  /**< Counter enable bit used as trigger output */
    TIM_TS_MMS_UPDATE = 2U   /**< Update event used as trigger output       */
} tim_ts_mms_t;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Export driver modules.*/
#include <tim_ts_instances.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Initializes an TIM_TS instance.
 *
 * @param[out] tdp      pointer to a @p tim_ts_driver_t structure to be
 *                      initialized
 *
 * @api
 */
void tim_ts_init(tim_ts_driver_t *tdp);

/**
 * @brief   Enables the dithering feature.
 * @note    This configuration becomes effective after tim_ts_start.
 *
 * @param[out] tdp      pointer to a @p tim_ts_driver_t structure
 * @param[in] flag      configuration flag. 'true' means the dithering is
 *                      enabled, 'false' means it is disbled
 * @return              previous dithering configuration.
 *
 * @api
 */
bool tim_ts_enable_dithering(tim_ts_driver_t *tdp, bool flag);

/**
 * @brief   Enables the UIF remapping feature.
 * @note    This configuration becomes effective after tim_ts_start.
 *
 * @param[out] tdp      pointer to a @p tim_ts_driver_t structure
 * @param[in] flag      configuration flag. 'true' means the update interrupt
 *                      flag is copied in the counter, 'false' means it is not
 * @return              previous UIF remapping configuration.
 *
 * @api
 */
bool tim_ts_enable_uif_remapping(tim_ts_driver_t *tdp, bool flag);

/**
 * @brief   Enables the auto-reload preload feature.
 * @note    This configuration becomes effective after tim_ts_start.
 *
 * @param[out] tdp      pointer to a @p tim_ts_driver_t structure
 * @param[in] flag      configuration flag. 'true' enables the auto-reload
 *                      register buffering, 'false' disables it
 * @return              previous auto-reload preload configuration.
 *
 * @api
 */
bool tim_ts_enable_autoreload_preload(tim_ts_driver_t *tdp, bool flag);

/**
 * @brief   Enables the one pulse mode feature.
 * @note    This configuration becomes effective after tim_ts_start.
 *
 * @param[out] tdp      pointer to a @p tim_ts_driver_t structure
 * @param[in] flag      configuration flag. 'true' means the counter stops at
 *                      each update event, 'false' means it continues to count
 * @return              previous one pulse mode configuration.
 *
 * @api
 */
bool tim_ts_enable_one_pulse_mode(tim_ts_driver_t *tdp, bool flag);

/**
 * @brief   Sets the update request source.
 * @note    This configuration becomes effective after tim_ts_start.
 *
 * @param[out] tdp      pointer to a @p tim_ts_driver_t structure
 * @param[in] flag      configuration flag. 'true' means that only the counter
 *                      overflow/underflow event generates an interrupt/DMA
 *                      request, 'false' means that the interrupt/DMA event is
 *                      generated for a counter overflow/underflow, setting the
 *                      update generation bit or an update generation through
 *                      the slave mode controller
 * @return              previous update request source configuration.
 *
 * @api
 */
bool tim_ts_set_source_ofuf_only(tim_ts_driver_t *tdp, bool flag);

/**
 * @brief   Enables the update event generation feature.
 * @note    This configuration becomes effective after tim_ts_start.
 *
 * @param[out] tdp      pointer to a @p tim_ts_driver_t structure
 * @param[in] flag      configuration flag. 'false' means the update event
 *                      generation is disabled, 'true' means an update event is
 *                      generated for a counter overflow/underflow, setting the
 *                      update generation bit or an update generation through
 *                      the slave mode controller.
 *                      @note the counter and the prescaler are reinitialized if
 *                      the UG bit is set or if a hardware reset is received
 *                      from the slave mode controller.
 * @return              previous update event generation configuration.
 *
 * @api
 */
bool tim_ts_enable_update_event_generation(tim_ts_driver_t *tdp, bool flag);

/**
 * @brief   Sets the master mode selection.
 * @note    This configuration becomes effective after tim_ts_start.
 *
 * @param[out] tdp      pointer to a @p tim_ts_driver_t structure
 * @param[in] mmode     master mode to be configured. It can be one of the
 *                      following values:
 *                 @ref TIM_TS_MMS_RESET
 *                 @ref TIM_TS_MMS_ENABLE
 *                 @ref TIM_TS_MMS_UPDATE
 * @return              previous master mode configurataion. It can be one of
 *                      the following values:
 *                 @ref TIM_TS_MMS_RESET
 *                 @ref TIM_TS_MMS_ENABLE
 *                 @ref TIM_TS_MMS_UPDATE
 *
 * @api
 */
tim_ts_mms_t tim_ts_set_master_mode_selection(tim_ts_driver_t *tdp,
                                              tim_ts_mms_t mmode);

/**
 * @brief   Configures TIM_TS autoreload value.
 * @note    This configuration becomes effective after tim_ts_start.
 *
 * @param[out] tdp      pointer to a @p tim_ts_driver_t structure
 * @param[in] ar        autoreload value to be configured
 * @return              previous autoreload value.
 *
 * @api
 */
uint32_t tim_ts_set_autoreload(tim_ts_driver_t *tdp, uint32_t ar);

/**
 * @brief   Configures TIM_TS prescaler.
 * @note    Can be set also on-fly. the value is loaded on each update event.
 *
 * @param[out] tdp      pointer to a @p tim_ts_driver_t structure
 * @param[in] presc     prescaler to be configured
 * @return              previous prescaler.
 *
 * @api
 */
uint16_t tim_ts_set_prescaler(tim_ts_driver_t *tdp, uint16_t presc);

/**
 * @brief   Configures TIM_TS interrupt priority.
 * @note    This configuration becomes effective after tim_ts_start.
 *
 * @param[out] tdp      pointer to a @p tim_ts_driver_t structure
 * @param[in] prio      interrupt priority to be configured
 * @return              previous interrupt priority.
 *
 * @api
 */
uint32_t tim_ts_set_prio(tim_ts_driver_t *tdp, uint32_t prio);

/**
 * @brief   Configures pointers to TIM_TS callback functions.
 *
 * @param[out] tdp      pointer to a @p tim_ts_driver_t structure
 * @param[in] cb        pointer to TIM_tS callback function
 * @return              pointer to previuos TIM_TS callback function.
 *
 * @api
 */
tim_ts_cb_t tim_ts_set_cb(tim_ts_driver_t *tdp, tim_ts_cb_t cb);

/**
 * @brief   Starts a TIM_TS instance.
 *
 * @param[in] tdp       pointer to a @p tim_ts_driver_t structure
 *
 * @api
 */
void tim_ts_start(tim_ts_driver_t *tdp);

/**
 * @brief   Gets the TIM_TS counter.
 *
 * @param[in] tdp       pointer to a @p tim_ts_driver_t structure
 * @return              counter.
 *
 * @api
 */
uint32_t tim_ts_get_counter(tim_ts_driver_t *tdp);

/**
 * @brief   Stops a TIM_TS instance.
 *
 * @param[in] tdp       pointer to a @p tim_ts_driver_t structure
 *
 * @api
 */
void tim_ts_stop(tim_ts_driver_t *tdp);

/**
 * @brief   De-initializes a TIM_TS instance.
 *
 * @param[in] tdp       pointer to a @p tim_ts_driver_t structure to
 *                      be de-initialized
 *
 * @api
 */
void tim_ts_deinit(tim_ts_driver_t *tdp);

/**
 * @brief   Serves a TIM_TS interrupt.
 * @note    Not an API, used internally.
 *
 * @param[in] tdp       pointer to a @p tim_ts_driver_t structure
 *
 * @isr
 */
void __tim_ts_serve_interrupt(tim_ts_driver_t *tdp);

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _TIM_TS_H_ */

/** @} */
