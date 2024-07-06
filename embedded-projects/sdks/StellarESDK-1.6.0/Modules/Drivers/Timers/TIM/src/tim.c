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
 * @file    tim.c
 * @brief   TIM driver source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup TIMERS
 * @ingroup DRIVERS
 * @addtogroup TIM
 * @ingroup TIMERS
 * @{
 */

#include <tim.h>
#include <tim_private.h>

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   Mask of the handled TIM IRQ status bits.
 */
#define TIM_DIER_IRQ_MASK              (TIM_DIER_CC4IE | TIM_DIER_CC3IE |   \
                                        TIM_DIER_CC2IE | TIM_DIER_CC1IE |   \
                                        TIM_DIER_UIE   | TIM_DIER_BIE)

/**
 * @brief   Mask of TIM CR1 fixed bits.
 */
#define TIM_CR1_FIXED_BITS             (TIM_CR1_ARPE   | TIM_CR1_URS    |   \
                                        TIM_CR1_CEN)

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
 * @brief   Sets output compare 1 mode.
 *
 * @param[in] oc1m      output compare 1 mode value to be set
 * @return              oc1m value in ccmr1 register
 *
 * @notapi
 */
static uint32_t tim_ccmr1_oc1m_value(uint8_t oc1m) {

    uint32_t ccmr1 = 0U;

    if ((oc1m & 0x8U) != 0U) {
        ccmr1 |= TIM_CCMR1_OC1M_3;
    }
    ccmr1 |= (((uint32_t)oc1m) & 0x7U) << TIM_CCMR1_OC1M_Pos;

    return ccmr1;
}

/**
 * @brief   Sets output compare 2 mode.
 *
 * @param[in] oc2m      output compare 2 mode value to be set
 * @return              oc2m value in ccmr1 register
 *
 * @notapi
 */
static uint32_t tim_ccmr1_oc2m_value(uint8_t oc2m) {

    uint32_t ccmr1 = 0U;

    if ((oc2m & 0x8U) != 0U) {
        ccmr1 |= TIM_CCMR1_OC2M_3;
    }
    ccmr1 |= (((uint32_t)oc2m) & 0x7U) << TIM_CCMR1_OC2M_Pos;

    return ccmr1;
}

/**
 * @brief   Sets output compare 3 mode.
 *
 * @param[in] oc3m      output compare 3 mode value to be set
 * @return              oc3m value in ccmr2 register
 *
 * @notapi
 */
static uint32_t tim_ccmr2_oc3m_value(uint8_t oc3m) {

    uint32_t ccmr2 = 0U;

    if ((oc3m & 0x8U) != 0U) {
        ccmr2 |= TIM_CCMR2_OC3M_3;
    }
    ccmr2 |= (((uint32_t)oc3m) & 0x7U) << TIM_CCMR2_OC3M_Pos;

    return ccmr2;
}

/**
 * @brief   Sets output compare 4 mode.
 *
 * @param[in] oc4m      output compare 4 mode value to be set
 * @return              oc4m value in ccmr2 register
 *
 * @notapi
 */
static uint32_t tim_ccmr2_oc4m_value(uint8_t oc4m) {

    uint32_t ccmr2 = 0U;

    if ((oc4m & 0x8U) != 0U) {
        ccmr2 |= TIM_CCMR2_OC4M_3;
    }
    ccmr2 |= (((uint32_t)oc4m) & 0x7U) << TIM_CCMR2_OC4M_Pos;

    return ccmr2;
}

/**
 * @brief   Sets output compare 5 mode.
 *
 * @param[in] oc5m      output compare 5 mode value to be set
 * @return              oc5m value in ccmr3 register
 *
 * @notapi
 */
static uint32_t tim_ccmr3_oc5m_value(uint8_t oc5m) {

    uint32_t ccmr3 = 0U;

    if ((oc5m & 0x8U) != 0U) {
        ccmr3 |= TIM_CCMR3_OC5M_3;
    }
    ccmr3 |= (((uint32_t)oc5m) & 0x7U) << TIM_CCMR3_OC5M_Pos;

    return ccmr3;
}

/**
 * @brief   Sets output compare 6 mode.
 *
 * @param[in] oc6m      output compare 6 mode value to be set
 * @return              oc6m value in ccmr3 register
 *
 * @notapi
 */
static uint32_t tim_ccmr3_oc6m_value(uint8_t oc6m) {

    uint32_t ccmr3 = 0U;

    if ((oc6m & 0x8U) != 0U) {
        ccmr3 |= TIM_CCMR3_OC6M_3;
    }
    ccmr3 |= (((uint32_t)oc6m) & 0x7U) << TIM_CCMR3_OC6M_Pos;

    return ccmr3;
}

/**
 * @brief   Resets TIM parameters.
 *
 * @param[out] tdp      pointer to an @p tim_driver_t structure to
 *                      be reset
 *
 * @notapi
 */
static void tim_reset_paramters(tim_driver_t *tdp) {

    uint8_t i;

    /* Set all driver configuration parameters to default values.*/
    tdp->is_enable = false;
    tdp->is_opm_en = false;
    tdp->ckd       = TIM_CKD_1;
    tdp->prio      = IRQ_MIN_PRIORITY;
    tdp->presc     = 0x0U;
    tdp->cms       = TIM_CMS_EDGE_ALIGNED;
    tdp->rdt       = 0U;
    tdp->fdt       = 0U;

    for (i = 0U; i < TIM_MAX_CH_NUM; i++) {
        tdp->ch_mode[i] = TIM_MODE_PERIODIC;
    }

    tdp->trgo       = TIM_TRGO_RESET;
    tdp->trgo2      = TIM_TRGO2_RESET;
    tdp->sms        = TIM_SMODE_DISABLED;
    tdp->trgi       = TIM_TRGI_ITR0;
    tdp->etf        = 0U;
    tdp->etps       = TIM_ETPS_DIV1;
    tdp->etp        = TIM_ETPS_NOT_INVERTED;
    tdp->is_bk_en   = false;
    tdp->bkp        = TIM_BKP_LOW;
    tdp->is_bk2_en  = false;
    tdp->bk2p       = TIM_BKP_LOW;
    tdp->ti1s       = TIM_TI1S_TI1;
    tdp->is_ccpc_en = false;

    for (i = 0U; i < (uint8_t)TIM_DMA_OPS_MAX; i++) {
        tdp->dma_conf[i].dma_stream_id       = DMA1_STREAM0_ID;
        tdp->dma_conf[i].dma_stream_bus_prio = DMA_PRIORITY_LOW;
        tdp->dma_conf[i].dma_stream_irq_prio = IRQ_MIN_PRIORITY;
        tdp->dma_descriptor[i]               = NULL;
    }

    for (i = 0U; i < (uint8_t)TIM_CB_OPS_MAX; i++) {
        tdp->cb[i] = NULL;
    }
}

/**
 * @brief   DMA update interrupt handler.
 *
 * @param[in] arg       argument associated to the callback
 * @param[in] sts       DMA callback cause status flags
 *
 * @isr
 */
static void tim_serve_dma_update_interrupt(void *arg, uint32_t sts) {

    /*lint -e9087 */
    tim_driver_t *tdp = (tim_driver_t *)arg;
    /*lint +e9087 */

    /* Checking for errors.*/
    if ((sts & DMA_STS_TEIF) != 0U) {
        /* TODO: DMA critical error.*/
    } else {
        /* Trasmission completed.*/
        if ((sts & DMA_STS_TCIF) != 0U) {
            /* If defined, invoke error callback.*/
            if (tdp->cb[TIM_CB_OPS_DMA_UPDATE] != NULL) {
                tdp->cb[TIM_CB_OPS_DMA_UPDATE](tdp);
            }
             
        } else {
            ; /* close "if" as required by MISRA */
        }
    }
}

/**
 * @brief   DMA channel 1 interrupt handler.
 *
 * @param[in] arg       argument associated to the callback
 * @param[in] sts       DMA callback cause status flags
 *
 * @isr
 */
