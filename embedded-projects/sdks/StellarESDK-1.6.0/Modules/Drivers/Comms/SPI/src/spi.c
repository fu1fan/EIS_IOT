/****************************************************************************
 *
 * Copyright (c) 2022 STMicroelectronics - All Rights Reserved
 *
 * License terms: STMicroelectronics Proprietary in accordance with licensing
 * terms SLA0098 at www.st.com.
 *
 * THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
 * INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *****************************************************************************/
/**
 * @file    spi.c
 * @brief   SPI driver source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup COMMS
 * @ingroup DRIVERS
 * @addtogroup SPI
 * @ingroup COMMS
 * @{
 */

#include <spi.h>
#include <spi_private.h>

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

#define SPI_CR1_FIXED_BITS  (0U)
#define SPI_CR2_FIXED_BITS  (SPI_CR2_FRXTH)

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

static volatile uint32_t blackhole;
static volatile uint32_t tx_pattern;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Enters in transmit/receiving mode.
 * @note    Called when SPI driver enters transmit/receiving mode.
 *
 * @param[in] \_sdp\_   pointer to a @p spi_driver_t structure
 *
 * @notapi
 */
#define SPI_OP_IN_PROGRESS(_sdp_)                                   \
    osal_start_timeout(&((_sdp_)->timeout));                        \
    (_sdp_)->op_busy = SPI_OP_BUSY;

/**
 * @brief  Exits from transmit/receiving mode.
 * @note   Called when SPI driver exits transmit/receiving mode.
 *
 * @param[in] \_sdp\_   pointer to a @p spi_driver_t structure
 *
 * @notapi
 */
#define SPI_OP_DONE(_sdp_)                                          \
    (_sdp_)->op_busy = SPI_OP_READY;

/**
 * @brief  Waits for transmit/receiving end.
 * @note   Called to wait for end of transmit/receiving.
 *
 * @param[in] \_sdp\_   pointer to a @p spi_driver_t structure
 *
 * @notapi
 */
#define SPI_WAIT_FOR_OP_COMPLETION(_sdp_)                           \
    if (((_sdp_)->drv_mode == SPI_DRV_MODE_INT_SYNC) ||             \
        ((_sdp_)->drv_mode == SPI_DRV_MODE_DMA_SYNC)) {             \
        while ((_sdp_)->op_busy == SPI_OP_BUSY) {                   \
            if (osal_check_timeout((_sdp_)->timeout) == true) {     \
                (_sdp_)->status = SPI_STATUS_TIMEOUT;               \
                break;                                              \
            }                                                       \
        }                                                           \
    }

/**
 * @brief   Resets SPI parameters.
 *
 * @param[out] sdp      pointer to an @p spi_driver_t structure
 *
 * @notapi
 */
static void spi_reset_paramters(spi_driver_t *sdp) {

    uint8_t i;

    /* Set all driver configuration parameters to default values.*/
    sdp->tx_buf                       = NULL;
    sdp->rx_buf                       = NULL;
    sdp->rx_n_frames                  = 0U;
    sdp->tx_n_frames                  = 0U;
    sdp->op_busy                      = SPI_OP_READY;
    sdp->prio                         = IRQ_MIN_PRIORITY;
    sdp->drv_mode                     = SPI_DRV_MODE_INT_SYNC;
    sdp->dma_conf.dma_stream_rx_id    = DMA1_STREAM0_ID;
    sdp->dma_conf.dma_stream_tx_id    = DMA1_STREAM1_ID;
    sdp->dma_conf.dma_stream_bus_prio = DMA_PRIORITY_LOW;
    sdp->dma_conf.dma_stream_irq_prio = IRQ_MIN_PRIORITY;
    sdp->dma_tx_descriptor            = NULL;
    sdp->dma_rx_descriptor            = NULL;
    sdp->dma_tx_mode                  = 0U;
    sdp->dma_rx_mode                  = 0U;
    sdp->mode                         = SPI_MODE_MASTER;
    sdp->cmode                        = SPI_CMODE_FULL_DUPLEX;
    sdp->is_rxonly                    = false;
    sdp->baud                         = SPI_BAUDRATE_DIV2;
    sdp->cpha                         = SPI_CPHA_0;
    sdp->cpol                         = SPI_CPOL_0;
    sdp->fformat                      = SPI_FRAMEFORMAT_MSB;
    sdp->fsize                        = SPI_FRAMESIZE_8;
    sdp->nssp_mode                    = SPI_NSS_PULSE_MODE_OFF;
    sdp->nss_mode                     = SPI_NSS_MODE_HARDWARE;
    sdp->nss_gpio                     = gpio_iopack(GPIO_PORT_A, GPIO_PIN_0);
    sdp->timeout.start                = 0UL;
    sdp->timeout.value                = 0UL;
    sdp->status                       = SPI_STATUS_OK;

    for (i = 0U; i < (uint8_t)SPI_CB_OPS_MAX; i++) {
        sdp->cb[i] = NULL;
    }
}

/**
 * @brief   DMA callback (full duplex).
 *
 * @param[in] arg       argument associated to the callback
 * @param[in] sts       DMA callback cause status flags
 *
 * @notapi
 */
