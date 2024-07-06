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
 * @file    lin.c
 * @brief   LIN driver source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup LIN
 * @ingroup SYSTEM
 * @{
 */

#include <lin.h>
#include <lin_private.h>

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
/* Local function prototypes                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Enters in transmit mode.
 * @note    Called when LIN driver enters in transmit mode.
 *
 * @param[in] \_ldp\_   pointer to a @p lin_driver_t structure
 *
 * @notapi
 */
#define LIN_TX_IN_PROGRESS(_ldp_)                                   \
    (_ldp_)->tx_busy = true;

/**
 * @brief  Exits from transmit mode.
 * @note   Called when LIN driver exits from transmit mode.
 *
 * @param[in] \_ldp\_   pointer to a @p lin_driver_t structure
 *
 * @notapi
 */
#define LIN_TX_DONE(_ldp_)                                          \
    (_ldp_)->tx_busy = false;

/**
 * @brief  Waits for transmit end.
 * @note   Called to wait for end of transmit.
 *
 * @param[in] \_ldp\_   pointer to a @p lin_driver_t structure
  *
 * @notapi
 */
#define LIN_WAIT_FOR_TX_COMPLETION(_ldp_)                           \
    if ((_ldp_)->drv_mode == LIN_DRV_MODE_SYNC) {                   \
        while ((_ldp_)->tx_busy == true) {;}                        \
    }

/**
 * @brief   Enters in receiving mode.
 * @note    Called when LIN driver enters receiving mode.
 *
 * @param[in] \_ldp\_   pointer to a @p lin_driver_t structure
 *
 * @notapi
 */
#define LIN_RX_IN_PROGRESS(_ldp_)                                   \
    (_ldp_)->rx_busy = true;

/**
 * @brief   Exits from receiving mode.
 * @note    Called when LIN driver exits from receiving mode.
 *
 * @param[in] \_ldp\_   pointer to a @p lin_driver_t structure
 *
 * @notapi
 */
#define LIN_RX_DONE(_ldp_)                                          \
    (_ldp_)->rx_busy = false;

/**
 * @brief   Waits for receive end.
 * @note    Called to wait for end of receiving mode.
 *
 * @param[in] \_ldp\_   pointer to a @p lin_driver_t structure
 *
 * @notapi
 */
#define LIN_WAIT_FOR_RX_COMPLETION(_ldp_)                           \
    if ((_ldp_)->drv_mode == LIN_DRV_MODE_SYNC) {                   \
        while ((_ldp_)->rx_busy == true) {;}                        \
    }

/**
 * @brief   Enters in protected ID transmit mode.
 * @note    Called when LIN driver enters in protected ID transmit mode.
 *
 * @param[in] \_ldp\_   pointer to a @p lin_driver_t structure
 *
 * @notapi
 */
#define LIN_TX_PID_IN_PROGRESS(_ldp_)                               \
    (_ldp_)->tx_pid_busy = true;

/**
 * @brief  Exits from protected ID transmit mode.
 * @note   Called when LIN driver exits from protected ID transmit mode.
 *
 * @param[in] \_ldp\_   pointer to a @p lin_driver_t structure
 *
 * @notapi
 */
#define LIN_TX_PID_DONE(_ldp_)                                      \
    (_ldp_)->tx_pid_busy = false;

/**
 * @brief  Waits for protected ID transmit end.
 * @note   Called to wait for end of protected ID transmit.
 *
 * @param[in] \_ldp\_   pointer to a @p lin_driver_t structure
  *
 * @notapi
 */
#define LIN_WAIT_FOR_TX_PID_COMPLETION(_ldp_)                       \
    if ((_ldp_)->drv_mode == LIN_DRV_MODE_SYNC) {                   \
        while ((_ldp_)->tx_pid_busy == true) {;}                    \
    }

/**
 * @brief   Enters in protected ID receive mode.
 * @note    Called when LIN driver enters in protected ID receive mode.
 *
 * @param[in] \_ldp\_   pointer to a @p lin_driver_t structure
 *
 * @notapi
 */
#define LIN_RX_PID_IN_PROGRESS(_ldp_)                               \
    (_ldp_)->rx_pid_busy = true;

/**
 * @brief  Exits from protected ID receive mode.
 * @note   Called when LIN driver exits from protected ID receive mode.
 *
 * @param[in] \_ldp\_   pointer to a @p lin_driver_t structure
 *
 * @notapi
 */
#define LIN_RX_PID_DONE(_ldp_)                                      \
    (_ldp_)->rx_pid_busy = false;

/**
 * @brief  Waits for protected ID receive end.
 * @note   Called to wait for end of protected ID receive.
 *
 * @param[in] \_ldp\_   pointer to a @p lin_driver_t structure
  *
 * @notapi
 */
#define LIN_WAIT_FOR_RX_PID_COMPLETION(_ldp_)                       \
    if ((_ldp_)->drv_mode == LIN_DRV_MODE_SYNC) {                   \
        while ((_ldp_)->rx_pid_busy == true) {;}                    \
    }

/**
 * @brief   Resets LIN parameters.
 *
 * @param[out] ldp      pointer to an @p lin_driver_t structure
 *
 * @notapi
 */
static void lin_lld_reset_paramters(lin_driver_t *ldp) {

    uint8_t i;

    ldp->lin         = NULL;
    ldp->clock       = 0U;
    ldp->mode        = LIN_MODE_MASTER;
    ldp->drv_mode    = LIN_DRV_MODE_SYNC;
    ldp->tx_pid      = 0U;
    ldp->tx_pid_busy = false;
    ldp->tx_cks      = 0U;
    ldp->tx_status   = LIN_TX_STATUS_IDLE;
    ldp->rx_pid      = NULL;
    ldp->rx_pid_busy = false;
    ldp->rx_cks      = NULL;
    ldp->rx_status   = LIN_RX_STATUS_IDLE;
    ldp->tx_buf      = NULL;
    ldp->tx_len      = 0U;
    ldp->tx_busy     = false;
    ldp->rx_buf      = NULL;
    ldp->rx_len      = 0U;
    ldp->rx_busy     = false;
    ldp->prio        = IRQ_MIN_PRIORITY;
    ldp->baud        = LIN_BAUDRATE_9600;
    ldp->presc       = LIN_PRESCALER_DIV1;
    ldp->bdl         = LIN_BREAK_DETECT_LEN_10BIT;

    for (i = 0U; i < (uint8_t)LIN_CB_OPS_MAX; i++) {
        ldp->cb[i] = NULL;
    }
}

/**
 * @brief   Returns the checksum (classic checksum based only on data)
 *
 * @param[in] data      pointer to data on which to calculate the checksum
 * @param[in] len       number of bytes on which to calculate the checksum
 * @return              checksum.
 *
 * @notapi
 */
static uint8_t lin_lld_get_checksum(uint8_t *data, size_t len) {

    uint16_t checksum = 0U;
    uint8_t newdata;
    uint8_t  i;

    for (i = 0U; i < len; i++) {
        newdata = data[i]; /* for MISRA check */
        checksum += (uint16_t)newdata;
        if (checksum > 256U) {
            checksum -= 255U;
        }
    }

    return (uint8_t)(255U - checksum);
}

/**
 * @brief   Returns the parity value related to the id
 *
 * @param[in] id        id for which to calculate the parity
 * @return              parity value.
 *
 * @notapi
 */
static uint8_t lin_lld_get_parity(uint8_t id) {

    uint8_t id_bit[6];
    uint8_t p[2];
    uint8_t i;

    /* Get id bits.*/
    for (i = 0U; i < 6U; i++) {
        id_bit[i] = 0x1U & ((id & (1U << i)) >> i);
    }

    /* Calculate parity bits.*/
    p[0] = (id_bit[0] ^ id_bit[1] ^ id_bit[2] ^ id_bit[4]);
    p[1] = (id_bit[1] ^ id_bit[3] ^ id_bit[4] ^ id_bit[5]);

    return (uint8_t)((p[1] << 1U) | p[0]);
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void lin_init(lin_driver_t *ldp) {

    /* Reset LIN parameters.*/
    lin_lld_reset_paramters(ldp);

    /* Set pointer to LIN register block.*/
    ldp->lin = lin_dev_get_reg_ptr(ldp);

    /* Set LIN clock value based on clocktree configuration.*/
    ldp->clock = lin_dev_get_clock(ldp);
}

lin_mode_t lin_set_mode(lin_driver_t *ldp, lin_mode_t mode) {

    lin_mode_t prev_mode;

    prev_mode = ldp->mode;
    ldp->mode = mode;

    return prev_mode;
}

lin_drv_mode_t lin_set_drv_mode(lin_driver_t *ldp, lin_drv_mode_t drv_mode) {

    lin_drv_mode_t prev_drv_mode;

    prev_drv_mode = ldp->drv_mode;
    ldp->drv_mode = drv_mode;

    return prev_drv_mode;
}

uint32_t lin_set_prio(lin_driver_t *ldp, uint32_t prio) {

    uint32_t prev_prio;

    prev_prio = ldp->prio;
    ldp->prio = prio;

    return prev_prio;
}

lin_baud_t lin_set_baud(lin_driver_t *ldp, lin_baud_t baud) {

    lin_baud_t prev_baud;

    prev_baud = ldp->baud;
    ldp->baud = baud;

    return prev_baud;
}

lin_presc_t lin_set_presc(lin_driver_t *ldp, lin_presc_t presc) {

    lin_presc_t prev_presc;

    prev_presc = ldp->presc;
    ldp->presc = presc;

    return prev_presc;
}

lin_bdl_t lin_set_bdl(lin_driver_t *ldp, lin_bdl_t bdl) {

    lin_bdl_t prev_bdl;

    prev_bdl = ldp->bdl;
    ldp->bdl = bdl;

    return prev_bdl;
}

lin_cb_t lin_set_cb(lin_driver_t *ldp, lin_cb_ops_t cb_op, lin_cb_t cb) {

    lin_cb_t prev_cb;

    prev_cb = ldp->cb[cb_op];
    ldp->cb[cb_op] = cb;

    return prev_cb;
}

void lin_start(lin_driver_t *ldp) {

    uint32_t presc, brr;

    IRQn_Type vector;

    /* Enable clock.*/
    lin_dev_clock_enable(ldp);

    /* BRR calculation.*/
    static const uint32_t prescvals[] = {1, 2, 4, 6, 8, 10, 12, 16, 32, 64, 128, 256};
    presc = prescvals[ldp->presc];
    brr = (uint32_t)((ldp->clock / presc) / ((uint32_t)ldp->baud));

    /* Setting up and starting LIN.*/
    ldp->lin->BRR = brr;
    ldp->lin->PRESC = (uint32_t)ldp->presc;

    /* Configure LIN interrupt.*/
    vector = lin_dev_get_vector(ldp);
    irq_set_priority(vector, ldp->prio);
    irq_enable(vector);

    /* Configure LIN break detection lenght.*/
    if (ldp->bdl == LIN_BREAK_DETECT_LEN_11BIT) {
        ldp->lin->CR2 |= UART_CR2_LBDL;
    }

    /* Enable LIN mode.*/
    ldp->lin->CR2 |= UART_CR2_LINEN;

    /* Setting up and starting LIN.*/
    ldp->lin->CR1 = UART_CR1_UE | UART_CR1_TE | UART_CR1_RE; /* Enable bits last.*/
    ldp->lin->ICR = ldp->lin->ISR;
}

void lin_write_id(lin_driver_t *ldp, uint8_t id) {

    volatile uint32_t isr;

    if (ldp->mode == LIN_MODE_MASTER && id <= LIN_MAX_ID) {

        /* Add parity bits to id.*/
        ldp->tx_pid = LIN_SET_PID(lin_lld_get_parity(id), id);

        /* Send break request.*/
        ldp->lin->RQR |= UART_RQR_SBKRQ;

        do {
            isr = ldp->lin->ISR;
        } while ((isr & UART_ISR_SBKF) != 0U);

        LIN_TX_PID_IN_PROGRESS(ldp);

        /* Start sync byte transmission.*/
        ldp->tx_status = LIN_TX_STATUS_SYNC;

        ldp->lin->TDR = (uint32_t)(LIN_SYNC_BYTE);

        /* Enable transmission complete interrupt.*/
        ldp->lin->CR1 |= UART_CR1_TCIE;

        /* Wait for id send completion.*/
        LIN_WAIT_FOR_TX_PID_COMPLETION(ldp);
    }
}

void lin_read_id(lin_driver_t *ldp, uint8_t *pid) {

    if (ldp->mode == LIN_MODE_SLAVE) {
        LIN_RX_PID_IN_PROGRESS(ldp);

        ldp->rx_pid = pid;

        /* Enable LIN break detection interrupt.*/
        ldp->lin->CR2 |= UART_CR2_LBDIE;

        /* Wait for id receive completion.*/
        LIN_WAIT_FOR_RX_PID_COMPLETION(ldp);
    }
}

size_t lin_write(lin_driver_t *ldp, uint8_t *data, size_t len) {

    if (len > 0U && len <= LIN_MAX_DATA_BYTE_NUM) {
        LIN_TX_IN_PROGRESS(ldp);

        ldp->tx_buf    = data;
        ldp->tx_len    = len;
        ldp->tx_cks    = lin_lld_get_checksum(data, len);
        ldp->tx_status = LIN_TX_STATUS_DATA;

        /* Start transmission of the first byte.*/
        ldp->lin->TDR = (uint32_t)(*ldp->tx_buf++);
        ldp->tx_len--;

        /* Enable transmission complete interrupt.*/
        ldp->lin->CR1 |= UART_CR1_TCIE;

        /* Wait for TX data transfer completion.*/
        LIN_WAIT_FOR_TX_COMPLETION(ldp);
    }

    return (size_t)len;
}

size_t lin_read(lin_driver_t *ldp, uint8_t *data, uint8_t *checksum, size_t len) {

    if (len > 0U && len <= LIN_MAX_DATA_BYTE_NUM) {
        LIN_RX_IN_PROGRESS(ldp);

        ldp->rx_buf    = data;
        ldp->rx_len    = len;
        ldp->rx_cks    = checksum;
        ldp->rx_status = LIN_RX_STATUS_DATA;

        /* Enable data receive interrupt.*/
        ldp->lin->CR1 |= UART_CR1_RXNEIE_RXFNEIE;

        /* Wait for RX data receiving completion.*/
        LIN_WAIT_FOR_RX_COMPLETION(ldp);
    }

    return (size_t)len;
}

bool lin_check_checksum(uint8_t *data, size_t len, uint8_t checksum) {

    if (checksum == lin_lld_get_checksum(data, len)) {
        return true;
    } else {
        return false;
    }
}

bool lin_check_parity(uint8_t pid) {

    if (LIN_GET_PARITY_BITS(pid) == lin_lld_get_parity(LIN_GET_ID(pid))) {
        return true;
    } else {
        return false;
    }
}

void lin_stop(lin_driver_t *ldp) {

    IRQn_Type vector;

    /* Disable LIN interrupt.*/
    vector = lin_dev_get_vector(ldp);
    irq_disable(vector);

    /* Stop procedure.*/
    ldp->lin->CR1   = 0U;
    ldp->lin->CR2   = 0U;
    ldp->lin->BRR   = 0U;
    ldp->lin->PRESC = 0U;

    /* Disable clock.*/
    lin_dev_clock_disable(ldp);
}

void lin_deinit(lin_driver_t *ldp) {

    /* Set LIN clock value to default value.*/
    ldp->clock = 0UL;

    /* Reset LIN parameters.*/
    lin_lld_reset_paramters(ldp);
}

void __lin_serve_interrupt(lin_driver_t *ldp) {

    uint32_t isr, cr1;
    uint8_t sync;

    /* Reading and clearing status.*/
    isr = ldp->lin->ISR;
    ldp->lin->ICR = isr;

    /* One read on control register.*/
    cr1 = ldp->lin->CR1;

    /* Break event detection condition.*/
    if ((isr & UART_ISR_LBDF) != 0U) {

        /* Disable LIN break detection interrupt.*/
        ldp->lin->CR2 &= ~UART_CR2_LBDIE;

        /* Ready for sync reception.*/
        ldp->rx_status = LIN_RX_STATUS_SYNC;

        /* Flush Receive Data Register after break reception.*/
        uint8_t temp = (uint8_t)ldp->lin->RDR;
        (void)temp;

        /* Enable data receive interrupt.*/
        ldp->lin->CR1 |= UART_CR1_RXNEIE_RXFNEIE;

        return;
    }

    /* TX ready condition.*/
    if (((cr1 & UART_CR1_TCIE) != 0U) &&
        (isr & UART_ISR_TC) != 0U) {
        /* Write Sync.*/
        if (ldp->tx_status == LIN_TX_STATUS_SYNC) {

            /* Ready for protected ID transmission.*/
            ldp->tx_status = LIN_TX_STATUS_ID;

            ldp->lin->TDR = (uint32_t)(ldp->tx_pid);
        /* Write protected I.*/
        } else if (ldp->tx_status == LIN_TX_STATUS_ID) {

            /* Disable transmission complete interrupt.*/
            cr1 &= ~UART_CR1_TCIE;

            /* Return in idle.*/
            ldp->tx_status = LIN_TX_STATUS_IDLE;

            /* If TX protected ID complete callback is defined, it is invoked.*/
            if (ldp->cb[LIN_CB_OPS_TX_PID] != NULL) {
                ldp->cb[LIN_CB_OPS_TX_PID](ldp);
            }

            LIN_TX_PID_DONE(ldp);
        /* Write data.*/
        } else if (ldp->tx_status == LIN_TX_STATUS_DATA) {
            /* Data transmission.*/
            if (ldp->tx_buf != NULL) {
                if (ldp->tx_len > 0U) {
                    ldp->lin->TDR = (uint32_t)(*ldp->tx_buf++);
                    ldp->tx_len--;
                } else {
                    /* Ready for checksum transmission.*/
                    ldp->tx_status = LIN_TX_STATUS_CHECKSUM;

                    ldp->lin->TDR = (uint32_t)(ldp->tx_cks);
                }
            }
        /* Trasmission completed.*/
        } else if (ldp->tx_status == LIN_TX_STATUS_CHECKSUM) {
            /* Disable transmission complete interrupt.*/
            cr1 &= ~UART_CR1_TCIE;

            /* Return in idle.*/
            ldp->tx_status = LIN_TX_STATUS_IDLE;
		    
            /* If TX complete callback is defined, it is invoked.*/
            if (ldp->cb[LIN_CB_OPS_TX] != NULL) {
                ldp->cb[LIN_CB_OPS_TX](ldp);
            }
		    
            LIN_TX_DONE(ldp);
        } else {
            ; /* close "if" as required by MISRA */
        }
    }

    /* RX ready condition.*/
    if (((cr1 & UART_CR1_RXNEIE_RXFNEIE) != 0U) &&
        (isr & UART_ISR_RXNE_RXFNE) != 0U) {
        /* Read Sync.*/
        if (ldp->rx_status == LIN_RX_STATUS_SYNC) {
            sync = (uint8_t)ldp->lin->RDR;

            if (sync != LIN_SYNC_BYTE) {
                /* Disable data receive interrupt.*/
                cr1 &= ~UART_CR1_RXNEIE_RXFNEIE;

                /* Return in idle.*/
                ldp->rx_status = LIN_RX_STATUS_IDLE;
			
                /* If error protected ID callback is defined, it is invoked.*/
                if (ldp->cb[LIN_CB_OPS_ERR_PID] != NULL) {
                    ldp->cb[LIN_CB_OPS_ERR_PID](ldp);
                }
            } else {
                /* Ready for protected ID reception.*/
                ldp->rx_status = LIN_RX_STATUS_ID;
            }
        /* Read protected ID.*/
        } else if (ldp->rx_status == LIN_RX_STATUS_ID) {
            *ldp->rx_pid = (uint8_t)ldp->lin->RDR;

            /* Disable data receive interrupt.*/
            cr1 &= ~UART_CR1_RXNEIE_RXFNEIE;

            /* Return in idle.*/
            ldp->rx_status = LIN_RX_STATUS_IDLE;

            /* If protected ID callback is defined, it is invoked.*/
            if (ldp->cb[LIN_CB_OPS_RX_PID] != NULL) {
                ldp->cb[LIN_CB_OPS_RX_PID](ldp);
            }
        /* Read data and checksum.*/
        } else if (ldp->rx_status == LIN_RX_STATUS_DATA) {
            if (ldp->rx_len > 0U) {
                *ldp->rx_buf++ = (uint8_t)ldp->lin->RDR;
                ldp->rx_len--;
            } else {
                /* Read checksum.*/
                *ldp->rx_cks = (uint8_t)ldp->lin->RDR;

                /* Disable data receive interrupt.*/
                cr1 &= ~UART_CR1_RXNEIE_RXFNEIE;

                /* Return in idle.*/
                ldp->rx_status = LIN_RX_STATUS_IDLE;

                /* If RX complete callback is defined, it is invoked.*/
                if (ldp->cb[LIN_CB_OPS_RX] != NULL) {
                    ldp->cb[LIN_CB_OPS_RX](ldp);
                }

                LIN_RX_DONE(ldp);
            }
        } else {
            ; /* close "if" as required by MISRA */
        }
    }

    /* One write on control registers.*/
    ldp->lin->CR1 = cr1;
}

/** @} */
