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
 * @file    crc_dev.c
 * @brief   CRC device specific source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup CRC
 * @ingroup SYSTEM
 * @{
 */

#include <crc.h>
#include <crc_private.h>
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
 * @brief   Returns pointer to the CRC register block.
 *
 * @param[in] cdp       pointer to a @p crc_driver_t structure
 * @return              pointer to the CRC register block.
 *
 * @notapi
 */
CRC_TypeDef *crc_dev_get_reg_ptr(crc_driver_t *cdp) {

    (void)cdp;

    return CRC;
}

/**
 * @brief   Enables CRC clock.
 *
 * @param[in] cdp       pointer to a @p crc_driver_t structure
 *
 * @notapi
 */
void crc_dev_clock_enable(crc_driver_t *cdp) {

    (void)cdp;

    clock_enable_CRC(true);
}

/**
 * @brief   Disables CRC clock.
 *
 * @param[in] cdp       pointer to a @p crc_driver_t structure
 *
 * @notapi
 */
void crc_dev_clock_disable(crc_driver_t *cdp) {

    (void)cdp;

    clock_disable_CRC();
}

/** @} */
