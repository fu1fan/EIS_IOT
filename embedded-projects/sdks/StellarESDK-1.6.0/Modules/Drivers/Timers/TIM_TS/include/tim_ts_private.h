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
 * @file    tim_ts_private.h
 * @brief   TIM_TS driver private header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup TIMERS
 * @ingroup DRIVERS
 * @addtogroup TIM_TS
 * @ingroup TIMERS
 * @{
 */

#ifndef _TIM_TS_PRIVATE_H_
#define _TIM_TS_PRIVATE_H_

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
 * @brief   TIM_TS driver structure.
 */
struct tim_ts_driver {
    /**
     * @brief   Associated TIM_TS unit.
     */
    TIM_TypeDef              *tim_ts;
    /**
     * @brief   Dithering enable flag.
     */
    bool                     is_dithering_en;
    /**
     * @brief   UIF status bit remapping enable flag.
     */
    bool                     is_uif_remap_en;
    /**
     * @brief   Auto-reload preload enable flag.
     */
    bool                     is_arpre_en;
    /**
     * @brief   One pulse mode enable flag.
     */
    bool                     is_opm_en;
    /**
     * @brief   Update request source selection flag.
     */
    bool                     counter_ofuf_only;
    /**
     * @brief   Update event generation enable flag.
     */
    bool                     is_uev_en;
    /**
     * @brief   Master mode selection.
     */
    tim_ts_mms_t             master_mode_sel;
    /**
     * @brief   Prescaler value.
     */
    uint16_t                 presc;
    /**
     * @brief   Auto-reload value.
     */
    uint32_t                 ar_value;
    /**
     * @brief   Update interrupt enable flag.
     */
    bool                     is_update_int_en;
    /**
     * @brief   Interrupt priority.
     */
    uint32_t                 prio;
    /**
     * @brief   Callback associated to the update events or @p NULL.
     */
    tim_ts_cb_t              cb;
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

IRQ_HANDLER(IRQ_TIM_TS_HANDLER);

TIM_TypeDef *tim_ts_dev_get_reg_ptr(tim_ts_driver_t *tdp);
uint32_t tim_ts_dev_get_clock(tim_ts_driver_t *tdp);
void tim_ts_dev_clock_enable(tim_ts_driver_t *tdp);
void tim_ts_dev_clock_disable(tim_ts_driver_t *tdp);
IRQn_Type tim_ts_dev_get_vector(tim_ts_driver_t *tdp);
void tim_ts_dev_disable_irq(tim_ts_driver_t *tdp);

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _TIM_TS_PRIVATE_H_ */

/** @} */

