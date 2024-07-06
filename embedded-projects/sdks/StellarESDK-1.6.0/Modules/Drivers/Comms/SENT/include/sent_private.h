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
 * @file    sent_private.h
 * @brief   SENT driver private header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup COMMS
 * @ingroup DRIVERS
 * @addtogroup SENT
 * @ingroup COMMS
 * @{
 */

#ifndef _SENT_PRIVATE_H_
#define _SENT_PRIVATE_H_

#include <tim.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    SENT timestamp buffer size
 * @{
 */
#define SENT_TS_BUFF_SIZE                   20U
/** @} */

/**
 * @name    SENT total timestamp buffer size
 * @note    Required to concat previous excess data
 * @{
 */
#define SENT_TOT_TS_BUFF_SIZE               31U
/** @} */

/**
 * @name    SENT FIFO frame size
 * @note    It must be a power of 2.
 * @{
 */
#define SENT_FIFO_FRAMES_SIZE               16UL
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
 * @brief   SENT driver structure.
 * @note    This structure is internal to the SENT driver.
 */
struct sent_driver {
    /**
     * @brief   Pointer to TIM instance.
     */
    tim_driver_t             *tim;
    /**
     * @brief   Channels enable flag.
     */
    volatile bool            is_ch_en[SENT_CHANNEL_NUM];
    /**
     * @brief   Receiving in progress flag.
     */
    volatile sent_rx_sts_t   rx_busy;
    /**
     * @brief   Timestamps buffer.
     */
    uint16_t                 ts_buff[SENT_CHANNEL_NUM][SENT_TS_BUFF_SIZE];
    /**
     * @brief   Timestamps buffer index.
     */
    uint16_t                 ts_index[SENT_CHANNEL_NUM];
    /**
     * @brief   Timestamps excess buffer from previous iteration.
     */
    uint16_t                 ts_tot_buff[SENT_CHANNEL_NUM][SENT_TOT_TS_BUFF_SIZE];
    /**
     * @brief   Number of excess timestamps.
     */
    uint8_t                  ts_excess_num[SENT_CHANNEL_NUM];
    /**
     * @brief   FIFO for received frames.
     */
    sent_frame_t             frames[SENT_CHANNEL_NUM][SENT_FIFO_FRAMES_SIZE];
    /**
     * @brief   Write index for received frames FIFO.
     */
    uint32_t                 wr_ptr[SENT_CHANNEL_NUM];
    /**
     * @brief   Read index for received frames FIFO.
     */
    uint32_t                 rd_ptr[SENT_CHANNEL_NUM];
    /**
     * @brief   TIM interrupt priority.
     */
    uint32_t                 prio;
    /**
     * @brief   Driver mode.
     */
    sent_drv_mode_t          drv_mode;
    /**
     * @brief   Frequency of TIM used for SENT emulation.
     */
    uint32_t                 freq;
    /**
     * @brief   DMA mode enable flag.
     */
    bool                     is_dma_en[SENT_CHANNEL_NUM];
    /**
     * @brief   DMA configuration.
     */
    sent_dma_conf_t          dma_conf[SENT_CHANNEL_NUM];
    /**
     * @brief   Nibbles per received frame.
     */
    sent_nibbles_t           nibbles[SENT_CHANNEL_NUM];
    /**
     * @brief   Byte order.
     */
    sent_order_t             order[SENT_CHANNEL_NUM];
    /**
     * @brief   CRC algorithm selected.
     */
    sent_crc_t               crc_type[SENT_CHANNEL_NUM];
    /**
     * @brief   Pointers to SENT callback functions.
     * @note    Set to @p NULL if not required.
     */
    sent_cb_t                callback[SENT_CHANNEL_NUM];
    /**
     * @brief   Application private pointer.
     */
    void                     *priv;
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _SENT_PRIVATE_H_ */

/** @} */
