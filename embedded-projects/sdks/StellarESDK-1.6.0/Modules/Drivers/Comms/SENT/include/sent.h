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
 * @file    sent.h
 * @brief   SENT driver header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup COMMS
 * @ingroup DRIVERS
 * @addtogroup SENT
 * @ingroup COMMS
 * @{
 */

#ifndef _SENT_H_
#define _SENT_H_

#include <typedefs.h>
#include <osal.h>
#include <tim.h>
#include <dma.h>
#include <clock.h>
#include <irq.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Timer interval
 * @{
 */
#define SENT_TIM_INTERVAL                   50000U
/** @} */

/**
 * @name    Frame valid signature
 * @{
 */
#define SENT_FRAME_VALID                    0xCAU
/** @} */

/**
 * @name    Frame valid index in frame attribute field
 * @{
 */
#define SENT_FRAME_VALID_IDX                0U
/** @} */

/**
 * @name    CRC index in frame attribute field
 * @{
 */
#define SENT_FRAME_CRC_IDX                  1U
/** @} */

/**
 * @name    Status index in frame attribute field
 * @{
 */
#define SENT_FRAME_STATUS_IDX               2U
/** @} */

/**
 * @name    Channel index in frame attribute field
 * @{
 */
#define SENT_FRAME_CHANNEL_IDX              3U
/** @} */

/**
 * @name    Number of channels
 * @{
 */
#define SENT_CHANNEL_NUM                    4U
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Sent time analysis enable flag.
 * @note    Set to 1 to enable the time analysis.
 * @{
 */
#define SENT_TIME_ANALYSIS                  0U
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of an SENT driver.
 */
typedef struct sent_driver sent_driver_t;

/**
 * @brief   SENT frame.
 */
typedef struct {
    uint32_t data;                /**< Data                               */
    uint8_t  attrib[4];           /**< Valid, crc, status, channel number */
    uint32_t timestamp;           /**< Timestamp (at sync time)           */
#if (SENT_TIME_ANALYSIS == 1)
    uint64_t decode_time;         /**< Time to frame decode               */
#endif
} sent_frame_t;

/**
 * @enum sent_rx_sts_t
 * SENT RX progress status.
 */
typedef enum {
    SENT_RX_READY = 0U,           /**< RX status ready */
    SENT_RX_BUSY  = 1U,           /**< RX status busy  */
} sent_rx_sts_t;

/**
 * @enum sent_drv_mode_t
 * SENT supported driver mode configurations.
 */
typedef enum {
    SENT_DRV_MODE_SYNCHRONOUS,    /**< Synchronous driver mode  */
    SENT_DRV_MODE_ASYNCHRONOUS,   /**< Asynchronous driver mode */
} sent_drv_mode_t;

/**
 * @enum sent_nibbles_t
 * SENT number of valid nibbles in received frame.
 */
typedef enum {
    SENT_NIBBLES_1 = 1,           /**< 1 valid nibbles per frame */
    SENT_NIBBLES_2 = 2,           /**< 2 valid nibbles per frame */
    SENT_NIBBLES_3 = 3,           /**< 3 valid nibbles per frame */
    SENT_NIBBLES_4 = 4,           /**< 4 valid nibbles per frame */
    SENT_NIBBLES_5 = 5,           /**< 5 valid nibbles per frame */
    SENT_NIBBLES_6 = 6,           /**< 6 valid nibbles per frame */
    SENT_NIBBLES_7 = 7,           /**< 7 valid nibbles per frame */
    SENT_NIBBLES_8 = 8,           /**< 8 valid nibbles per frame */
} sent_nibbles_t;

/**
 * @enum sent_order_t
 * SENT LSB/MSB order of valid nibbles in received frame.
 */
typedef enum {
    SENT_ORDER_LSB,               /**< Last significant nibbles first */
    SENT_ORDER_MSB                /**< Most significant nibbles first */
} sent_order_t;

/**
 * @enum sent_crc_t
 * SENT supported crc algorithm configurations.
 */
typedef enum {
    SENT_CRC_DATA,                /**< CRC only on data nibbles       */
    SENT_CRC_STATUS_DATA          /**< CRC on status and data nibbles */
} sent_crc_t;

/**
 * @enum sent_channel_t
 * SENT driver channels identification.
 */
typedef enum {
    SENT_CHANNEL_1,               /**< Channel 1 */
    SENT_CHANNEL_2,               /**< Channel 2 */
    SENT_CHANNEL_3,               /**< Channel 3 */
    SENT_CHANNEL_4                /**< Channel 4 */
} sent_channel_t;

/**
 * @brief   Type of SENT DMA configuration.
 *
 */
typedef tim_dma_conf_t sent_dma_conf_t;

/**
 * @brief   Type of SENT callbacks.
 *
 * @param[in] sdp       pointer to the @p sent_driver_t object
 */
typedef void (*sent_cb_t)(sent_driver_t *sdp, sent_channel_t ch);


/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

#if (SENT_TIME_ANALYSIS == 1)
#define GET_START_TIME(a) \
    a.decode_time = osal_get_tick();
#else
#define GET_START_TIME(a) \

#endif

#if (SENT_TIME_ANALYSIS == 1)
#define GET_END_TIME(a) \
    a.decode_time = osal_tk2ns(osal_get_tick() - a.decode_time);
#else
#define GET_END_TIME(a) \

#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Export driver modules.*/
#include <sent_instances.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Initializes SENT instance.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 *
 * @api
 */
void sent_init(sent_driver_t *sdp);

/**
 * @brief   Returns the TIM driver used by this SENT instance.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 * @return              pointer to a @p tim_driver_t structure.
 *
 * @api
 */
tim_driver_t *sent_get_tim(sent_driver_t *sdp);

/**
 * @brief   Configures SENT interrupt priority.
 * @note    This configuration becomes effective only after sent_start.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 * @param[in] prio      interrupt priority to be configured
 * @return              previous interrupt priority.
 *
 * @api
 */
uint32_t sent_set_prio(sent_driver_t *sdp, uint32_t prio);

/**
 * @brief   Configures SENT driver mode (synchronous or asynchronous).
 * @note    This configuration becomes effective only after sent_start.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 * @param[in] drv_mode  driver mode to be configured. It can be one of the
 *                      following values:
 *            @arg @ref SENT_DRV_MODE_SYNCHRONOUS
 *            @arg @ref SENT_DRV_MODE_ASYNCHRONOUS
 * @return              previous driver mode. It can be one of the following
 *                      values:
 *            @arg @ref SENT_DRV_MODE_SYNCHRONOUS
 *            @arg @ref SENT_DRV_MODE_ASYNCHRONOUS
 *
 * @api
 */
sent_drv_mode_t sent_set_drv_mode(sent_driver_t *sdp, sent_drv_mode_t drv_mode);

/**
 * @brief   Configures frequency of TIM used for the SENT emulation.
 * @note    This configuration becomes effective only after sent_start.
 * @note    By default, the TIM frequency is set to 1MHz.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 * @param[in] freq      TIM frequency [Hz] to be configured.
 * @return              previous TIM frequency [Hz]
 *
 * @api
 */
uint32_t sent_set_freq(sent_driver_t *sdp, uint32_t freq);

/**
 * @brief   Enables/Disables channel DMA mode
 * @note    This configuration becomes effective only after sent_start.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 * @param[in] ch        channel for which enable/disable DMA. It can be one of
 *                      the following values:
 *            @arg @ref SENT_CHANNEL_1
 *            @arg @ref SENT_CHANNEL_2
 *            @arg @ref SENT_CHANNEL_3
 *            @arg @ref SENT_CHANNEL_4
 * @param[in] enable    DMA mode enable status ('true' means the DMA is
 *                      enabled, 'false' otherwise)
 * @return              previuos DMA mode enable status.
 *
 * @api
 */
bool sent_enable_dma(sent_driver_t *sdp, sent_channel_t ch, bool enable);

/**
 * @brief   Configures channel DMA.
 * @note    This configuration becomes effective only after sent_start.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structuree
 * @param[in] ch        channel to be configured. It can be one of the
 *                      following values:
 *            @arg @ref SENT_CHANNEL_1
 *            @arg @ref SENT_CHANNEL_2
 *            @arg @ref SENT_CHANNEL_3
 *            @arg @ref SENT_CHANNEL_4
 * @param[in] dma_conf  pointer to DMA configuration.
 *
 * @api
 */
void sent_set_dma_conf(sent_driver_t *sdp, sent_channel_t ch,
                       sent_dma_conf_t *dma_conf) ;

/**
 * @brief   Configures number of nibbles in received frame.
 * @note    This configuration becomes effective only after sent_start.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 * @param[in] ch        channel to be configured. It can be one of the
 *                      following values:
 *            @arg @ref SENT_CHANNEL_1
 *            @arg @ref SENT_CHANNEL_2
 *            @arg @ref SENT_CHANNEL_3
 *            @arg @ref SENT_CHANNEL_4
 * @param[in] nibbles   number of nibbles into the frame. It can be one of the
 *                      following values:
 *            @arg @ref SENT_NIBBLES_1
 *            @arg @ref SENT_NIBBLES_2
 *            @arg @ref SENT_NIBBLES_3
 *            @arg @ref SENT_NIBBLES_4
 *            @arg @ref SENT_NIBBLES_5
 *            @arg @ref SENT_NIBBLES_6
 *            @arg @ref SENT_NIBBLES_7
 *            @arg @ref SENT_NIBBLES_8
 * @return              previuos number of nibbles. It can be one of the
 *                      following values:
 *            @arg @ref SENT_NIBBLES_1
 *            @arg @ref SENT_NIBBLES_2
 *            @arg @ref SENT_NIBBLES_3
 *            @arg @ref SENT_NIBBLES_4
 *            @arg @ref SENT_NIBBLES_5
 *            @arg @ref SENT_NIBBLES_6
 *            @arg @ref SENT_NIBBLES_7
 *            @arg @ref SENT_NIBBLES_8
 *
 * @api
 */
sent_nibbles_t sent_set_nibbles(sent_driver_t *sdp, sent_channel_t ch,
                                sent_nibbles_t nibbles);

/**
 * @brief   Configures order of nibbles in a received frame.
 * @note    This configuration becomes effective only after sent_start.
 * @note    LSB means the first received nibbles is stored in the last
 *          significant byte of the frame, MSB means the first received nibbles
 *          is stored in the most significant byte of the frame.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 * @param[in] ch        channel to be configured. It can be one of the
 *                      following values:
 *            @arg @ref SENT_CHANNEL_1
 *            @arg @ref SENT_CHANNEL_2
 *            @arg @ref SENT_CHANNEL_3
 *            @arg @ref SENT_CHANNEL_4
 * @param[in] nibbles   nibbles order into a frame. It can be one of the
 *                      following values:
 *            @arg @ref SENT_ORDER_LSB
 *            @arg @ref SENT_ORDER_MSB
 * @return              previuos nibbles order configuration. It can be one of
 *                      the following values:
 *            @arg @ref SENT_ORDER_LSB
 *            @arg @ref SENT_ORDER_MSB
 *
 * @api
 */
sent_order_t sent_set_order(sent_driver_t *sdp, sent_channel_t ch,
                            sent_order_t order);

/**
 * @brief   Configures the CRC algorithm
 * @note    This configuration becomes effective only after sent_start.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 * @param[in] ch        channel to be configured. It can be one of the
 *                      following values:
 *            @arg @ref SENT_CHANNEL_1
 *            @arg @ref SENT_CHANNEL_2
 *            @arg @ref SENT_CHANNEL_3
 *            @arg @ref SENT_CHANNEL_4
 * @param[in] crc_type  CRC algorithm to use. It can be one of the following
 *                      values:
 *            @arg @ref SENT_CRC_DATA
 *            @arg @ref SENT_CRC_STATUS_DATA
 * @return              previuos CRC algorithm. It can be one of the following
 *                      values:
 *            @arg @ref SENT_CRC_DATA
 *            @arg @ref SENT_CRC_STATUS_DATA
 *
 * @api
 */
sent_crc_t sent_set_crc(sent_driver_t *sdp, sent_channel_t ch,
                        sent_crc_t crc_type);

/**
 * @brief   Configures pointers to SENT callback functions.
 * @note    This configuration becomes effective only after sent_start.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 * @param[in] ch        channel to be configured. It can be one of the
 *                      following values:
 *            @arg @ref SENT_CHANNEL_1
 *            @arg @ref SENT_CHANNEL_2
 *            @arg @ref SENT_CHANNEL_3
 *            @arg @ref SENT_CHANNEL_4
 * @param[in] callback  pointer to SENT callback function
 * @return              pointer to previuos SENT callback function.
 *
* @api
 */
sent_cb_t sent_set_cb(sent_driver_t *sdp, sent_channel_t ch, sent_cb_t callback);

/**
 * @brief   Starts a SENT instance.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure.
 *
 * @api
 */
void sent_start(sent_driver_t *sdp);

/**
 * @brief   Sets application private data pointer.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 * @param[in] priv      pointer to application private data.
 *
 * @api
 */
void sent_set_private(sent_driver_t *sdp, void *priv);

/**
 * @brief   Gets application private data pointer.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 * @return              The aplication private data pointer.
 *
 * @api
 */
void *sent_get_private(sent_driver_t *sdp);

/**
 * @brief   Starts acquisition on specficed SENT channel.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 * @param[in] ch        channel to be started. It can be one of the following
 *                      values:
 *            @arg @ref SENT_CHANNEL_1
 *            @arg @ref SENT_CHANNEL_2
 *            @arg @ref SENT_CHANNEL_3
 *            @arg @ref SENT_CHANNEL_4
 *
 * @api
 */
void sent_start_channel(sent_driver_t *sdp, sent_channel_t ch);

/**
 * @brief   Checks if there are available frames.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 * @param[in] ch        channel to be checked. It can be one of the following
 *                      values:
 *            @arg @ref SENT_CHANNEL_1
 *            @arg @ref SENT_CHANNEL_2
 *            @arg @ref SENT_CHANNEL_3
 *            @arg @ref SENT_CHANNEL_4
 * @return              availability flag ('true' means a frame is available, 
 *                      'false' otherwise).
 *
 * @api
 */
bool sent_is_frame_available(sent_driver_t *sdp, sent_channel_t ch);

/**
 * @brief   Receives a frame.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 * @param[in] ch        channel from which to receive. It can be one of the
 *                      following values:
 *            @arg @ref SENT_CHANNEL_1
 *            @arg @ref SENT_CHANNEL_2
 *            @arg @ref SENT_CHANNEL_3
 *            @arg @ref SENT_CHANNEL_4
 * @param[out] frame    received frame.
 *
 * @api
 */
void sent_receive(sent_driver_t *sdp, sent_channel_t ch, sent_frame_t *frame);

/**
 * @brief   Stops SENT channel.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure
 * @param[in] ch        channel to be stopped. It can be one of the following
 *                      values:
 *            @arg @ref SENT_CHANNEL_1
 *            @arg @ref SENT_CHANNEL_2
 *            @arg @ref SENT_CHANNEL_3
 *            @arg @ref SENT_CHANNEL_4
 *
 * @api
 */
void sent_stop_channel(sent_driver_t *sdp, sent_channel_t ch);

/**
 * @brief   Stops SENT instance.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure.
 *
 * @api
 */
void sent_stop(sent_driver_t *sdp);

/**
 * @brief   Deinitializes SENT instance.
 *
 * @param[in] sdp       pointer to a @p sent_driver_t structure.
 *
 * @api
 */
void sent_deinit(sent_driver_t *sdp);

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Checks if a SENT frame is valid.
 *
 * @param[in] frame     pointer to a @p sent_frame_t structure
 * @return              "1" means the frame is valis, "0" otherwise.
 *
 * @api
 */
static inline uint32_t sent_frame_is_valid(sent_frame_t *frame) {

    return (frame->attrib[SENT_FRAME_VALID_IDX] == SENT_FRAME_VALID) ? 1UL : 0UL;
}

/**
 * @brief   Returns SENT frame status.
 *
 * @param[in] frame     pointer to a @p sent_frame_t structure
 * @return              frame status.
 *
 * @api
 */
static inline uint8_t sent_frame_status(sent_frame_t *frame) {

    return frame->attrib[SENT_FRAME_STATUS_IDX];
}

/**
 * @brief   Returns SENT frame CRC.
 *
 * @param[in] frame     pointer to a @p sent_frame_t structure
 * @return              frame CRC.
 *
 * @api
 */
static inline uint8_t sent_frame_crc(sent_frame_t *frame) {

    return frame->attrib[SENT_FRAME_CRC_IDX];
}

/**
 * @brief   Returns SENT frame channel number.
 *
 * @param[in] frame     pointer to a @p sent_frame_t structure
 * @return              frame channel number.
 *
 * @api
 */
static inline uint8_t sent_frame_channel(sent_frame_t *frame) {

    return frame->attrib[SENT_FRAME_CHANNEL_IDX];
}

/**
 * @brief   Returns SENT frame timestamp.
 *
 * @param[in] frame     pointer to a @p sent_frame_t structure
 * @return              frame timestamp.
 *
 * @api
 */
static inline uint32_t sent_frame_timestamp(sent_frame_t *frame) {

    return frame->timestamp;
}

/**
 * @brief   Returns SENT frame data.
 *
 * @param[in] frame     pointer to a @p sent_frame_t structure
 * @return              frame data.
 *
 * @api
 */
static inline uint32_t sent_frame_data(sent_frame_t *frame) {

    return frame->data;
}

#if (SENT_TIME_ANALYSIS == 1)
/**
 * @brief   Returns SENT frame decode time.
 *
 * @param[in] frame     pointer to a @p sent_frame_t structure
 * @return              frame decode time.
 *
 * @api
 */
static inline uint64_t sent_frame_time(sent_frame_t *frame) {

    return frame->decode_time;
}
#endif

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _SENT_H_ */

/** @} */
