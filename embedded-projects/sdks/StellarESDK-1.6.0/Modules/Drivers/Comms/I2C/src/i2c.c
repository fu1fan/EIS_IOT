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
 * @file    i2c.c
 * @brief   I2C driver source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup COMMS
 * @ingroup DRIVERS
 * @addtogroup I2C
 * @ingroup COMMS
 * @{
 */

#include <i2c.h>
#include <i2c_private.h>

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
 * @brief   Enters in transfer mode.
 * @note    Called when I2C driver enters in transfer mode.
 *
 * @param[in] \_idp\_   pointer to a @p I2C_driver_t structure
 *
 * @notapi
 */
#define I2C_OP_IN_PROGRESS(_idp_)                          \
    (_idp_)->op_busy = I2C_OP_BUSY

/**
 * @brief  Exits from transfer mode.
 * @note   Called when I2C driver exits transfer mode.
 *
 * @param[in] \_idp\_   pointer to a @p I2C_driver_t structure
 *
 * @notapi
 */
#define I2C_OP_DONE(_idp_)                                 \
    (_idp_)->op_busy = I2C_OP_READY

/**
 * @brief  Waits for transfer end.
 * @note   Called to wait for end of transfer.
 *
 * @param[in] \_idp\_   pointer to a @p I2C_driver_t structure
 *
 * @notapi
 */
#define I2C_WAIT_FOR_OP_COMPLETION(_idp_)                  \
    if ((_idp_)->drv_mode == I2C_DRV_MODE_SYNCHRONOUS) {   \
        while ((_idp_)->op_busy == I2C_OP_BUSY) {;}        \
    }

/**
 * @brief   Resets I2C parameters.
 *
 * @param[out] idp      pointer to an @p i2c_driver_t structure
 *
 * @notapi
 */
static void i2c_reset_paramters(i2c_driver_t *idp) {

    uint8_t i;

    /* Set all driver configuration parameters to default values.*/
    idp->mode                         = I2C_MODE_MASTER_TX;
    idp->data                         = NULL;
    idp->size                         = 0U;
    idp->op_busy                      = I2C_OP_READY;
    idp->prio                         = IRQ_MIN_PRIORITY;
    idp->drv_mode                     = I2C_DRV_MODE_SYNCHRONOUS;
    idp->is_dma_en                    = false;
    idp->dma_conf.dma_stream_tx_id    = DMA1_STREAM0_ID;
    idp->dma_conf.dma_stream_rx_id    = DMA1_STREAM1_ID;
    idp->dma_conf.dma_stream_bus_prio = DMA_PRIORITY_LOW;
    idp->dma_conf.dma_stream_irq_prio = IRQ_MIN_PRIORITY;
    idp->dma_rx_descriptor            = NULL;
    idp->dma_tx_descriptor            = NULL;
    idp->oa1_mode                     = I2C_OA1_MODE_7BIT;
    idp->oa1                          = 0U;
    idp->is_oa2_en                    = false;
    idp->oa2                          = 0U;
    idp->oa2_mask                     = I2C_OA2_NOMASK;
    idp->is_gc_en                     = false;
    idp->is_no_stretch_en             = false;

    for (i = 0U; i < (uint8_t)I2C_CB_OPS_MAX; i++) {
        idp->cb[i] = NULL;
    }
}

/**
 * @brief   Configures transfer parameters.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @param[in] mode      transfer mode (reload or autoend). It can be one of the
 *                      following parameters:
 *            @arg @ref I2C_TRANSFER_MODE_RELOAD
 *            @arg @ref I2C_TRANSFER_MODE_AUTOEND
 * @param[in] size      number of bytes to be transfered
 *
 * @notapi
 */
static void i2c_lld_set_transfer(i2c_driver_t *idp, uint32_t mode, uint8_t size) {

    uint32_t mask = I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_NBYTES;
    uint32_t value;

    value = mode | (((uint32_t)size) << I2C_CR2_NBYTES_Pos);

    /* Configure transfer mode (reload or autoend) and transfer size.*/
    idp->i2c->CR2 = (idp->i2c->CR2 & ~(mask)) | (value);
}


/**
 * @brief   Transmits in master mode an amount of data in interrupt mode.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @param[in] address   address of the slave to transmit to
 * @param[in] data      pointer to the buffer to be transmitted
 * @param[in] size      maximum number of bytes to be transmitted
 *
 * @notapi
 */
static void i2c_lld_master_transmit(i2c_driver_t *idp, uint16_t address,
                                    uint8_t *data, uint16_t size) {

    idp->data  = data;
    idp->count = size;

    /* Set transfer mode and transfer size.*/
    if (idp->count > I2C_MAX_NBYTE_SIZE) {
        idp->size = I2C_MAX_NBYTE_SIZE;
        i2c_lld_set_transfer(idp, I2C_TRANSFER_MODE_RELOAD, ((uint8_t)idp->size));
    } else {
        idp->size = idp->count;
        i2c_lld_set_transfer(idp, I2C_TRANSFER_MODE_AUTOEND, ((uint8_t)idp->size));
    }

    /* Set slave address and transfer direction.*/
    REG_SET_FIELD(idp->i2c->CR2, I2C_CR2_SADD, address);
    REG_SET_FIELD(idp->i2c->CR2, I2C_CR2_RD_WRN, 0U);
    REG_SET_FIELD(idp->i2c->CR2, I2C_CR2_START, 1U);

    /* Enable interrupts.*/
    idp->i2c->CR1 |= I2C_CR1_ERRIE  |
                     I2C_CR1_TCIE   |
                     I2C_CR1_STOPIE |
                     I2C_CR1_NACKIE |
                     I2C_CR1_TXIE;
}

/**
 * @brief   Receives in master mode an amount of data in interrupt mode.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @param[in] address   address of the slave to read from
 * @param[in] data      pointer to the buffer to be received
 * @param[in] size      maximum number of bytes to be received
 *
 * @notapi
 */
static void i2c_lld_master_receive(i2c_driver_t *idp, uint16_t address,
                                   uint8_t *data, uint16_t size) {

    idp->data  = data;
    idp->count = size;

    /* Set transfer mode and transfer size.*/
    if (idp->count > I2C_MAX_NBYTE_SIZE) {
        idp->size = I2C_MAX_NBYTE_SIZE;
        i2c_lld_set_transfer(idp, I2C_TRANSFER_MODE_RELOAD, ((uint8_t)idp->size));
    } else {
        idp->size = idp->count;
        i2c_lld_set_transfer(idp, I2C_TRANSFER_MODE_AUTOEND, ((uint8_t)idp->size));
    }

    /* Set slave address and transfer direction.*/
    REG_SET_FIELD(idp->i2c->CR2, I2C_CR2_SADD, address);
    REG_SET_FIELD(idp->i2c->CR2, I2C_CR2_RD_WRN, 1U);
    /* Start transfer.*/
    REG_SET_FIELD(idp->i2c->CR2, I2C_CR2_START, 1U);

    /* Enable interrupts.*/
    idp->i2c->CR1 |= I2C_CR1_ERRIE  |
                     I2C_CR1_TCIE   |
                     I2C_CR1_STOPIE |
                     I2C_CR1_NACKIE |
                     I2C_CR1_RXIE;
}

/**
 * @brief   Shared interrupt handler (master mode).
 * @note    Not an API, used internally.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 *
 * @notapi
 */
static void i2c_lld_master_isr(i2c_driver_t *idp) {

    uint32_t cr1, isr;

    cr1 = idp->i2c->CR1;
    isr = idp->i2c->ISR;

    /* NACK reception event.*/
    if (((cr1 & I2C_CR1_NACKIE) != 0U) &&
        ((isr & I2C_ISR_NACKF) != 0U)) {

        /* Clear not acknowledge flag.*/
        idp->i2c->ICR |= I2C_ICR_NACKCF;

        /* Get new value of ISR register after the clearing of the not
           acknowledge flag.*/
        isr = idp->i2c->ISR;

        /* Flush TX data register.*/
        if ((isr & I2C_ISR_TXIS) != 0U) {
            idp->i2c->TXDR = 0x00U;
        }
        /* Get new value of ISR register after the flushing of TX data
           register.*/
        isr = idp->i2c->ISR;
        if ((isr & I2C_ISR_TXE) == 0U) {
            idp->i2c->ISR |= I2C_ISR_TXE;
        }

        /* If defined, invoke error callback.*/
        if (idp->cb[I2C_CB_OPS_ERR] != NULL) {
            idp->cb[I2C_CB_OPS_ERR](idp);
        }

        I2C_OP_DONE(idp);
    }

    /* Receive buffer not empty event.*/
    if (((cr1 & I2C_CR1_RXIE) != 0U) &&
        ((isr & I2C_ISR_RXNE) != 0U)) {

        *idp->data++ = (uint8_t)idp->i2c->RXDR;

        idp->size--;
        idp->count--;
    }

    /* Transmit buffer empty event.*/
    if (((cr1 & I2C_CR1_TXIE) != 0U) &&
        ((isr & I2C_ISR_TXIS) != 0U)) {
        idp->i2c->TXDR = *idp->data++;

        idp->size--;
        idp->count--;
    }

    /* Transfer complete reload event.*/
    if (((cr1 & I2C_CR1_TCIE) != 0U) &&
        ((isr & I2C_ISR_TCR)  != 0U)) {

        if ((idp->size == 0U) && (idp->count != 0U)) {
            /* Update transfer mode and tranfer size.*/
            if (idp->count > I2C_MAX_NBYTE_SIZE) {
                idp->size = I2C_MAX_NBYTE_SIZE;
                i2c_lld_set_transfer(idp, I2C_TRANSFER_MODE_RELOAD, ((uint8_t)idp->size));
            } else {
                idp->size = idp->count;
                i2c_lld_set_transfer(idp, I2C_TRANSFER_MODE_AUTOEND, ((uint8_t)idp->size));
            }

        } else {
            /* TODO: completed, error.*/
        }
    }

    /* Transfer complete event.*/
    if (((cr1 & I2C_CR1_TCIE) != 0U) &&
        ((isr & I2C_ISR_TC)  != 0U)) {

        if (idp->count != 0U) {
            /* If defined, invoke error callback.*/
            if (idp->cb[I2C_CB_OPS_ERR] != NULL) {
                idp->cb[I2C_CB_OPS_ERR](idp);
            }

            I2C_OP_DONE(idp);
        }
    }

    /* Stop detection event.*/
    if (((cr1 & I2C_CR1_STOPIE) != 0U) &&
        ((isr & I2C_ISR_STOPF) != 0U)) {

        /* Clear STOP detection flag.*/
        idp->i2c->ICR |= I2C_ICR_STOPCF;

        /* Get new value of ISR register after the clearing of the STOP
           detection flag.*/
        /* isr = idp->i2c->ISR; */

        /* Disable interrutps (master transmit mode).*/
        if (idp->mode == I2C_MODE_MASTER_TX) {
            idp->i2c->CR1 &= ~(I2C_CR1_ERRIE  |
                               I2C_CR1_TCIE   |
                               I2C_CR1_STOPIE |
                               I2C_CR1_NACKIE |
                               I2C_CR1_TXIE);
        /* Disable interrutps (master receive mode).*/
        } else {
            idp->i2c->CR1 &= ~(I2C_CR1_ERRIE  |
                               I2C_CR1_TCIE   |
                               I2C_CR1_STOPIE |
                               I2C_CR1_NACKIE |
                               I2C_CR1_RXIE);
        }

        /* Get new value of CR1 register after the interrupt disabling.*/
        /* cr1 = idp->i2c->CR1; */

        /* If defined, invoke transfer complete callback.*/
        if (idp->cb[I2C_CB_OPS_DONE] != NULL) {
            idp->cb[I2C_CB_OPS_DONE](idp);
        } else {
            ; /* close "if" as required by MISRA */
        }

        I2C_OP_DONE(idp);
    }
}

/**
 * @brief   Transmits in slave mode an amount of data in interrupt mode.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @param[in] data      pointer to the buffer to be transmitted
 * @param[in] size      maximum number of bytes to be transmitted
 *
 * @notapi
 */
static void i2c_lld_slave_transmit(i2c_driver_t *idp, uint8_t *data,
                                   uint16_t size) {

    idp->data  = data;
    idp->count = size;

    /* Enable address acknowledge.*/
    idp->i2c->CR2 &= ~I2C_CR2_NACK;

    /* Enable interrupts.*/
    idp->i2c->CR1 |= I2C_CR1_ERRIE  |
                     I2C_CR1_STOPIE |
                     I2C_CR1_NACKIE |
                     I2C_CR1_ADDRIE |
                     I2C_CR1_TXIE;
}

/**
 * @brief   Receives in slave mode an amount of data in interrupt mode.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @param[in] data      pointer to the buffer to be received
 * @param[in] size      maximum number of bytes to be received
 *
 * @notapi
 */
static void i2c_lld_slave_receive(i2c_driver_t *idp, uint8_t *data,
                                  uint16_t size) {

    idp->data  = data;
    idp->count = size;

    /* Enable address acknowledge.*/
    idp->i2c->CR2 &= ~I2C_CR2_NACK;

    /* Enable interrupts.*/
    idp->i2c->CR1 |= I2C_CR1_ERRIE  |
                     I2C_CR1_STOPIE |
                     I2C_CR1_NACKIE |
                     I2C_CR1_ADDRIE |
                     I2C_CR1_RXIE;
}

/**
 * @brief   Shared interrupt handler (slave mode).
 * @note    Not an API, used internally.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 *
 * @notapi
 */
static void i2c_lld_slave_isr(i2c_driver_t *idp) {

    uint32_t cr1, isr;

    cr1 = idp->i2c->CR1;
    isr = idp->i2c->ISR;

    /* Stop detection event.*/
    if (((cr1 & I2C_CR1_STOPIE) != 0U) &&
        ((isr & I2C_ISR_STOPF) != 0U)) {

        /* Clear STOP detection flag.*/
        idp->i2c->ICR |= I2C_ICR_STOPCF;

        /* Get new value of ISR register after the clearing of the STOP
           detection flag.*/
        isr = idp->i2c->ISR;

        /* Disable interrutps (slave transmit mode).*/
        if (idp->mode == I2C_MODE_SLAVE_TX) {
            idp->i2c->CR1 &= ~(I2C_CR1_ERRIE  |
                               I2C_CR1_STOPIE |
                               I2C_CR1_NACKIE |
                               I2C_CR1_ADDRIE |
                               I2C_CR1_TXIE);
        /* Disable interrutps (slave receive mode).*/
        } else {
            idp->i2c->CR1 &= ~(I2C_CR1_ERRIE  |
                               I2C_CR1_STOPIE |
                               I2C_CR1_NACKIE |
                               I2C_CR1_ADDRIE |
                               I2C_CR1_RXIE);
        }

        /* Get new value of CR1 register after the interrupt disabling.*/
        cr1 = idp->i2c->CR1;

        /* If defined, invoke transfer complete callback.*/
        if (idp->cb[I2C_CB_OPS_DONE] != NULL) {
            idp->cb[I2C_CB_OPS_DONE](idp);
        } else {
            ; /* close "if" as required by MISRA */
        }

        I2C_OP_DONE(idp);
    }

    /* NACK reception event.*/
    if (((cr1 & I2C_CR1_NACKIE) != 0U) &&
        ((isr & I2C_ISR_NACKF) != 0U)) {
        /* If the transfer is completed (count = 0), the master will send a
           NACK (normal behaviour), otherwise a NACK means that an transfer
           error is occurred.*/
        if (idp->count == 0U) {

            /* Clear not acknowledge flag.*/
            idp->i2c->ICR |= I2C_ICR_NACKCF;

            /* Get new value of ISR register after the clearing of the not
               acknowledge flag.*/
            isr = idp->i2c->ISR;

            /* Flush TX data register.*/
            if ((isr & I2C_ISR_TXIS) != 0U) {
                idp->i2c->TXDR = 0x00U;
            }
            /* Get new value of ISR register after the flushing of TX data
               register.*/
            isr = idp->i2c->ISR;
            if ((isr & I2C_ISR_TXE) == 0U) {
                idp->i2c->ISR |= I2C_ISR_TXE;
            }
        } else {
            /* If defined, invoke error callback.*/
            if (idp->cb[I2C_CB_OPS_ERR] != NULL) {
                idp->cb[I2C_CB_OPS_ERR](idp);
            }

            I2C_OP_DONE(idp);
        }
    }

    /* Receive buffer not empty event.*/
    if (((cr1 & I2C_CR1_RXIE) != 0U) &&
        ((isr & I2C_ISR_RXNE) != 0U)) {

        if (idp->count > 0U) {
            *idp->data++ = (uint8_t)idp->i2c->RXDR;

            /* idp->size--; */
            idp->count--;
        }
    }

    /* Address match event.*/
    if (((cr1 & I2C_CR1_ADDRIE) != 0U) &&
        ((isr & I2C_ISR_ADDR) != 0U)) {

        /* Clear address matched flag.*/
        idp->i2c->ICR |= I2C_ICR_ADDRCF;

        /* Get new value of ISR register after the clearing of the address
           matched flag.*/
        isr = idp->i2c->ISR;

        /* TODO: invoke address match callback.*/
    }

    /* Transmit buffer empty event.*/
    if (((cr1 & I2C_CR1_TXIE) != 0U) &&
        ((isr & I2C_ISR_TXIS) != 0U)) {

        if (idp->count > 0U) {
            idp->i2c->TXDR = *idp->data++;

            /* idp->size--; */
            idp->count--;
        }
    }
}

/**
 * @brief   DMA master TX interrupt handler.
 *
 * @param[in] arg       argument associated to the callback
 * @param[in] sts       DMA callback cause status flags
 *
 * @isr
 */
static void i2c_serve_dma_master_tx_interrupt(void *arg, uint32_t sts) {

    /*lint -e9087 */
    i2c_driver_t *idp = (i2c_driver_t *)arg;
    /*lint +e9087 */

    /* Disable DMA request.*/
    idp->i2c->CR1 &= ~I2C_CR1_TXDMAEN;

    /* Checking for errors.*/
    if ((sts & DMA_STS_TEIF) != 0U) {
        if (idp->cb[I2C_CB_OPS_ERR] != NULL) {
            /* DMA critical error.*/
            idp->cb[I2C_CB_OPS_ERR](idp);
        }

        I2C_OP_DONE(idp);
    } else {
        /* Trasmission completed.*/
        if ((sts & DMA_STS_TCIF) != 0U) {

            if (idp->count == 0U) {
                /* Transfer completed. Enable stop detection interrupt.*/
                idp->i2c->CR1 |= I2C_CR1_STOPIE;
            } else {
                /* Prepare new transfer via DMA.*/
                /* Update pointer to next data to transmit.*/
                idp->data += idp->size;

                if (idp->count > I2C_MAX_NBYTE_SIZE) {
                    idp->size = I2C_MAX_NBYTE_SIZE;
                } else {
                    idp->size = idp->count;
                }

                /* Configure DMA TX number of data items.*/
                dma_stream_set_count(idp->dma_tx_descriptor, (uint32_t)(idp->size));

                /* Configure DMA TX stream memory address.*/
                dma_stream_set_memory(idp->dma_tx_descriptor, (uint32_t)(idp->data));

                /* Enable DMA TX stream.*/
                dma_stream_enable(idp->dma_tx_descriptor);

                /* Enable transfer complete interrupt.*/
                idp->i2c->CR1 |= I2C_CR1_TCIE;
            }
        } else {
            ; /* close "if" as required by MISRA */
        }
    }
}

/**
 * @brief   DMA master RX interrupt handler.
 *
 * @param[in] arg       argument associated to the callback
 * @param[in] sts       DMA callback cause status flags
 *
 * @isr
 */
static void i2c_serve_dma_master_rx_interrupt(void *arg, uint32_t sts) {

    /*lint -e9087 */
    i2c_driver_t *idp = (i2c_driver_t *)arg;
    /*lint +e9087 */

    /* Disable DMA request.*/
    idp->i2c->CR1 &= ~I2C_CR1_RXDMAEN;

    /* Checking for errors.*/
    if ((sts & DMA_STS_TEIF) != 0U) {
        if (idp->cb[I2C_CB_OPS_ERR] != NULL) {
            /* DMA critical error.*/
            idp->cb[I2C_CB_OPS_ERR](idp);
        }

        I2C_OP_DONE(idp);
    } else {
        /* Trasmission completed.*/
        if ((sts & DMA_STS_TCIF) != 0U) {

            if (idp->count == 0U) {
                /* Transfer completed. Enable stop detection interrupt.*/
                idp->i2c->CR1 |= I2C_CR1_STOPIE;
            } else {
                /* Prepare new transfer via DMA.*/
                /* Update pointer to next data to transmit.*/
                idp->data += idp->size;

                if (idp->count > I2C_MAX_NBYTE_SIZE) {
                    idp->size = I2C_MAX_NBYTE_SIZE;
                } else {
                    idp->size = idp->count;
                }

                /* Configure DMA RX number of data items.*/
                dma_stream_set_count(idp->dma_rx_descriptor, (uint32_t)(idp->size));

                /* Configure DMA RX stream memory address.*/
                dma_stream_set_memory(idp->dma_rx_descriptor, (uint32_t)(idp->data));

                /* Enable DMA RX stream.*/
                dma_stream_enable(idp->dma_rx_descriptor);

                /* Enable transfer complete interrupt.*/
                idp->i2c->CR1 |= I2C_CR1_TCIE;
            }
        } else {
            ; /* close "if" as required by MISRA */
        }
    }
}

/**
 * @brief   Transmits in master mode an amount of data in DMA mode.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @param[in] address   address of the slave to transmit to
 * @param[in] data      pointer to the buffer to be transmitted
 * @param[in] size      maximum number of bytes to be transmitted
 *
 * @notapi
 */
static void i2c_lld_master_transmit_dma(i2c_driver_t *idp, uint16_t address,
                                        uint8_t *data, uint16_t size) {

    idp->data  = data;
    idp->count = size;

    /* Set transfer mode and transfer size.*/
    if (idp->count > I2C_MAX_NBYTE_SIZE) {
        idp->size = I2C_MAX_NBYTE_SIZE;
        i2c_lld_set_transfer(idp, I2C_TRANSFER_MODE_RELOAD, ((uint8_t)idp->size));
    } else {
        idp->size = idp->count;
        i2c_lld_set_transfer(idp, I2C_TRANSFER_MODE_AUTOEND, ((uint8_t)idp->size));
    }

    /* Configure DMA stream interrupt callback.*/
    /*lint -e9087 */
    dma_stream_set_callback(idp->dma_tx_descriptor, i2c_serve_dma_master_tx_interrupt, (void *)idp);
    /*lint +e9087 */

    /* Configure DMA TX number of data items.*/
    dma_stream_set_count(idp->dma_tx_descriptor, (uint32_t)(idp->size));

    /* Configure DMA TX stream memory address.*/
    dma_stream_set_memory(idp->dma_tx_descriptor, (uint32_t)(idp->data));

    /* Enable DMA TX stream.*/
    dma_stream_enable(idp->dma_tx_descriptor);

    /* Set slave address and transfer direction.*/
    REG_SET_FIELD(idp->i2c->CR2, I2C_CR2_SADD, address);
    REG_SET_FIELD(idp->i2c->CR2, I2C_CR2_RD_WRN, 0U);
    REG_SET_FIELD(idp->i2c->CR2, I2C_CR2_START, 1U);

    /* Update counter value.*/
    idp->count -= idp->size;

    /* Enable interrupts.*/
    idp->i2c->CR1 |= I2C_CR1_ERRIE  |
                     I2C_CR1_NACKIE;

    /* Enable DMA request.*/
    idp->i2c->CR1 |= I2C_CR1_TXDMAEN;
}

/**
 * @brief   Receives in master mode an amount of data in DMA mode.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @param[in] address   address of the slave to read from
 * @param[in] data      pointer to the buffer to be received
 * @param[in] size      maximum number of bytes to be received
 *
 * @notapi
 */
static void i2c_lld_master_receive_dma(i2c_driver_t *idp, uint16_t address,
                                       uint8_t *data, uint16_t size) {

    idp->data  = data;
    idp->count = size;

    if (idp->count > I2C_MAX_NBYTE_SIZE) {
        idp->size = I2C_MAX_NBYTE_SIZE;
        i2c_lld_set_transfer(idp, I2C_TRANSFER_MODE_RELOAD, ((uint8_t)idp->size));
    } else {
        idp->size = idp->count;
        i2c_lld_set_transfer(idp, I2C_TRANSFER_MODE_AUTOEND, ((uint8_t)idp->size));
    }

    /* Configure DMA stream interrupt callback.*/
    /*lint -e9087 */
    dma_stream_set_callback(idp->dma_rx_descriptor, i2c_serve_dma_master_rx_interrupt, (void *)idp);
    /*lint +e9087 */

    /* Configure DMA RX number of data items.*/
    dma_stream_set_count(idp->dma_rx_descriptor, (uint32_t)(idp->size));

    /* Configure DMA RX stream memory address.*/
    dma_stream_set_memory(idp->dma_rx_descriptor, (uint32_t)(idp->data));

    /* Enable DMA RX stream.*/
    dma_stream_enable(idp->dma_rx_descriptor);

    /* Set slave address and transfer direction.*/
    REG_SET_FIELD(idp->i2c->CR2, I2C_CR2_SADD, address);
    REG_SET_FIELD(idp->i2c->CR2, I2C_CR2_RD_WRN, 1U);
    REG_SET_FIELD(idp->i2c->CR2, I2C_CR2_START, 1U);

    /* Update counter value.*/
    idp->count -= idp->size;

    /* Enable interrupts.*/
    idp->i2c->CR1 |= I2C_CR1_ERRIE  |
                     I2C_CR1_NACKIE;

    /* Enable DMA request.*/
    idp->i2c->CR1 |= I2C_CR1_RXDMAEN;
}

/**
 * @brief   Shared interrupt handler (master mode, DMA mode).
 * @note    Not an API, used internally.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 *
 * @notapi
 */
static void i2c_lld_master_dma_isr(i2c_driver_t *idp) {

    uint32_t cr1, isr;

    cr1 = idp->i2c->CR1;
    isr = idp->i2c->ISR;

    /* NACK reception event.*/
    if (((cr1 & I2C_CR1_NACKIE) != 0U) &&
        ((isr & I2C_ISR_NACKF) != 0U)) {

        /* Clear not acknowledge flag.*/
        idp->i2c->ICR |= I2C_ICR_NACKCF;

        /* Get new value of ISR register after the clearing of the not
           acknowledge flag.*/
        isr = idp->i2c->ISR;

        /* Flush TX data register.*/
        if ((isr & I2C_ISR_TXIS) != 0U) {
            idp->i2c->TXDR = 0x00U;
        }
        /* Get new value of ISR register after the flushing of TX data
           register.*/
        isr = idp->i2c->ISR;
        if ((isr & I2C_ISR_TXE) == 0U) {
            idp->i2c->ISR |= I2C_ISR_TXE;
        }

        /* If defined, invoke error callback.*/
        if (idp->cb[I2C_CB_OPS_ERR] != NULL) {
            idp->cb[I2C_CB_OPS_ERR](idp);
        }

        I2C_OP_DONE(idp);
    }

    /* Transfer complete reload event.*/
    if (((cr1 & I2C_CR1_TCIE) != 0U) &&
        ((isr & I2C_ISR_TCR)  != 0U)) {

        /* Disable transfer complete interrupt.*/
        idp->i2c->CR1 &= ~I2C_CR1_TCIE;

        /* Update transfer mode and tranfer size.*/
        if (idp->count > I2C_MAX_NBYTE_SIZE) {
            //idp->size = I2C_MAX_NBYTE_SIZE;
            i2c_lld_set_transfer(idp, I2C_TRANSFER_MODE_RELOAD, ((uint8_t)idp->size));
        } else {
            //idp->size = idp->count;
            i2c_lld_set_transfer(idp, I2C_TRANSFER_MODE_AUTOEND, ((uint8_t)idp->size));
        }

        idp->count -= idp->size;

        /* Enable DMA request.*/
        if (idp->mode == I2C_MODE_MASTER_TX) {
            idp->i2c->CR1 |= I2C_CR1_TXDMAEN;
        } else {
            idp->i2c->CR1 |= I2C_CR1_RXDMAEN;
        }
    }

    /* Transfer complete event.*/
    if (((cr1 & I2C_CR1_TCIE) != 0U) &&
        ((isr & I2C_ISR_TC)  != 0U)) {

        if (idp->count != 0U) {
            /* If defined, invoke error callback.*/
            if (idp->cb[I2C_CB_OPS_ERR] != NULL) {
                idp->cb[I2C_CB_OPS_ERR](idp);
            }

            I2C_OP_DONE(idp);
        }
    }

    /* Stop detection event.*/
    if (((cr1 & I2C_CR1_STOPIE) != 0U) &&
        ((isr & I2C_ISR_STOPF) != 0U)) {

        /* Clear STOP detection flag.*/
        idp->i2c->ICR |= I2C_ICR_STOPCF;

        /* Get new value of ISR register after the clearing of the STOP
           detection flag.*/
        /* isr = idp->i2c->ISR; */

        /* Disable interrutps (master transmit mode).*/
        if (idp->mode == I2C_MODE_MASTER_TX) {
            idp->i2c->CR1 &= ~(I2C_CR1_ERRIE  |
                               I2C_CR1_TCIE   |
                               I2C_CR1_STOPIE |
                               I2C_CR1_NACKIE);
        /* Disable interrutps (master receive mode).*/
        } else {
            idp->i2c->CR1 &= ~(I2C_CR1_ERRIE  |
                               I2C_CR1_TCIE   |
                               I2C_CR1_STOPIE |
                               I2C_CR1_NACKIE);
        }

        /* Get new value of CR1 register after the interrupt disabling.*/
        /* cr1 = idp->i2c->CR1; */

        /* If defined, invoke transfer complete callback.*/
        if (idp->cb[I2C_CB_OPS_DONE] != NULL) {
            idp->cb[I2C_CB_OPS_DONE](idp);
        } else {
            ; /* close "if" as required by MISRA */
        }

        I2C_OP_DONE(idp);
    }
}

/**
 * @brief   DMA slave TX interrupt handler.
 *
 * @param[in] arg       argument associated to the callback
 * @param[in] sts       DMA callback cause status flags
 *
 * @isr
 */
static void i2c_serve_dma_slave_tx_interrupt(void *arg, uint32_t sts) {

    /*lint -e9087 */
    i2c_driver_t *idp = (i2c_driver_t *)arg;
    /*lint +e9087 */

    /* Disable DMA request.*/
    idp->i2c->CR1 &= ~I2C_CR1_TXDMAEN;

    /* Checking for errors.*/
    if ((sts & DMA_STS_TEIF) != 0U) {
        if (idp->cb[I2C_CB_OPS_ERR] != NULL) {
            /* DMA critical error.*/
            idp->cb[I2C_CB_OPS_ERR](idp);
        }

        I2C_OP_DONE(idp);
    } else {
        ; /* close "if" as required by MISRA */
    }
}

/**
 * @brief   DMA slave RX interrupt handler.
 *
 * @param[in] arg       argument associated to the callback
 * @param[in] sts       DMA callback cause status flags
 *
 * @isr
 */
static void i2c_serve_dma_slave_rx_interrupt(void *arg, uint32_t sts) {

    /*lint -e9087 */
    i2c_driver_t *idp = (i2c_driver_t *)arg;
    /*lint +e9087 */

    /* Disable DMA request.*/
    idp->i2c->CR1 &= ~I2C_CR1_RXDMAEN;

    /* Checking for errors.*/
    if ((sts & DMA_STS_TEIF) != 0U) {
        if (idp->cb[I2C_CB_OPS_ERR] != NULL) {
            /* DMA critical error.*/
            idp->cb[I2C_CB_OPS_ERR](idp);
        }

        I2C_OP_DONE(idp);
    } else {
        ; /* close "if" as required by MISRA */
    }
}

/**
 * @brief   Transmits in slave mode an amount of data in DMA mode.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @param[in] data      pointer to the buffer to be transmitted
 * @param[in] size      maximum number of bytes to be transmitted
 *
 * @notapi
 */
static void i2c_lld_slave_transmit_dma(i2c_driver_t *idp, uint8_t *data,
                                       uint16_t size) {

    idp->data  = data;
    idp->count = size;

    /* Configure DMA stream interrupt callback.*/
    /*lint -e9087 */
    dma_stream_set_callback(idp->dma_tx_descriptor, i2c_serve_dma_slave_tx_interrupt, (void *)idp);
    /*lint +e9087 */

    /* Configure DMA TX number of data items.*/
    dma_stream_set_count(idp->dma_tx_descriptor, (uint32_t)(idp->count));

    /* Configure DMA TX stream memory address.*/
    dma_stream_set_memory(idp->dma_tx_descriptor, (uint32_t)(idp->data));

    /* Enable DMA TX stream.*/
    dma_stream_enable(idp->dma_tx_descriptor);

    /* Enable address acknowledge.*/
    idp->i2c->CR1 &= ~I2C_CR2_NACK;

    /* Enable interrupts.*/
    idp->i2c->CR1 |= (I2C_CR1_ERRIE  |
                      I2C_CR1_STOPIE |
                      I2C_CR1_NACKIE |
                      I2C_CR1_ADDRIE);

    /* Enable DMA request.*/
    idp->i2c->CR1 |= I2C_CR1_TXDMAEN;
}

/**
 * @brief   Receives in slave mode an amount of data in DMA mode.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @param[in] data      pointer to the buffer to be received
 * @param[in] size      maximum number of bytes to be received
 *
 * @notapi
 */
static void i2c_lld_slave_receive_dma(i2c_driver_t *idp, uint8_t *data,
                                      uint16_t size) {

    idp->data  = data;
    idp->count = size;

    /* Configure DMA stream interrupt callback.*/
    /*lint -e9087 */
    dma_stream_set_callback(idp->dma_rx_descriptor, i2c_serve_dma_slave_rx_interrupt, (void *)idp);
    /*lint +e9087 */

    /* Configure DMA TX number of data items.*/
    dma_stream_set_count(idp->dma_rx_descriptor, (uint32_t)(idp->count));

    /* Configure DMA TX stream memory address.*/
    dma_stream_set_memory(idp->dma_rx_descriptor, (uint32_t)(idp->data));

    /* Enable DMA TX stream.*/
    dma_stream_enable(idp->dma_rx_descriptor);

    /* Enable address acknowledge.*/
    idp->i2c->CR1 &= ~I2C_CR2_NACK;

    /* Enable interrupts.*/
    idp->i2c->CR1 |= (I2C_CR1_ERRIE  |
                      I2C_CR1_STOPIE |
                      I2C_CR1_NACKIE |
                      I2C_CR1_ADDRIE);

    /* Enable DMA request.*/
    idp->i2c->CR1 |= I2C_CR1_RXDMAEN;
}

/**
 * @brief   Shared interrupt handler (slave mode, DMA mode).
 * @note    Not an API, used internally.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 *
 * @notapi
 */
static void i2c_lld_slave_dma_isr(i2c_driver_t *idp) {

    uint32_t cr1, isr;
    bool completed = false;

    cr1 = idp->i2c->CR1;
    isr = idp->i2c->ISR;

    /* Stop detection event.*/
    if (((cr1 & I2C_CR1_STOPIE) != 0U) &&
        ((isr & I2C_ISR_STOPF) != 0U)) {

        /* Clear STOP detection flag.*/
        idp->i2c->ICR |= I2C_ICR_STOPCF;

        /* Get new value of ISR register after the clearing of the STOP
           detection flag.*/
        isr = idp->i2c->ISR;

        /* Disable interrutps (slave transmit mode).*/
        if (idp->mode == I2C_MODE_SLAVE_TX) {
            idp->i2c->CR1 &= ~(I2C_CR1_ERRIE  |
                               I2C_CR1_STOPIE |
                               I2C_CR1_NACKIE);
        /* Disable interrutps (slave receive mode).*/
        } else {
            idp->i2c->CR1 &= ~(I2C_CR1_ERRIE  |
                               I2C_CR1_STOPIE |
                               I2C_CR1_NACKIE);
        }

        /* Get new value of CR1 register after the interrupt disabling.*/
        cr1 = idp->i2c->CR1;

        /* If defined, invoke transfer complete callback.*/
        if (idp->cb[I2C_CB_OPS_DONE] != NULL) {
            idp->cb[I2C_CB_OPS_DONE](idp);
        } else {
            ; /* close "if" as required by MISRA */
        }

        I2C_OP_DONE(idp);
    }

    /* NACK reception event.*/
    if (((cr1 & I2C_CR1_NACKIE) != 0U) &&
        ((isr & I2C_ISR_NACKF) != 0U)) {
        /* If the transfer is completed (DMA counter = 0), the master will send
           a NACK (normal behaviour), otherwise a NACK means that an transfer
           error is occurred.*/
        if (idp->mode == I2C_MODE_SLAVE_TX) {
            if (dma_stream_get_count(idp->dma_tx_descriptor) == 0U) {
                completed = true;
            }
        }
        if (idp->mode == I2C_MODE_SLAVE_RX) {
            if (dma_stream_get_count(idp->dma_rx_descriptor) == 0U) {
                completed = true;
            }
        }

        if (((bool)completed) == true) {

            /* Clear not acknowledge flag.*/
            idp->i2c->ICR |= I2C_ICR_NACKCF;

            /* Get new value of ISR register after the clearing of the not
               acknowledge flag.*/
            isr = idp->i2c->ISR;

            /* Flush TX data register.*/
            if ((isr & I2C_ISR_TXIS) != 0U) {
                idp->i2c->TXDR = 0x00U;
            }
            /* Get new value of ISR register after the flushing of TX data
               register.*/
            isr = idp->i2c->ISR;
            if ((isr & I2C_ISR_TXE) == 0U) {
                idp->i2c->ISR |= I2C_ISR_TXE;
            }
        } else {
            /* If defined, invoke error callback.*/
            if (idp->cb[I2C_CB_OPS_ERR] != NULL) {
                idp->cb[I2C_CB_OPS_ERR](idp);
            }

            I2C_OP_DONE(idp);
        }
    }

    /* Address match event.*/
    if (((cr1 & I2C_CR1_ADDRIE) != 0U) &&
        ((isr & I2C_ISR_ADDR) != 0U)) {

        /* Clear address matched flag.*/
        idp->i2c->ICR |= I2C_ICR_ADDRCF;

        /* Get new value of ISR register after the clearing of the address
           matched flag.*/
        /* isr = idp->i2c->ISR; */

        /* TODO: invoke address match callback.*/
    }
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void i2c_init(i2c_driver_t *idp) {

    /* Set pointer to I2C register block.*/
    idp->i2c  = i2c_dev_get_reg_ptr(idp);
    /* Set I2C clock value based on clocktree configuration.*/
    idp->clock = i2c_dev_get_clock(idp);

    /* Reset I2C parameters.*/
    i2c_reset_paramters(idp);
}

uint32_t i2c_set_prio(i2c_driver_t *idp, uint32_t prio) {

    uint32_t prev_prio;

    prev_prio = idp->prio;
    idp->prio = prio;

    return prev_prio;
}

i2c_drv_mode_t i2c_set_drv_mode(i2c_driver_t *idp, i2c_drv_mode_t drv_mode) {

    i2c_drv_mode_t prev_drv_mode;

    prev_drv_mode = idp->drv_mode;
    idp->drv_mode = drv_mode;

    return prev_drv_mode;
}

bool i2c_enable_dma(i2c_driver_t *idp, bool enable) {

    bool prev_is_dma_en;

    prev_is_dma_en = idp->is_dma_en;
    idp->is_dma_en = enable;

    return prev_is_dma_en;
}

void i2c_set_dma_conf(i2c_driver_t *idp, i2c_dma_conf_t *dma_conf) {

    idp->dma_conf.dma_stream_tx_id    = dma_conf->dma_stream_tx_id;
    idp->dma_conf.dma_stream_rx_id    = dma_conf->dma_stream_rx_id;
    idp->dma_conf.dma_stream_bus_prio = dma_conf->dma_stream_bus_prio;
    idp->dma_conf.dma_stream_irq_prio = dma_conf->dma_stream_irq_prio;
}

uint32_t i2c_set_timing(i2c_driver_t *idp, uint32_t timing) {

    uint32_t prev_timing;

    prev_timing = idp->timing;
    idp->timing = timing;

    return prev_timing;
}

i2c_oa1_mode_t i2c_set_own_address1_mode(i2c_driver_t *idp,
                                         i2c_oa1_mode_t oa1_mode) {

    i2c_oa1_mode_t prev_oa1_mode;

    prev_oa1_mode = idp->oa1_mode;
    idp->oa1_mode = oa1_mode;

    return prev_oa1_mode;
}

uint16_t i2c_set_own_address1(i2c_driver_t *idp, uint16_t oa1) {

    uint16_t prev_oa1;

    prev_oa1 = idp->oa1;
    idp->oa1 = oa1;

    return prev_oa1;
}

bool i2c_enable_own_address2(i2c_driver_t *idp, bool enable) {

    bool prev_is_oa2_en;

    prev_is_oa2_en = idp->is_oa2_en;
    idp->is_oa2_en = enable;

    return prev_is_oa2_en;
}

uint32_t i2c_set_own_address2(i2c_driver_t *idp, uint32_t oa2) {

    uint32_t prev_oa2;

    prev_oa2 = idp->oa2;
    idp->oa2 = oa2;

    return prev_oa2;
}

i2c_oa2_mask_t i2c_set_address2_mask(i2c_driver_t *idp, i2c_oa2_mask_t oa2_mask) {

    i2c_oa2_mask_t prev_oa2_mask;

    prev_oa2_mask = idp->oa2_mask;
    idp->oa2_mask = oa2_mask;

    return prev_oa2_mask;
}

bool i2c_enable_general_call(i2c_driver_t *idp, bool enable) {

    bool prev_is_gc_en;

    prev_is_gc_en = idp->is_gc_en;
    idp->is_gc_en = enable;

    return prev_is_gc_en;
}

bool i2c_enable_no_stretch(i2c_driver_t *idp, bool enable) {

    bool prev_is_no_stretch_en;

    prev_is_no_stretch_en = idp->is_no_stretch_en;
    idp->is_no_stretch_en = enable;

    return prev_is_no_stretch_en;
}

i2c_cb_t i2c_set_cb(i2c_driver_t *idp, i2c_cb_ops_t cb_op, i2c_cb_t cb) {

    i2c_cb_t prev_cb;

    prev_cb = idp->cb[cb_op];
    idp->cb[cb_op] = cb;

    return prev_cb;
}

void i2c_start(i2c_driver_t *idp) {

    IRQn_Type vector;
    uint32_t dma_tx_trigger, dma_rx_trigger;
    uint32_t dma_tx_mode, dma_rx_mode;

    /* Enable clock.*/
    i2c_dev_clock_enable(idp);

    /* Configure I2C interrupt.*/ 
    vector = i2c_dev_get_vector(idp);
    irq_set_priority(vector, idp->prio);
    irq_enable(vector); 

    /* Configure DMA.*/
    if (idp->is_dma_en == true) {
        /* Allocates DMA streams for I2C transmission/reception.*/
        idp->dma_tx_descriptor = dma_stream_take(idp->dma_conf.dma_stream_tx_id,
                                                 idp->dma_conf.dma_stream_irq_prio,
                                                 NULL,
                                                 NULL);
        idp->dma_rx_descriptor = dma_stream_take(idp->dma_conf.dma_stream_rx_id,
                                                 idp->dma_conf.dma_stream_irq_prio,
                                                 NULL,
                                                 NULL);

        /* Configure DMA stream peripheral addresses.*/
        dma_stream_set_peripheral(idp->dma_tx_descriptor, (uint32_t)(&idp->i2c->TXDR));
        dma_stream_set_peripheral(idp->dma_rx_descriptor, (uint32_t)(&idp->i2c->RXDR));

        /* Configure DMA TX/RX triggers.*/
        dma_tx_trigger = i2c_dev_get_dma_tx_trigger(idp);
        dma_rx_trigger = i2c_dev_get_dma_rx_trigger(idp);
        dma_stream_set_trigger(idp->dma_tx_descriptor, dma_tx_trigger);
        dma_stream_set_trigger(idp->dma_rx_descriptor, dma_rx_trigger);

        /* Configure DMA priority level, data transfer direction and enable
           transfer complete and transfer error interrupts.*/
        dma_tx_mode = DMA_CCR_PL_VALUE(idp->dma_conf.dma_stream_bus_prio) |
                      DMA_CCR_DIR_M2P | DMA_CCR_TCIE | DMA_CCR_TEIE;
        /* Configure DMA priority level, data transfer direction and enable
           transfer complete and transfer error interrupts.*/
        dma_rx_mode = DMA_CCR_PL_VALUE(idp->dma_conf.dma_stream_bus_prio) |
                      DMA_CCR_DIR_P2M | DMA_CCR_TCIE | DMA_CCR_TEIE;

        /* Configure DMA memory increment mode.*/
        dma_tx_mode |= DMA_CCR_MINC;
        dma_rx_mode |= DMA_CCR_MINC;

        /* Configure DMA frame size.*/
        dma_tx_mode |= DMA_CCR_PSIZE_BYTE | DMA_CCR_MSIZE_BYTE;
        dma_rx_mode |= DMA_CCR_PSIZE_BYTE | DMA_CCR_MSIZE_BYTE;

        /* Set up calculated DMA mode.*/
        dma_stream_set_transfer_mode(idp->dma_tx_descriptor, dma_tx_mode);
        dma_stream_set_transfer_mode(idp->dma_rx_descriptor, dma_rx_mode);
    }

    /* Configure I2C frequency.*/
    idp->i2c->TIMINGR = idp->timing;

    /* Disable own address 1 before setting the own address 1 configuration.*/
    idp->i2c->OAR1 &= ~I2C_OAR1_OA1EN;

    /* Configure own address 1 mode and set own addres 1.*/
    if (idp->oa1_mode == I2C_OA1_MODE_7BIT) {
        /* 7-bit own address 1.*/
        idp->i2c->OAR1 |= (I2C_OAR1_OA1EN | ((uint32_t)idp->oa1 << I2C_OAR1_OA1_Pos));
    } else {
        /* 10-bit own address 1.*/
        idp->i2c->OAR1 |= (I2C_OAR1_OA1EN | I2C_OAR1_OA1MODE | ((uint32_t)idp->oa1 << I2C_OAR1_OA1_Pos));
        idp->i2c->CR2  |= (I2C_CR2_ADD10);
    }
  
    /* Disable own address 2 before setting the own address 2 configuration.*/
    idp->i2c->OAR2 &= ~I2C_OAR2_OA2EN;

    /* Configure own address 2.*/
    if (idp->is_oa2_en == true) {
        idp->i2c->OAR2 = I2C_OAR2_OA2EN | ((uint32_t)idp->oa2_mask << I2C_OAR2_OA2MSK_Pos) | 
                         ((uint32_t)idp->oa2 << I2C_OAR2_OA2_Pos);
    } else {
        idp->i2c->OAR2 &= ~I2C_OAR2_OA2EN;
    }

    /* Configure general call.*/
    if (idp->is_gc_en == true) {
        idp->i2c->CR1 |= I2C_CR1_GCEN;
    } else {
        idp->i2c->CR1 &= ~I2C_CR1_GCEN;
    }

    /* Configure clock no-stretching.*/
    if (idp->is_no_stretch_en == true) {
        idp->i2c->CR1 |= I2C_CR1_NOSTRETCH;
    } else {
        idp->i2c->CR1 &= ~I2C_CR1_NOSTRETCH;
    }

    /* Enable I2C.*/
    idp->i2c->CR1 |= I2C_CR1_PE;  
}

uint16_t i2c_master_transmit(i2c_driver_t *idp, uint16_t address,
                             uint8_t *data, uint16_t size) {

    if (size > 0U) {
        I2C_OP_IN_PROGRESS(idp);

        /* Set I2C mode.*/
        idp->mode = I2C_MODE_MASTER_TX;

        if (idp->is_dma_en == true) {
            i2c_lld_master_transmit_dma(idp, address, data, size);  
        } else {
            i2c_lld_master_transmit(idp, address, data, size);
        }

        /* Wait for completion */
        I2C_WAIT_FOR_OP_COMPLETION(idp);
    }

    return size;
}

uint16_t i2c_master_receive(i2c_driver_t *idp, uint16_t address, uint8_t *data,
                            uint16_t size) {

    if (size > 0U) {
        I2C_OP_IN_PROGRESS(idp);

        /* Set I2C mode.*/
        idp->mode = I2C_MODE_MASTER_RX;

        if (idp->is_dma_en == true) {
           i2c_lld_master_receive_dma(idp, address, data, size); 
        } else {
            i2c_lld_master_receive(idp, address, data, size);
        }

        /* Wait for completion */
        I2C_WAIT_FOR_OP_COMPLETION(idp);
    }

    return size;
}

uint16_t i2c_slave_transmit(i2c_driver_t *idp, uint8_t *data, uint16_t size) {

    if (size > 0U) {
        I2C_OP_IN_PROGRESS(idp);

        /* Set I2C mode.*/
        idp->mode = I2C_MODE_SLAVE_TX;

        if (idp->is_dma_en == true) {
            i2c_lld_slave_transmit_dma(idp, data, size); 
        } else {
            i2c_lld_slave_transmit(idp, data, size);
        }

        /* Wait for completion */
        I2C_WAIT_FOR_OP_COMPLETION(idp);
    }

    return size;
}

uint16_t i2c_slave_receive(i2c_driver_t *idp, uint8_t *data, uint16_t size) {

    if (size > 0U) {
        I2C_OP_IN_PROGRESS(idp);

        /* Set I2C mode.*/
        idp->mode = I2C_MODE_SLAVE_RX;

        if (idp->is_dma_en == true) {
           i2c_lld_slave_receive_dma(idp, data, size); 
        } else {
            i2c_lld_slave_receive(idp, data, size);
        }

        /* Wait for completion */
        I2C_WAIT_FOR_OP_COMPLETION(idp);
    }

    return size;
}

void i2c_stop(i2c_driver_t *idp) {

    IRQn_Type vector;

    /* Disable I2C interrupt.*/
    vector = i2c_dev_get_vector(idp);
    irq_disable(vector);

    /* If DMA is disabled, disable DMA TX/RX streams.*/
    if (idp->is_dma_en == true) {
        /* Free and stop DMA TX stream.*/
        dma_stream_free(idp->dma_tx_descriptor);
        dma_stream_disable(idp->dma_tx_descriptor);
        /* Free and stop DMA RX stream.*/
        dma_stream_free(idp->dma_rx_descriptor);
        dma_stream_disable(idp->dma_rx_descriptor);
    }

    /* Set all registers to default values.*/
    idp->i2c->CR1     = 0x0U;
    idp->i2c->CR2     = 0x0U;
    idp->i2c->OAR1    = 0x0U;
    idp->i2c->OAR2    = 0x0U;
    idp->i2c->TIMINGR = 0x0U;
 
    /* Disable clock.*/
    i2c_dev_clock_disable(idp);
}

void i2c_deinit(i2c_driver_t *idp) {

    /* Set pointer to I2C register block to NULL.*/
    idp->i2c  = NULL;
    /* Set I2C clock value to default value.*/
    idp->clock = 0UL;

    /* Reset I2C parameters.*/
    i2c_reset_paramters(idp);
}

void __i2c_serve_ev_interrupt(i2c_driver_t *idp) {

    if ((idp->mode == I2C_MODE_MASTER_TX) ||
        (idp->mode == I2C_MODE_MASTER_RX)) {
        if (idp->is_dma_en == true) {
            i2c_lld_master_dma_isr(idp);
        } else {
            i2c_lld_master_isr(idp);
        }
    } else {
        if (idp->is_dma_en == true) {
            i2c_lld_slave_dma_isr(idp);
        } else {
            i2c_lld_slave_isr(idp);
        }
    }
}

void __i2c_serve_er_interrupt(i2c_driver_t *idp) {

    (void)idp;
    /* TODO: implement error management.*/
}
 
/** @} */