static void spi_dma_fd_callback(void *arg, uint32_t sts) {

    /*lint -e9087 */
    spi_driver_t *sdp = (spi_driver_t *)arg;
    /*lint +e9087 */

    /* Checking for errors.*/
    if ((sts & DMA_STS_TEIF) != 0U) {
        /* DMA critical error.*/
        if (sdp->cb[SPI_CB_OPS_ERR] != NULL) {
            sdp->cb[SPI_CB_OPS_ERR](sdp);
        }
    } else {
        if ((sts & DMA_STS_TCIF) != 0U) {
            /* If operation completed event callback is defined, it is invoked.*/
            if (sdp->cb[SPI_CB_OPS_DONE] != NULL) {
                sdp->cb[SPI_CB_OPS_DONE](sdp);
            }
        } else {
            ; /* close "if" as required by MISRA */
        }
    }

    /* Event flags must be cleared before to re-enable the stream. Since no
       interrupt is associated to TX stream, related event flags are cleared
       here, in the RX stream handler.*/
    dma_stream_clear_interrupts(sdp->dma_tx_descriptor);

    /* De-assert master chip select in multi-slave communication to stop the
       communication.*/
    if (sdp->mode == SPI_MODE_MASTER && sdp->nss_mode == SPI_NSS_MODE_SOFTWARE) {
         gpio_set_pin(sdp->nss_gpio);
    }

    SPI_OP_DONE(sdp);
}

/**
 * @brief   DMA RX callback (half duplex).
 *
 * @param[in] arg       argument associated to the callback
 * @param[in] sts       DMA callback cause status flags
 *
 * @notapi
 */
static void spi_dma_rx_hd_callback(void *arg, uint32_t sts) {

    /*lint -e9087 */
    spi_driver_t *sdp = (spi_driver_t *)arg;
    /*lint +e9087 */

    /* Disable DMA RX request.*/
    sdp->spi->CR2 &= ~SPI_CR2_RXDMAEN;

    /* Disable SPI in master mode receive.*/
    if (sdp->mode == SPI_MODE_MASTER) {
        sdp->spi->CR1 &= ~SPI_CR1_SPE;
    }

    /* Waiting BSY flag is clear.*/
    while ((sdp->spi->SR & SPI_SR_BSY) != 0U) {
        ;
    }

    /* In master receive mode flush RX FIFO.*/
    if (sdp->mode == SPI_MODE_MASTER) {
        while ((sdp->spi->SR & SPI_SR_FRLVL) != 0U) {
            (void)REG_READ(sdp->spi->DR);
        }
    }

    /* Checking for errors.*/
    if ((sts & DMA_STS_TEIF) != 0U) {
        /* DMA critical error.*/
        if (sdp->cb[SPI_CB_OPS_ERR] != NULL) {
            sdp->cb[SPI_CB_OPS_ERR](sdp);
        }
    } else {
        if ((sts & DMA_STS_TCIF) != 0U) {
            /* If operation completed event callback is defined, it is invoked.*/
            if (sdp->cb[SPI_CB_OPS_DONE] != NULL) {
                sdp->cb[SPI_CB_OPS_DONE](sdp);
            }
        } else {
            ; /* close "if" as required by MISRA */
        }
    }

    /* De-assert master chip select in multi-slave communication to stop the
       communication.*/
    if (sdp->mode == SPI_MODE_MASTER && sdp->nss_mode == SPI_NSS_MODE_SOFTWARE) {
         gpio_set_pin(sdp->nss_gpio);
    }

    SPI_OP_DONE(sdp);
}

/**
 * @brief   DMA TX callback (half duplex).
 *
 * @param[in] arg       argument associated to the callback
 * @param[in] sts       DMA callback cause status flags
 *
 * @notapi
 */
static void spi_dma_tx_hd_callback(void *arg, uint32_t sts) {

    /*lint -e9087 */
    spi_driver_t *sdp = (spi_driver_t *)arg;
    /*lint +e9087 */

    /* Disable DMA TX request.*/
    sdp->spi->CR2 &= ~SPI_CR2_TXDMAEN;

    /* Waiting for the TX FIFO to be emptied.*/
    while ((sdp->spi->SR & SPI_SR_FTLVL) != 0U) {
        ;
    }

    /* Waiting BSY flag is clear.*/
    while ((sdp->spi->SR & SPI_SR_BSY) != 0U) {
        ;
    }

    /* Checking for errors.*/
    if ((sts & DMA_STS_TEIF) != 0U) {
        /* DMA critical error.*/
        if (sdp->cb[SPI_CB_OPS_ERR] != NULL) {
            sdp->cb[SPI_CB_OPS_ERR](sdp);
        }
    } else {
        if ((sts & DMA_STS_TCIF) != 0U) {
            /* If operation completed event callback is defined, it is invoked.*/
            if (sdp->cb[SPI_CB_OPS_DONE] != NULL) {
                sdp->cb[SPI_CB_OPS_DONE](sdp);
            }
        } else {
            ; /* close "if" as required by MISRA */
        }
    }

    /* De-assert master chip select in multi-slave communication to stop the
       communication.*/
    if (sdp->mode == SPI_MODE_MASTER && sdp->nss_mode == SPI_NSS_MODE_SOFTWARE) {
         gpio_set_pin(sdp->nss_gpio);
    }

    SPI_OP_DONE(sdp);
}

/**
 * @brief   Shared interrupt handler (full duplex).
 * @note    Not an API, used internally.
 *
 * @param[in] sdp       pointer to a @p spi_driver_t structure
 *
 * @notapi
 */