static void tim_serve_dma_ch1_interrupt(void *arg, uint32_t sts) {

    /*lint -e9087 */
    tim_driver_t *tdp = (tim_driver_t *)arg;
    /*lint +e9087 */

    /* Checking for errors.*/
    if ((sts & DMA_STS_TEIF) != 0U) {
        /* TODO: DMA critical error.*/
    } else {
        /* Trasmission completed.*/
        if ((sts & DMA_STS_TCIF) != 0U) {
            /* If defined, invoke error callback.*/
            if (tdp->cb[TIM_CB_OPS_DMA_CC1] != NULL) {
                tdp->cb[TIM_CB_OPS_DMA_CC1](tdp);
            }
        } else {
            ; /* close "if" as required by MISRA */
        }
    }
}

/**
 * @brief   DMA channel 2 interrupt handler.
 *
 * @param[in] arg       argument associated to the callback
 * @param[in] sts       DMA callback cause status flags
 *
 * @isr
 */
static void tim_serve_dma_ch2_interrupt(void *arg, uint32_t sts) {

    /*lint -e9087 */
    tim_driver_t *tdp = (tim_driver_t *)arg;
    /*lint +e9087 */

    /* Checking for errors.*/
    if ((sts & DMA_STS_TEIF) != 0U) {
        /* TODO: DMA critical error.*/
    } else {
        /* Trasmission completed.*/
        if ((sts & DMA_STS_TCIF) != 0U) {
            /* If defined, invoke error callback.*/
            if (tdp->cb[TIM_CB_OPS_DMA_CC2] != NULL) {
                tdp->cb[TIM_CB_OPS_DMA_CC2](tdp);
            }
        } else {
            ; /* close "if" as required by MISRA */
        }
    }
}

/**
 * @brief   DMA channel 3 interrupt handler.
 *
 * @param[in] arg       argument associated to the callback
 * @param[in] sts       DMA callback cause status flags
 *
 * @isr
 */
static void tim_serve_dma_ch3_interrupt(void *arg, uint32_t sts) {

    /*lint -e9087 */
    tim_driver_t *tdp = (tim_driver_t *)arg;
    /*lint +e9087 */

    /* Checking for errors.*/
    if ((sts & DMA_STS_TEIF) != 0U) {
        /* TODO: DMA critical error.*/
    } else {
        /* Trasmission completed.*/
        if ((sts & DMA_STS_TCIF) != 0U) {
            /* If defined, invoke error callback.*/
            if (tdp->cb[TIM_CB_OPS_DMA_CC3] != NULL) {
                tdp->cb[TIM_CB_OPS_DMA_CC3](tdp);
            }
        } else {
            ; /* close "if" as required by MISRA */
        }
    }
}

/**
 * @brief   DMA channel 4 interrupt handler.
 *
 * @param[in] arg       argument associated to the callback
 * @param[in] sts       DMA callback cause status flags
 *
 * @isr
 */
static void tim_serve_dma_ch4_interrupt(void *arg, uint32_t sts) {

    /*lint -e9087 */
    tim_driver_t *tdp = (tim_driver_t *)arg;
    /*lint +e9087 */

    /* Checking for errors.*/
    if ((sts & DMA_STS_TEIF) != 0U) {
        /* TODO: DMA critical error.*/
    } else {
        /* Trasmission completed.*/
        if ((sts & DMA_STS_TCIF) != 0U) {
            /* If defined, invoke error callback.*/
            if (tdp->cb[TIM_CB_OPS_DMA_CC4] != NULL) {
                tdp->cb[TIM_CB_OPS_DMA_CC4](tdp);
            }
        } else {
            ; /* close "if" as required by MISRA */
        }
    }
}

/**
 * @brief   Enables the update DMA request.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] dma_dir   DMA direction. It can be one of the following values:
 *            @arg @ref TIM_DMA_DIR_P2M
 *            @arg @ref TIM_DMA_DIR_M2P
 * @param[in] data      pointer to the DMA buffer from which to read or in
 *                      which to write based on the selected DMA direction.
 * @param[in] size      DMA size
 *
 * @notapi
 */
static void tim_lld_enable_update_dma(tim_driver_t *tdp, tim_dma_dir_t dma_dir,
                                      uint16_t *data, uint32_t size) {

    uint32_t dier;
    uint32_t dma_trigger;
    uint32_t dma_mode;

    if (tdp->is_enable == true) {
        /* Enable DMA request.*/
        dier = tdp->tim->DIER;
        if ((dier & TIM_DIER_UDE) == 0U) {
            tdp->tim->DIER = TIM_DIER_UDE | dier;
        }

        /* Allocates DMA stream.*/
        /*lint -e9087 */
        tdp->dma_descriptor[TIM_DMA_OPS_UPDATE] = dma_stream_take(tdp->dma_conf[TIM_DMA_OPS_UPDATE].dma_stream_id,
                                                  tdp->dma_conf[TIM_DMA_OPS_UPDATE].dma_stream_irq_prio,
                                                  tim_serve_dma_update_interrupt,
                                                  (void *)tdp);
        /*lint +e9087 */

        /* Configure DMA stream peripheral addresses.*/
        dma_stream_set_peripheral(tdp->dma_descriptor[TIM_DMA_OPS_UPDATE], (uint32_t)(&tdp->tim->ARR));

        /* Configure DMA trigger.*/
        dma_trigger = tim_dev_get_update_dma_trigger(tdp);
        dma_stream_set_trigger(tdp->dma_descriptor[TIM_DMA_OPS_UPDATE], dma_trigger);

        /* Configure DMA priority level and circular mode and enable transfer
           complete interrupt.*/
        dma_mode = DMA_CCR_PL_VALUE(tdp->dma_conf[TIM_DMA_OPS_UPDATE].dma_stream_bus_prio) |
                                    DMA_CCR_CIRC | DMA_CCR_TCIE;

        /* Configure DMA data transfer direction.*/
        if (dma_dir == TIM_DMA_DIR_P2M) {
            dma_mode |= DMA_CCR_DIR_P2M;
        } else {
            dma_mode |= DMA_CCR_DIR_M2P;
        }

        /* Configure DMA memory increment mode.*/
        dma_mode |= DMA_CCR_MINC;

        /* Configure DMA frame size.*/
        dma_mode |= DMA_CCR_PSIZE_HWORD | DMA_CCR_MSIZE_HWORD;

        /* Set up calculated DMA modes.*/
        dma_stream_set_transfer_mode(tdp->dma_descriptor[TIM_DMA_OPS_UPDATE], dma_mode);

        /* Configure DMA number of data items.*/
        dma_stream_set_count(tdp->dma_descriptor[TIM_DMA_OPS_UPDATE], size);

        /* Configure DMA stream memory address.*/
        dma_stream_set_memory(tdp->dma_descriptor[TIM_DMA_OPS_UPDATE], (uint32_t)(data));

        /* Enable DMA..*/
        dma_stream_enable(tdp->dma_descriptor[TIM_DMA_OPS_UPDATE]);
    }
}

/**
 * @brief   Disables the update DMA request.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @notapi
 */
static void tim_lld_disable_update_dma(tim_driver_t *tdp) {

    if (tdp->is_enable == true) {
        /* Disable DMA request.*/
        tdp->tim->DIER &= ~TIM_DIER_UDE;

        /* Free and stop DMA stream.*/
        dma_stream_free(tdp->dma_descriptor[TIM_DMA_OPS_UPDATE]);
        dma_stream_disable(tdp->dma_descriptor[TIM_DMA_OPS_UPDATE]);
    }
}

/**
 * @brief   Returns the capture/compare register address related to the TIM
 *          channel.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] channel   TIM channel ([1:tdp->ch_num] && <= 4) for which to get
 *                      the capture/compare register address
 * @return              capture/compare register address.
 *
 * @notapi
 */
static uint32_t tim_lld_get_ccr(tim_driver_t *tdp, uint8_t channel) {

    if (channel == 1U) {
        return (uint32_t)(&tdp->tim->CCR1);
    } else if (channel == 2U) {
        return (uint32_t)(&tdp->tim->CCR2);
    } else if (channel == 3U) {
        return (uint32_t)(&tdp->tim->CCR3);
    } else {
        return (uint32_t)(&tdp->tim->CCR4);
    }
}

