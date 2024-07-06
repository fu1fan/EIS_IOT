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
 * @file    cmu_private.h
 * @brief   CMU driver private header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SAFETY
 * @ingroup DRIVERS
 * @addtogroup CMU
 * @ingroup SAFETY
 * @{
 */

#ifndef _CMU_PRIVATE_H_
#define _CMU_PRIVATE_H_

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
 * @brief   Type of CMU frequency meter configuration.
 */
typedef struct {
    /**
     * @brief   Frequency measure clock selection.
     */
    cmu_clk_sel_t            cksel1;
    /**
     * @brief   CLKMT0_RMN division factor 2^rcdiv.
     */
    cmu_rcdiv_t              rcdiv;
    /**
     * @brief   Measurement duration expressed in selected clock cycles.
     */
    uint32_t                 duration;
} cmu_fm_cfg_t;

/**
 * @brief   CMU driver structure.
 * @note    This structure is internal to the CMU driver.
 */
struct cmu_driver {
    /**
     * @brief   Pointer to CMU registers block.
     */
    CMU_TypeDef              *cmu;
    /**
     * @brief   Frequency meter configuration.
     * @note    This only effects CMU instances that utilizes clock metering.
     */
    cmu_fm_cfg_t             freq_meter_cfg;
    /**
     * @brief   High frequency reference.
     */
    uint16_t                 hfref;
    /**
     * @brief   Low frequency reference.
     */
    uint16_t                 lfref;
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

CMU_TypeDef *cmu_dev_get_reg_ptr(cmu_driver_t *cdp);
void cmu_dev_clock_enable(cmu_driver_t *cdp);
void cmu_dev_clock_disable(cmu_driver_t *cdp);

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _CMU_PRIVATE_H_ */

/** @} */

