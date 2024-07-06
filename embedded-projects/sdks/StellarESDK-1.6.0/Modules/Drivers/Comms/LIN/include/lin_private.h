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
 * @file    lin_private.h
 * @brief   LIN driver private header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup LIN
 * @ingroup SYSTEM
 * @{
 */

#ifndef _LIN_PRIVATE_H_
#define _LIN_PRIVATE_H_

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
 * @brief   LIN driver structure.
 * @note    This structure is internal to the LIN driver.
 */
struct lin_driver {
    /**
     * @brief   Pointer to LIN registers block.
     */
    UART_TypeDef             *lin;
    /**
     * @brief   LIN Clock value.
     */
    uint32_t                 clock;
    /**
     * @brief   Driver mode (master or slave).
     */
    lin_mode_t               mode;
    /**
     * @brief   Driver mode (synchronous or asynchronous).
     */
    lin_drv_mode_t           drv_mode;
    /**
     * @brief   Protected ID (ID + parity bits) to be trasmitted in master
     *          mode.
     */
    uint8_t                  tx_pid;
    /**
     * @brief   Protected ID (ID + parity bits) transmission in progress flag.
     */
    bool                     tx_pid_busy;
    /**
     * @brief   Checksum to be trasmitted in master mode.
     */
    uint8_t                  tx_cks;
    /**
     * @brief   Transmission status.
     */
    lin_tx_sts_t             tx_status;
    /**
     * @brief   Pointer to protected ID (ID + parity bits) received in slave
     *          mode.
     */
    uint8_t                  *rx_pid;
    /**
     * @brief   Protected ID (ID + parity bits) reception in progress flag.
     */
    bool                     rx_pid_busy;
    /**
     * @brief   Pointer to checksum received in slave mode.
     */
    uint8_t                  *rx_cks;
    /**
     * @brief   Reception status.
     */
    lin_rx_sts_t             rx_status;
    /**
     * @brief   Pointer to transmission buffer.
     */
    uint8_t                  *tx_buf;
    /**
     * @brief   Transmission buffer length.
     */
    uint32_t                 tx_len;
    /**
     * @brief   Transmission in progress flag.
     */
    bool                     tx_busy;
    /**
     * @brief   Pointer to receiving buffer.
     */
    uint8_t                  *rx_buf;
    /**
     * @brief   Receiving buffer length.
     */
    uint32_t                 rx_len;
    /**
     * @brief  Receiving in progress flag.
     */
    bool                     rx_busy;
    /**
     * @brief   LIN interrupt priority.
     */
    uint32_t                 prio;
    /**
     * @brief   LIN baud rate.
     */
    lin_baud_t               baud;
    /**
     * @brief   LIN clock prescaler.
     */
    lin_presc_t              presc;
    /**
     * @brief   LIN break detection length.
     */
    lin_bdl_t                bdl;
    /**
     * @brief   Pointers to LIN callback functions.
     * @note    Set to @p NULL if not required.
     */
    lin_cb_t                 cb[LIN_CB_OPS_MAX];
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

IRQ_HANDLER(IRQ_UART1_HANDLER);
IRQ_HANDLER(IRQ_UART2_HANDLER);
IRQ_HANDLER(IRQ_UART3_HANDLER);

UART_TypeDef *lin_dev_get_reg_ptr(lin_driver_t *ldp);
uint32_t lin_dev_get_clock(lin_driver_t *ldp);
void lin_dev_clock_enable(lin_driver_t *ldp);
void lin_dev_clock_disable(lin_driver_t *ldp);
IRQn_Type lin_dev_get_vector(lin_driver_t *ldp);

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _LIN_PRIVATE_H_ */

/** @} */