/**
 * @brief   Enables the capture/compare DMA request.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] channel   TIM channel ([1:tdp->ch_num] && <= 4) for which to
 *                      enable the capture/compare request
 * @param[in] dma_dir   DMA direction. It can be one of the following values:
 *            @arg @ref TIM_DMA_DIR_P2M
 *            @arg @ref TIM_DMA_DIR_M2P
 *            @arg @ref TIM_DMA_DIR_M2M
 * @param[in] dma_size  DMA size. It can be one of the following values:
 *            @arg @ref TIM_DMA_SIZE_BYTE
 *            @arg @ref TIM_DMA_SIZE_HWORD
 *            @arg @ref TIM_DMA_SIZE_WORD
 * @param[in] dma_circ  DMA circular mode. It can be one of the following
 *                      values:
 *            @arg @ref TIM_DMA_CIRC_DISABLED
 *            @arg @ref TIM_DMA_CIRC_ENABLED
 * @param[in] src       DMA source address
 * @param[in] size      DMA size
 * @param[in] src       DMA destination address
 *
 * @notapi
 */
static void tim_lld_enable_channel_dma(tim_driver_t *tdp, uint8_t channel,
                                       tim_dma_dir_t dma_dir,
                                       tim_dma_size_t dma_size,
                                       tim_dma_circ_t dma_circ,
                                       uint32_t src, uint32_t size,
                                       uint32_t dst) {

    uint32_t dier;
    const dma_descriptor_t *dma_descriptor;
    uint32_t dma_trigger;
    uint32_t dma_mode;

    if (tdp->is_enable == true) {
        /* Enable DMA request.*/
        if (tdp->is_enable == true) {
            /* Enable DMA request.*/
            dier = tdp->tim->DIER;
            if ((dier & (TIM_DIER_CC1DE << (channel - 1U))) == 0U) {
                tdp->tim->DIER = (TIM_DIER_CC1DE << (channel - 1U)) | dier;
            }
        }

        /* Allocates DMA stream.*/
        dma_descriptor = dma_stream_take(tdp->dma_conf[channel].dma_stream_id,
                                         tdp->dma_conf[channel].dma_stream_irq_prio,
                                         NULL,
                                         NULL);

        /* Save channel DMA descriptor.*/
        tdp->dma_descriptor[channel] = dma_descriptor;

        /* Configure DMA stream peripheral addresses.*/
        if (dma_dir == TIM_DMA_DIR_M2M) {
            /* In DMA M2M, the peripheral address is the source address.*/
            dma_stream_set_peripheral(dma_descriptor, (uint32_t)(src));
        } else {
            dma_stream_set_peripheral(dma_descriptor, (uint32_t)(dst));
        }

        if (channel == 1U) {
            /* Configure DMA stream interrupt callback.*/
            /*lint -e9087 */
            dma_stream_set_callback(dma_descriptor, tim_serve_dma_ch1_interrupt, (void *)tdp);
            /*lint +e9087 */
        } else if (channel == 2U) {
            /* Configure DMA stream interrupt callback.*/
            /*lint -e9087 */
            dma_stream_set_callback(dma_descriptor, tim_serve_dma_ch2_interrupt, (void *)tdp);
            /*lint +e9087 */
        } else if (channel == 3U) {
            /* Configure DMA stream interrupt callback.*/
            /*lint -e9087 */
            dma_stream_set_callback(dma_descriptor, tim_serve_dma_ch3_interrupt, (void *)tdp);
            /*lint +e9087 */
        } else {
            /* Configure DMA stream interrupt callback.*/
            /*lint -e9087 */
            dma_stream_set_callback(dma_descriptor, tim_serve_dma_ch4_interrupt, (void *)tdp);
            /*lint +e9087 */
        }

        /* Configure DMA trigger.*/
        dma_trigger = tim_dev_get_channel_dma_trigger(tdp, channel);
        dma_stream_set_trigger(dma_descriptor, dma_trigger);

        /* Configure DMA priority level and circular mode and enable transfer
           complete interrupt.*/
        dma_mode = DMA_CCR_PL_VALUE(tdp->dma_conf[channel].dma_stream_bus_prio) |
                                    DMA_CCR_CIRC | DMA_CCR_TCIE;

        /* Configure DMA data transfer direction.*/
        if (dma_dir == TIM_DMA_DIR_P2M) {
            dma_mode |= DMA_CCR_DIR_P2M;
        } else if (dma_dir == TIM_DMA_DIR_M2P) {
            dma_mode |= DMA_CCR_DIR_M2P;
        } else {
            dma_mode |= DMA_CCR_DIR_M2M;
        }

        /* Configure DMA circular mode.*/
        if (dma_circ == TIM_DMA_CIRC_ENABLED) {
            dma_mode |= DMA_CCR_CIRC;
        } else {
            dma_mode &= ~DMA_CCR_CIRC;
        }

        /* Configure DMA frame size.*/
        if (dma_size == TIM_DMA_SIZE_BYTE) {
            dma_mode |= DMA_CCR_PSIZE_BYTE | DMA_CCR_MSIZE_BYTE;
        } else if (dma_size == TIM_DMA_SIZE_HWORD) {
            dma_mode |= DMA_CCR_PSIZE_HWORD | DMA_CCR_MSIZE_HWORD;
        } else {
            dma_mode |= DMA_CCR_PSIZE_WORD | DMA_CCR_MSIZE_WORD;
        }

        /* Configure DMA memory increment mode.*/
        dma_mode |= DMA_CCR_MINC;

        /* Set up calculated DMA modes.*/
        dma_stream_set_transfer_mode(dma_descriptor, dma_mode);

        /* Configure DMA number of data items.*/
        dma_stream_set_count(dma_descriptor, size);

        /* Configure DMA stream memory address.*/
        if (dma_dir == TIM_DMA_DIR_M2M) {
            /* In DMA M2M, the memory address is the destination address.*/
            dma_stream_set_memory(dma_descriptor, (uint32_t)(dst));
        } else {
            dma_stream_set_memory(dma_descriptor, (uint32_t)(src));
        }

        /* Enable DMA..*/
        dma_stream_enable(dma_descriptor);
    }
}

/**
 * @brief   Disables the capture/compare DMA request.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] channel   TIM channel ([1:tdp->ch_num] && <= 4) for which to
 *                      enable the capture/compare request
 *
 * @notapi
 */
