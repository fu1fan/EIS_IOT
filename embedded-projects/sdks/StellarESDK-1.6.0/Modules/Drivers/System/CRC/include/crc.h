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
 * @file    crc.h
 * @brief   CRC driver header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup CRC
 * @ingroup SYSTEM
 * @{
 */

#ifndef _CRC_H_
#define _CRC_H_

#include <dma.h>
#include <typedefs.h>

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
 * @brief   Type of an CRC driver.
 */
typedef struct crc_driver crc_driver_t;

/**
 * @enum crc_sts_t
 * CRC operation progress status.
 */
typedef enum {
    CRC_OP_READY = 0U,            /**< Operation status ready */
    CRC_OP_BUSY  = 1U,            /**< Operation status busy  */
} crc_op_sts_t;

/**
 * @enum crc_drv_mode_t
 * CRC driver mode configuration.
 */
typedef enum {
    CRC_DRV_MODE_SYNC,            /**< Synchronous DMA mode                           */
    CRC_DRV_MODE_ASYNC,           /**< Asynchronous DMA mode (only if DMA is enabled) */
} crc_drv_mode_t;

/**
 * @enum crc_mode_t
 * CRC supported operational modes
 */
typedef enum {
    CRC_MODE_CONCAT,              /**< Concatanation with previous CRC calcalation */
    CRC_MODE_NEW                  /**< New CRC calculation                         */
} crc_mode_t;

/**
 * @enum crc_rev_in_t
 * CRC supported reverse input data
 */
typedef enum {
    CRC_REV_IN_NONE,              /**< Bit order not affected         */
    CRC_REV_IN_BYTE,              /**< Bit reversal done by byte      */
    CRC_REV_IN_HWORD,             /**< Bit reversal done by half-word */
    CRC_REV_IN_WORD               /**< Bit reversal done by word      */
} crc_rev_in_t;

/**
 * @enum crc_rev_out_t
 * CRC supported reverse output data
 */
typedef enum {
    CRC_REV_OUT_NONE,             /**< Bit order not affected     */
    CRC_REV_OUT_BIT               /**< Bit-reversed output format */
} crc_rev_out_t;

/**
 * @enum crc_polysize_t
 * CRC supported polynomial sizes
 */
typedef enum {
    CRC_POLYSIZE_32,              /**< 32 bit polynomial */
    CRC_POLYSIZE_16,              /**< 16 bit polynomial */
    CRC_POLYSIZE_8,               /**<  8 bit polynomial */
    CRC_POLYSIZE_7                /**<  7 bit polynomial */
} crc_polysize_t;

/**
 * @brief   Type of CRC DMA configuration.
 */
typedef struct {
    /**
     * @brief   CRC DMA stream to be used for operations.
     */
    uint32_t                 dma_stream_id;
    /**
     * @brief   CRC DMA stream bus priority (3..0).
     */
    uint32_t                 dma_stream_bus_prio;
    /**
     * @brief   CRC IRQ priority to be assigned to the DMA stream (3..15).
     */
    uint32_t                 dma_stream_irq_prio;
} crc_dma_conf_t;

/**
 * @brief   Type of CRC callbacks.
 *
 * @param[in] cdp       pointer to the @p crc_driver_t object
 */
typedef void (*crc_cb_t)(crc_driver_t *cdp);

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Export driver modules.*/
#include <crc_instances.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Initializes CRC instance.
 *
 * @param[out] cdp      pointer to a @p crc_driver_t structure
 *
 * @api
 */
void crc_init(crc_driver_t *cdp);

/**
 * @brief   Configures CRC DMA driver mode.
 * @note    This configuration becomes effective after crc_start.
 *
 * @param[out] cdp      pointer to a @p crc_driver_t structure
 * @param[in] drv_mode  driver mode configuration. It can be one of the
 *                      following values:
 *            @arg @ref CRC_DRV_MODE_SYNC
 *            @arg @ref CRC_DRV_MODE_ASYNC
 * @return              previous driver mode configuration. It can be one of
 *                      the following values:
 *            @arg @ref CRC_DRV_MODE_SYNC
 *            @arg @ref CRC_DRV_MODE_ASYNC
 *
 * @api
 */
crc_drv_mode_t crc_set_drv_mode(crc_driver_t *cdp, crc_drv_mode_t drv_mode);

/**
 * @brief   Sets the CRC initial value.
 * @note    This configuration becomes effective after crc_start.
 *
 * @param[in] cdp        pointer to a @p crc_driver_t structure
 * @param[in] init_value initial value to be configured
 * @return               previous initial value.
 *
 * @api
 */
uint32_t crc_set_init(crc_driver_t *cdp, uint32_t init_value);

/**
 * @brief   Sets the CRC reverse input data.
 * @note    This configuration becomes effective after crc_start.
 *
 * @param[in] cdp       pointer to a @p crc_driver_t structure
 * @param[in] rev_in    reverse input data to be configured. It can be one of
 *                      the following values:
 *            @arg @ref CRC_REV_IN_NONE
 *            @arg @ref CRC_REV_IN_BYTE
 *            @arg @ref CRC_REV_IN_HWORD
 *            @arg @ref CRC_REV_IN_WORD
 * @return              previous reverse input data value. It can be one of the
 *                      following values:
 *            @arg @ref CRC_REV_IN_NONE
 *            @arg @ref CRC_REV_IN_BYTE
 *            @arg @ref CRC_REV_IN_HWORD
 *            @arg @ref CRC_REV_IN_WORD
 *
 * @api
 */
crc_rev_in_t crc_set_rev_in(crc_driver_t *cdp, crc_rev_in_t rev_in);

/**
 * @brief   Sets the CRC reverse output data.
 * @note    This configuration becomes effective after crc_start.
 *
 * @param[in] cdp       pointer to a @p crc_driver_t structure
 * @param[in] rev_out   reverse output data to be configured. It can be one of
 *                      the following values:
 *            @arg @ref CRC_REV_OUT_NONE
 *            @arg @ref CRC_REV_OUT_BIT
 * @return              previous reverse output data value. It can be one of
 *                      the following values:
 *            @arg @ref CRC_REV_OUT_NONE
 *            @arg @ref CRC_REV_OUT_BIT
 *
 * @api
 */
crc_rev_out_t crc_set_rev_out(crc_driver_t *cdp, crc_rev_out_t rev_out);

/**
 * @brief   Sets the CRC polynomial size.
 * @note    This configuration becomes effective after crc_start.
 *
 * @param[in] cdp       pointer to a @p crc_driver_t structure
 * @param[in] polysize  polynomial size to be configured. It can be one of
 *                      the following values:
 *            @arg @ref CRC_POLYSIZE_32
 *            @arg @ref CRC_POLYSIZE_16
 *            @arg @ref CRC_POLYSIZE_8
 *            @arg @ref CRC_POLYSIZE_7
 * @return              previous polynomial size. It can be one of the
 *                      following values:
 *            @arg @ref CRC_POLYSIZE_32
 *            @arg @ref CRC_POLYSIZE_16
 *            @arg @ref CRC_POLYSIZE_8
 *            @arg @ref CRC_POLYSIZE_7
 *
 * @api
 */
crc_polysize_t crc_set_polysize(crc_driver_t *cdp, crc_polysize_t polysize);

/**
 * @brief   Enables/Disables DMA.
 * @note    This configuration becomes effective after crc_start.
 *
 * @param[out] cdp      pointer to a @p crc_driver_t structure
 * @param[in] enable    DMA mode enable status ("true" means the DMA is
 *                      enabled, "false" otherwise)
 * @return              previous DMA mode enable status.
 *
 * @api
 */
bool crc_enable_dma(crc_driver_t *cdp, bool enable);

/**
 * @brief   Configures CRC DMA configuration.
 * @note    This configuration becomes effective after crc_start.
 *
 * @param[out] cdp      pointer to a @p crc_driver_t structure
 * @param[in] dma_conf  pointer to DMA configuration to be configured
 *
 * @api
 */
void crc_set_dma_conf(crc_driver_t *cdp, crc_dma_conf_t *dma_conf);

/**
 * @brief   Configures pointers to CRC callback functions.
 *
 * @param[out] cdp      pointer to a @p crc_driver_t structure
 * 
 * @param[in] cb        pointer to CRC callback function
 * @return              pointer to previuos CRC callback function.
 *
 * @api
 */
crc_cb_t crc_set_cb(crc_driver_t *cdp, crc_cb_t cb);

/**
 * @brief   Starts CRC instance.
 * 
 * @param[in] cdp       pointer to a @p crc_driver_t structure
 *
 * @api
 */
void crc_start(crc_driver_t *cdp);

/**
 * @brief   Runs CRC calculation.
 *
 * @param[in] cdp       pointer to a @p crc_driver_t structure
 * @param[in] mode      operating mode of the CRC. It can be one of the
 *                      following values:
 *            @arg @ref CRC_MODE_CONCAT
 *            @arg @ref CRC_MODE_NEW
 * @param[in] buffer    pointer to input buffer containing the elements for
 *                      the CRC calculation
 * @param[in] buff_len  number of elements in the buffer
 *
 * @api
 */
void crc_calculate(crc_driver_t *cdp, crc_mode_t mode, uint8_t buffer[],
                   uint32_t buff_len);

/**
 * @brief   Returns the CRC calculation result.
 *
 * @param[in] udp       pointer to a @p crc_driver_t structure
 * @return              calculation result.
 *
 * @api
 */
uint32_t crc_get_result(crc_driver_t *cdp);

/**
 * @brief   Stops CRC instance.
 *
 * @param[in] cdp       pointer to a @p crc_driver_t structure
 *
 * @api
 */
void crc_stop(crc_driver_t *cdp);

/**
 * @brief   Deinitializes CRC instance.
 *
 * @param[out] cdp      pointer to a @p crc_driver_t structure
 *
 * @api
 */
void crc_deinit(crc_driver_t *cdp);

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _CRC_H_ */

/** @} */
