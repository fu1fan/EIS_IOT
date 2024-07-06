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
 * @file    pmu_dev.c
 * @brief   PMU device specific source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup PMU
 * @ingroup SYSTEM
 * @{
 */

#include <pmu.h>
#include <pmu_private.h>
#include <irq.h>
#include <platform.h>

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

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
 * @brief   Returns pointer to the PMU register block.
 *
 * @param[in] ipp       pointer to a @p pmu_driver_t structure
 * @return              pointer to the PMU register block.
 *
 * @notapi
 */
PMC_DIG_TypeDef *pmu_dev_get_reg_ptr(pmu_driver_t *pdp) {

    (void)(pdp);

    return (PMC_DIG);
}

/**
 * @brief   Returns PMU interrupt vector.
 *
 * @param[in] pdp       pointer to a @p pmu_driver_t structure
 * @return              PMU interrupt vector
 *
 * @notapi
 */
IRQn_Type pmu_dev_get_vector(pmu_driver_t *pdp) {

    (void)pdp;

    return IRQ_PMC_DIG_VECTOR;
}

/**
 * @brief   PMU interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(IRQ_PMC_DIG_HANDLER) {

    IRQ_PROLOGUE();

    __pmu_serve_interrupt(&DRV_PMU);

    IRQ_EPILOGUE();
}

/** @} */
