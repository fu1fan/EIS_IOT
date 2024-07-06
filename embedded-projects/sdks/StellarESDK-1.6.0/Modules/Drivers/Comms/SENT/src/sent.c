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
 * @file    sent.c
 * @brief   SENT driver source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup COMMS
 * @ingroup DRIVERS
 * @addtogroup SENT
 * @ingroup COMMS
 * @{
 */

#include <sent.h>
#include <sent_private.h>

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/**
 * @name    Invalid TIM channel.
 * @{
 */
#define SENT_TIM_CHANNEL_INVALID            255U
/** @} */

/**
 * @name    FIFO empty return code.
 * @{
 */
#define SENT_FIFO_EMPTY                     1UL
/** @} */

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
 * @brief   Enters in receiving mode.
 * @note    Called when SENT driver enters receiving mode.
 *
 * @param[in] \_sdp\_   pointer to a @p sent_driver_t structure
 *
 * @notapi
 */
#define SENT_RX_IN_PROGRESS(_sdp_)                         \
    (_sdp_)->rx_busy = SENT_RX_BUSY

/**
 * @brief   Exits from receiving mode.
 * @note    Called when SENT driver exits receiving mode.
 *
 * @param[in] \_sdp\_   pointer to a @p sent_driver_t structure
 *
 * @notapi
 */
#define SENT_RX_DONE(_sdp_)                                \
    (_sdp_)->rx_busy = SENT_RX_READY

/**
 * @brief   Waits for receive end.
 * @note    Called to wait for end of receiving mode.
 *
 * @param[in] \_sdp\_   pointer to a @p sent_driver_t structure
 *
 * @notapi
 */
#define SENT_WAIT_FOR_RX_COMPLETION(_sdp_)                 \
    if ((_sdp_)->drv_mode == SENT_DRV_MODE_SYNCHRONOUS) {  \
        while ((_sdp_)->rx_busy == SENT_RX_BUSY) {;}       \
    }

/**
 * @brief   Converts a SENT channel to a TIM channel.
 *
 * @param[in] ch        SENT channel identifier
 * @return              TIM channel identifier
 *
 * @notapi
 */
static inline uint8_t sent_channel_to_tim_channel(sent_channel_t ch) {

    return (uint8_t)(((uint8_t)(ch)) + 1U);
}

/**
 * @brief   Checks if SENT FIFO is empty.
 *
 * @param[in] sdp       pointer to an @p sent_driver_t structure
 * @param[in] ch        SENT channel identifier
 * @return              FIFO status flag. It can be one of the following
 *                      values:
 *            @arg @ref SENT_FIFO_EMPTY
 *            @arg @ref ~SENT_FIFO_EMPTY
 *
 * @notapi
 */
static inline uint32_t sent_is_fifo_empty(sent_driver_t *sdp,
                                          sent_channel_t ch) {

    return ((sdp->frames[ch][sdp->rd_ptr[ch]].attrib[SENT_FRAME_VALID_IDX] ==
             SENT_FRAME_VALID) ? ~SENT_FIFO_EMPTY : SENT_FIFO_EMPTY);
}

/**
 * @brief   Increments SENT FIFO pointer.
 *
 * @param[in] ptr       pointer to to be increment
 * @return              incremented FIFO pointer.
 *
 * @notapi
 */
static inline uint32_t sent_inc_fifo_ptr(uint32_t ptr) {

    return ((++ptr) & (SENT_FIFO_FRAMES_SIZE - 1U));
}

/**
 * @brief   Checks the CRC of SENT frame.
 *
 * @param[in] sdp       pointer to an @p sent_driver_t structure
 * @param[in] ch        SENT channel identifier
 * @param[in] frame     frame containing all required data
 * @param[in] bits      frame length in bits
 * @return              result of the check ("true" means the CRC check is
 *                      passed, "false" otherwise).
 *
 * @notapi
 */
static bool __attribute__((section(".itcm")))
                           sent_crc_check(sent_driver_t *sdp, sent_channel_t ch,
                                          sent_frame_t frame, uint8_t bits) {

    static const uint8_t crc_lookup[16] = {0, 13, 7, 10, 14, 3, 9, 4,
                                           1, 12, 6, 11, 15, 2, 8, 5};
    uint8_t calculated_crc, i;

    /* Initializes checksum with seed "0101".*/
    calculated_crc = 5U;

    /* CRC on status.*/
    if(sdp->crc_type[ch] == SENT_CRC_STATUS_DATA) {
        calculated_crc = (uint8_t)(calculated_crc ^
                                  (frame.attrib[SENT_FRAME_STATUS_IDX]));
    }

    /* CRC on data.*/
    for (i = bits; i > 0U; i -= 4U) {
        calculated_crc = (uint8_t)(crc_lookup[calculated_crc] ^
                                 ((frame.data >> (i - 4U)) & 0x0FU));
    }

    /* If the calculated CRC corresponds, the frame is correct.*/
    return (crc_lookup[calculated_crc] == frame.attrib[SENT_FRAME_CRC_IDX]);
}

/**
 * @brief   Decodes all SENT frames
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 * @param[in] ch        SENT channel identifier
 *
 * @notapi
 */
static void __attribute__((section(".itcm")))
                      sent_decode_frame(sent_driver_t *sdp, sent_channel_t ch) {

    bool frame_avail = false;
    uint8_t nibble_counter, nibble_data, ts_num, bit_num, i, j, len;
    uint8_t end_idx = 0U;
    uint16_t tick_time, tick_num;
    uint32_t tmp1, tmp2;
    sent_frame_t frame;

    /* Status + N nibbles + CRC => N + 3 timestamps.*/
    ts_num = (uint8_t)((uint8_t)sdp->nibbles[ch] + 3U);
    /* Number of data bits.*/
    bit_num = (uint8_t)((uint8_t)sdp->nibbles[ch] * 4U);

    /* Add the new data in the total buffer containing the excess of last
       iteration.*/
    for(i = 0U; i < SENT_TS_BUFF_SIZE; i++) {
        sdp->ts_tot_buff[ch][i + sdp->ts_excess_num[ch]] = sdp->ts_buff[ch][i];
    }

    /* Save buffer length.*/
    len = SENT_TS_BUFF_SIZE + sdp->ts_excess_num[ch];

    i = 0U;

    /* To have a complete frame the sync must be present
       before BUFFER SIZE - ts_num timestamps.*/
    while(i < (SENT_TS_BUFF_SIZE + sdp->ts_excess_num[ch] - ts_num)) {

        /* Decoding start time.*/
        GET_START_TIME(frame);

        /* Initialize the variable where the values of the nibbles of each
           correct frame will be stored.*/
        frame.data = 0U;

        /* Initialize the nibble counter.*/
        nibble_counter = 0U;

        /* Initialize the variable where the value of a single nibble will be
           stored.*/
        nibble_data = 0U;

        /* Sync pulse assumption.*/
        tmp1 = sdp->ts_tot_buff[ch][i];
        tmp2 = sdp->ts_tot_buff[ch][i + 1U];

        /* Increment index to take into account the timestamps taken.*/
        i++;

        /* Calculate single tick time + TIM overflow handling.*/
        tick_time = (uint16_t)((tmp2 > tmp1) ? (tmp2 - tmp1) / 56U :
                         ((tmp2 + SENT_TIM_INTERVAL) - tmp1) / 56U);

        /* Explore the remaining ts_num timestamps.*/
        for(j = 1U; j < ts_num; j++) {

            tmp1 = tmp2;
            tmp2 = sdp->ts_tot_buff[ch][i + j];

            /* Calculate number of nibble ticks + TIM overflow handling.*/
            tick_num = (uint16_t)((tmp2 > tmp1) ? (tmp2 - tmp1) / tick_time :
                            ((tmp2 + SENT_TIM_INTERVAL) - tmp1) / tick_time);

            /* If the value is between 12 and 27, the nibble is validated.*/
            if ((tick_num >= 12U) && (tick_num <= 27U)) {
                /* Get the nibble data.*/
                nibble_data = (uint8_t)(tick_num - 12U);

                /* Update the frame data with the new nibble.*/
                if ( j == 1U) {
                    /* First nibble is status.*/
                    frame.attrib[SENT_FRAME_STATUS_IDX] = nibble_data;
                } else if(j == ts_num - 1U) {
                    /* Last nibble is crc.*/
                    frame.attrib[SENT_FRAME_CRC_IDX] = nibble_data;
                } else {
                    /* Other nibbles are data.*/
                    frame.data = (sdp->order[ch] == SENT_ORDER_MSB) ?
                            ((frame.data << 4U) | nibble_data) :
                            (frame.data | (((uint32_t)nibble_data) << 4U));
                }
                /* Updates the nibble counter.*/
                nibble_counter++;
            } else {
                /* Invalid nibble, wrong synch pulse assumpion.*/
                break;
            }

            /* Check if the frame has been completed.*/
            if (nibble_counter == (ts_num - 1U)) {

                /* End of used elements index.*/
                end_idx = i + j;
                i = end_idx;

                /* Invoke CRC function on the frame. If the CRC corresponds,
                   the frame will be validated.*/
                if (sent_crc_check(sdp, ch, frame, bit_num) == true) {
                    /* Save the frame (data, status, crc).*/
                    frame.attrib[SENT_FRAME_VALID_IDX] = SENT_FRAME_VALID;

                    frame.attrib[SENT_FRAME_CHANNEL_IDX] = 1U;

                    frame.timestamp = sdp->ts_tot_buff[ch][i];

                    /* Decoding end time.*/
                    GET_END_TIME(frame);

                    sdp->frames[ch][sdp->wr_ptr[ch]] = frame;

                    /* Increment write pointer on frame fifo.*/
                    sdp->wr_ptr[ch] = sent_inc_fifo_ptr(sdp->wr_ptr[ch]);
                    if (sdp->wr_ptr[ch] == sdp->rd_ptr[ch]) {
                        sdp->rd_ptr[ch] = sent_inc_fifo_ptr(sdp->rd_ptr[ch]);
                    }
                    /* A frame is available.*/
                    frame_avail = true;
                    /* Reset frame data.*/
                    frame.data = 0U;
                }
            }
        }
    }
    /* Save the number of excess elements.*/
    sdp->ts_excess_num[ch] = SENT_TS_BUFF_SIZE +
                             sdp->ts_excess_num[ch] -
                             end_idx;

    /* Max shift handling.*/
    if(sdp->ts_excess_num[ch] > (SENT_TOT_TS_BUFF_SIZE - SENT_TS_BUFF_SIZE)){
        sdp->ts_excess_num[ch] = (uint8_t)(SENT_TOT_TS_BUFF_SIZE -
                                           SENT_TS_BUFF_SIZE);
    }

    /* Shift the excess elements.*/
    for(i = 0U; i < sdp->ts_excess_num[ch]; i++){
        sdp->ts_tot_buff[ch][i] =
                        sdp->ts_tot_buff[ch][len + i - sdp->ts_excess_num[ch]];
    }

    SENT_RX_DONE(sdp);

    /* Decoding process finished successfully. Invoke callback.*/
    if (((bool)frame_avail == true) && (sdp->callback[ch] != NULL)) {
        sdp->callback[ch](sdp, ch);
    }
}

/**
 * @brief   Generic TIM channel callback
 * @details Called on signal falling edge, gets the timestamps. As soon as a
 *          minimum number of timestamps have been captured, SENT frame
 *          decoding starts.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] ch        SENT channel identifier
 *
 * @notapi
 */
static void sent_tim_callback(tim_driver_t *tdp, sent_channel_t ch) {

    sent_driver_t *sdp;
    sdp = tim_get_private(tdp);

    if (sdp != NULL) {
        if(sdp->is_dma_en[ch] == false) {
            sdp->ts_buff[ch][sdp->ts_index[ch]] =
            (uint16_t)tim_get_comparator(tdp, sent_channel_to_tim_channel(ch));

            sdp->ts_index[ch]++;

            if (sdp->ts_index[ch] == SENT_TS_BUFF_SIZE) {
                sent_decode_frame(sdp, ch);
                sdp->ts_index[ch] = 0;
            }
        } else {
            sent_decode_frame(sdp, ch);
        }
    }
}

/**
 * @brief   TIM channel #1 callback called on signal falling edge
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @notapi
 */
static void tim_ch1_callback(tim_driver_t *tdp) {

    sent_tim_callback(tdp, SENT_CHANNEL_1);
}

/**
 * @brief   TIM channel #2 callback called on signal falling edge
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @notapi
 */
static void tim_ch2_callback(tim_driver_t *tdp) {

    sent_tim_callback(tdp, SENT_CHANNEL_2);
}

/**
 * @brief   TIM channel #3 callback called on signal falling edge
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @notapi
 */
static void tim_ch3_callback(tim_driver_t *tdp) {

    sent_tim_callback(tdp, SENT_CHANNEL_3);
}

/**
 * @brief   TIM channel #4 callback called on signal falling edge
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @notapi
 */
static void tim_ch4_callback(tim_driver_t *tdp) {

    sent_tim_callback(tdp, SENT_CHANNEL_4);
}

/**
 * @brief   Gets frame from SENT FIFO
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 * @param[in] ch        SENT channel identifier
 * @param[out] frame    frame from the FIFO
 *
 * @notapi
 */
static void sent_fifo_pop(sent_driver_t *sdp, sent_channel_t ch,
                          sent_frame_t *frame) {

    uint32_t idx;

    idx = sdp->rd_ptr[ch];

    /* Copy the frame.*/
    frame->data = sdp->frames[ch][idx].data;
    frame->attrib[0] = sdp->frames[ch][idx].attrib[0];
    frame->attrib[1] = sdp->frames[ch][idx].attrib[1];
    frame->attrib[2] = sdp->frames[ch][idx].attrib[2];
    frame->attrib[3] = sdp->frames[ch][idx].attrib[3];
    frame->timestamp = sdp->frames[ch][idx].timestamp;
    #if(SENT_TIME_ANALYSIS == 1U)
        frame->decode_time = sdp->frames[ch][idx].decode_time;
    #endif

    /* Set the frame entry in FIFO as invalid.*/
    sdp->frames[ch][idx].attrib[SENT_FRAME_VALID_IDX] = 0U;

    /* Increment the FIFO read pointer.*/
    sdp->rd_ptr[ch] = sent_inc_fifo_ptr(idx);
}

/**
 * @brief   Resets SENT parameters.
 *
 * @param[out] sdp      pointer to an @p sent_driver_t structure to
 *                      be reset
 *
 * @notapi
 */
static void sent_lld_reset_paramters(sent_driver_t *sdp) {

    uint8_t i;

    /* Set all driver configuration parameters to default values.*/
    sdp->prio     = IRQ_MIN_PRIORITY;
    sdp->drv_mode = SENT_DRV_MODE_SYNCHRONOUS;
    sdp->rx_busy  = SENT_RX_READY;
    sdp->freq     = 10000000UL;

    for (i = 0; i < ((uint8_t)SENT_CHANNEL_NUM); i++) {
        /* Reset SENT related parameters.*/
        sdp->nibbles[i]  = SENT_NIBBLES_6;
        sdp->order[i]    = SENT_ORDER_MSB;
        sdp->crc_type[i] = SENT_CRC_DATA;

        /* Reset DMA configuration parameters.*/
        sdp->is_dma_en[i]                    = false;
        sdp->dma_conf[i].dma_stream_id       = DMA1_STREAM1_ID;
        sdp->dma_conf[i].dma_stream_bus_prio = DMA_PRIORITY_MAX;
        sdp->dma_conf[i].dma_stream_irq_prio = IRQ_PRIORITY_10;

        /* Reset circular output buffer pointers.*/
        sdp->wr_ptr[i] = 0U;
        sdp->rd_ptr[i] = 0U;

        /* Set callback to NULL.*/
        sdp->callback[i] = NULL;
    }
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void sent_init(sent_driver_t *sdp) {

    /* Set SENT driver instance as private data for TIM.*/
    if (sdp == &DRV_SENT1) {
        sdp->tim = &DRV_TIM1;
    } else if (sdp == &DRV_SENT2) {
        sdp->tim = &DRV_TIM2;
    } else if (sdp == &DRV_SENT3) {
        sdp->tim = &DRV_TIM3;
    } else if (sdp == &DRV_SENT4) {
        sdp->tim = &DRV_TIM4;
    } else if (sdp == &DRV_SENT5) {
        sdp->tim = &DRV_TIM5;
    } else if (sdp == &DRV_SENT8) {
        sdp->tim = &DRV_TIM8;
    } else if (sdp == &DRV_SENT15) {
        sdp->tim = &DRV_TIM15;
    } else {
        sdp->tim = &DRV_TIM16;
    }

    /* Set SENT driver instance as private data for TIM.*/
    tim_set_private(sdp->tim, sdp);

    /* Reset SENT parameters.*/
    sent_lld_reset_paramters(sdp);
}

tim_driver_t *sent_get_tim(sent_driver_t *sdp) {

    return sdp->tim;
}

uint32_t sent_set_prio(sent_driver_t *sdp, uint32_t prio) {

    uint32_t prev_prio;

    prev_prio = sdp->prio;
    sdp->prio = prio;

    return prev_prio;
}

sent_drv_mode_t sent_set_drv_mode(sent_driver_t *sdp,
                                  sent_drv_mode_t drv_mode) {

    sent_drv_mode_t prev_mode;

    prev_mode = sdp->drv_mode;
    sdp->drv_mode = drv_mode;

    return prev_mode;
}

uint32_t sent_set_freq(sent_driver_t *sdp, uint32_t freq) {

    uint32_t prev_freq;

    prev_freq = sdp->freq;
    sdp->freq = freq;

    return prev_freq;
}

bool sent_enable_dma(sent_driver_t *sdp, sent_channel_t ch, bool enable) {

    bool prev_is_dma_en;

    prev_is_dma_en = sdp->is_dma_en[ch];
    sdp->is_dma_en[ch] = enable;

    return prev_is_dma_en;
}

void sent_set_dma_conf(sent_driver_t *sdp, sent_channel_t ch,
                       sent_dma_conf_t *dma_conf) {

    sdp->dma_conf[ch].dma_stream_id       = dma_conf->dma_stream_id;
    sdp->dma_conf[ch].dma_stream_bus_prio = dma_conf->dma_stream_bus_prio;
    sdp->dma_conf[ch].dma_stream_irq_prio = dma_conf->dma_stream_id;
}

sent_nibbles_t sent_set_nibbles(sent_driver_t *sdp, sent_channel_t ch,
                                sent_nibbles_t nibbles) {

    sent_nibbles_t prev_nibbles;

    sdp->is_ch_en[ch] = true;

    prev_nibbles = sdp->nibbles[ch];
    sdp->nibbles[ch] = nibbles;

    return prev_nibbles;
}

sent_order_t sent_set_order(sent_driver_t *sdp, sent_channel_t ch,
                            sent_order_t order) {

    sent_order_t prev_order;

    prev_order = sdp->order[ch];
    sdp->order[ch] = order;

    return prev_order;
}

sent_crc_t sent_set_crc(sent_driver_t *sdp, sent_channel_t ch,
                        sent_crc_t crc_type) {

    sent_crc_t prev_crc;

    prev_crc = sdp->crc_type[ch];
    sdp->crc_type[ch] = crc_type;

    return prev_crc;
}

sent_cb_t sent_set_cb(sent_driver_t *sdp, sent_channel_t ch,
                      sent_cb_t callback) {

    sent_cb_t prev_cb;

    prev_cb = sdp->callback[ch];
    sdp->callback[ch] = callback;

    return prev_cb;
}

void sent_set_private(sent_driver_t *sdp, void *priv) {

    sdp->priv = priv;
}

void *sent_get_private(sent_driver_t *sdp) {

    return sdp->priv;
}

void sent_start(sent_driver_t *sdp) {

    uint8_t ch;

    (void)tim_set_presc(sdp->tim,
                        (tim_presc_t)TIM_DIVIDE_BY(CLOCK_TIMCLK1 / sdp->freq));

    /* TIM pre-configuration.*/
    for(ch = 0U; ch < SENT_CHANNEL_NUM; ch++) {
        if(sdp->is_ch_en[ch] == true){
            /* No DMA mode.*/
            if(sdp->is_dma_en[ch] == false) {
                switch (ch) {
                case (uint8_t)SENT_CHANNEL_1:
                    (void)tim_set_cb(sdp->tim, TIM_CB_OPS_CC1, tim_ch1_callback);
                    break;
                case (uint8_t)SENT_CHANNEL_2:
                    (void)tim_set_cb(sdp->tim, TIM_CB_OPS_CC2, tim_ch2_callback);
                    break;
                case (uint8_t)SENT_CHANNEL_3:
                    (void)tim_set_cb(sdp->tim, TIM_CB_OPS_CC3, tim_ch3_callback);
                    break;
                case (uint8_t)SENT_CHANNEL_4:
                    (void)tim_set_cb(sdp->tim, TIM_CB_OPS_CC4, tim_ch4_callback);
                    break;
                default:
                    /* Default case as required by MISRA.*/
                    break;
                }
            } else {
                /* DMA mode.*/
                switch (ch) {
                case (uint8_t)SENT_CHANNEL_1:
                    (void)tim_set_cb(sdp->tim, TIM_CB_OPS_DMA_CC1,
                                     tim_ch1_callback);

                    /* Configure channel DMA.*/
                    tim_set_dma_conf(sdp->tim, TIM_DMA_OPS_CH1,
                                    &sdp->dma_conf[SENT_CHANNEL_1]);
                    break;
                case (uint8_t)SENT_CHANNEL_2:
                    (void)tim_set_cb(sdp->tim, TIM_CB_OPS_DMA_CC2,
                                     tim_ch2_callback);

                    /* Configure channel DMA.*/
                    tim_set_dma_conf(sdp->tim, TIM_DMA_OPS_CH2,
                                    &sdp->dma_conf[SENT_CHANNEL_2]);
                    break;
                case (uint8_t)SENT_CHANNEL_3:
                    (void)tim_set_cb(sdp->tim, TIM_CB_OPS_DMA_CC3,
                                     tim_ch3_callback);

                    /* Configure channel DMA.*/
                    tim_set_dma_conf(sdp->tim, TIM_DMA_OPS_CH3,
                                    &sdp->dma_conf[SENT_CHANNEL_3]);
                    break;
                case (uint8_t)SENT_CHANNEL_4:
                    (void)tim_set_cb(sdp->tim, TIM_CB_OPS_DMA_CC4,
                                     tim_ch4_callback);

                    /* Configure channel DMA.*/
                    tim_set_dma_conf(sdp->tim, TIM_DMA_OPS_CH4,
                                    &sdp->dma_conf[SENT_CHANNEL_4]);
                    break;
                default:
                    /* Default case as required by MISRA.*/
                    break;
                }
            }

            /* Configure tim channel in input capture falling edge mode.*/
            switch (ch) {
            case (uint8_t)SENT_CHANNEL_1:
                (void)tim_set_ch_mode(sdp->tim,
                                  sent_channel_to_tim_channel(SENT_CHANNEL_1),
                                  TIM_MODE_ICU_FALLING_EDGE);
                break;
            case (uint8_t)SENT_CHANNEL_2:
                (void)tim_set_ch_mode(sdp->tim,
                                  sent_channel_to_tim_channel(SENT_CHANNEL_2),
                                  TIM_MODE_ICU_FALLING_EDGE);
                break;
            case (uint8_t)SENT_CHANNEL_3:
                (void)tim_set_ch_mode(sdp->tim,
                                  sent_channel_to_tim_channel(SENT_CHANNEL_3),
                                  TIM_MODE_ICU_FALLING_EDGE);
                break;
            case (uint8_t)SENT_CHANNEL_4:
                (void)tim_set_ch_mode(sdp->tim,
                                  sent_channel_to_tim_channel(SENT_CHANNEL_4),
                                  TIM_MODE_ICU_FALLING_EDGE);
                break;
            default:
                /* Default case as required by MISRA.*/
                break;
            }
        }
    }

    /* Start TIM used for SENT emulation.*/
    tim_start(sdp->tim);
    tim_set_interval(sdp->tim, SENT_TIM_INTERVAL);
}

void sent_start_channel(sent_driver_t *sdp, sent_channel_t ch) {

    tim_enable_channel_callback(sdp->tim, sent_channel_to_tim_channel(ch));

    /* If DMA mode is enabled.*/
    if(sdp->is_dma_en[ch] == true) {
        tim_enable_channel_dma_ic(sdp->tim,
                                  sent_channel_to_tim_channel(ch),
                                  sdp->ts_buff[ch],
                                  SENT_TS_BUFF_SIZE);
    }
    /* Enable TIM used for SENT emulation.*/
    tim_enable(sdp->tim);
}

bool sent_is_frame_available(sent_driver_t *sdp, sent_channel_t ch) {

    if(sent_is_fifo_empty(sdp, ch) == SENT_FIFO_EMPTY) {
        return false;
    } else {
        return true;
    }
}

void sent_receive(sent_driver_t *sdp, sent_channel_t ch, sent_frame_t *frame) {

    /* TODO: interrupt critical section ...*/

    SENT_RX_IN_PROGRESS(sdp);

    if (sent_is_fifo_empty(sdp, ch) == SENT_FIFO_EMPTY) {

        if (sdp->drv_mode == SENT_DRV_MODE_SYNCHRONOUS) {

            /* Wait for incoming frames.*/
            SENT_WAIT_FOR_RX_COMPLETION(sdp);

            /* Copy the frame.*/
            sent_fifo_pop(sdp, ch, frame);
        }
    } else {
        SENT_RX_DONE(sdp);

        /* Copy the frame.*/
        sent_fifo_pop(sdp, ch, frame);
    }
}

void sent_stop_channel(sent_driver_t *sdp, sent_channel_t ch) {

    tim_disable_channel_callback(sdp->tim, sent_channel_to_tim_channel(ch));
}

void sent_stop(sent_driver_t *sdp) {

    /* Stop timer reloading.*/
    tim_set_interval(sdp->tim, 0U);

    /* Disable TIM used for SENT emulation.*/
    tim_disable(sdp->tim);

    /* Stop TIM used for SENT emulation.*/
    tim_stop(sdp->tim);
}

void sent_deinit(sent_driver_t *sdp) {

    /* Set pointer to TIM register block to NULL.*/
    sdp->tim = NULL;

    /* Set SENT driver instance as private data for TIM.*/
    tim_set_private(sdp->tim, sdp);

    /* Reset SENT parameters.*/
    sent_lld_reset_paramters(sdp);
}

/** @} */
