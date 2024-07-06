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
 * @file    cmu.h
 * @brief   CMU driver header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SAFETY
 * @ingroup DRIVERS
 * @addtogroup CMU
 * @ingroup SAFETY
 * @{
 */

#ifndef _CMU_H_
#define _CMU_H_

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
 * @brief   Type of an CMU driver.
 */
typedef struct cmu_driver cmu_driver_t;

/**
 * @enum cmu_clk_sel_t
 * CMU clock to be measured. Only effects CMU instances that utilizes clock
 * metering.
 */
typedef enum {
    CMU_CLKMT0_RMN = 0U,          /**< Reference clock IRCOSC */
    CMU_CLKMT1     = 1U,          /**< Reference clock LSI    */
} cmu_clk_sel_t;

/**
 * @enum cmu_rcdiv_t
 * CMU CLKMT0_RMN division factor.
 */
typedef enum {
    CMU_RCDIV_1 = 0U,             /**< CLKMT0_RMN divided by 1 */
    CMU_RCDIV_2 = 1U,             /**< CLKMT0_RMN divided by 2 */
    CMU_RCDIV_4 = 2U,             /**< CLKMT0_RMN divided by 4 */
    CMU_RCDIV_8 = 3U,             /**< CLKMT0_RMN divided by 8 */
} cmu_rcdiv_t;

/**
 * @enum cmu_int_t
 * CMU interrupt type.
 */
typedef enum {
    CMU_INT_OLRI,                 /**< Xosc less than reference interruption  */
    CMU_INT_FLLI,                 /**< Freq less than low ref interruption    */
    CMU_INT_FHHI,                 /**< Freq higher than high ref interruption */
    CMU_INT_FLCI,                 /**< Freq less than ref clock interruption  */
    CMU_INT_NONE                  /**< No interruption occurred               */
} cmu_int_t;

/**
 * @brief   Type of CMU callbacks.
 *
 * @param[in] cdp       pointer to the @p cmu_driver_t object
 */
typedef void (*cmu_cb_t)(cmu_driver_t *cdp);

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Export driver modules.*/
#include <cmu_instances.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Initializes CMU instance.
 *
 * @param[out] cdp      pointer to a @p cmu_driver_t structure
 *
 * @api
 */
void cmu_init(cmu_driver_t *cdp);

/**
 * @brief   Sets the CMU clock source.
 * @note    This only effects CMU instances that utilizes clock metering.
 * @note    This configuration becomes effective after cmu_start.
 *
 * @param[out] cdp      pointer to a @p cmu_driver_t structure
 * @param[in] clk_src   clock source
 *
 * @api
 */
cmu_clk_sel_t cmu_set_clk_source(cmu_driver_t *cdp, cmu_clk_sel_t clk_src);

/**
 * @brief   Sets the high frequency reference value for the CLKMN1 frequency.
 * @note    Max value = 4095.
 * @note    This configuration becomes effective after cmu_start.
 *
 * @param[out] cdp      pointer to a @p cmu_driver_t structure
 * @param[in] hfref     high frequency reference value
 *
 * @api
 */
uint16_t cmu_set_high_freq_ref(cmu_driver_t *cdp, uint16_t hfref);

/**
 * @brief   Sets the low frequency reference value for the CLKMN1 frequency.
 * @note    Max value = 4095.
 * @note    This configuration becomes effective after cmu_start.
 *
 * @param[out] cdp      pointer to a @p cmu_driver_t structure
 * @param[in] lfref     low frequency reference value
 *
 * @api
 */
uint16_t cmu_set_low_freq_ref(cmu_driver_t *cdp, uint16_t lfref);

/**
 * @brief   Sets the frequency meter clock.
 * @note    This only effects CMU instances that utilizes clock metering.
 * @note    This configuration becomes effective after cmu_start.
 *
 * @param[out] cdp      pointer to a @p cmu_driver_t structure
 * @param[in] cksel     clock selection value
 *
 * @api
 */
cmu_clk_sel_t cmu_set_freq_meter_clock(cmu_driver_t *cdp, cmu_clk_sel_t cksel);

/**
 * @brief   Sets the CLKMT0_RMN division factor.
 * @note    This only effects CMU instances that utilizes clock metering.
 * @note    This configuration becomes effective after cmu_start.
 *
 * @param[out] cdp      pointer to a @p cmu_driver_t structure
 * @param[in] rcdiv     division factor value
 *
 * @api
 */
cmu_rcdiv_t cmu_set_division_factor(cmu_driver_t *cdp, cmu_rcdiv_t rcdiv);

/**
 * @brief   Sets the measurement duration expressed in selected clock cycles.
 * @note    This only effects CMU instances that utilizes clock metering.
 * @note    This configuration becomes effective after cmu_start.
 *
 * @param[out] cdp      pointer to a @p cmu_driver_t structure
 * @param[in] duration  measurement duration value
 *
 * @api
 */
uint32_t cmu_set_measurement_duration(cmu_driver_t *cdp, uint32_t duration);

/**
 * @brief   Starts a CMU instance.
 *
 * @param[out] cdp      pointer to a @p cmu_driver_t structure
 *
 * @api
 */
void cmu_start(cmu_driver_t *cdp);

/**
 * @brief   Gets the monitored frequency value.
 *
 * @param[in] cdp       pointer to a @p cmu_driver_t structure
 * @return              frequency value.
 *
 * @api
 */
float cmu_get_monitored_frequency(cmu_driver_t *cdp);

/**
 * @brief   Gets the type of interrupt occurred.
 * @note    Must be used in the FCCU CH17 callback to discriminate which
 *          peripheral generated the interrupt and which interrupt is generated.
 *
 * @param[in] cdp       pointer to a @p cmu_driver_t structure
 * @return              interrupt type.
 *
 * @api
 */
cmu_int_t cmu_get_event(cmu_driver_t *cdp);

/**
 * @brief   Clears the interrupt event flag
 *
 * @param[in] cdp       pointer to a @p cmu_driver_t structure
 * @param[in] event     type of interrupt event to clean.
 *
 * @api
 */
void cmu_clear_event(cmu_driver_t *cdp, cmu_int_t event);

/**
 * @brief   Stops a CMU instance.
 *
 * @param[out] cdp      pointer to a @p cmu_driver_t structure
 *
 * @api
 */
void cmu_stop(cmu_driver_t *cdp);

/**
 * @brief   Deinitializes CMU instance.
 *
 * @param[out] cdp      pointer to a @p cmu_driver_t structure
 *
 * @api
 */
void cmu_deinit(cmu_driver_t *cdp);

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _CMU_H_ */

/** @} */
