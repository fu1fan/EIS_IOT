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
 * @file    lin.h
 * @brief   LIN driver header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup LIN
 * @ingroup SYSTEM
 * @{
 */

#ifndef _LIN_H_
#define _LIN_H_

#include <typedefs.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    LIN sync byte value.
 * @{
 */
#define LIN_SYNC_BYTE                       0x55U
/** @} */

/**
 * @name    LIN maximum number of data bytes.
 * @{
 */
#define LIN_MAX_DATA_BYTE_NUM               8U
/** @} */

/**
 * @name    LIN maximum ID.
 * @{
 */
#define LIN_MAX_ID                          63U
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
 * @brief   Type of an LIN driver.
 */
typedef struct lin_driver lin_driver_t;

/**
 * @enum lin_mode_t
 * LIN mode configuration.
 */
typedef enum {
    LIN_MODE_MASTER,    /**< LIN Master mode */
    LIN_MODE_SLAVE,     /**< LIN Slave mode  */
} lin_mode_t;

/**
 * @enum lin_drv_mode_t
 * LIN driver mode configuration.
 */
typedef enum {
    LIN_DRV_MODE_SYNC,       /**< Synchronous mode  */
    LIN_DRV_MODE_ASYNC,      /**< Asynchronous mode */
} lin_drv_mode_t;

/**
 * @enum lin_tx_sts_t
 * LIN TX supported states.
 */
typedef enum {
    LIN_TX_STATUS_IDLE,      /**< Master/Slave idle (Master ready to transmit Break or data, Slave ready to transmit data) */
    LIN_TX_STATUS_SYNC,      /**< Master ready to transmit Sync                                                            */
    LIN_TX_STATUS_ID,        /**< Master ready to transmit ID                                                              */
    LIN_TX_STATUS_DATA,      /**< Master/Slave ready to transmit data                                                      */
    LIN_TX_STATUS_CHECKSUM   /**< Master/Slave ready to transmit checksum                                                  */
} lin_tx_sts_t;

/**
 * @enum lin_rx_sts_t
 * LIN RX supported states.
 */
typedef enum {
    LIN_RX_STATUS_IDLE,      /**< Master/Slave idle (Master ready to receive data, Slave ready to receive Break or data) */
    LIN_RX_STATUS_SYNC,      /**< Slave ready to receive Sync                                                            */
    LIN_RX_STATUS_ID,        /**< Slave ready to receive ID                                                              */
    LIN_RX_STATUS_DATA,      /**< Master/Slave ready to receive data                                                     */
    LIN_RX_STATUS_CHECKSUM   /**< Master/Slave ready to receive checksum                                                 */
} lin_rx_sts_t;

/**
 * @enum lin_baud_t
 * LIN baudrate configuration (LIN maximum baudrate = 20Kbit/s).
 */
typedef enum {
    LIN_BAUDRATE_110    = 110U,        /**<    110 bits per second */
    LIN_BAUDRATE_300    = 300U,        /**<    300 bits per second */
    LIN_BAUDRATE_600    = 600U,        /**<    600 bits per second */
    LIN_BAUDRATE_1200   = 1200U,       /**<   1200 bits per second */
    LIN_BAUDRATE_2400   = 2400U,       /**<   2400 bits per second */
    LIN_BAUDRATE_4800   = 4800U,       /**<   4800 bits per second */
    LIN_BAUDRATE_9600   = 9600U,       /**<   9600 bits per second */
    LIN_BAUDRATE_14400  = 14400U,      /**<  14400 bits per second */
    LIN_BAUDRATE_19200  = 19200U       /**<  19200 bits per second */
} lin_baud_t;

/**
 * @enum lin_presc_t
 * LIN prescaler configuration.
 */
typedef enum {
    LIN_PRESCALER_DIV1   = 0U,         /**< input clock not divided    */
    LIN_PRESCALER_DIV2   = 1U,         /**< input clock divided by 2   */
    LIN_PRESCALER_DIV4   = 2U,         /**< input clock divided by 4   */
    LIN_PRESCALER_DIV6   = 3U,         /**< input clock divided by 6   */
    LIN_PRESCALER_DIV8   = 4U,         /**< input clock divided by 8   */
    LIN_PRESCALER_DIV10  = 5U,         /**< input clock divided by 10  */
    LIN_PRESCALER_DIV12  = 6U,         /**< input clock divided by 12  */
    LIN_PRESCALER_DIV16  = 7U,         /**< input clock divided by 16  */
    LIN_PRESCALER_DIV32  = 8U,         /**< input clock divided by 32  */
    LIN_PRESCALER_DIV64  = 9U,         /**< input clock divided by 64  */
    LIN_PRESCALER_DIV128 = 10U,        /**< input clock divided by 128 */
    LIN_PRESCALER_DIV256 = 11U,        /**< input clock divided by 256 */
} lin_presc_t;

/**
 * @enum lin_bdl_t
 * LIN supported break detection length.
 */
typedef enum {
    LIN_BREAK_DETECT_LEN_10BIT,   /**< 10-bit break detection length */
    LIN_BREAK_DETECT_LEN_11BIT    /**< 11-bit break detection length */
} lin_bdl_t;

/**
 * @brief   Type of LIN callbacks.
 *
 * @param[in] ldp       pointer to the @p lin_driver_t object
 */
typedef void (*lin_cb_t)(lin_driver_t *ldp);

/**
 * @enum lin_cb_ops_t
 * LIN callback operation type.
 */
typedef enum {
    LIN_CB_OPS_TX_PID,  /**< Master protected ID transmission completed callback */
    LIN_CB_OPS_RX_PID,  /**< Slave protected ID reception completed callback     */
    LIN_CB_OPS_ERR_PID, /**< Slave protected ID reception error callback         */
    LIN_CB_OPS_TX,      /**< Master/Slave transmission completed callback        */
    LIN_CB_OPS_RX,      /**< Master/Slave reception completed callback           */
    LIN_CB_OPS_MAX      /**< Number of driver operations                         */
} lin_cb_ops_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns ID from the received protected ID (ID + parity bits).
 *
 * @param[in] \_pid\_   protected id from which to get the ID
 *
 * @notapi
 */
#define LIN_GET_ID(_pid_)                                           \
    (0x3FU & (_pid_))

/**
 * @brief   Returns parity bits from the received protected ID (ID + parity
 *          bits).
 *
 * @param[in] \_pid\_   protected id from which to get the parity bits
 *
 * @notapi
 */
#define LIN_GET_PARITY_BITS(_pid_)                                  \
    (0x3U & ((_pid_) >> 6U))

/**
 * @brief   Sets protected ID (ID + parity bits).
 *
 * @param[in] \_pbits\_  parity bits to includes in the protected ID
 * @param[in] \_id\_     ID to includes in the protected ID
 *
 * @notapi
 */
#define LIN_SET_PID(_pbits_, _id_)                                  \
    (((0x3U & (_pbits_)) << 6U) | (0x3FU & (_id_)))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Export driver modules.*/
#include <lin_instances.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Initializes LIN instance.
 *
 * @param[out] ldp      pointer to a @p lin_driver_t structure
 *
 * @api
 */
void lin_init(lin_driver_t *ldp);

/**
 * @brief   Configures LIN mode (master or slave).
 * @note    This configuration becomes effective after lin_start.
 *
 * @param[out] ldp      pointer to a @p lin_driver_t structure
 * @param[in] mode      LIN mode to be configured. It can be one of the
 *                      following values:
 *            @arg @ref LIN_MODE_MASTER
 *            @arg @ref LIN_MODE_SLAVE
 * @return              previous LIN mode. It can be one of the following
 *                      values:
 *            @arg @ref LIN_MODE_MASTER
 *            @arg @ref LIN_MODE_SLAVE
 *
 * @api
 */
lin_mode_t lin_set_mode(lin_driver_t *ldp, lin_mode_t mode);

/**
 * @brief   Configures LIN driver mode (asynchronous or synchronous).
 * @note    This configuration becomes effective after lin_start.
 *
 * @param[out] ldp      pointer to a @p lin_driver_t structure
 * @param[in] drv_mode  LIN driver mode to be configured. It can be one of the
 *                      following values:
 *            @arg @ref LIN_DRV_MODE_SYNC
 *            @arg @ref LIN_DRV_MODE_ASYNC
 * @return              previous LIN driver mode. It can be one of the
 *                      following values:
 *            @arg @ref LIN_DRV_MODE_SYNC
 *            @arg @ref LIN_DRV_MODE_ASYNC
 *
 * @api
 */
lin_drv_mode_t lin_set_drv_mode(lin_driver_t *ldp, lin_drv_mode_t drv_mode);

/**
 * @brief   Configures LIN interrupt priority.
 * @note    This configuration becomes effective after lin_start.
 *
 * @param[out] ldp      pointer to a @p lin_driver_t structure
 * @param[in] prio      interrupt priority to be configured
 * @return              previous interrupt priority.
 *
 * @api
 */
uint32_t lin_set_prio(lin_driver_t *ldp, uint32_t prio);

/**
 * @brief   Configures LIN baudrate.
 * @note    This configuration becomes effective after lin_start.
 *
 * @param[out] ldp      pointer to a @p lin_driver_t structure
 * @param[in] baud      baudrate to be configured. It can be one of the
 *                      following values:
 *            @arg @ref LIN_BAUDRATE_110
 *            @arg @ref LIN_BAUDRATE_300
 *            @arg @ref LIN_BAUDRATE_600
 *            @arg @ref LIN_BAUDRATE_1200
 *            @arg @ref LIN_BAUDRATE_2400
 *            @arg @ref LIN_BAUDRATE_4800
 *            @arg @ref LIN_BAUDRATE_9600
 *            @arg @ref LIN_BAUDRATE_14400
 *            @arg @ref LIN_BAUDRATE_19200
 * @return              previous baudrate. It can be one of the following
 *                      values:
 *            @arg @ref LIN_BAUDRATE_110
 *            @arg @ref LIN_BAUDRATE_300
 *            @arg @ref LIN_BAUDRATE_600
 *            @arg @ref LIN_BAUDRATE_1200
 *            @arg @ref LIN_BAUDRATE_2400
 *            @arg @ref LIN_BAUDRATE_4800
 *            @arg @ref LIN_BAUDRATE_9600
 *            @arg @ref LIN_BAUDRATE_14400
 *            @arg @ref LIN_BAUDRATE_19200
 *
 * @api
 */
lin_baud_t lin_set_baud(lin_driver_t *ldp, lin_baud_t baud);

/**
 * @brief   Configures LIN prescaler.
 * @note    This configuration becomes effective after lin_start.
 *
 * @param[out] ldp      pointer to a @p lin_driver_t structure
 * @param[in] presc     prescaler to be configured. It can be one of the
 *                      following values:
 *            @arg @ref LIN_PRESCALER_DIV1
 *            @arg @ref LIN_PRESCALER_DIV2
 *            @arg @ref LIN_PRESCALER_DIV4
 *            @arg @ref LIN_PRESCALER_DIV6
 *            @arg @ref LIN_PRESCALER_DIV8
 *            @arg @ref LIN_PRESCALER_DIV10
 *            @arg @ref LIN_PRESCALER_DIV12
 *            @arg @ref LIN_PRESCALER_DIV16
 *            @arg @ref LIN_PRESCALER_DIV32
 *            @arg @ref LIN_PRESCALER_DIV64
 *            @arg @ref LIN_PRESCALER_DIV128
 *            @arg @ref LIN_PRESCALER_DIV256
 * @return              previous prescaler. It can be one of the following
 *                      values:
 *            @arg @ref LIN_PRESCALER_DIV1
 *            @arg @ref LIN_PRESCALER_DIV2
 *            @arg @ref LIN_PRESCALER_DIV4
 *            @arg @ref LIN_PRESCALER_DIV6
 *            @arg @ref LIN_PRESCALER_DIV8
 *            @arg @ref LIN_PRESCALER_DIV10
 *            @arg @ref LIN_PRESCALER_DIV12
 *            @arg @ref LIN_PRESCALER_DIV16
 *            @arg @ref LIN_PRESCALER_DIV32
 *            @arg @ref LIN_PRESCALER_DIV64
 *            @arg @ref LIN_PRESCALER_DIV128
 *            @arg @ref LIN_PRESCALER_DIV256
 *
 * @api
 */
lin_presc_t lin_set_presc(lin_driver_t *ldp, lin_presc_t presc);

/**
 * @brief   Configures LIN break detection length.
 * @note    This configuration becomes effective after lin_start.
 *
 * @param[out] ldp      pointer to a @p lin_driver_t structure
 * @param[in] bdl       break detection length to be configured. It can be one
 *                      of the following values:
 *            @arg @ref LIN_BREAK_DETECT_LEN_10BIT
 *            @arg @ref LIN_BREAK_DETECT_LEN_11BIT
 * @return              previous break detection length. It can be one of the
 *                      following values:
 *            @arg @ref LIN_BREAK_DETECT_LEN_10BIT
 *            @arg @ref LIN_BREAK_DETECT_LEN_11BIT
 *
 * @api
 */
lin_bdl_t lin_set_bdl(lin_driver_t *ldp, lin_bdl_t bdl);

/**
 * @brief   Configures pointers to LIN callback functions.
 *
 * @param[out] ldp      pointer to a @p lin_driver_t structure
 * @param[in] cb_op     callback operation type. It can be one of the following
 *                      values:
 *            @arg @ref LIN_CB_OPS_TX_PID
 *            @arg @ref LIN_CB_OPS_RX_PID
 *            @arg @ref LIN_CB_OPS_ERR_PID
 *            @arg @ref LIN_CB_OPS_TX
 *            @arg @ref LIN_CB_OPS_RX
 * @param[in] cb        pointer to LIN callback function
 * @return              pointer to previuos LIN callback function.
 *
 * @api
 */
lin_cb_t lin_set_cb(lin_driver_t *ldp, lin_cb_ops_t cb_op, lin_cb_t cb);

/**
 * @brief   Starts an LIN instance.
 *
 * @param[in] ldp       pointer to a @p lin_driver_t structure
 *
 * @api
 */
void lin_start(lin_driver_t *ldp);

/**
 * @brief   Sends LIN ID.
 * @note    Only LIN master can invoke this API.
 *
 * @param[in] ldp       pointer to a @p lin_driver_t structure
 * @param[in] id        ID to be sent [0:63]
 *
 * @api
 */
void lin_write_id(lin_driver_t *ldp, uint8_t id);

/**
 * @brief   Receives LIN protected ID (ID + parity bits).
 * @note    Only LIN slave can invoke this API.
 *
 * @param[in] ldp       pointer to a @p lin_driver_t structure
 * @param[out] pid      protected ID to be received
 *
 * @api
 */
void lin_read_id(lin_driver_t *ldp, uint8_t *pid);

/**
 * @brief   Writes data via LIN.
 * @note    The number of bytes to send must be in the range [1:8].
 *
 * @param[in] ldp       pointer to a @p lin_driver_t structure
 * @param[in] data      pointer to the buffer to be written
 * @param[in] len       number of bytes to be written [1:8]
 * @return              The number of bytes copied from the buffer.
 *
 * @api
 */
size_t lin_write(lin_driver_t *ldp, uint8_t *data, size_t len);

/**
 * @brief   Reads data and checksum via LIN.
 * @note    The number of bytes to read must be in the range [1:8].
 *
 * @param[in] ldp       pointer to a @p lin_driver_t structure
 * @param[out] data     pointer to the buffer to be read
 * @param[out] checksum pointer to the checksum to be read
 * @param[in] len       number of bytes to be read [1:8]
 * @return              The number of bytes copied into the buffer.
 *
 * @api
 */
size_t lin_read(lin_driver_t *ldp, uint8_t *data, uint8_t *checksum, size_t len);

/**
 * @brief   Check the checksum.
 * @note    Only LIN slave can invoke this API.
 *
 * @param[in] data      pointer to the buffer received by the slave on which to
 *                      calculate the checksum
 * @param[in] len       number of bytes [1:8] received by the slave on which to
 *                      calculate the checksum
 * @param[in] checksum  checksum received to the slave
 * @return              check status ('true' if the checksum received by the
 *                      slave and the checksum calculate on data are equals,
 *                      'false' otherwise).
 *
 * @api
 */
bool lin_check_checksum(uint8_t *data, size_t len, uint8_t checksum);

/**
 * @brief   Check the parity.
 * @note    Only LIN slave can invoke this API.
 * @note    This function receives the pid, i.e. the protected ID (ID + parity
 *          bits) and check if the parity bits contained in the pid are equal
 *          to the parity bits calculated from the id contained in the pid.
 *
 * @param[in] pid       protected ID (ID + parity) on which to check the parity
 * @return              check status ('true' if the parity bits received by the
 *                      slave in the pid and the parity bits calculate from the
 *                      received id are equals, 'false' otherwise).
 *
 * @api
 */
bool lin_check_parity(uint8_t pid);

/**
 * @brief   Stops a LIN instance.
 *
 * @param[in] ldp       pointer to a @p lin_driver_t structure
 *
 * @api
 */
void lin_stop(lin_driver_t *ldp);

/**
 * @brief   Deinitializes LIN instance.
 *
 * @param[out] ldp      pointer to a @p lin_driver_t structure
 *
 * @api
 */
void lin_deinit(lin_driver_t *ldp);

/**
 * @brief   Shared interrupt handler.
 * @note    Not an API, used internally.
 *
 * @param[in] ldp       pointer to a @p lin_driver_t structure
 *
 * @isr
 */
void __lin_serve_interrupt(lin_driver_t *ldp);

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _LIN_H_ */

/** @} */
