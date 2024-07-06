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
 * @file    pmu_private.h
 * @brief   PMU driver private header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup PMU
 * @ingroup SYSTEM
 * @{
 */

#ifndef _PMU_PRIVATE_H_
#define _PMU_PRIVATE_H_

#include <irq.h>
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
 * @brief   PMU driver structure.
 * @note    This structure is internal to the PMU driver.
 */
struct pmu_driver {
    /**
     * @brief   Pointer to PMU registers block.
     */
    PMC_DIG_TypeDef          *pmu;
    /**
     * @brief   PMU interrupt priority.
     */
    uint32_t                 prio;
    /**
     * @brief   PMU interrupt enable flag.
     */
    bool                     is_int_en;
    /**
     * @brief   PMU voltage detect reset type.
     */
    pmu_reset_t              res_type[PMU_VD_NUM];
    /**
     * @brief   PMU voltage detect interrupt enable flag.
     */
    bool                     int_en[PMU_VD_NUM];
    /**
     * @brief   PMU voltage detect FCCU enable flag.
     */
    bool                     fccu_en[PMU_VD_NUM];
    /**
     * @brief   Pointers to PMU callback functions.
     * @note    Set to @p NULL if not required.
     */
    pmu_cb_t                 cb[PMU_VD_NUM];
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

IRQ_HANDLER(IRQ_PMC_DIG_HANDLER);

PMC_DIG_TypeDef *pmu_dev_get_reg_ptr(pmu_driver_t *pdp);
IRQn_Type pmu_dev_get_vector(pmu_driver_t *pdp);

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _PMU_PRIVATE_H_ */

/** @} */

