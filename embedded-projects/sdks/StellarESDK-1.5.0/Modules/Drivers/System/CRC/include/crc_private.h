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
 * @file    crc_private.h
 * @brief   CRC driver private header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup CRC
 * @ingroup SYSTEM
 * @{
 */

#ifndef _CRC_PRIVATE_H_
#define _CRC_PRIVATE_H_

#include <dma.h>
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
 * @brief   CRC driver structure.
 * @note    This structure is internal to the CRC driver.
 */
struct crc_driver {
    /**
     * @brief   Pointer to CRC registers block.
     */
    CRC_TypeDef              *crc;
    /**
     * @brief   Operation in progress flag.
     */
    volatile crc_op_sts_t    op_busy;
    /**
     * @brief   CRC driver mode.
     */
    crc_drv_mode_t           drv_mode;
    /**
     * @brief   CRC init value.
     */
    uint32_t                 init;
    /**
     * @brief   CRC reverse input mode.
     */
    crc_rev_in_t             rev_in_mode;
    /**
     * @brief   CRC reverse output mode.
     */
    crc_rev_out_t            rev_out_mode;
    /**
     * @brief   CRC reverse output mode.
     */
    crc_polysize_t           polysize;
    /**
     * @brief   DMA mode (enabled or disabled).
     */
    bool                     is_dma_en;
    /**
     * @brief   DMA configuration.
     */
    crc_dma_conf_t           dma_conf;
    /**
     * @brief   Allocated DMA descriptor.
     */
    const dma_descriptor_t   *dma_descriptor;
    /**
     * @brief   Pointers to CRC callback functions.
     * @note    Set to @p NULL if not required.
     */
    crc_cb_t                 cb;
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

CRC_TypeDef *crc_dev_get_reg_ptr(crc_driver_t *cdp);
void crc_dev_clock_enable(crc_driver_t *cdp);
void crc_dev_clock_disable(crc_driver_t *cdp);

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _CRC_PRIVATE_H_ */

/** @} */
