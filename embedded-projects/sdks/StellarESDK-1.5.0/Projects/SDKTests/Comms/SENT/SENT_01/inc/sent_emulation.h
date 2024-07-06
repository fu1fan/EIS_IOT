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

#include <typedefs.h>
#include <test_env.h>
#include <platform.h>
#include <tim.h>
#include <dma.h>
#include <gpio.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

#define SENT_SIGNAL_ASSERT        1UL
#define SENT_SIGNAL_DEASSERT      0UL

/*
 * Buffer size computation (worst case):
 *  Calibration = 56
 *  Status      = 27
 *  Data        = 27 * 6
 *  CRC         = 27
 *  Last pulse  = 6
 *
 *  Total       = 278
 *
 */
#define SENT_BUFFER_SIZE          300UL

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/**
 * @brief   Configures SENT emulation
 *
 * @param[in] gpio      unique pin identifier related to the pin used for the
 *                      SENT emulation
 * @param[in] tdp       pointer to a @p tim_driver_t structure of TIM used for
 *                      the SENT emulation
 * @param[in] dma_stream_id DMA stream related to the channel1 of the TIM used
 *                      for the SENT emulation
 * @param[in] dma_callback  DMA callback related to the channel1 of the TIM
 *                      used for the SENT emulation
 *
 * @api
 */
void sent_emul_config(gpio_io_t gpio, tim_driver_t *tdp,
                      uint32_t dma_stream_id, tim_cb_t dma_callback);

/**
 * @brief   Returns the address of the data buffer containing the SENT frame.
 *
 * @return              data buffer address
 *
 * @api
 */
uint32_t sent_emul_get_buffer_addr(void);

/**
 * @brief   Creates new SENT frame with the provided data.
 *
 * @param[in] status    status nibble
 * @param[in] data      data nibbles
 * @param[in] nibbles   number of nibbles
 * @return              composed frame
 *
 * @api
 */
uint32_t sent_emul_new_frame(uint8_t status, uint32_t data, uint8_t nibbles);

/**
 * @brief   Trasmits the sent frame
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure of TIM used for
 *                      the SENT emulation
 * @param[in] len       length of buffer to be transmitted
 *
 * @api
 */
void sent_emul_transmit(tim_driver_t *tdp, uint32_t len);

/**
 * @brief   Stops the sent transmission
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure of TIM used for
 *                      the SENT emulation
 *
 * @api
 */
void sent_emul_stop(tim_driver_t *tdp) ;

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

/** @} */
