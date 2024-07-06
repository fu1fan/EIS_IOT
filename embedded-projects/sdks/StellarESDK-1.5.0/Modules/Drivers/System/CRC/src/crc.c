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
 * @file    crc.c
 * @brief   CRC driver source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup CRC
 * @ingroup SYSTEM
 * @{
 */

#include <crc.h>
#include <crc_private.h>

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
 * @brief  Exits from calculation mode.
 * @note   Called when CRC driver exits from calculation mode.
 *
 * @param[in] \_cdp\_   pointer to a @p crc_driver_t structure
 *
 * @notapi
 */
#define CRC_OP_DONE(_cdp_)                                 \
    (_cdp_)->op_busy = CRC_OP_READY

/**
 * @brief  Enters in calculation mode.
 * @note   Called when CRC driver enters in calculation mode.
 *
 * @param[in] \_cdp\_   pointer to a @p crc_driver_t structure
 *
 * @notapi
 */
#define CRC_OP_IN_PROGRESS(_cdp_)                          \
    (_cdp_)->op_busy = CRC_OP_BUSY

/**
 * @brief  Waits for DMA calculation end.
 * @note   Called to wait for end of calculation.
 *
 * @param[in] \_cdp\_   pointer to a @p crc_driver_t structure
  *
 * @notapi
 */
#define CRC_WAIT_FOR_OP_COMPLETION(_cdp_)                  \
    if ((_cdp_)->drv_mode == CRC_DRV_MODE_SYNC) {          \
        while ((_cdp_)->op_busy == CRC_OP_BUSY) {;}        \
    }

/**
 * @brief   Resets CRC parameters.
 *
 * @param[out] cdp      pointer to a @p crc_driver_t structure
 *
 * @notapi
 */
static void crc_reset_parameters(crc_driver_t *cdp) {

    cdp->drv_mode                     = CRC_DRV_MODE_SYNC;
    cdp->op_busy                      = CRC_OP_READY;
    cdp->init                         = 0xFFFFFFFFU;
    cdp->rev_in_mode                  = CRC_REV_IN_NONE;
    cdp->rev_out_mode                 = CRC_REV_OUT_NONE;
    cdp->polysize                     = CRC_POLYSIZE_32;
    cdp->is_dma_en                    = false;
    cdp->dma_conf.dma_stream_id       = DMA1_STREAM0_ID;
    cdp->dma_conf.dma_stream_bus_prio = DMA_PRIORITY_LOW;
    cdp->dma_conf.dma_stream_irq_prio = IRQ_MIN_PRIORITY;
    cdp->dma_descriptor               = NULL;
    cdp->cb                           = NULL;
}

/**
 * @brief   Sets the CRC parameters
 * 
 * @param[out] cdp      pointer to a @p crc_driver_t structure
 *
 * @notapi
 */
static void crc_set_parameters(crc_driver_t *cdp) {

    /* Set init value.*/
    cdp->crc->INIT = cdp->init;

    /* Set rev in mode.*/
    switch(cdp->rev_in_mode) {
        case CRC_REV_IN_WORD:
            cdp->crc->CR |= CRC_CR_REV_IN_1 | CRC_CR_REV_IN_0;
            break;
        case CRC_REV_IN_HWORD:
            cdp->crc->CR |= CRC_CR_REV_IN_1;
            break;
        case CRC_REV_IN_BYTE:
            cdp->crc->CR |= CRC_CR_REV_IN_0;
            break;
        case CRC_REV_IN_NONE:
            cdp->crc->CR &= ~(CRC_CR_REV_IN_1 | CRC_CR_REV_IN_0);
            break;
        default:
             /* default case as required by MISRA */
            break;
    }

    /* Set rev out mode.*/
    switch (cdp->rev_out_mode) {
        case CRC_REV_OUT_BIT:
            cdp->crc->CR |= CRC_CR_REV_OUT;
            break;
        case CRC_REV_OUT_NONE:
            cdp->crc->CR &= ~CRC_CR_REV_OUT;
            break;
        default:
            /* default case as required by MISRA */
            break;
    }

    /* Set polysize mode.*/
    switch (cdp->polysize) {
        case CRC_POLYSIZE_32:
            cdp->crc->CR &= ~(CRC_CR_POLYSIZE_1 | CRC_CR_POLYSIZE_0);
            break;
        case CRC_POLYSIZE_16:
            cdp->crc->CR |= CRC_CR_POLYSIZE_0;
            break;
        case CRC_POLYSIZE_8:
            cdp->crc->CR |= CRC_CR_POLYSIZE_1;
            break;
        case CRC_POLYSIZE_7:
            cdp->crc->CR |= CRC_CR_POLYSIZE_1 | CRC_CR_POLYSIZE_0;
            break;
        default:
            /* default case as required by MISRA */
            break;
    }
}

/**
 * @brief   Write 8 bit in the data register.
 * 
 * @param[out] cdp      pointer to a @p crc_driver_t structure
 * @param[in] data      data to be written
 *
 * @notapi
 */
static void crc_write8bit(crc_driver_t *cdp, uint8_t data) {

    volatile uint8_t *pReg;

    pReg = (volatile uint8_t *)(&cdp->crc->DR);
    *pReg = data;
}

/**
 * @brief   Write 16 bit in the data register.
 *
 * @param[out] cdp      pointer to a @p crc_driver_t structure
 * @param[in] data      data to be written
 *
 * @notapi
 */
static void crc_write16bit(crc_driver_t *cdp, uint16_t data) {

    volatile uint16_t *pReg;

    /*lint -e9087 -e740 */
    pReg = (volatile uint16_t *)(&cdp->crc->DR);
    *pReg = data;
    /*lint +e9087 +e740 */
}

/**
 * @brief   Write 32 bit in the data register.
 *
 * @param[out] cdp      pointer to a @p crc_driver_t structure
 * @param[in] data      data to be written
 *
 * @notapi
 */
static void crc_write32bit(crc_driver_t *cdp, uint32_t data) {

    cdp->crc->DR = data;
}

/**
 * @brief   DMA interrupt handler.
 *
 * @param[in] arg       argument associated to the callback
 * @param[in] sts       DMA callback cause status flags
 *
 * @isr
 */
static void crc_serve_dma_interrupt(void *arg, uint32_t sts) {

    (void)sts;

    /*lint -e9087 */
    crc_driver_t *cdp = (crc_driver_t *)arg;
    /*lint +e9087 */

    /* Trasmission completed.*/
    if ((sts & DMA_STS_TCIF) != 0U) {
        if (cdp->cb != NULL) {
            cdp->cb(cdp);;
        } else {
            ; /* close "if" as required by MISRA */
        }
    } else {
        ; /* close "if" as required by MISRA */
    }

    CRC_OP_DONE(cdp);
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void crc_init(crc_driver_t *cdp) {

    /* Set pointer to CRC register block.*/
    cdp->crc = crc_dev_get_reg_ptr(cdp);
}

crc_drv_mode_t crc_set_drv_mode(crc_driver_t *cdp, crc_drv_mode_t drv_mode) {

    crc_drv_mode_t prev_drv_mode;

    prev_drv_mode = cdp->drv_mode;
    cdp->drv_mode = drv_mode;

    return prev_drv_mode;
}

uint32_t crc_set_init(crc_driver_t *cdp, uint32_t init_value) {

    uint32_t prev_init;

    prev_init = cdp->init;
    cdp->init = init_value;

    return prev_init;
}

crc_rev_in_t crc_set_rev_in(crc_driver_t *cdp, crc_rev_in_t rev_in) {

    crc_rev_in_t prev_rev_in_mode;

    prev_rev_in_mode = cdp->rev_in_mode;
    cdp->rev_in_mode = rev_in;

    return prev_rev_in_mode;
}

crc_rev_out_t crc_set_rev_out(crc_driver_t *cdp, crc_rev_out_t rev_out) {

    crc_rev_out_t prev_rev_out_mode;

    prev_rev_out_mode = cdp->rev_out_mode;
    cdp->rev_out_mode = rev_out;

    return prev_rev_out_mode;
}

crc_polysize_t crc_set_polysize(crc_driver_t *cdp, crc_polysize_t polysize) {

    crc_polysize_t prev_polysize;

    prev_polysize = cdp->polysize;
    cdp->polysize = polysize;

    return prev_polysize;
}

bool crc_enable_dma(crc_driver_t *cdp, bool enable) {

    bool prev_is_dma_en;

    prev_is_dma_en = cdp->is_dma_en;
    cdp->is_dma_en = enable;

    return prev_is_dma_en;
}

void crc_set_dma_conf(crc_driver_t *cdp, crc_dma_conf_t *dma_conf) {

    cdp->dma_conf.dma_stream_id       = dma_conf->dma_stream_id;
    cdp->dma_conf.dma_stream_bus_prio = dma_conf->dma_stream_bus_prio;
    cdp->dma_conf.dma_stream_irq_prio = dma_conf->dma_stream_irq_prio;
}

crc_cb_t crc_set_cb(crc_driver_t *cdp, crc_cb_t cb) {

    crc_cb_t prev_cb;

    prev_cb = cdp->cb;
    cdp->cb = cb;

    return prev_cb;
}

void crc_start(crc_driver_t *cdp) {

    uint32_t dma_mode;

    /* Enable peripheral clock.*/
    crc_dev_clock_enable(cdp);

    /* Set CRC parameters.*/
    crc_set_parameters(cdp);

    /* If DMA is enabled, configure DMA.*/
    if(cdp->is_dma_en == true) {
        /* Allocates DMA stream.*/
        /*lint -e9087 */
        cdp->dma_descriptor = dma_stream_take(cdp->dma_conf.dma_stream_id,
                                              cdp->dma_conf.dma_stream_irq_prio,
                                              crc_serve_dma_interrupt,
                                              (void *)cdp);
        /*lint +e9087 */

        /* Configure DMA priority level, data transfer direction and enable
           transfer complete interrupt.*/
        dma_mode = DMA_CCR_PL_VALUE(cdp->dma_conf.dma_stream_bus_prio) |
                                    DMA_CCR_DIR_M2M                    |
                                    DMA_CCR_TCIE;

        /* Configure DMA memory increment mode only for source (PINC).*/
        dma_mode |= DMA_CCR_PINC;

        /* Configure DMA frame size.*/
        dma_mode |= DMA_CCR_PSIZE_BYTE | DMA_CCR_MSIZE_BYTE;

        /* Set up calculated DMA modes.*/
        dma_stream_set_transfer_mode(cdp->dma_descriptor, dma_mode);

        /* Configure DMA stream memory address (destination address).*/
        dma_stream_set_memory(cdp->dma_descriptor, (uint32_t)&cdp->crc->DR);
    } else {
        ; /* close "if" as required by MISRA */
    }
}

void crc_calculate(crc_driver_t *cdp, crc_mode_t mode, uint8_t buffer[],
                       uint32_t buff_len) {

    uint32_t index, data;

    /* Discriminate between concatenation and new initialization modes.*/
    if(mode == CRC_MODE_NEW) {
        cdp->crc->DR = cdp->crc->INIT;
        /* Software reset of the registers.*/
        REG_SET_FIELD(cdp->crc->CR, CRC_CR_RESET, 0x1U);
    } else {
        ; /* close "if" as required by MISRA.*/
    }

    if(cdp->is_dma_en == false) {
        /* Polling mode*/
        /* Compute the CRC of data buffer array*/
        for(index = 0; index < (uint32_t)(buff_len / 4U); index++) {
            data = ((((uint32_t)buffer[4U * index]) << 24U)      |
                    (((uint32_t)buffer[4U * index + 1U]) << 16U) |
                    (((uint32_t)buffer[4U * index + 2U]) << 8U)  |
                    ((uint32_t)buffer[4U * index + 3U]));
            crc_write32bit(cdp, data);
        }
        /* Last bytes specific handling*/
        if ((buff_len % 4U) != 0U) {
            /* 8 bits left case*/
            if((buff_len % 4U) == 1U) {
                crc_write8bit(cdp, buffer[4U * index]);
            }
            /* 16 bits left case*/
            else if((buff_len % 4U) == 2U) {
                crc_write16bit(cdp, ((((uint16_t)buffer[4U * index]) << 8U) | 
                                      ((uint16_t)buffer[4U * index + 1U])));
            }
            /* 24 bits left case*/
            else if((buff_len % 4U) == 3U) {
                crc_write16bit(cdp, ((((uint16_t)buffer[4U * index]) << 8U) |
                                      ((uint16_t)buffer[4U * index + 1U])));
                crc_write8bit(cdp, buffer[4U * index + 2U]);
            }
            else{
                ; /* close "if" as required by MISRA */
            }
        }
    } else {
        /* DMA mode*/
        CRC_OP_IN_PROGRESS(cdp);

        /* Configure DMA stream peripheral addresses (source address).*/
        dma_stream_set_peripheral(cdp->dma_descriptor, (uint32_t)(buffer));

        /* Configure DMA number of data items.*/
        dma_stream_set_count(cdp->dma_descriptor, buff_len);

        /* Enable DMA. It will be disabled on the DMA end of transfer.*/
        dma_stream_enable(cdp->dma_descriptor);

        /* Wait for completion */
        CRC_WAIT_FOR_OP_COMPLETION(cdp);
    }
}

uint32_t crc_get_result(crc_driver_t *cdp) {

    return cdp->crc->DR;
}

void crc_stop(crc_driver_t *cdp) {

    /* Disable peripheral clock.*/
    crc_dev_clock_disable(cdp);

}

void crc_deinit(crc_driver_t *cdp) {

    /* Set pointer to CRC register block to NULL.*/
    cdp->crc = NULL;

    /* Reset CRC parameters.*/
    crc_reset_parameters(cdp);
}

/** @} */
