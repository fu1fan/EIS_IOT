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
 * @file    smpu_dev.c
 * @brief   SMPU device specific source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup SMPU
 * @ingroup SYSTEM
 * @{
 */

#include <smpu.h>
#include <smpu_private.h>
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
 * @brief   Returns pointer to the SMPU register block.
 *
 * @param[in] sdp       pointer to a @p smpu_driver_t structure
 * @return              pointer to the SMPU register block.
 *
 * @notapi
 */
SMPU_TypeDef *smpu_dev_get_reg_ptr(smpu_driver_t *sdp) {

    (void)sdp;

    return SMPU;
}

/**
 * @brief   Enables SMPU clock.
 *
 * @param[in] sdp       pointer to a @p smpu_driver_t structure
 *
 * @notapi
 */
void smpu_dev_clock_enable(smpu_driver_t *sdp) {

    (void)sdp;

    clock_enable_SMPU(true);
}

/**
 * @brief   Disables SMPU clock.
 *
 * @param[in] sdp       pointer to a @p smpu_driver_t structure
 *
 * @notapi
 */
void smpu_dev_clock_disable(smpu_driver_t *sdp) {

    (void)sdp;

    clock_disable_SMPU();
}

/** @} */
