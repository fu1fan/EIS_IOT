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
 * @file    pmu.h
 * @brief   PMU driver header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup PMU
 * @ingroup SYSTEM
 * @{
 */

#ifndef _PMU_H_
#define _PMU_H_

#include <typedefs.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Number of voltage detects.
 */
#define PMU_VD_NUM                          18U

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
 * @brief   Type of an PMU driver.
 */
typedef struct pmu_driver pmu_driver_t;

/**
 * @enum pmu_vd_t
 * PMU supported voltage detects.
 */
typedef enum {
    PMU_VD_LVD119_C,
    PMU_VD_LVD119_FL,
    PMU_VD_LVD119_DD,
    PMU_VD_LVD119_RC,
    PMU_VD_LVD119_PLL0,
    PMU_VD_LVD119_PLL1,
    PMU_VD_HVD140_C,
    PMU_VD_UVD145_RC,
    PMU_VD_LVD290_DACCMP,
    PMU_VD_LVD290_OSC,
    PMU_VD_UVD380_AS,
    PMU_VD_UVD380_DACCMP,
    PMU_VD_LVD290_C,
    PMU_VD_LVD290_FL,
    PMU_VD_LVD290_IO1,
    PMU_VD_LVD290_IO0,
    PMU_VD_LVD290_AS,
    PMU_VD_LVD290_AD
} pmu_vd_t;

/**
 * @enum pmu_vd_descriptor_t
 * PMU voltage detect descriptor type.
 */
typedef struct {
    /**
     * @brief   Voltage detect.
     */
    pmu_vd_t            vd;
    /**
     * @brief   Position in the registers Reset Event Enable, Reset Event
     *          Select, Interrupt Enable and FCCU Event Enable of the bit
     *          related to the specified PMU voltage detect.
     */
    uint8_t             pos;
} pmu_vd_descriptor_t;

/**
 * @enum pmu_reset_t
 * PMU supported reset settings.
 */
typedef enum {
    PMU_RES_NO_RESET,
    PMU_RES_DESTRUCTIVE_RESET,
    PMU_RES_FUNCTIONAL_RESET
} pmu_reset_t;

/**
 * @brief   Type of PMU callbacks.
 *
 * @param[in] pdp       pointer to the @p pmu_driver_t object
 */
typedef void (*pmu_cb_t)(pmu_driver_t *pdp);

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Export driver modules.*/
#include <pmu_instances.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Initializes PMU instance.
 *
 * @param[out] pdp      pointer to a @p pmu_driver_t structure
 *
 * @api
 */
void pmu_init(pmu_driver_t *pdp);

/**
 * @brief   Configures PMU interrupt priority.
 * @note    This configuration becomes effective after pmu_start.
 *
 * @param[out] pdp      pointer to a @p pmu_driver_t structure
 * @param[in] prio      interrupt priority to be configured
 * @return              previous interrupt priority.
 *
 * @api
 */
uint32_t pmu_set_prio(pmu_driver_t *pdp, uint32_t prio);

/**
 * @brief   Configures PMU voltage detect reset type.
 * @note    This configuration becomes effective after pmu_start.
 *
 * @param[out] pdp      pointer to a @p pmu_driver_t structure
 * @param[in] vd        voltage detect for which to configure the reset type.
 *                      It can be one of the following values:
 *            @arg @ref PMU_VD_LVD119_C
 *            @arg @ref PMU_VD_LVD119_FL
 *            @arg @ref PMU_VD_LVD119_DD
 *            @arg @ref PMU_VD_LVD119_RC
 *            @arg @ref PMU_VD_LVD119_PLL0
 *            @arg @ref PMU_VD_LVD119_PLL1
 *            @arg @ref PMU_VD_HVD140_C
 *            @arg @ref PMU_VD_UVD145_RC
 *            @arg @ref PMU_VD_LVD290_DACCMP
 *            @arg @ref PMU_VD_LVD290_OSC
 *            @arg @ref PMU_VD_UVD380_AS
 *            @arg @ref PMU_VD_UVD380_DACCMP
 *            @arg @ref PMU_VD_LVD290_C
 *            @arg @ref PMU_VD_LVD290_FL
 *            @arg @ref PMU_VD_LVD290_IO1
 *            @arg @ref PMU_VD_LVD290_IO0
 *            @arg @ref PMU_VD_LVD290_AS
 *            @arg @ref PMU_VD_LVD290_AD
 * @param[in] res_type  reset type to be configured. It can be one of the
 *                      following values:
 *            @arg @ref PMU_RES_NO_RESET
 *            @arg @ref PMU_RES_DESTRUCTIVE_RESET
 *            @arg @ref PMU_RES_FUNCTIONAL_RESET
 * @return              previos reset type. It can be one of the following
 *                      values:
 *            @arg @ref PMU_RES_NO_RESET
 *            @arg @ref PMU_RES_DESTRUCTIVE_RESET
 *            @arg @ref PMU_RES_FUNCTIONAL_RESET
 *
 * @api
 */
pmu_reset_t pmu_set_reset(pmu_driver_t *pdp, pmu_vd_t vd, pmu_reset_t res_type);

/**
 * @brief   Enables/Disables PMU voltage detect interrupt.
 * @note    This configuration becomes effective after pmu_start.
 *
 * @param[out] pdp      pointer to a @p pmu_driver_t structure
 * @param[in] vd        voltage detect for which to configure the reset type.
 *                      It can be one of the following values:
 *            @arg @ref PMU_VD_LVD119_C
 *            @arg @ref PMU_VD_LVD119_FL
 *            @arg @ref PMU_VD_LVD119_DD
 *            @arg @ref PMU_VD_LVD119_RC
 *            @arg @ref PMU_VD_LVD119_PLL0
 *            @arg @ref PMU_VD_LVD119_PLL1
 *            @arg @ref PMU_VD_HVD140_C
 *            @arg @ref PMU_VD_UVD145_RC
 *            @arg @ref PMU_VD_LVD290_DACCMP
 *            @arg @ref PMU_VD_LVD290_OSC
 *            @arg @ref PMU_VD_UVD380_AS
 *            @arg @ref PMU_VD_UVD380_DACCMP
 *            @arg @ref PMU_VD_LVD290_C
 *            @arg @ref PMU_VD_LVD290_FL
 *            @arg @ref PMU_VD_LVD290_IO1
 *            @arg @ref PMU_VD_LVD290_IO0
 *            @arg @ref PMU_VD_LVD290_AS
 *            @arg @ref PMU_VD_LVD290_AD
 * @param[in] enable    enable flag status ("true" means the interrupt is
 *                      enabled, "false" otherwise)
 * @return              previos enable flag status.
 *
 * @api
 */
bool pmu_enable_interrupt(pmu_driver_t *pdp, pmu_vd_t vd, bool enable);

/**
 * @brief   Enables/Disables PMU voltage detect FCCU event.
 * @note    This configuration becomes effective after pmu_start.
 *
 * @param[out] pdp      pointer to a @p pmu_driver_t structure
 * @param[in] vd        voltage detect for which to configure the reset type.
 *                      It can be one of the following values:
 *            @arg @ref PMU_VD_LVD119_C
 *            @arg @ref PMU_VD_LVD119_FL
 *            @arg @ref PMU_VD_LVD119_DD
 *            @arg @ref PMU_VD_LVD119_RC
 *            @arg @ref PMU_VD_LVD119_PLL0
 *            @arg @ref PMU_VD_LVD119_PLL1
 *            @arg @ref PMU_VD_HVD140_C
 *            @arg @ref PMU_VD_UVD145_RC
 *            @arg @ref PMU_VD_LVD290_DACCMP
 *            @arg @ref PMU_VD_LVD290_OSC
 *            @arg @ref PMU_VD_UVD380_AS
 *            @arg @ref PMU_VD_UVD380_DACCMP
 *            @arg @ref PMU_VD_LVD290_C
 *            @arg @ref PMU_VD_LVD290_FL
 *            @arg @ref PMU_VD_LVD290_IO1
 *            @arg @ref PMU_VD_LVD290_IO0
 *            @arg @ref PMU_VD_LVD290_AS
 *            @arg @ref PMU_VD_LVD290_AD
 * @param[in] enable    enable flag status ("true" means the FCCU event is
 *                      enabled, "false" otherwise)
 * @return              previos enable flag status.
 *
 * @api
 */
bool pmu_enable_fccu(pmu_driver_t *pdp, pmu_vd_t vd, bool enable);

/**
 * @brief   Configures pointers to PMU callback functions.
 *
 * @param[out] pdp      pointer to a @p pmu_driver_t structure
 * @param[in] vd        voltage detect for which to configure the callback. It
 *                      can be one of the following values:
 *            @arg @ref PMU_VD_LVD119_C
 *            @arg @ref PMU_VD_LVD119_FL
 *            @arg @ref PMU_VD_LVD119_DD
 *            @arg @ref PMU_VD_LVD119_RC
 *            @arg @ref PMU_VD_LVD119_PLL0
 *            @arg @ref PMU_VD_LVD119_PLL1
 *            @arg @ref PMU_VD_HVD140_C
 *            @arg @ref PMU_VD_UVD145_RC
 *            @arg @ref PMU_VD_LVD290_DACCMP
 *            @arg @ref PMU_VD_LVD290_OSC
 *            @arg @ref PMU_VD_UVD380_AS
 *            @arg @ref PMU_VD_UVD380_DACCMP
 *            @arg @ref PMU_VD_LVD290_C
 *            @arg @ref PMU_VD_LVD290_FL
 *            @arg @ref PMU_VD_LVD290_IO1
 *            @arg @ref PMU_VD_LVD290_IO0
 *            @arg @ref PMU_VD_LVD290_AS
 *            @arg @ref PMU_VD_LVD290_AD
 * @param[in] cb        pointer to PMU callback function
 * @return              pointer to previuos PMU callback function.
 *
 * @api
 */
pmu_cb_t pmu_set_cb(pmu_driver_t *pdp, pmu_vd_t vd, pmu_cb_t cb);

/**
 * @brief   Starts PMU instance.
 *
 * @param[in] pdp       pointer to a @p pmu_driver_t structure
 *
 * @api
 */
void pmu_start(pmu_driver_t *pdp);

/**
 * @brief   Returns PMU voltage detect event pending status.
 *
 * @param[out] pdp      pointer to a @p pmu_driver_t structure
 * @param[in] vd        voltage detect for which to return the event pending
 *                      status. It can be one of the following values:
 *            @arg @ref PMU_VD_LVD119_C
 *            @arg @ref PMU_VD_LVD119_FL
 *            @arg @ref PMU_VD_LVD119_DD
 *            @arg @ref PMU_VD_LVD119_RC
 *            @arg @ref PMU_VD_LVD119_PLL0
 *            @arg @ref PMU_VD_LVD119_PLL1
 *            @arg @ref PMU_VD_HVD140_C
 *            @arg @ref PMU_VD_UVD145_RC
 *            @arg @ref PMU_VD_LVD290_DACCMP
 *            @arg @ref PMU_VD_LVD290_OSC
 *            @arg @ref PMU_VD_UVD380_AS
 *            @arg @ref PMU_VD_UVD380_DACCMP
 *            @arg @ref PMU_VD_LVD290_C
 *            @arg @ref PMU_VD_LVD290_FL
 *            @arg @ref PMU_VD_LVD290_IO1
 *            @arg @ref PMU_VD_LVD290_IO0
 *            @arg @ref PMU_VD_LVD290_AS
 *            @arg @ref PMU_VD_LVD290_AD
 * @return              PMU voltage detect event pending status ("true" means
 *                      event has not been detected, "false" otherwise).
 *
 * @api
 */
bool pmu_get_status(pmu_driver_t *pdp, pmu_vd_t vd);

/**
 * @brief   Clears the event pending status for a specified PMU voltage detect.
 *
 * @param[out] pdp      pointer to an @p pmu_driver_t structure
 * @param[in] vd        PMU voltage detect for which to clear the event pending
 *                      status. It can be one of the following values:
 *            @arg @ref PMU_VD_LVD119_C
 *            @arg @ref PMU_VD_LVD119_FL
 *            @arg @ref PMU_VD_LVD119_DD
 *            @arg @ref PMU_VD_LVD119_RC
 *            @arg @ref PMU_VD_LVD119_PLL0
 *            @arg @ref PMU_VD_LVD119_PLL1
 *            @arg @ref PMU_VD_HVD140_C
 *            @arg @ref PMU_VD_UVD145_RC
 *            @arg @ref PMU_VD_LVD290_DACCMP
 *            @arg @ref PMU_VD_LVD290_OSC
 *            @arg @ref PMU_VD_UVD380_AS
 *            @arg @ref PMU_VD_UVD380_DACCMP
 *            @arg @ref PMU_VD_LVD290_C
 *            @arg @ref PMU_VD_LVD290_FL
 *            @arg @ref PMU_VD_LVD290_IO1
 *            @arg @ref PMU_VD_LVD290_IO0
 *            @arg @ref PMU_VD_LVD290_AS
 *            @arg @ref PMU_VD_LVD290_AD
 *
 * @api
 */
void pmu_clear_status(pmu_driver_t *pdp, pmu_vd_t vd);

/**
 * @brief   Stops PMU instance.
 *
 * @param[in] pdp       pointer to a @p pmu_driver_t structure
 *
 * @api
 */
void pmu_stop(pmu_driver_t *pdp);

/**
 * @brief   Deinitializes PMU instance.
 *
 * @param[out] pdp       pointer to a @p pmu_driver_t structure
 *
 * @api
 */
void pmu_deinit(pmu_driver_t *pdp);

/**
 * @brief   Shared interrupt handler.
 * @note    Not an API, used internally.
 *
 * @param[in] pdp       pointer to a @p pmu_driver_t structure
 *
 * @isr
 */
void __pmu_serve_interrupt(pmu_driver_t *pdp);

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _PMU_H_ */

/** @} */