static void tim_lld_disable_channel_dma(tim_driver_t *tdp, uint8_t channel) {

    if (tdp->is_enable == true) {
        /* Disable DMA request.*/
        tdp->tim->DIER &= ~(TIM_DIER_CC1DE << (channel - 1U));

        /* Free and stop DMA stream.*/
        dma_stream_free(tdp->dma_descriptor[channel]);
        dma_stream_disable(tdp->dma_descriptor[channel]);
    }
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void tim_init(tim_driver_t *tdp) {

    /* Set pointer to TIM register block.*/
    tdp->tim     = tim_dev_get_reg_ptr(tdp);
    /* Set TIM clock value based on clocktree configuration.*/
    tdp->clock   = tim_dev_get_clock(tdp);
    /* Set BDTR flag.*/
    tdp->is_bdtr = tim_dev_is_bdtr(tdp);
    /* Set the number of channels related to the TIM.*/
    tdp->ch_num  = tim_dev_get_ch_num(tdp);

    /* Reset TIM parameters.*/
    tim_reset_paramters(tdp);
}

uint32_t tim_get_clock(tim_driver_t *tdp) {

    return (tdp->clock);
}

bool tim_enable_opm(tim_driver_t *tdp, bool enable) {

    bool prev_is_opm_en;

    prev_is_opm_en = tdp->is_opm_en;
    tdp->is_opm_en = enable;

    return prev_is_opm_en;
}

tim_ckd_t tim_set_ckd(tim_driver_t *tdp, tim_ckd_t ckd) {

    tim_ckd_t prev_ckd;

    prev_ckd = tdp->ckd;
    tdp->ckd = ckd;

    return prev_ckd;
}

uint32_t tim_set_prio(tim_driver_t *tdp, uint32_t prio) {

    uint32_t prev_prio;

    prev_prio = tdp->prio;
    tdp->prio = prio;

    return prev_prio;
}

tim_presc_t tim_set_presc(tim_driver_t *tdp, tim_presc_t presc) {

    tim_presc_t prev_presc;

    prev_presc = tdp->presc;
    tdp->presc = presc;

    return prev_presc;
}

tim_cms_t tim_set_cms(tim_driver_t *tdp, tim_cms_t cms) {

    tim_cms_t prev_cms;

    prev_cms = tdp->cms;
    tdp->cms = cms;

    return prev_cms;
}

void tim_set_dt(tim_driver_t *tdp, uint32_t rdt, uint32_t fdt) {

    tdp->rdt = rdt;
    tdp->fdt = fdt;
}

tim_mode_t tim_set_ch_mode(tim_driver_t *tdp, uint8_t channel,
                           tim_mode_t ch_mode) {

    tim_mode_t prev_ch_mode;

    prev_ch_mode               = tdp->ch_mode[channel - 1U];
    tdp->ch_mode[channel - 1U] = ch_mode;

    return prev_ch_mode;
}

tim_trgo_t tim_set_trgo(tim_driver_t *tdp, tim_trgo_t trgo) {

    tim_trgo_t prev_trgo;

    prev_trgo = tdp->trgo;
    tdp->trgo = trgo;

    return prev_trgo;
}

tim_trgo2_t tim_set_trgo2(tim_driver_t *tdp, tim_trgo2_t trgo2) {

    tim_trgo2_t prev_trgo2;

    prev_trgo2 = tdp->trgo2;
    tdp->trgo2 = trgo2;

    return prev_trgo2;
}

tim_sms_t tim_set_sms(tim_driver_t *tdp, tim_sms_t sms) {

    tim_sms_t prev_sms;

    prev_sms = tdp->sms;
    tdp->sms = sms;

    return prev_sms;
}

tim_trgi_t tim_set_trgi(tim_driver_t *tdp, tim_trgi_t trgi) {

    tim_trgi_t prev_trgi;

    prev_trgi = tdp->trgi;
    tdp->trgi = trgi;

    return prev_trgi;
}

uint8_t tim_set_etf(tim_driver_t *tdp, uint8_t etf) {

    uint8_t prev_etf;

    prev_etf = tdp->etf;
    tdp->etf = etf;

    return prev_etf;

}

tim_etps_t tim_set_etps(tim_driver_t *tdp, tim_etps_t etps) {

    tim_etps_t prev_etps;

    prev_etps = tdp->etps;
    tdp->etps = etps;

    return prev_etps;
}

tim_etp_t tim_set_etp(tim_driver_t *tdp, tim_etp_t etp) {

    tim_etp_t prev_etp;

    prev_etp = tdp->etp;
    tdp->etp = etp;

    return prev_etp;
}

void tim_set_bk(tim_driver_t *tdp, tim_bkp_t polarity) {

    tdp->is_bk_en = true;
    tdp->bkp = polarity;
}

void tim_set_bk2(tim_driver_t *tdp, tim_bkp_t polarity) {

    tdp->is_bk2_en = true;
    tdp->bk2p = polarity;
}

tim_ti1s_t tim_set_ti1_selection(tim_driver_t *tdp, tim_ti1s_t ti1s) {

    tim_ti1s_t prev_ti1s;

    prev_ti1s = tdp->ti1s;
    tdp->ti1s = ti1s;

    return prev_ti1s;
}

bool tim_enable_preload_control(tim_driver_t *tdp, bool enable) {

    bool prev_is_ccpc_en;

    prev_is_ccpc_en = tdp->is_ccpc_en;
    tdp->is_ccpc_en = enable;

    return prev_is_ccpc_en;
}

void tim_set_dma_conf(tim_driver_t *tdp, tim_dma_ops_t dma_op,
                      tim_dma_conf_t *dma_conf) {

    tdp->dma_conf[dma_op].dma_stream_id       = dma_conf->dma_stream_id;
    tdp->dma_conf[dma_op].dma_stream_bus_prio = dma_conf->dma_stream_bus_prio;
    tdp->dma_conf[dma_op].dma_stream_irq_prio = dma_conf->dma_stream_irq_prio;
}

tim_cb_t tim_set_cb(tim_driver_t *tdp, tim_cb_ops_t cb_op, tim_cb_t cb) {

    tim_cb_t prev_cb;

    prev_cb        = tdp->cb[cb_op];
    tdp->cb[cb_op] = cb;

    return prev_cb;
}

void tim_start(tim_driver_t *tdp) {

    uint32_t cr1, cr2, ccer, smcr, ccmr1, ccmr2, ccmr3;
    uint8_t i;
    tim_sms_t sms;

    /* Enable clock.*/
    tim_dev_clock_enable(tdp);

    /* Update enable status.*/
    tdp->is_enable = true;

    /* Configure and enable interrupts.*/
    tim_dev_enable_irq(tdp, tdp->prio);

    /* Channel settings.*/
    ccer  = 0U;
    ccmr1 = 0U;
    ccmr2 = 0U;
    ccmr3 = 0U;

    if ((tdp->sms == TIM_SMODE_QUAD_ENCODER_MODE1)          ||
        (tdp->sms == TIM_SMODE_QUAD_ENCODER_MODE2)          ||
        (tdp->sms == TIM_SMODE_QUAD_ENCODER_MODE3)          ||
        (tdp->sms == TIM_SMODE_QUAD_ENCODER_TI1FP1_RISING)  ||
        (tdp->sms == TIM_SMODE_QUAD_ENCODER_TI1FP2_RISING)  ||
        (tdp->sms == TIM_SMODE_QUAD_ENCODER_TI1FP1_FALLING) ||
        (tdp->sms == TIM_SMODE_QUAD_ENCODER_TI1FP2_FALLING)) {

        ccer  |= TIM_CCER_CC1E | TIM_CCER_CC2E;
        ccmr1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;
    } else {
        /* Mode-specific initializations.*/
        for (i = 0U; i < tdp->ch_num; i++) {
            switch (tdp->ch_mode[i]) {
                case TIM_MODE_PERIODIC:
                    break;
                case TIM_MODE_PWM_ACTIVE_HIGH_COMP:
                    switch(i) {
                        case 0U:
                            ccer |= TIM_CCER_CC1NE;
                            break;
                        case 1U:
                            ccer |= TIM_CCER_CC2NE;
                            break;
                        case 2U:
                            ccer |= TIM_CCER_CC3NE;
                            break;
                        case 3U:
                            ccer |= TIM_CCER_CC4NE;
                            break;
                        default:
                            /* for MISRA compliance */
                            break;
                    }
                    /* Fall-through.*/
                case TIM_MODE_PWM_ACTIVE_HIGH:
                    switch(i) {
                        case 0U:
                            ccer  |= TIM_CCER_CC1E;
                            ccmr1 |= tim_ccmr1_oc1m_value(6U) | TIM_CCMR1_OC1PE;
                            break;
                        case 1U:
                            ccer  |= TIM_CCER_CC2E;
                            ccmr1 |= tim_ccmr1_oc2m_value(6U) | TIM_CCMR1_OC2PE;
                            break;
                        case 2U:
                            ccer  |= TIM_CCER_CC3E;
                            ccmr2 |= tim_ccmr2_oc3m_value(6U) | TIM_CCMR2_OC3PE;
                            break;
                        case 3U:
                            ccer  |= TIM_CCER_CC4E;
                            ccmr2 |= tim_ccmr2_oc4m_value(6U) | TIM_CCMR2_OC4PE;
                            break;
                        case 4U:
                            ccer  |= TIM_CCER_CC5E;
                            ccmr3 |= tim_ccmr3_oc5m_value(6U) | TIM_CCMR3_OC5PE;
                            break;
                        case 5U:
                            ccer  |= TIM_CCER_CC6E;
                            ccmr3 |= tim_ccmr3_oc6m_value(6U) | TIM_CCMR3_OC6PE;
                            break;
                        default:
                            /* for MISRA compliance */
                            break;
                    }
                    break;
                case TIM_MODE_PWM_ACTIVE_LOW_COMP:
                    switch(i) {
                        case 0U:
                            ccer |= TIM_CCER_CC1NE;
                            break;
                        case 1U:
                            ccer |= TIM_CCER_CC2NE;
                            break;
                        case 2U:
                            ccer |= TIM_CCER_CC3NE;
                            break;
                        case 3U:
                            ccer |= TIM_CCER_CC4NE;
                            break;
                        default:
                            /* for MISRA compliance */
                            break;
                    }
                    /* Fall-through.*/
                case TIM_MODE_PWM_ACTIVE_LOW:
                    switch(i) {
                        case 0U:
                            ccer  |= TIM_CCER_CC1E;
                            ccmr1 |= tim_ccmr1_oc1m_value(7U) | TIM_CCMR1_OC1PE;
                            break;
                        case 1U:
                            ccer  |= TIM_CCER_CC2E;
                            ccmr1 |= tim_ccmr1_oc2m_value(7U) | TIM_CCMR1_OC2PE;
                            break;
                        case 2U:
                            ccer  |= TIM_CCER_CC3E;
                            ccmr2 |= tim_ccmr2_oc3m_value(7U) | TIM_CCMR2_OC3PE;
                            break;
                        case 3U:
                            ccer  |= TIM_CCER_CC4E;
                            ccmr2 |= tim_ccmr2_oc4m_value(7U) | TIM_CCMR2_OC4PE;
                            break;
                        case 4U:
                            ccer  |= TIM_CCER_CC5E;
                            ccmr3 |= tim_ccmr3_oc5m_value(7U) | TIM_CCMR3_OC5PE;
                            break;
                        case 5U:
                            ccer  |= TIM_CCER_CC6E;
                            ccmr3 |= tim_ccmr3_oc6m_value(7U) | TIM_CCMR3_OC6PE;
                            break;
                        default:
                            /* for MISRA compliance */
                            break;
                    }
                    break;
                case TIM_MODE_PWM_FROZEN:
                    switch(i) {
                        case 0U:
                            ccer  |= TIM_CCER_CC1E;
                            ccmr1 |= tim_ccmr1_oc1m_value(0U);
                            break;
                        case 1U:
                            ccer  |= TIM_CCER_CC2E;
                            ccmr1 |= tim_ccmr1_oc2m_value(0U);
                            break;
                        case 2U:
                            ccer  |= TIM_CCER_CC3E;
                            ccmr2 |= tim_ccmr2_oc3m_value(0U);
                            break;
                        case 3U:
                            ccer  |= TIM_CCER_CC4E;
                            ccmr2 |= tim_ccmr2_oc4m_value(0U);
                            break;
                        case 4U:
                            ccer  |= TIM_CCER_CC5E;
                            ccmr3 |= tim_ccmr3_oc5m_value(0U);
                            break;
                        case 5U:
                            ccer  |= TIM_CCER_CC6E;
                            ccmr3 |= tim_ccmr3_oc6m_value(0U);
                            break;
                        default:
                            /* for MISRA compliance */
                            break;
                    }
                    break;
                case TIM_MODE_PWM_ACTIVE_LEVEL:
                    switch(i) {
                        case 0U:
                            ccer  |= TIM_CCER_CC1E;
                            ccmr1 |= tim_ccmr1_oc1m_value(1U);
                            break;
                        case 1U:
                            ccer  |= TIM_CCER_CC2E;
                            ccmr1 |= tim_ccmr1_oc2m_value(1U);
                            break;
                        case 2U:
                            ccer  |= TIM_CCER_CC3E;
                            ccmr2 |= tim_ccmr2_oc3m_value(1U);
                            break;
                        case 3U:
                            ccer  |= TIM_CCER_CC4E;
                            ccmr2 |= tim_ccmr2_oc4m_value(1U);
                            break;
                        case 4U:
                            ccer  |= TIM_CCER_CC5E;
                            ccmr3 |= tim_ccmr3_oc5m_value(1U);
                            break;
                        case 5U:
                            ccer  |= TIM_CCER_CC6E;
                            ccmr3 |= tim_ccmr3_oc6m_value(1U);
                            break;
                        default:
                            /* for MISRA compliance */
                            break;
                    }
                    break;
                case TIM_MODE_PWM_INACTIVE_LEVEL:
                    switch(i) {
                        case 0U:
                            ccer  |= TIM_CCER_CC1E;
                            ccmr1 |= tim_ccmr1_oc1m_value(2U);
                            break;
                        case 1U:
                            ccer  |= TIM_CCER_CC2E;
                            ccmr1 |= tim_ccmr1_oc2m_value(2U);
                            break;
                        case 2U:
                            ccer  |= TIM_CCER_CC3E;
                            ccmr2 |= tim_ccmr2_oc3m_value(2U);
                            break;
                        case 3U:
                            ccer  |= TIM_CCER_CC4E;
                            ccmr2 |= tim_ccmr2_oc4m_value(2U);
                            break;
                        case 4U:
                            ccer  |= TIM_CCER_CC5E;
                            ccmr3 |= tim_ccmr3_oc5m_value(2U);
                            break;
                        case 5U:
                            ccer  |= TIM_CCER_CC6E;
                            ccmr3 |= tim_ccmr3_oc6m_value(2U);
                            break;
                        default:
                            /* for MISRA compliance */
                            break;
                    }
                    break;
                case TIM_MODE_PWM_COMBINED_MODE1:
                    switch(i) {
                        case 0U:
                            ccer  |= TIM_CCER_CC1E;
                            ccmr1 |= tim_ccmr1_oc1m_value(12U) | TIM_CCMR1_OC1PE;
                            break;
                        case 1U:
                            ccer  |= TIM_CCER_CC2E;
                            ccmr1 |= tim_ccmr1_oc2m_value(12U) | TIM_CCMR1_OC2PE;
                            break;
                        case 2U:
                            ccer  |= TIM_CCER_CC3E;
                            ccmr2 |= tim_ccmr2_oc3m_value(12U) | TIM_CCMR2_OC3PE;
                            break;
                        case 3U:
                            ccer  |= TIM_CCER_CC4E;
                            ccmr2 |= tim_ccmr2_oc4m_value(12U) | TIM_CCMR2_OC4PE;
                            break;
                        case 4U:
                            ccer  |= TIM_CCER_CC5E;
                            ccmr3 |= tim_ccmr3_oc5m_value(12U) | TIM_CCMR3_OC5PE;
                            break;
                        case 5U:
                            ccer  |= TIM_CCER_CC6E;
                            ccmr3 |= tim_ccmr3_oc6m_value(12U) | TIM_CCMR3_OC6PE;
                            break;
                        default:
                            /* for MISRA compliance */
                            break;
                    }
                    break;
                case TIM_MODE_PWM_COMBINED_MODE2:
                    switch(i) {
                        case 0U:
                            ccer  |= TIM_CCER_CC1E;
                            ccmr1 |= tim_ccmr1_oc1m_value(13U) | TIM_CCMR1_OC1PE;
                            break;
                        case 1U:
                            ccer  |= TIM_CCER_CC2E;
                            ccmr1 |= tim_ccmr1_oc2m_value(13U) | TIM_CCMR1_OC2PE;
                            break;
                        case 2U:
                            ccer  |= TIM_CCER_CC3E;
                            ccmr2 |= tim_ccmr2_oc3m_value(13U) | TIM_CCMR2_OC3PE;
                            break;
                        case 3U:
                            ccer  |= TIM_CCER_CC4E;
                            ccmr2 |= tim_ccmr2_oc4m_value(13U) | TIM_CCMR2_OC4PE;
                            break;
                        case 4U:
                            ccer  |= TIM_CCER_CC5E;
                            ccmr3 |= tim_ccmr3_oc5m_value(13U) | TIM_CCMR3_OC5PE;
                            break;
                        case 5U:
                            ccer  |= TIM_CCER_CC6E;
                            ccmr3 |= tim_ccmr3_oc6m_value(13U) | TIM_CCMR3_OC6PE;
                            break;
                        default:
                            /* for MISRA compliance */
                            break;
                    }
                    break;
                case TIM_MODE_PWM_ASYMMETRIC_MODE1:
                    switch(i) {
                        case 0U:
                            ccer  |= TIM_CCER_CC1E;
                            ccmr1 |= tim_ccmr1_oc1m_value(14U) | TIM_CCMR1_OC1PE;
                            break;
                        case 1U:
                            ccer  |= TIM_CCER_CC2E;
                            ccmr1 |= tim_ccmr1_oc2m_value(14U) | TIM_CCMR1_OC2PE;
                            break;
                        case 2U:
                            ccer  |= TIM_CCER_CC3E;
                            ccmr2 |= tim_ccmr2_oc3m_value(14U) | TIM_CCMR2_OC3PE;
                            break;
                        case 3U:
                            ccer  |= TIM_CCER_CC4E;
                            ccmr2 |= tim_ccmr2_oc4m_value(14U) | TIM_CCMR2_OC4PE;
                            break;
                        case 4U:
                            ccer  |= TIM_CCER_CC5E;
                            ccmr3 |= tim_ccmr3_oc5m_value(14U) | TIM_CCMR3_OC5PE;
                            break;
                        case 5U:
                            ccer  |= TIM_CCER_CC6E;
                            ccmr3 |= tim_ccmr3_oc6m_value(14U) | TIM_CCMR3_OC6PE;
                            break;
                        default:
                            /* for MISRA compliance */
                            break;
                    }
                    break;
                case TIM_MODE_PWM_ASYMMETRIC_MODE2:
                    switch(i) {
                        case 0U:
                            ccer  |= TIM_CCER_CC1E;
                            ccmr1 |= tim_ccmr1_oc1m_value(15U) | TIM_CCMR1_OC1PE;
                            break;
                        case 1U:
                            ccer  |= TIM_CCER_CC2E;
                            ccmr1 |= tim_ccmr1_oc2m_value(15U) | TIM_CCMR1_OC2PE;
                            break;
                        case 2U:
                            ccer  |= TIM_CCER_CC3E;
                            ccmr2 |= tim_ccmr2_oc3m_value(15U) | TIM_CCMR2_OC3PE;
                            break;
                        case 3U:
                            ccer  |= TIM_CCER_CC4E;
                            ccmr2 |= tim_ccmr2_oc4m_value(15U) | TIM_CCMR2_OC4PE;
                            break;
                        case 4U:
                            ccer  |= TIM_CCER_CC5E;
                            ccmr3 |= tim_ccmr3_oc5m_value(15U) | TIM_CCMR3_OC5PE;
                            break;
                        case 5U:
                            ccer  |= TIM_CCER_CC6E;
                            ccmr3 |= tim_ccmr3_oc6m_value(15U) | TIM_CCMR3_OC6PE;
                            break;
                        default:
                            /* for MISRA compliance */
                            break;
                    }
                    break;
                case TIM_MODE_ICU_RISING_EDGE:
                    switch(i) {
                        case 0U:
                            ccer  |= TIM_CCER_CC1E;
                            ccmr1 |= TIM_CCMR1_CC1S_0;
                            break;
                        case 1U:
                            ccer  |= TIM_CCER_CC2E;
                            ccmr1 |= TIM_CCMR1_CC2S_0;
                            break;
                        case 2U:
                            ccer  |= TIM_CCER_CC3E;
                            ccmr2 |= TIM_CCMR2_CC3S_0;
                            break;
                        case 3U:
                            ccer  |= TIM_CCER_CC4E;
                            ccmr2 |= TIM_CCMR2_CC4S_0;
                            break;
                        default:
                            /* for MISRA compliance */
                            break;
                    }
                    break;
                case TIM_MODE_ICU_FALLING_EDGE:
                    switch(i) {
                        case 0U:
                            ccer  |= TIM_CCER_CC1E | TIM_CCER_CC1P;
                            ccmr1 |= TIM_CCMR1_CC1S_0;
                            break;
                        case 1U:
                            ccer  |= TIM_CCER_CC2E | TIM_CCER_CC2P;
                            ccmr1 |= TIM_CCMR1_CC2S_0;
                            break;
                        case 2U:
                            ccer  |= TIM_CCER_CC3E | TIM_CCER_CC3P;
                            ccmr2 |= TIM_CCMR2_CC3S_0;
                            break;
                        case 3U:
                            ccer  |= TIM_CCER_CC4E | TIM_CCER_CC4P;
                            ccmr2 |= TIM_CCMR2_CC4S_0;
                            break;
                        default:
                            /* for MISRA compliance */
                            break;
                    }
                    break;
                case TIM_MODE_ICU_BOTH_EDGES:
                    switch(i) {
                        case 0U:
                            ccer  |= TIM_CCER_CC1E | TIM_CCER_CC1P | TIM_CCER_CC1NP;
                            ccmr1 |= TIM_CCMR1_CC1S_0;
                            break;
                        case 1U:
                            ccer  |= TIM_CCER_CC2E | TIM_CCER_CC2P | TIM_CCER_CC2NP;
                            ccmr1 |= TIM_CCMR1_CC2S_0;
                            break;
                        case 2U:
                            ccer  |= TIM_CCER_CC3E | TIM_CCER_CC3P | TIM_CCER_CC3NP;
                            ccmr2 |= TIM_CCMR2_CC3S_0;
                            break;
                        case 3U:
                            ccer  |= TIM_CCER_CC4E | TIM_CCER_CC4P | TIM_CCER_CC4NP;
                            ccmr2 |= TIM_CCMR2_CC4S_0;
                            break;
                        default:
                            /* for MISRA compliance */
                            break;
                    }
                    break;
                case TIM_MODE_ICU_RISING_EDGE_IND:
                    switch(i) {
                        case 0U:
                            ccer  |= TIM_CCER_CC1E;
                            ccmr1 |= TIM_CCMR1_CC1S_1;
                            break;
                        case 1U:
                            ccer  |= TIM_CCER_CC2E;
                            ccmr1 |= TIM_CCMR1_CC2S_1;
                            break;
                        case 2U:
                            ccer  |= TIM_CCER_CC3E;
                            ccmr2 |= TIM_CCMR2_CC3S_1;
                            break;
                        case 3U:
                            ccer  |= TIM_CCER_CC4E;
                            ccmr2 |= TIM_CCMR2_CC4S_1;
                            break;
                        default:
                            /* for MISRA compliance */
                            break;
                    }
                    break;
                case TIM_MODE_ICU_FALLING_EDGE_IND:
                    switch(i) {
                        case 0U:
                            ccer  |= TIM_CCER_CC1E | TIM_CCER_CC1P;
                            ccmr1 |= TIM_CCMR1_CC1S_1;
                            break;
                        case 1U:
                            ccer  |= TIM_CCER_CC2E | TIM_CCER_CC2P;
                            ccmr1 |= TIM_CCMR1_CC2S_1;
                            break;
                        case 2U:
                            ccer  |= TIM_CCER_CC3E | TIM_CCER_CC3P;
                            ccmr2 |= TIM_CCMR2_CC3S_1;
                            break;
                        case 3U:
                            ccer  |= TIM_CCER_CC4E | TIM_CCER_CC4P;
                            ccmr2 |= TIM_CCMR2_CC4S_1;
                            break;
                        default:
                            /* for MISRA compliance */
                            break;
                    }
                    break;
                default:
                    /* for MISRA compliance */
                    break;
            }
        }
    }

    /* Set clock division.*/
    cr1 = ((uint32_t)(tdp->ckd)) << TIM_CR1_CKD_Pos;

    /* Set center-aligned mode selection.*/
    cr1 |= ((uint32_t)(tdp->cms)) << TIM_CR1_CMS_Pos;

    /* Configure one pulse mode.*/
    if (tdp->is_opm_en == true) {
        cr1 |= TIM_CR1_OPM;
    }

    cr2 = 0UL;

    /* Configure capture/compare preloaded control.*/
    if (tdp->is_ccpc_en == true) {
        cr2 |= TIM_CR2_CCPC;
    }

    /* Configure trigger outputs.*/
    if (tdp->trgo >= TIM_TRGO_ENCODERCLK) {
        cr2 |= TIM_CR2_MMS_3 | ((0x7UL & ((uint32_t)tdp->trgo)) << TIM_CR2_MMS_Pos);
    } else {
        cr2 |= ((uint32_t)tdp->trgo) << TIM_CR2_MMS_Pos;
    }
    /* Configure trigger outputs 2.*/
    cr2 |= ((uint32_t)tdp->trgo2) << TIM_CR2_MMS2_Pos;

    /* Configure TIM input 1 selection.*/
    if (tdp->ti1s == TIM_TI1S_XOR) {
        cr2 |= TIM_CR2_TI1S;
    }

    /* Configure slave mode control.*/
    smcr = 0UL;

    if (tdp->sms == TIM_SMODE_QUAD_ENCODER_TI1FP1_FALLING) {
        sms = TIM_SMODE_QUAD_ENCODER_TI1FP1_RISING;
        ccmr1 |= TIM_CCMR1_OC1PE;
    } else if (tdp->sms == TIM_SMODE_QUAD_ENCODER_TI1FP2_FALLING) {
        sms = TIM_SMODE_QUAD_ENCODER_TI1FP2_RISING;
        ccmr1 |= TIM_CCMR1_OC2PE;
    } else {
        sms = tdp->sms;
    }

    smcr |= ((TIM_SMCR_SMS_Msk  & ((uint32_t)sms))       << TIM_SMCR_SMS_Pos)  |
            ((0x7UL             & ((uint32_t)tdp->trgi)) << TIM_SMCR_TS_Pos)   |
            ((TIM_SMCR_ETF_Msk  & ((uint32_t)tdp->etf))  << TIM_SMCR_ETF_Pos)  |
            ((TIM_SMCR_ETPS_Msk & ((uint32_t)tdp->etps)) << TIM_SMCR_ETPS_Pos) |
            ((TIM_SMCR_ETP_Msk  & ((uint32_t)tdp->etp))  << TIM_SMCR_ETP_Pos)  |
            ((0x3UL             & (((uint32_t)tdp->trgi) >> 3U)) << 20U);

    /* Timer initialization.*/
    tdp->tim->CR1   = cr1;
    tdp->tim->CR2   = cr2;
    tdp->tim->SMCR  = smcr;
    tdp->tim->PSC   = tdp->presc;
    tdp->tim->CNT   = 0U;
    tdp->tim->ARR   = 0U;
    tdp->tim->CCMR1 = ccmr1;
    tdp->tim->CCMR2 = ccmr2;
    tdp->tim->CCMR3 = ccmr3;
    tdp->tim->CCR1  = 0U;
    tdp->tim->CCR2  = 0U;
    tdp->tim->CCR3  = 0U;
    tdp->tim->CCR4  = 0U;
    tdp->tim->CCER  = ccer;
    if (tdp->is_bdtr == true) {
        tdp->tim->BDTR = TIM_BDTR_AOE;
        tdp->tim->DTR2 = 0U;
        if (tdp->rdt != 0U || tdp->fdt != 0U) {
            const uint8_t dtg  = tim_dev_get_rdtg(tdp);
            const uint8_t dtgf = tim_dev_get_fdtg(tdp);
            tdp->tim->BDTR |= TIM_BDTR_DTG_VALUE(dtg);
            tdp->tim->DTR2 |= TIM_DTR2_DTAE | TIM_DTR2_DTGF_VALUE(dtgf);
        }
        if (tdp->is_bk_en == true) {
            tdp->tim->BDTR |= TIM_BDTR_BKE;
            if (tdp->bkp == TIM_BKP_HIGH) {
                tdp->tim->BDTR |= TIM_BDTR_BKP;
            }
        }
        if (tdp->is_bk2_en == true) {
            tdp->tim->BDTR |= TIM_BDTR_BK2E;
            if (tdp->bk2p == TIM_BKP_HIGH) {
                tdp->tim->BDTR |= TIM_BDTR_BK2P;
            }
        }
    }
    tdp->tim->EGR   = TIM_EGR_UG;
    tdp->tim->SR    = 0U;
    tdp->tim->DIER  = 0U;
}

void tim_enable(tim_driver_t *tdp) {

    if (tdp->is_enable == true) {
        tdp->tim->CR1 |= TIM_CR1_FIXED_BITS;
    }
}

void tim_disable(tim_driver_t *tdp) {

    if (tdp->is_enable == true) {
        tdp->tim->CR1 &= ~(TIM_CR1_FIXED_BITS);
        tdp->tim->SR  = 0U;
    }
}

void tim_enable_update_callback(tim_driver_t *tdp) {

    uint32_t dier;

    if (tdp->is_enable == true) {
        dier = tdp->tim->DIER;
        if ((dier & TIM_DIER_UIE) == 0U) {
            tdp->tim->SR   = ~TIM_SR_UIF;
            tdp->tim->DIER =  TIM_DIER_UIE | dier;
        }
    }
}

void tim_disable_update_callback(tim_driver_t *tdp) {

    if (tdp->is_enable == true) {
        tdp->tim->DIER &= ~TIM_DIER_UIE;
    }
}

void tim_enable_channel_callback(tim_driver_t *tdp, uint8_t channel) {

    uint32_t dier;

    if ((tdp->is_enable == true) && (channel <= tdp->ch_num)) {
        dier = tdp->tim->DIER;
        if ((dier & (2UL << (channel - 1U))) == 0U) {
            tdp->tim->SR   = ~(2UL << (channel - 1U));
            tdp->tim->DIER =  (2UL << (channel - 1U)) | dier;
        }
    }
}

void tim_disable_channel_callback(tim_driver_t *tdp, uint8_t channel) {

    if ((tdp->is_enable == true) && (channel <= tdp->ch_num)) {
        tdp->tim->DIER &= ~(2UL << (channel - 1U));
    }
}

void tim_enable_break_callback(tim_driver_t *tdp) {

    uint32_t dier;

    if (tdp->is_enable == true) {
        dier = tdp->tim->DIER;
        if ((dier & TIM_DIER_BIE) == 0U) {
            tdp->tim->SR   = ~(TIM_SR_B2IF | TIM_SR_BIF);
            tdp->tim->DIER =  TIM_DIER_BIE | dier;
        }
    }
}

void tim_disable_break_callback(tim_driver_t *tdp) {

    if (tdp->is_enable == true) {
        tdp->tim->DIER &= ~TIM_DIER_BIE;
    }
}

void tim_enable_com_callback(tim_driver_t *tdp) {

    uint32_t dier;

    if (tdp->is_enable == true) {
        dier = tdp->tim->DIER;
        if ((dier & TIM_DIER_COMIE) == 0U) {
            tdp->tim->SR   = ~TIM_SR_COMIF;
            tdp->tim->DIER =  TIM_DIER_COMIE | dier;
        }
    }
}

void tim_disable_com_callback(tim_driver_t *tdp) {

    if (tdp->is_enable == true) {
        tdp->tim->DIER &= ~TIM_DIER_COMIE;
    }
}

void tim_generate_com(tim_driver_t *tdp) {

    if (tdp->is_enable == true) {
        tdp->tim->EGR |= TIM_EGR_COMG;
    }
}

void tim_enable_update_dma_ic(tim_driver_t *tdp, uint16_t *data, uint32_t size) {

    tim_lld_enable_update_dma(tdp, TIM_DMA_DIR_P2M, data, size);
}

void tim_enable_update_dma_oc(tim_driver_t *tdp, uint16_t *data, uint32_t size) {

    tim_lld_enable_update_dma(tdp, TIM_DMA_DIR_M2P, data, size);
}

void tim_disable_update_dma_ic(tim_driver_t *tdp) {

    tim_lld_disable_update_dma(tdp);
}

void tim_disable_update_dma_oc(tim_driver_t *tdp) {

    tim_lld_disable_update_dma(tdp);
}

void tim_enable_channel_dma(tim_driver_t *tdp, uint8_t channel,
                            tim_dma_dir_t dma_dir, tim_dma_size_t dma_size,
                            tim_dma_circ_t dma_circ, uint32_t src,
                            uint32_t size, uint32_t dst) {

    tim_lld_enable_channel_dma(tdp, channel, dma_dir, dma_size, dma_circ,
                               src, size, dst);
}

void tim_disable_channel_dma(tim_driver_t *tdp, uint8_t channel) {

    tim_lld_disable_channel_dma(tdp, channel);
}

void tim_enable_channel_dma_ic(tim_driver_t *tdp, uint8_t channel,
                               uint16_t *data, uint32_t size) {

    uint32_t dst;

    dst = tim_lld_get_ccr(tdp, channel);

    tim_lld_enable_channel_dma(tdp, channel, TIM_DMA_DIR_P2M,
                               TIM_DMA_SIZE_HWORD, TIM_DMA_CIRC_ENABLED,
                               (uint32_t)(data), size, dst);
}

void tim_enable_channel_dma_oc(tim_driver_t *tdp, uint8_t channel,
                               uint16_t *data, uint32_t size) {

    uint32_t dst;

    dst = tim_lld_get_ccr(tdp, channel);

    tim_lld_enable_channel_dma(tdp, channel, TIM_DMA_DIR_M2P,
                               TIM_DMA_SIZE_HWORD, TIM_DMA_CIRC_ENABLED,
                               (uint32_t)(data), size, dst);
}

void tim_disable_channel_dma_ic(tim_driver_t *tdp, uint8_t channel) {

    tim_lld_disable_channel_dma(tdp, channel);
}

void tim_disable_channel_dma_oc(tim_driver_t *tdp, uint8_t channel) {

    tim_lld_disable_channel_dma(tdp, channel);
}

void tim_set_interval(tim_driver_t *tdp, uint32_t interval) {

    if (tdp->is_enable == true) {
        tdp->tim->ARR = interval - 1U;
    }
}

uint32_t tim_get_interval(tim_driver_t *tdp) {

    if (tdp->is_enable == true) {
        return tdp->tim->ARR;
    } else {
        return 0U;
    }
}

void tim_set_comparator(tim_driver_t *tdp, uint8_t channel, uint32_t cmpval) {

    if ((tdp->is_enable == true) && (channel <= tdp->ch_num)) {
        switch (channel) {
            case 1U:
                tdp->tim->CCR1 = cmpval;
                break;
            case 2U:
                tdp->tim->CCR2 = cmpval;
                break;
            case 3U:
                tdp->tim->CCR3 = cmpval;
                break;
            case 4U:
                tdp->tim->CCR4 = cmpval;
                break;
            case 5U:
                tdp->tim->CCR5 = cmpval;
                break;
            case 6U:
                tdp->tim->CCR6 = cmpval;
                break;
            default:
                /* dummy comment as required by MISRA.*/
                break;
        }
    }
}

uint32_t tim_get_comparator(tim_driver_t *tdp, uint8_t channel) {

    if ((tdp->is_enable == true) && (channel <= tdp->ch_num)) {
        switch (channel) {
            case 1U:
                return tdp->tim->CCR1;
            case 2U:
                return tdp->tim->CCR2;
            case 3U:
                return tdp->tim->CCR3;
            case 4U:
                return tdp->tim->CCR4;
            case 5U:
                return tdp->tim->CCR5;
            case 6U:
                return tdp->tim->CCR6;
            default:
                return 0U;
        }
    } else {
        return 0U;
    }
}

tim_dir_t tim_get_direction(tim_driver_t *tdp) {

    if (tdp->is_enable == true) {
        if ((tdp->tim->CR1 & TIM_CR1_DIR) == 0U) {
            return TIM_DIR_UPCOUNTER;
        } else {
            return TIM_DIR_DOWNCOUNTER;
        }
    } else {
        return TIM_DIR_UPCOUNTER;
    }
}

uint16_t tim_get_counter(tim_driver_t *tdp) {

    if (tdp->is_enable == true) {
        return (uint16_t)(tdp->tim->CNT);
    } else {
        return 0U;
    }
}

void tim_set_private(tim_driver_t *tdp, void *priv) {

    tdp->priv = priv;
}

void *tim_get_private(tim_driver_t *tdp) {

    return tdp->priv;
}

void tim_stop(tim_driver_t *tdp) {

    /* Stopping the timer.*/
    tdp->tim->CR1   = 0U;
    tdp->tim->CR2   = 0U;
    tdp->tim->SMCR  = 0U;
    tdp->tim->PSC   = 0U;
    tdp->tim->CCMR1 = 0U;
    tdp->tim->CCMR2 = 0U;
    tdp->tim->CCMR3 = 0U;
    tdp->tim->DIER  = 0U;
    tdp->tim->SR    = 0U;

    /* Disable interrupts.*/
    tim_dev_disable_irq(tdp);

    /* Update enable status.*/
    tdp->is_enable = false;

    /* Disable clock.*/
    tim_dev_clock_disable(tdp);
}

void tim_deinit(tim_driver_t *tdp) {

    /* Set pointer to TIM register block to NULL.*/
    tdp->tim     = NULL;
    /* Set TIM clock value to 0.*/
    tdp->clock   = 0U;
    /* Set BDTR flag to false.*/
    tdp->is_bdtr = false;
    /* Set the number of channels related to the TIM to 0.*/
    tdp->ch_num  = 0U;

    /* Reset TIM parameters.*/
    tim_reset_paramters(tdp);
}

void __tim_serve_interrupt(tim_driver_t *tdp) {

    uint32_t sr;
    uint32_t sr_mask;

    sr = tdp->tim->SR;
    sr_mask = tdp->tim->DIER & TIM_DIER_IRQ_MASK;
    if ((sr_mask & TIM_DIER_BIE) != 0U) {
        sr_mask |= TIM_SR_B2IF;
    }

    sr &= sr_mask;
    tdp->tim->SR = ~sr;

    if (((sr & TIM_SR_UIF) != 0U) && (tdp->cb[TIM_CB_OPS_UPDATE] != NULL)) {
        tdp->cb[TIM_CB_OPS_UPDATE](tdp);
    }

    if (tdp->ch_num >= 1U) {
        if (((sr & TIM_SR_CC1IF) != 0U) && (tdp->cb[TIM_CB_OPS_CC1] != NULL)) {
            tdp->cb[TIM_CB_OPS_CC1](tdp);
        }
    }

    if (tdp->ch_num >= 2U) {
        if (((sr & TIM_SR_CC2IF) != 0U) && (tdp->cb[TIM_CB_OPS_CC2] != NULL)) {
            tdp->cb[TIM_CB_OPS_CC2](tdp);
        }
    }

    if (tdp->ch_num >= 4U) {
        if (((sr & TIM_SR_CC3IF) != 0U) && (tdp->cb[TIM_CB_OPS_CC3] != NULL)) {
            tdp->cb[TIM_CB_OPS_CC3](tdp);
        }

        if (((sr & TIM_SR_CC4IF) != 0U) && (tdp->cb[TIM_CB_OPS_CC4] != NULL)) {
            tdp->cb[TIM_CB_OPS_CC4](tdp);
        }
    }

    if (((sr & TIM_SR_BIF) != 0U) && (tdp->cb[TIM_CB_OPS_BK] != NULL)) {
        tdp->cb[TIM_CB_OPS_BK](tdp);
    }

    if (((sr & TIM_SR_B2IF) != 0U) && (tdp->cb[TIM_CB_OPS_BK2] != NULL)) {
        tdp->cb[TIM_CB_OPS_BK2](tdp);
    }

    if (((sr & TIM_SR_COMIF) != 0U) && (tdp->cb[TIM_CB_OPS_COM] != NULL)) {
        tdp->cb[TIM_CB_OPS_COM](tdp);
    }
}

/** @} */
