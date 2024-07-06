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
 * @file    cmu_dev.c
 * @brief   CMU device specific source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SAFETY
 * @ingroup DRIVERS
 * @addtogroup CMU
 * @ingroup SAFETY
 * @{
 */

#include <cmu.h>
#include <cmu_private.h>
#include <clock.h>
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
 * @brief   Returns pointer to the CMU register block.
 *
 * @param[in] cdp       pointer to a @p cmu_driver_t structure
 * @return              pointer to the CMU register block.
 *
 * @notapi
 */
CMU_TypeDef *cmu_dev_get_reg_ptr(cmu_driver_t *cdp) {

    CMU_TypeDef *cmu;

    if (cdp == &DRV_CMU0) {
        cmu = CMU0;
    } else if (cdp == &DRV_CMU1) {
        cmu = CMU1;
    } else if (cdp == &DRV_CMU2) {
        cmu = CMU2;
    } else if (cdp == &DRV_CMU3) {
        cmu = CMU3;
    } else if (cdp == &DRV_CMU4) {
        cmu = CMU4;
    } else {
        cmu = CMU5;
    }

    return cmu;
}

/**
 * @brief   Enables CMU clock.
 *
 * @param[in] cdp       pointer to a @p cmu_driver_t structure
 *
 * @notapi
 */
void cmu_dev_clock_enable(cmu_driver_t *cdp) {

    if (cdp == &DRV_CMU0) {
        clock_enable_CMU0();
    } else if (cdp == &DRV_CMU1) {
        clock_enable_CMU1();
    } else if (cdp == &DRV_CMU2) {
        clock_enable_CMU2();
    } else if (cdp == &DRV_CMU3) {
        clock_enable_CMU3();
    } else if (cdp == &DRV_CMU4) {
        clock_enable_CMU4();
    } else {
        clock_enable_CMU5();
    }
}

/**
 * @brief   Disables CMU clock.
 *
 * @param[in] cdp       pointer to a @p cmu_driver_t structure
 *
 * @notapi
 */
void cmu_dev_clock_disable(cmu_driver_t *cdp) {

    if (cdp == &DRV_CMU0) {
        clock_disable_CMU0();
    } else if (cdp == &DRV_CMU1) {
        clock_disable_CMU1();
    } else if (cdp == &DRV_CMU2) {
        clock_disable_CMU2();
    } else if (cdp == &DRV_CMU3) {
        clock_disable_CMU3();
    } else if (cdp == &DRV_CMU4) {
        clock_disable_CMU4();
    } else {
        clock_disable_CMU5();
    }
}

/** @} */
