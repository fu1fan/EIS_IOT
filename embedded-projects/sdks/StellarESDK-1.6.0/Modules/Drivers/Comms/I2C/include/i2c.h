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
 * @file    i2c.h
 * @brief   I2C driver header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup COMMS
 * @ingroup DRIVERS
 * @addtogroup I2C
 * @ingroup COMMS
 * @{
 */

#ifndef _I2C_H_
#define _I2C_H_

#include <typedefs.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Maximum number of bytes for a single transfer
 * @{
 */
#define I2C_MAX_NBYTE_SIZE                  255U
/** @} */

/**
 * @name    I2C transfer modes
 * @{
 */
#define I2C_TRANSFER_MODE_RELOAD            I2C_CR2_RELOAD
#define I2C_TRANSFER_MODE_AUTOEND           I2C_CR2_AUTOEND
/** @} */

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
 * @brief   Type of an I2C driver.
 */
typedef struct i2c_driver i2c_driver_t;

/**
 * @enum i2c_mode_t
 * I2C supported modes.
 */
typedef enum {
    I2C_MODE_MASTER_TX,           /**< Master transmit mode */
    I2C_MODE_MASTER_RX,           /**< Master receive mode  */
    I2C_MODE_SLAVE_TX,            /**< Slave transmit mode  */
    I2C_MODE_SLAVE_RX             /**< Slave receive mode   */
} i2c_mode_t;

/**
 * @enum i2c_op_sts_t
 * I2C transfer progress status.
 */
typedef enum {
    I2C_OP_READY = 0U,            /**< Transfer operation status ready */
    I2C_OP_BUSY  = 1U,            /**< Transfer operation status busy  */
} i2c_op_sts_t;

/**
 * @enum i2c_drv_mode_t
 * I2C driver mode configuration.
 */
typedef enum {
    I2C_DRV_MODE_SYNCHRONOUS,     /**< Synchronous driver mode  */
    I2C_DRV_MODE_ASYNCHRONOUS,    /**< Asynchronous driver mode */
} i2c_drv_mode_t;

/**
 * @brief   Type of I2C DMA configuration.
 */
typedef struct {
    /**
     * @brief   I2C TX DMA stream to be used for operations.
     */
    uint32_t                 dma_stream_tx_id;
    /**
     * @brief   I2C RX DMA stream to be used for operations.
     */
    uint32_t                 dma_stream_rx_id;
    /**
     * @brief   I2C DMA stream bus priority (3..0).
     */
    uint32_t                 dma_stream_bus_prio;
    /**
     * @brief   I2C DMA stream IRQ priority (3..15).
     */
    uint32_t                 dma_stream_irq_prio;
} i2c_dma_conf_t;

/**
 * @enum i2c_oa1_mode_t
 * I2C supported own 1 address modes.
 */
typedef enum {
    I2C_OA1_MODE_7BIT,       /**< I2C own address 1 mode = 7 bit  */
    I2C_OA1_MODE_10BIT,      /**< I2C own address 1 mode = 10 bit */
} i2c_oa1_mode_t;

/**
 * @enum i2c_oa2_mask_t
 * I2C supported own address 2 masks.
 */
typedef enum {
    I2C_OA2_NOMASK,          /**< No mask                            */
    I2C_OA2_MASK_1,          /**< OA2[1] is masked and don’t care    */
    I2C_OA2_MASK_2_1,        /**< OA2[2:1] are masked and don’t care */
    I2C_OA2_MASK_3_1,        /**< OA2[3:1] are masked and don’t care */
    I2C_OA2_MASK_4_1,        /**< OA2[4:1] are masked and don’t care */
    I2C_OA2_MASK_5_1,        /**< OA2[5:1] are masked and don’t care */
    I2C_OA2_MASK_6_1,        /**< OA2[6:1] are masked and don’t care */
    I2C_OA2_MASK_7_1,        /**< OA2[7:1] are masked and don’t care */
} i2c_oa2_mask_t;

/**
 * @brief   Type of I2C callbacks.
 *
 * @param[in] idp       pointer to the @p i2c_driver_t object
 */
typedef void (*i2c_cb_t)(i2c_driver_t *idp);

/**
 * @enum i2c_cb_ops_t
 * I2C callback operation type.
 */
typedef enum {
    I2C_CB_OPS_DONE,         /**< Transfer completed event    */
    I2C_CB_OPS_ERR,          /**< Error event                 */
    I2C_CB_OPS_MAX           /**< Number of driver operations */
} i2c_cb_ops_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Export driver modules.*/
#include <i2c_instances.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Initializes I2C instance.
 *
 * @param[out] idp      pointer to a @p i2c_driver_t structure
 *
 * @api
 */
void i2c_init(i2c_driver_t *idp);

/**
 * @brief   Configures I2C interrupt priority.
 * @note    This configuration becomes effective after i2c_start.
 *
 * @param[out] idp      pointer to a @p i2c_driver_t structure
 * @param[in] prio      interrupt priority to be configured
 * @return              previous interrupt priority.
 *
 * @api
 */
uint32_t i2c_set_prio(i2c_driver_t *idp, uint32_t prio);

/**
 * @brief   Configures I2C driver mode (synchronous or asynchronous).
 * @note    This configuration becomes effective after i2c_start.
 *
 * @param[out] idp      pointer to a @p i2c_driver_t structure
 * @param[in] drv_mode  driver mode to be configured. It can be one of the
 *                      following values:
 *            @arg @ref I2C_DRV_MODE_SYNCHRONOUS
 *            @arg @ref I2C_DRV_MODE_ASYNCHRONOUS
 * @return              previous driver mode. It can be one of the following
 *                      values:
 *            @arg @ref I2C_DRV_MODE_SYNCHRONOUS
 *            @arg @ref I2C_DRV_MODE_ASYNCHRONOUS
 *
 * @api
 */
i2c_drv_mode_t i2c_set_drv_mode(i2c_driver_t *idp, i2c_drv_mode_t drv_mode);

/**
 * @brief   Enables/Disables DMA.
 * @note    This configuration becomes effective after i2c_start.
 *
 * @param[out] idp      pointer to a @p i2c_driver_t structure
 * @param[in] enable    DMA mode enable status ("true" means the DMA is
 *                      enabled, "false" otherwise)
 * @return              previous DMA mode enable status.
 *
 * @api
 */
bool i2c_enable_dma(i2c_driver_t *idp, bool enable);

/**
 * @brief   Configures I2C DMA.
 * @note    This configuration becomes effective after i2c_start.
 *
 * @param[out] idp      pointer to a @p i2c_driver_t structure
 * @param[in] dma_conf  pointer to DMA configuration to be configured
 *
 * @api
 */
void i2c_set_dma_conf(i2c_driver_t *idp, i2c_dma_conf_t *dma_conf);

/**
 * @brief   Configures the I2C timings.
 * @note    This configuration becomes effective after i2c_start.
 * @note    This API sets content of register TIMINGR, i.e. timing prescaler,
 *          data setup time, data hold time, SCL high period (master mode) and
 *          SCL low period (master mode).
 *
 * @param[out] idp      pointer to a @p i2c_driver_t structure
 * @param[in] timing    TIMINGR register value to be configured
 * @return              previuos TIMINGR register value.
 *
 * @api
 */
uint32_t i2c_set_timing(i2c_driver_t *idp, uint32_t timing);

/**
 * @brief   Configures the own address 1 mode (7-bit or 10-bit).
 * @note    This configuration becomes effective after i2c_start.
 *
 * @param[out] idp      pointer to a @p i2c_driver_t structure
 * @param[in] oa1_mode  own address 1 mode to be configured. It can be one of
 *                      the following values:
 *            @arg @ref I2C_OA1_MODE_7BIT,
 *            @arg @ref I2C_OA1_MODE_10BIT
 * @return              previuos own address 1 mode. It can be one of the
 *                      following values:
 *            @arg @ref I2C_OA1_MODE_7BIT,
 *            @arg @ref I2C_OA1_MODE_10BIT
 *
 * @api
 */
i2c_oa1_mode_t i2c_set_own_address1_mode(i2c_driver_t *idp,
                                         i2c_oa1_mode_t oa1_mode);

/**
 * @brief   Configures the own address 1.
 * @note    This configuration becomes effective after i2c_start.
 * @note    Own address 1 can be a 7-bit or 10-bit address based on the
 *          addressing mode set with i2c_set_addressing_mode.
 *
 * @param[out] idp      pointer to a @p i2c_driver_t structure
 * @param[in] oa1       own address 1 to be configured
 * @return              previous own address 1.
 *
 * @api
 */
uint16_t i2c_set_own_address1(i2c_driver_t *idp, uint16_t oa1);

/**
 * @brief   Enables/Disables own address 2.
 * @note    This configuration becomes effective after i2c_start.
 *
 * @param[out] idp      pointer to a @p i2c_driver_t structure
 * @param[in] enable    own address 2 eanable status
 * @return              previous own address 2 eanable status.
 *
 * @api
 */
bool i2c_enable_own_address2(i2c_driver_t *idp, bool enable);

/**
 * @brief   Configures the own address 2.
 * @note    This configuration becomes effective after i2c_start.
 * @note    Own address 2 is 7-bit.
 *
 * @param[out] idp      pointer to a @p i2c_driver_t structure
 * @param[in] oa2       own address 2 to be configured
 * @return              previous own address 2.
 *
 * @api
 */
uint32_t i2c_set_own_address2(i2c_driver_t *idp, uint32_t oa2);

/**
 * @brief   Configures the own address 2 mask if own address 2 is enabled.
 * @note    This configuration becomes effective after i2c_start.
 *
 * @param[out] idp      pointer to a @p i2c_driver_t structure
 * @param[in] oa2_mask  own address 2 mask to be configured. It can be one of
 *                      the following values:
 *            @arg @ref I2C_OA2_NOMASK
 *            @arg @ref I2C_OA2_MASK_1
 *            @arg @ref I2C_OA2_MASK_2_1
 *            @arg @ref I2C_OA2_MASK_3_1
 *            @arg @ref I2C_OA2_MASK_4_1
 *            @arg @ref I2C_OA2_MASK_5_1
 *            @arg @ref I2C_OA2_MASK_6_1
 *            @arg @ref I2C_OA2_MASK_7_1
 * @return              previos own address 2 mask. It can be one of the
 *                      following values:
 *            @arg @ref I2C_OA2_NOMASK
 *            @arg @ref I2C_OA2_MASK_1
 *            @arg @ref I2C_OA2_MASK_2_1
 *            @arg @ref I2C_OA2_MASK_3_1
 *            @arg @ref I2C_OA2_MASK_4_1
 *            @arg @ref I2C_OA2_MASK_5_1
 *            @arg @ref I2C_OA2_MASK_6_1
 *            @arg @ref I2C_OA2_MASK_7_1
 *
 * @api
 */
i2c_oa2_mask_t i2c_set_address2_mask(i2c_driver_t *idp, i2c_oa2_mask_t oa2_mask);

/**
 * @brief   Enables/Disables general call mode.
 * @note    This configuration becomes effective after i2c_start.
 *
 * @param[out] idp      pointer to a @p i2c_driver_t structure
 * @param[in] enable    general call enable status ("true" means the general
 *                      call mode is enabled, "false" otherwise)
 * @return              previos general call mode status.
 *
 * @api
 */
bool i2c_enable_general_call(i2c_driver_t *idp, bool enable);

/**
 * @brief   Enables/Disables clock no-stretching mode.
 * @note    This configuration becomes effective after i2c_start.
 *
 * @param[out] idp      pointer to a @p i2c_driver_t structure
 * @param[in] enable    no-stretch enable status ("true" means the no-stretch
 *                      mode is enabled, i.e. clock stretching is disabled,
 *                      "false" means the no-stretch mode is disabled, i.e.
 *                      clock stretching is enabled)
 * @return              previos no-stretch enable status.
 *
 * @api
 */
bool i2c_enable_no_stretch(i2c_driver_t *idp, bool enable);

/**
 * @brief   Configures pointers to I2C callback functions.
 *
 * @param[out] idp      pointer to a @p i2c_driver_t structure
 * @param[in] cb_op     callback operation type. It can be one of the following
 *                      values:
 *            @arg @ref I2C_CB_OPS_DONE
 *            @arg @ref I2C_CB_OPS_ERR
 *            @arg @ref I2C_CB_OPS_MAX
 * @param[in] cb        pointer to I2C callback function
 * @return              pointer to previuos I2C callback function.
 *
 * @api
 */
i2c_cb_t i2c_set_cb(i2c_driver_t *idp, i2c_cb_ops_t cb_op, i2c_cb_t cb);

/**
 * @brief   Starts an I2C instance.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 *
 * @api
 */
void i2c_start(i2c_driver_t *idp);

/**
 * @brief   Transmits in master mode an amount of data.
 * @note    
 *          
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @param[in] address   address of the slave to transmit to
 * @param[in] data      pointer to the buffer to be transmitted
 * @param[in] size      maximum number of bytes to be transmitted
 * @return              number of bytes transmitted.
 *
 * @api
 */
uint16_t i2c_master_transmit(i2c_driver_t *idp, uint16_t address,
                             uint8_t *data, uint16_t size);

/**
 * @brief   Receives in master mode an amount of data.
 * @note    
 *          
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @param[in] address   address of the slave to read from
 * @param[in] data      pointer to the buffer to be received
 * @param[in] size      maximum number of bytes to be received
 * @return              number of bytes received.
 *
 * @api
 */
uint16_t i2c_master_receive(i2c_driver_t *idp, uint16_t address, uint8_t *data,
                            uint16_t size);

/**
 * @brief   Transmits in slave mode an amount of data.
 * @note    
 *          
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @param[in] data      pointer to the buffer to be transmitted
 * @param[in] size      maximum number of bytes to be transmitted
 * @return              number of bytes transmitted.
 *
 * @api
 */
uint16_t i2c_slave_transmit(i2c_driver_t *idp, uint8_t *data, uint16_t size);

/**
 * @brief   Receive in slave mode an amount of data.
 * @note    
 *          
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @param[in] data      pointer to the buffer to be received
 * @param[in] size      maximum number of bytes to be received
 * @return              number of bytes received.
 *
 * @api
 */
uint16_t i2c_slave_receive(i2c_driver_t *idp, uint8_t *data, uint16_t size);

/**
 * @brief   Stops an I2C instance.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 *
 * @api
 */
void i2c_stop(i2c_driver_t *idp);

/**
 * @brief   Deinitializes I2C instance.
 *
 * @param[out] idp      pointer to a @p i2c_driver_t structure
 *
 * @api
 */
void i2c_deinit(i2c_driver_t *idp);

/**
 * @brief   Shared event interrupt handler.
 * @note    Not an API, used internally.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 *
 * @isr
 */
void __i2c_serve_ev_interrupt(i2c_driver_t *idp);

/**
 * @brief   Shared error interrupt handler.
 * @note    Not an API, used internally.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 *
 * @isr
 */
void __i2c_serve_er_interrupt(i2c_driver_t *idp);

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _I2C_H_ */

/** @} */