__STATIC_INLINE void spi_lld_serve_interrupt_fd(spi_driver_t *sdp) {

    uint32_t sr, cr2;

    sr  = sdp->spi->SR;
    cr2 = sdp->spi->CR2;

    tx_pattern = 0xFFFFFFFFUL;

    /* TX buffer empty condition.*/
    if (((sr & SPI_SR_UDR) == 0U) &&
        ((sr & SPI_SR_TXE) != 0U) &&
        ((cr2 & SPI_CR2_TXEIE) != 0U)) {
        if (sdp->tx_buf != NULL) {
            /*lint -e9087 -e9005 */
            if (sdp->fsize <= SPI_FRAMESIZE_8) {
                *((uint8_t *)&sdp->spi->DR) = *sdp->tx_buf++;
            } else {
                sdp->spi->DR = *((uint16_t *)sdp->tx_buf);
                sdp->tx_buf += sizeof(uint16_t);
            }
        } else {
            if (sdp->fsize <= SPI_FRAMESIZE_8) {
                *((uint8_t *)&sdp->spi->DR) = (uint8_t)(tx_pattern);
            } else {
                sdp->spi->DR = (uint16_t)(tx_pattern);
            }
            /*lint +e9087 +e9005 */
        }

        sdp->tx_n_frames--;
        if (sdp->tx_n_frames == 0U) {
            sdp->spi->CR2 &= ~(SPI_CR2_TXEIE);
        }
    }

    /* RX buffer not empty condition.*/
    if (((sr & SPI_SR_OVR) == 0U) &&
        ((sr & SPI_SR_RXNE) != 0U) &&
        ((cr2 & SPI_CR2_RXNEIE) != 0U)) {
        if (sdp->rx_buf != NULL) {
            /*lint -e9087 -e9005 */
            if (sdp->fsize <= SPI_FRAMESIZE_8) {
                *sdp->rx_buf++ = (*((uint8_t *)&sdp->spi->DR));
            } else {
                *((uint16_t *)sdp->rx_buf) = (uint16_t)(sdp->spi->DR);
                sdp->rx_buf += sizeof(uint16_t);
            }
        } else {
            if (sdp->fsize <= SPI_FRAMESIZE_8) {
                blackhole = (*((uint8_t *)&sdp->spi->DR));
            } else {
                blackhole = (uint16_t)(sdp->spi->DR);
            }
            /*lint +e9087 +e9005 */
        }
        sdp->rx_n_frames--;
        if (sdp->rx_n_frames == 0U) {
            sdp->spi->CR2 &= ~(SPI_CR2_RXNEIE);
        }

    }

    /* Operation completed.*/
    if (sdp->tx_n_frames == 0U && sdp->rx_n_frames == 0U) {

        /* If operation completed event callback is defined, it is invoked.*/
        if (sdp->cb[SPI_CB_OPS_DONE] != NULL) {
            sdp->cb[SPI_CB_OPS_DONE](sdp);
        }

        /* De-assert master chip select in multi-slave communication to stop the
           communication.*/
        if (sdp->mode == SPI_MODE_MASTER && sdp->nss_mode == SPI_NSS_MODE_SOFTWARE) {
             gpio_set_pin(sdp->nss_gpio);
        }

        SPI_OP_DONE(sdp);
    }

    /* TODO: Add Error management.*/
}

/**
 * @brief   Shared interrupt handler (half duplex).
 * @note    Not an API, used internally.
 *
 * @param[in] sdp       pointer to a @p spi_driver_t structure
 *
 * @notapi
 */
__STATIC_INLINE void spi_lld_serve_interrupt_hd(spi_driver_t *sdp) {

    uint32_t sr, cr2;

    sr  = sdp->spi->SR;
    cr2 = sdp->spi->CR2;

    /* TX buffer empty condition.*/
    if (((sr & SPI_SR_UDR) == 0U) &&
        ((sr & SPI_SR_TXE) != 0U) &&
        ((cr2 & SPI_CR2_TXEIE) != 0U)) {
        if (sdp->tx_buf != NULL) {
            /*lint -e9087 -e9005 */
            if (sdp->fsize <= SPI_FRAMESIZE_8) {
                *((uint8_t *)&sdp->spi->DR) = *sdp->tx_buf++;
            } else {
                sdp->spi->DR = *((uint16_t *)sdp->tx_buf);
                sdp->tx_buf += sizeof(uint16_t);
            }
        }

        sdp->tx_n_frames--;
        if (sdp->tx_n_frames == 0U) {
            sdp->spi->CR2 &= ~(SPI_CR2_TXEIE);

            /* Waiting for the TX FIFO to be emptied.*/
            while ((sdp->spi->SR & SPI_SR_FTLVL) != 0U) {
                ;
            }

            /* Waiting BSY flag is clear.*/
            while ((sdp->spi->SR & SPI_SR_BSY) != 0U) {
                ;
            }

            /* If operation completed event callback is defined, it is invoked.*/
            if (sdp->cb[SPI_CB_OPS_DONE] != NULL) {
                sdp->cb[SPI_CB_OPS_DONE](sdp);
            }

            /* De-assert master chip select in multi-slave communication to stop the
               communication.*/
            if (sdp->mode == SPI_MODE_MASTER && sdp->nss_mode == SPI_NSS_MODE_SOFTWARE) {
                 gpio_set_pin(sdp->nss_gpio);
            }

            SPI_OP_DONE(sdp);
        }
    }

    /* RX buffer not empty condition.*/
    if (((sr & SPI_SR_OVR) == 0U) &&
        ((sr & SPI_SR_RXNE) != 0U) &&
        ((cr2 & SPI_CR2_RXNEIE) != 0U)) {
        if (sdp->rx_buf != NULL) {
            /*lint -e9087 -e9005 */
            if (sdp->fsize <= SPI_FRAMESIZE_8) {
                *sdp->rx_buf++ = (*((uint8_t *)&sdp->spi->DR));
            } else {
                *((uint16_t *)sdp->rx_buf) = (uint16_t)(sdp->spi->DR);
                sdp->rx_buf += sizeof(uint16_t);
            }
        }

        sdp->rx_n_frames--;
        if (sdp->rx_n_frames == 0U) {
            sdp->spi->CR2 &= ~(SPI_CR2_RXNEIE);

            /* Disable SPI in master mode receive.*/
            if (sdp->mode == SPI_MODE_MASTER) {
                sdp->spi->CR1 &= ~SPI_CR1_SPE;
            }

            /* Waiting BSY flag is clear.*/
            while ((sdp->spi->SR & SPI_SR_BSY) != 0U) {
                ;
            }

            /* In master receive mode flush RX FIFO.*/
            if (sdp->mode == SPI_MODE_MASTER) {
                while ((sdp->spi->SR & SPI_SR_FRLVL) != 0U) {
                    (void)REG_READ(sdp->spi->DR);
                }
            }

            /* If operation completed event callback is defined, it is invoked.*/
            if (sdp->cb[SPI_CB_OPS_DONE] != NULL) {
                sdp->cb[SPI_CB_OPS_DONE](sdp);
            }

            /* De-assert master chip select in multi-slave communication to stop the
               communication.*/
            if (sdp->mode == SPI_MODE_MASTER && sdp->nss_mode == SPI_NSS_MODE_SOFTWARE) {
                 gpio_set_pin(sdp->nss_gpio);
            }

            SPI_OP_DONE(sdp);
        }

    }

    /* TODO: Add Error management.*/
}

