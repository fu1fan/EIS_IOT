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
 * @file    smpu.h
 * @brief   SMPU driver header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup SMPU
 * @ingroup SYSTEM
 * @{
 */

#ifndef _SMPU_H_
#define _SMPU_H_

#include <typedefs.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

#define SMPU_REGIONS_NUM                    24U

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
 * @enum smpu_region_t
 * SMPU regions.
 */
typedef enum {
    SMPU_REGION_0  =  0U,         /**< Region  0 */
    SMPU_REGION_1  =  1U,         /**< Region  1 */
    SMPU_REGION_2  =  2U,         /**< Region  2 */
    SMPU_REGION_3  =  3U,         /**< Region  3 */
    SMPU_REGION_4  =  4U,         /**< Region  4 */
    SMPU_REGION_5  =  5U,         /**< Region  5 */
    SMPU_REGION_6  =  6U,         /**< Region  6 */
    SMPU_REGION_7  =  7U,         /**< Region  7 */
    SMPU_REGION_8  =  8U,         /**< Region  8 */
    SMPU_REGION_9  =  9U,         /**< Region  9 */
    SMPU_REGION_10 = 10U,         /**< Region 10 */
    SMPU_REGION_11 = 11U,         /**< Region 11 */
    SMPU_REGION_12 = 12U,         /**< Region 12 */
    SMPU_REGION_13 = 13U,         /**< Region 13 */
    SMPU_REGION_14 = 14U,         /**< Region 14 */
    SMPU_REGION_15 = 15U,         /**< Region 15 */
    SMPU_REGION_16 = 16U,         /**< Region 16 */
    SMPU_REGION_17 = 17U,         /**< Region 17 */
    SMPU_REGION_18 = 18U,         /**< Region 18 */
    SMPU_REGION_19 = 19U,         /**< Region 19 */
    SMPU_REGION_20 = 20U,         /**< Region 20 */
    SMPU_REGION_21 = 21U,         /**< Region 21 */
    SMPU_REGION_22 = 22U,         /**< Region 22 */
    SMPU_REGION_23 = 23U,         /**< Region 23 */
} smpu_region_t;

/**
 * @enum smpu_master_t
 * SMPU bus masters.
 */
typedef enum {
    SMPU_MASTER_CORE_2   =  1U,   /**< Master core 2   */
    SMPU_MASTER_CORE_HSM =  2U,   /**< Master core hsm */
    SMPU_MASTER_DMA2     =  4U,   /**< Master dma 2    */
    SMPU_MASTER_CORE_1   =  8U,   /**< Master core 1   */
    SMPU_MASTER_DMA1     = 11U,   /**< Master dma 1    */
    SMPU_MASTER_DEBUG    = 13U,   /**< Master debug    */
} smpu_master_t;

/**
 * @enum smpu_access_right_t
 * SMPU access rights.
 */
typedef enum {
    SMPU_NO_ACCESS         = 0U,  /**< No access         */
    SMPU_READ_ACCESS       = 1U,  /**< Read only access  */
    SMPU_WRITE_ACCESS      = 2U,  /**< Write only access */
    SMPU_READ_WRITE_ACCESS = 3U,  /**< Read/Write access */
} smpu_access_right_t;

/**
 * @enum smpu_err_attr_t
 * SMPU error attributes.
 */
typedef enum {
    SMPU_ERR_UM_IA         = 0U,  /**< User mode, instruction access       */
    SMPU_ERR_UM_DA         = 1U,  /**< User mode, data access              */
    SMPU_ERR_SM_IA         = 2U,  /**< Supervisor mode, instruction access */
    SMPU_ERR_SM_DA         = 3U,  /**< Supervisor mode, data access        */
} smpu_err_attr_t;

/**
 * @enum smpu_err_master_t
 * SMPU error bus masters.
 */
typedef enum {
    SMPU_NO_MSTR_ERR       =  0U, /**< No master error */
    SMPU_ERR_MSTR_CORE_2   =  1U, /**< Master core 2   */
    SMPU_ERR_MSTR_CORE_HSM =  2U, /**< Master core hsm */
    SMPU_ERR_MSTR_DMA2     =  4U, /**< Master dma 2    */
    SMPU_ERR_MSTR_CORE_1   =  8U, /**< Master core 1   */
    SMPU_ERR_MSTR_DMA1     = 11U, /**< Master dma 1    */
    SMPU_ERR_MSTR_DEBUG    = 13U, /**< Master debug    */
} smpu_err_master_t;

/**
 * @brief   Type of SMPU peripheral hardware informations.
 */
typedef struct {
    /**
     * @brief   hardware revision level.
     */
    uint8_t                  rev_level;
    /**
     * @brief   number of region descriptors.
     */
    uint8_t                  rgd_num;
} smpu_hw_info_t;

/**
 * @brief   Type of SMPU error informations.
 */
typedef struct {
    /**
     * @brief   master error source.
     */
    smpu_err_master_t        master;
    /**
     * @brief   master error overrun.
     */
    smpu_err_master_t        overrun;
    /**
     * @brief   error address.
     */
    uint32_t                 address;
    /**
     * @brief   involved error regions.
     */
    bool                     region[SMPU_REGIONS_NUM];
    /**
     * @brief   error attributes.
     */
    smpu_err_attr_t          attribute;
    /**
     * @brief   access type.
     */
    smpu_access_right_t      rw_access;
} smpu_error_info_t;

/**
 * @brief   Type of an SMPU driver.
 */
typedef struct smpu_driver smpu_driver_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Export driver modules.*/
#include <smpu_instances.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Initializes SMPU instance.
 *
 * @param[out] sdp      pointer to a @p smpu_driver_t structure
 *
 * @api
 */
void smpu_init(smpu_driver_t *sdp);

/**
 * @brief   Enables/disables the SMPU region.
 * @note    This configuration becomes effective after smpu_start.
 *
 * @param[out] sdp      pointer to a @p smpu_driver_t structure
 * @param[in] region    Region to be configured
 * @param[in] enable    Region enable status ("true" means the region is
 *                      enabled, "false" otherwise)
 * @return              Previous enable status.
 *
 * @api
 */
bool smpu_enable_region(smpu_driver_t *sdp, smpu_region_t region, bool enable);

/**
 * @brief   Sets SMPU region start address.
 * @note    This configuration becomes effective after smpu_start.
 *
 * @param[out] sdp      pointer to a @p smpu_driver_t structure
 * @param[in] region    Region to be configured
 * @param[in] address   Address to be configured
 * @return              Previous address.
 *
 * @api
 */
uint32_t smpu_set_start_address(smpu_driver_t *sdp, smpu_region_t region,
                                uint32_t address);

/**
 * @brief   Sets SMPU region end address.
 * @note    This configuration becomes effective after smpu_start.
 *
 * @param[out] sdp      pointer to a @p smpu_driver_t structure
 * @param[in] region    Region to be configured
 * @param[in] address   Address to be configured.
 * @return              Previous address.
 *
 * @api
 */
uint32_t smpu_set_end_address(smpu_driver_t *sdp, smpu_region_t region,
                              uint32_t address);

/**
 * @brief   Sets SMPU region bus master permissions.
 * @note    This configuration becomes effective after smpu_start.
 *
 * @param[out] sdp      pointer to a @p smpu_driver_t structure
 * @param[in] region    Region to be configured
 * @param[in] master    Bus master to be configured
 * @param[in] rights    rights to be configured.
 * @return              old region rights.
 *
 * @api
 */
uint32_t smpu_set_rights(smpu_driver_t *sdp, smpu_region_t region,
                         smpu_master_t master, smpu_access_right_t rights);

/**
 * @brief   Sets the SMPU region cacheability attribute.
 * @note    This configuration becomes effective after smpu_start.
 *
 * @param[out] sdp      pointer to a @p smpu_driver_t structure
 * @param[in] region    Region to be configured
 * @param[in] enable    Region cacheability enable status ("true" means the
 *                      region cacheability is enabled, "false" otherwise)
 * @return              Previous cacheability status.
 *
 * @api
 */
bool smpu_set_cacheability(smpu_driver_t *sdp, smpu_region_t region,
                           bool enable);

/**
 * @brief   Sets the SMPU region read-only attribute.
 * @note    Setting RO locks all four words of the RGD until a system reset.
 *          This configuration becomes effective after smpu_start.
 *
 * @param[out] sdp      pointer to a @p smpu_driver_t structure
 * @param[in] region    Region to be configured
 * @param[in] enable    Region read-only enable status ("true" means the
 *                      region read-only is enabled, "false" otherwise)
 * @return              Previous read-only status.
 *
 * @api
 */
bool smpu_set_read_only(smpu_driver_t *sdp, smpu_region_t region, bool enable);

/**
 * @brief   Removes an SMPU region descriptor.
 * @note    This configuration becomes effective after smpu_start.
 *
 * @param[out] sdp      pointer to a @p smpu_driver_t structure
 *
 * @api
 */
void smpu_remove_region(smpu_driver_t *sdp, smpu_region_t region);

/**
 * @brief   Starts SMPU instance.
 * 
 * @param[in] sdp       pointer to a @p smpu_driver_t structure
 *
 * @api
 */
void smpu_start(smpu_driver_t *sdp);

/**
 * @brief   Gets the SMPU module hardware info.
 *
 * @param[in] sdp       pointer to a @p smpu_driver_t structure
 * @return              hardware info structure.
 *
 * @api
 */
smpu_hw_info_t smpu_get_hardware_info(smpu_driver_t *sdp);

/**
 * @brief   Gets the SMPU error info.
 *
 * @param[in] sdp       pointer to a @p smpu_driver_t structure
 * @return              error info structure.
 *
 * @api
 */
smpu_error_info_t smpu_get_error(smpu_driver_t *sdp);

/**
 * @brief   Clears the SMPU errors.
 *
 * @param[in] sdp       pointer to a @p smpu_driver_t structure
 *
 * @api
 */
void smpu_clear_errors(smpu_driver_t *sdp);

/**
 * @brief   Stops SMPU instance.
 *
 * @param[in] sdp       pointer to a @p smpu_driver_t structure
 *
 * @api
 */
void smpu_stop(smpu_driver_t *sdp);

/**
 * @brief   Deinitializes SMPU instance.
 *
 * @param[out] sdp      pointer to a @p smpu_driver_t structure
 *
 * @api
 */
void smpu_deinit(smpu_driver_t *sdp);

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _SMPU_H_ */

/** @} */
