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
 * @file    smpu_private.h
 * @brief   SMPU driver private header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup SMPU
 * @ingroup SYSTEM
 * @{
 */

#ifndef _SMPU_PRIVATE_H_
#define _SMPU_PRIVATE_H_

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
 * @brief   Type of region descriptor configuration.
 */
typedef struct {
    /**
     * @brief   RGDn start address.
     */
    uint32_t                 start_addr;
    /**
     * @brief   RGDn end address.
     */
    uint32_t                 end_addr;
    /**
     * @brief   RGDn access rights.
     */
    uint32_t                 accs_rights;
    /**
     * @brief   Cacheability attribute.
     */
    bool                     cacheability;
    /**
     * @brief   Read only attribute.
     */
    bool                     read_only;
} smpu_region_cfg_t;

/**
 * @brief   SMPU driver structure.
 * @note    This structure is internal to the SMPU driver.
 */
struct smpu_driver {
    /**
     * @brief   Pointer to SMPU registers block.
     */
    SMPU_TypeDef             *smpu;
    /**
     * @brief   Pointer to SMPU registers block.
     */
    bool                     is_region_en[SMPU_REGIONS_NUM];
    /**
     * @brief   Region configuration.
     */
    smpu_region_cfg_t        region_cfg[SMPU_REGIONS_NUM];
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

SMPU_TypeDef *smpu_dev_get_reg_ptr(smpu_driver_t *sdp);
void smpu_dev_clock_enable(smpu_driver_t *sdp);
void smpu_dev_clock_disable(smpu_driver_t *sdp);

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _SMPU_PRIVATE_H_ */

/** @} */