/**
 * @brief   Exchanges frames via SPI (full duplex).
 *
 * @param[in] sdp           pointer to a @p spi_driver_t structure
 * @param[in] tx_buf        pointer to the tx buffer to be sent
 * @param[out] rx_buf       pointer to the rx buffer to be received
 * @param[in] n_frames      maximum number of frames to be exchanged
 * @return                  number of frames exchanged.
 *
 * @notapi
 */
static uint32_t spi_lld_exchange_fd(spi_driver_t *sdp, const uint8_t *tx_buf,
                                    uint8_t *rx_buf, uint32_t n_frames) {

    SPI_OP_IN_PROGRESS(sdp);

    sdp->tx_buf = tx_buf;
    sdp->rx_buf = rx_buf;
    sdp->tx_n_frames = n_frames;
    sdp->rx_n_frames = n_frames;

    /* Interrupt mode exchange.*/ 
    if (sdp->drv_mode == SPI_DRV_MODE_INT_SYNC ||
        sdp->drv_mode == SPI_DRV_MODE_INT_ASYNC) {

        /* Assert master chip select in multi-slave communication to start the
           communication.*/
        if (sdp->mode == SPI_MODE_MASTER && sdp->nss_mode == SPI_NSS_MODE_SOFTWARE) {
            gpio_clear_pin(sdp->nss_gpio);
        }

        /* Enable RX/TX interrupts.*/
        sdp->spi->CR2 |= SPI_CR2_RXNEIE | SPI_CR2_TXEIE;

    /* DMA mode exchange.*/ 
    } else {
        /* Configure DMA RX/TX number of data items.*/
        dma_stream_set_count(sdp->dma_tx_descriptor, sdp->tx_n_frames);
        dma_stream_set_count(sdp->dma_rx_descriptor, sdp->rx_n_frames);

        /* DMA modes depending on buffers organization.*/
        if (sdp->rx_buf != NULL) {
            dma_stream_set_memory(sdp->dma_rx_descriptor, (uint32_t)(sdp->rx_buf));

            /* Enable DMA memory increment mode.*/
            sdp->dma_rx_mode |= DMA_CCR_MINC;
            /* Setting up calculated RX DMA modes.*/
            dma_stream_set_transfer_mode(sdp->dma_rx_descriptor, sdp->dma_rx_mode);
        } else {
            blackhole = 0U; /* initialized as required by MISRA since it cannot be declared const.*/
            dma_stream_set_memory(sdp->dma_rx_descriptor, (uint32_t)(&blackhole));

            /* Disable DMA memory increment mode.*/
            sdp->dma_rx_mode &= ~DMA_CCR_MINC;
            /* Setting up calculated RX DMA modes.*/
            dma_stream_set_transfer_mode(sdp->dma_rx_descriptor, sdp->dma_rx_mode);
            (void)(blackhole); /* declared void as required by MISRA since its value is not used.*/
        }
        if (sdp->tx_buf != NULL) {
            dma_stream_set_memory(sdp->dma_tx_descriptor, (uint32_t)(sdp->tx_buf));

            /* Enable DMA memory increment mode.*/
            sdp->dma_tx_mode |= DMA_CCR_MINC;
            /* Setting up calculated TX DMA modes.*/
            dma_stream_set_transfer_mode(sdp->dma_tx_descriptor, sdp->dma_tx_mode);
        } else {
            tx_pattern = 0xFFFFFFFFUL; /* initialized as required by MISRA since it cannot be declared const.*/
            dma_stream_set_memory(sdp->dma_tx_descriptor, (uint32_t)(&(tx_pattern)));
            (void)(tx_pattern); /* declared void as required by MISRA since its value is not used.*/

            /* Disable DMA memory increment mode.*/
            sdp->dma_tx_mode &= ~DMA_CCR_MINC;
            /* Setting up calculated TX DMA modes.*/
            dma_stream_set_transfer_mode(sdp->dma_tx_descriptor, sdp->dma_tx_mode);
        }

        /* Assert master chip select in multi-slave communication to start the
           communication.*/
        if (sdp->mode == SPI_MODE_MASTER && sdp->nss_mode == SPI_NSS_MODE_SOFTWARE) {
            gpio_clear_pin(sdp->nss_gpio);
        }

        /* Starting DMAs.*/
        dma_stream_enable(sdp->dma_rx_descriptor);
        dma_stream_enable(sdp->dma_tx_descriptor);
    }

    SPI_WAIT_FOR_OP_COMPLETION(sdp);

    return n_frames;
}

/**
 * @brief   Sends frames via SPI (half duplex).
 *
 * @param[in] sdp           pointer to a @p spi_driver_t structure
 * @param[in] tx_buf        pointer to the tx buffer to be sent
 * @param[in] n_frames      maximum number of frames to be sent
 * @return                  number of frames sent.
 *
 * @notapi
 */
static uint32_t spi_lld_send_hd(spi_driver_t *sdp, const uint8_t *tx_buf,
                                uint32_t n_frames) {

    SPI_OP_IN_PROGRESS(sdp);

    sdp->tx_buf = tx_buf;
    sdp->tx_n_frames = n_frames;

    sdp->spi->CR1 &= ~SPI_CR1_SPE;
    sdp->spi->CR1 |= SPI_CR1_BIDIOE;

    /* Reset is_rxonly flag (it is set only if the SPI is configured as master
       receive in half duplex mode.*/
    sdp->is_rxonly = false;

    /* Interrupt mode send half duplex.*/ 
    if (sdp->drv_mode == SPI_DRV_MODE_INT_SYNC ||
        sdp->drv_mode == SPI_DRV_MODE_INT_ASYNC) {
        /* Enable TX interrupt.*/
        sdp->spi->CR2 |= SPI_CR2_TXEIE;

        /* If disabled, enable SPI.*/
        if ((sdp->spi->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE) {
            sdp->spi->CR1 |= SPI_CR1_SPE;
        }
    /* DMA mode send half duplex.*/ 
    } else {
        /* Configure DMA TX number of data items.*/
        dma_stream_set_count(sdp->dma_tx_descriptor, sdp->tx_n_frames);

        /* Configure DMA TX stream memory address.*/
        dma_stream_set_memory(sdp->dma_tx_descriptor, (uint32_t)(sdp->tx_buf));

        /* Assert master chip select in multi-slave communication to start the
           communication.*/
        if (sdp->mode == SPI_MODE_MASTER && sdp->nss_mode == SPI_NSS_MODE_SOFTWARE) {
            gpio_clear_pin(sdp->nss_gpio);
        }

        /* If disabled, enable SPI.*/
        if ((sdp->spi->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE) {
            sdp->spi->CR1 |= SPI_CR1_SPE;
        }

        /* Enable DMA stream.*/
        dma_stream_enable(sdp->dma_tx_descriptor);

        /* Enable DMA TX request generation whenever TXE is set.*/
        sdp->spi->CR2 |= SPI_CR2_TXDMAEN;
    }

    SPI_WAIT_FOR_OP_COMPLETION(sdp);

    return n_frames;
}

/**
 * @brief   Receives frames via SPI (half duplex).
 *
 * @param[in] sdp           pointer to a @p spi_driver_t structure
 * @param[out] rx_buf       pointer to the rx buffer to be received
 * @param[in] n_frames      maximum number of frames to be received
 * @return                  number of frames received.
 *
 * @notapi
 */
static uint32_t spi_lld_receive_hd(spi_driver_t *sdp, uint8_t *rx_buf,
                                   uint32_t n_frames) {

    SPI_OP_IN_PROGRESS(sdp);

    sdp->rx_buf = rx_buf;
    sdp->rx_n_frames = n_frames;

    sdp->spi->CR1 &= ~SPI_CR1_SPE;
    sdp->spi->CR1 &= ~SPI_CR1_BIDIOE;

    /* Set is_rxonly flag if the SPI is configured as master receive in half
       duplex mode (in this mode a specific stop procedure is required).*/
    if (sdp->mode == SPI_MODE_MASTER) {
        sdp->is_rxonly = true;
    }

    /* Interrupt mode send half duplex.*/ 
    if (sdp->drv_mode == SPI_DRV_MODE_INT_SYNC ||
        sdp->drv_mode == SPI_DRV_MODE_INT_ASYNC) {
        /* Enable RX interrupt.*/
        sdp->spi->CR2 |= SPI_CR2_RXNEIE;

        /* If disabled, enable SPI.*/
        if ((sdp->spi->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE) {
            sdp->spi->CR1 |= SPI_CR1_SPE;
        }
    /* DMA mode send half duplex.*/ 
    } else {
        /* Configure DMA RX number of data items.*/
        dma_stream_set_count(sdp->dma_rx_descriptor, sdp->rx_n_frames);

        /* Configure DMA TX stream memory address.*/
        dma_stream_set_memory(sdp->dma_rx_descriptor, (uint32_t)(sdp->rx_buf));

        /* Assert master chip select in multi-slave communication to start the
           communication.*/
        if (sdp->mode == SPI_MODE_MASTER && sdp->nss_mode == SPI_NSS_MODE_SOFTWARE) {
            gpio_clear_pin(sdp->nss_gpio);
        }

        /* If disabled, enable SPI.*/
        if ((sdp->spi->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE) {
            sdp->spi->CR1 |= SPI_CR1_SPE;
        }

        /* Enable DMA stream.*/
        dma_stream_enable(sdp->dma_rx_descriptor);

        /* Enable DMA RX request generation whenever RXNE is set.*/
        sdp->spi->CR2 |= SPI_CR2_RXDMAEN;
    }

    SPI_WAIT_FOR_OP_COMPLETION(sdp);

    return n_frames;
}

/**
 * @brief   Stops a SPI instance (except in receive only mode).
 *
 * @param[in] sdp       pointer to an @p spi_driver_t structure
 *
 * @notapi
 */
static void spi_lld_stop(spi_driver_t *sdp) {

    /* Wait for the TX FIFO to be emptied.*/
    while ((sdp->spi->SR & SPI_SR_FTLVL) != 0U) {
        ;
    }

    /* Wait the last data frame is processed.*/
    while ((sdp->spi->SR & SPI_SR_BSY) != 0U) {
        ;
    }

    /* Disable SPI.*/
    sdp->spi->CR1 &= ~SPI_CR1_SPE;

    /* Read data untill RX FIFO is emptied.*/
    while ((sdp->spi->SR & SPI_SR_FRLVL) != 0U) {
        (void)REG_READ(sdp->spi->DR);
    }
}

/**
 * @brief   Stops a SPI instance (receive only mode).
 *
 * @param[in] sdp       pointer to an @p spi_driver_t structure
 *
 * @notapi
 */
static void spi_lld_stop_rx_only(spi_driver_t *sdp) {

    /* Disable SPI.*/
    sdp->spi->CR1 &= ~SPI_CR1_SPE;

    /* Wait the last data frame is processed.*/
    while ((sdp->spi->SR & SPI_SR_BSY) != 0U) {
        ;
    }

    /* Read data untill RX FIFO is emptied.*/
    while ((sdp->spi->SR & SPI_SR_FRLVL) != 0U) {
        (void)REG_READ(sdp->spi->DR);
    }
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void spi_init(spi_driver_t *sdp) {

    /* Set pointer to SPI register block.*/
    sdp->spi   = spi_dev_get_reg_ptr(sdp);
    /* Set SPI clock value based on clocktree configuration.*/
    sdp->clock = spi_dev_get_clock(sdp);

    /* Reset SPI parameters.*/
    spi_reset_paramters(sdp);
}

uint32_t spi_set_prio(spi_driver_t *sdp, uint32_t prio) {

    uint32_t prev_prio;

    prev_prio = sdp->prio;
    sdp->prio = prio;

    return prev_prio;
}

spi_drv_mode_t spi_set_drv_mode(spi_driver_t *sdp, spi_drv_mode_t drv_mode) {

    spi_drv_mode_t prev_drv_mode;

    prev_drv_mode = sdp->drv_mode;
    sdp->drv_mode = drv_mode;

    return prev_drv_mode;
}

void spi_set_dma_conf(spi_driver_t *sdp, spi_dma_conf_t *dma_conf) {

    sdp->dma_conf.dma_stream_rx_id    = dma_conf->dma_stream_rx_id;
    sdp->dma_conf.dma_stream_tx_id    = dma_conf->dma_stream_tx_id;
    sdp->dma_conf.dma_stream_bus_prio = dma_conf->dma_stream_bus_prio;
    sdp->dma_conf.dma_stream_irq_prio = dma_conf->dma_stream_irq_prio;
}

spi_mode_t spi_set_mode(spi_driver_t *sdp, spi_mode_t mode) {

    spi_mode_t prev_mode;

    prev_mode = sdp->mode;
    sdp->mode = mode;

    return prev_mode;
}

spi_cmode_t spi_set_cmode(spi_driver_t *sdp, spi_cmode_t cmode) {

    spi_cmode_t prev_cmode;

    prev_cmode = sdp->cmode;
    sdp->cmode = cmode;

    return prev_cmode;
}

spi_baud_t spi_set_baud(spi_driver_t *sdp, spi_baud_t baud) {

    spi_baud_t prev_baud;

    prev_baud = sdp->baud;
    sdp->baud = baud;

    return prev_baud;
}

spi_cpha_t spi_set_cpha(spi_driver_t *sdp, spi_cpha_t cpha) {

    spi_cpha_t prev_cpha;

    prev_cpha = sdp->cpha;
    sdp->cpha = cpha;

    return prev_cpha;
}

spi_cpol_t spi_set_cpol(spi_driver_t *sdp, spi_cpol_t cpol) {

    spi_cpol_t prev_cpol;

    prev_cpol = sdp->cpol;
    sdp->cpol = cpol;

    return prev_cpol;
}

spi_fformat_t spi_set_fformat(spi_driver_t *sdp, spi_fformat_t fformat) {

    spi_fformat_t prev_fformat;

    prev_fformat = sdp->fformat;
    sdp->fformat = fformat;

    return prev_fformat;
}

spi_fsize_t spi_set_fsize(spi_driver_t *sdp, spi_fsize_t fsize) {

    spi_fsize_t prev_fsize;

    prev_fsize = sdp->fsize;
    sdp->fsize = fsize;

    return prev_fsize;
}

spi_nssp_mode_t spi_set_nssp_mode(spi_driver_t *sdp, spi_nssp_mode_t nssp_mode) {

    spi_nssp_mode_t prev_nssp_mode;

    prev_nssp_mode = sdp->nssp_mode;
    sdp->nssp_mode = nssp_mode;

    return prev_nssp_mode;
}

spi_nss_mode_t spi_set_nss_mode(spi_driver_t *sdp, spi_nss_mode_t nss_mode) {

    spi_nss_mode_t prev_nss_mode;

    prev_nss_mode = sdp->nss_mode;
    sdp->nss_mode = nss_mode;

    return prev_nss_mode;
}

gpio_io_t spi_set_nss_gpio(spi_driver_t *sdp, gpio_io_t nss_gpio) {

    gpio_io_t prev_nss_gpio;

    prev_nss_gpio = sdp->nss_gpio;
    sdp->nss_gpio = nss_gpio;

    /* De-assert master chip select in multi-slave communication to guarantee
       the slave is deselected.*/
    gpio_set_pin(sdp->nss_gpio);

    return prev_nss_gpio;
}

uint32_t spi_set_timeout(spi_driver_t *sdp, uint32_t timeout) {

    uint32_t prev_timeout;

    prev_timeout = osal_tk2ms(sdp->timeout.value);
    sdp->timeout.value = osal_ms2tk(timeout);

    return prev_timeout;
}

spi_cb_t spi_set_cb(spi_driver_t *sdp, spi_cb_ops_t cb_op, spi_cb_t cb) {

    spi_cb_t prev_cb;

    prev_cb = sdp->cb[cb_op];
    sdp->cb[cb_op] = cb;

    return prev_cb;
}

void spi_start(spi_driver_t *sdp) {

    uint32_t dma_tx_trigger, dma_rx_trigger;

    uint32_t cr1 = 0U;
    uint32_t cr2 = 0U;

    IRQn_Type vector;

    /* Enable clock.*/
    spi_dev_clock_enable(sdp);

    /* If interrupt mode is selected, configure SPI interrupt...*/
    if (sdp->drv_mode == SPI_DRV_MODE_INT_SYNC ||
        sdp->drv_mode == SPI_DRV_MODE_INT_ASYNC) {
        vector = spi_dev_get_vector(sdp);
        irq_set_priority(vector, sdp->prio);
        irq_enable(vector);
    /* ...otherwise configure DMA.*/
    } else {
        /* Allocates DMA streams for SPI transmission/reception.*/
        if (sdp->cmode == SPI_CMODE_FULL_DUPLEX) {
            /*lint -e9087 */
            sdp->dma_rx_descriptor = dma_stream_take(sdp->dma_conf.dma_stream_rx_id,
                                                     sdp->dma_conf.dma_stream_irq_prio,
                                                     spi_dma_fd_callback,
                                                     (void *)sdp);
            /*lint +e9087 */

            sdp->dma_tx_descriptor = dma_stream_take(sdp->dma_conf.dma_stream_tx_id,
                                                     sdp->dma_conf.dma_stream_irq_prio,
                                                     NULL,
                                                     NULL);
        } else {
            /* Allocates DMA streams for SPI transmission/reception.*/
            /*lint -e9087 */
            sdp->dma_rx_descriptor = dma_stream_take(sdp->dma_conf.dma_stream_rx_id,
                                                     sdp->dma_conf.dma_stream_irq_prio,
                                                     spi_dma_rx_hd_callback,
                                                     (void *)sdp);

            sdp->dma_tx_descriptor = dma_stream_take(sdp->dma_conf.dma_stream_tx_id,
                                                     sdp->dma_conf.dma_stream_irq_prio,
                                                     spi_dma_tx_hd_callback,
                                                     (void *)sdp);
            /*lint +e9087 */
        }

        /* Configure DMA stream peripheral addresses.*/
        dma_stream_set_peripheral(sdp->dma_rx_descriptor, (uint32_t)(&sdp->spi->DR));
        dma_stream_set_peripheral(sdp->dma_tx_descriptor, (uint32_t)(&sdp->spi->DR));

        /* Configure DMA triggers.*/
        dma_tx_trigger = spi_dev_get_dma_tx_trigger(sdp);
        dma_stream_set_trigger(sdp->dma_tx_descriptor, dma_tx_trigger);
        dma_rx_trigger = spi_dev_get_dma_rx_trigger(sdp);
        dma_stream_set_trigger(sdp->dma_rx_descriptor, dma_rx_trigger);

        /* Configure DMA priority level, data transfer direction and enable
           transfer complete and transfer error interrupts.*/
        sdp->dma_tx_mode = DMA_CCR_PL_VALUE(sdp->dma_conf.dma_stream_bus_prio) |
                           DMA_CCR_DIR_M2P | DMA_CCR_TCIE | DMA_CCR_TEIE;
        sdp->dma_rx_mode = DMA_CCR_PL_VALUE(sdp->dma_conf.dma_stream_bus_prio) |
                           DMA_CCR_DIR_P2M | DMA_CCR_TCIE | DMA_CCR_TEIE;

        /* Configure DMA frame size depending on the selected SPI frame size.*/
        if (sdp->fsize <= SPI_FRAMESIZE_8) {
            /* Frame width is 8 bits or smaller.*/
            sdp->dma_rx_mode |= DMA_CCR_PSIZE_BYTE | DMA_CCR_MSIZE_BYTE;
            sdp->dma_tx_mode |= DMA_CCR_PSIZE_BYTE | DMA_CCR_MSIZE_BYTE;
        } else {
            /* Frame width is larger than 8 bits.*/
            sdp->dma_rx_mode |= DMA_CCR_PSIZE_HWORD | DMA_CCR_MSIZE_HWORD;
            sdp->dma_tx_mode |= DMA_CCR_PSIZE_HWORD | DMA_CCR_MSIZE_HWORD;
        }

        if (sdp->cmode == SPI_CMODE_FULL_DUPLEX) {
            cr2 |= SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;
        } else {
            /* Enable DMA memory increment mode.*/
            sdp->dma_rx_mode |= DMA_CCR_MINC;
            sdp->dma_tx_mode |= DMA_CCR_MINC;

            /* Setting up calculated DMA modes.*/
            dma_stream_set_transfer_mode(sdp->dma_rx_descriptor, sdp->dma_rx_mode);
            dma_stream_set_transfer_mode(sdp->dma_tx_descriptor, sdp->dma_tx_mode);
        }
    }

    /* Configure SPI clock phase, clock polarity, master selection, baudrate
       and frame format.*/
    cr1 |= (((uint32_t)(sdp->fformat) << SPI_CR1_LSBFIRST_Pos) |
            ((uint32_t)(sdp->baud)    << SPI_CR1_BR_Pos)       |
            ((uint32_t)(sdp->mode)    << SPI_CR1_MSTR_Pos)     |
            ((uint32_t)(sdp->cpol)    << SPI_CR1_CPOL_Pos)     |
            ((uint32_t)(sdp->cpha)    << SPI_CR1_CPHA_Pos));


    /* Configure SPI frame size.*/
    cr2 |= ((uint32_t)(sdp->fsize)) << SPI_CR2_DS_Pos;

    /* Configure SPI chip select pulse mode (the configuration is taken into
       account only for master mode and software slave management disabled).*/
    if (sdp->mode == SPI_MODE_MASTER            &&
        sdp->nssp_mode == SPI_NSS_PULSE_MODE_ON &&
        sdp->nss_mode != SPI_NSS_MODE_SOFTWARE) {
        cr2 |= SPI_CR2_NSSP;
    }

    /* Enable SS in master mode (multimaster not supported).*/
    if (sdp->mode == SPI_MODE_MASTER) {
        cr2 |= SPI_CR2_SSOE;
    }

    if (sdp->cmode == SPI_CMODE_FULL_DUPLEX) {
        /* In half duplex mode enable SPI.*/
        cr1 |= SPI_CR1_SPE;
    } else {
        /* In half duplex mode select 1-line bidirectional data mode.*/
        cr1 |= SPI_CR1_BIDIMODE;
    }

    /* Configure SPI.*/
    sdp->spi->CR2 = cr2 | SPI_CR2_FIXED_BITS;
    sdp->spi->CR1 = cr1 | SPI_CR1_FIXED_BITS;

    /* Enable software slave management for multi-slave communication.*/
    if (sdp->mode == SPI_MODE_MASTER && sdp->nss_mode == SPI_NSS_MODE_SOFTWARE) {
        sdp->spi->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
    }
}

uint32_t spi_exchange(spi_driver_t *sdp, const uint8_t *tx_buf,
                      uint8_t *rx_buf, uint32_t n_frames) {

    sdp->status = SPI_STATUS_OK;

    if (sdp->cmode == SPI_CMODE_FULL_DUPLEX) {
        return spi_lld_exchange_fd(sdp, tx_buf, rx_buf, n_frames);
    } else {
        return 0U;
    }
}

uint32_t spi_send(spi_driver_t *sdp, const uint8_t *tx_buf, uint32_t n_frames) {

    sdp->status = SPI_STATUS_OK;

    if (sdp->cmode == SPI_CMODE_FULL_DUPLEX) {
        return spi_lld_exchange_fd(sdp, tx_buf, NULL, n_frames);
    } else {
        return spi_lld_send_hd(sdp, tx_buf, n_frames);
    }
}

uint32_t spi_receive(spi_driver_t *sdp, uint8_t *rx_buf, uint32_t n_frames) {

    sdp->status = SPI_STATUS_OK;

    if (sdp->cmode == SPI_CMODE_FULL_DUPLEX) {
        return spi_lld_exchange_fd(sdp, NULL, rx_buf, n_frames);
    } else {
        return spi_lld_receive_hd(sdp, rx_buf, n_frames);
    }
}

spi_status_t spi_get_status(spi_driver_t *sdp) {

    return sdp->status;
}

void spi_stop(spi_driver_t *sdp) {

    IRQn_Type vector;

    if (sdp->is_rxonly == true) {
        spi_lld_stop_rx_only(sdp);
    } else {
        spi_lld_stop(sdp);
    }

    if (sdp->drv_mode == SPI_DRV_MODE_INT_SYNC ||
        sdp->drv_mode == SPI_DRV_MODE_INT_ASYNC) {
        /* Disable interrupt.*/
        vector = spi_dev_get_vector(sdp);
        irq_disable(vector);
    } else {
        /* Freeing and stopping DMA.*/
        dma_stream_free(sdp->dma_tx_descriptor);
        dma_stream_free(sdp->dma_rx_descriptor);
        dma_stream_disable(sdp->dma_tx_descriptor);
        dma_stream_disable(sdp->dma_rx_descriptor);
    }

    /* Disable clock.*/
    spi_dev_clock_disable(sdp);
}

void spi_deinit(spi_driver_t *sdp) {

    /* Set pointer to SPI register block to NULL.*/
    sdp->spi   = NULL;
    /* Set SPI clock value to default value.*/
    sdp->clock = 0UL;

    /* Reset SPI parameters.*/
    spi_reset_paramters(sdp);
}

void __spi_serve_interrupt(spi_driver_t *sdp) {

    if (sdp->cmode == SPI_CMODE_FULL_DUPLEX) {
        spi_lld_serve_interrupt_fd(sdp);
    } else {
        spi_lld_serve_interrupt_hd(sdp);
    }
}

/** @} */
