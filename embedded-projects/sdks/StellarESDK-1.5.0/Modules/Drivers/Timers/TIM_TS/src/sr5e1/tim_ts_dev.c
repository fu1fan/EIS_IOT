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
 * @file    tim_ts_dev.c
 * @brief   TIM_TS device specific source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup TIMERS
 * @ingroup DRIVERS
 * @addtogroup TIM_TS
 * @ingroup TIMERS
 * @{
 */

#include <tim_ts.h>
#include <tim_ts_private.h>
#include <dma.h>
#include <clock.h>
#include <irq.h>
#include <platform.h>

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
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Returns pointer to the TIM_TS register block.
 *
 * @param[in] tdp       pointer to a @p tim_ts_driver_t structure
 * @return              pointer to the TIM register block.
 *
 * @notapi
 */
TIM_TypeDef *tim_ts_dev_get_reg_ptr(tim_ts_driver_t *tdp) {

    (void)tdp;

    return TIM_TS;
}

/**
 * @brief   Returns value of TIM_TS clock.
 *
 * @param[in] tdp       pointer to a @p tim_ts_driver_t structure
 * @return              TIM_TS clock.
 *
 * @notapi
 */
uint32_t tim_ts_dev_get_clock(tim_ts_driver_t *tdp) {

    (void)tdp;

    return CLOCK_TIMTSCLK;
}

/**
 * @brief   Enables TIM_TS clock.
 *
 * @param[in] tdp       pointer to a @p tim_ts_driver_t structure
 *
 * @notapi
 */
void tim_ts_dev_clock_enable(tim_ts_driver_t *tdp) {

    (void)tdp;

    clock_enable_TIM_TS(true);
}

/**
 * @brief   Disables TIM_TS clock.
 *
 * @param[in] tdp       pointer to a @p tim_ts_driver_t structure
 *
 * @notapi
 */
void tim_ts_dev_clock_disable(tim_ts_driver_t *tdp) {

    (void)tdp;

    clock_disable_TIM_TS();
}

/**
 * @brief   Gets the TIM_TS interrupt vector.
 *
 * @param[in] tdp       pointer to a @p tim_ts_driver_t structure
 * @return              interrupt vector.
 *
 * @notapi
 */
IRQn_Type tim_ts_dev_get_vector(tim_ts_driver_t *tdp) {

    (void)tdp;

    return IRQ_TIM_TS_VECTOR;
}

/**
 * @brief   Disables TIM_TS interrupts.
 *
 * @param[in] tdp       pointer to a @p tim_ts_driver_t structure
 *
 * @notapi
 */
void tim_ts_dev_disable_irq(tim_ts_driver_t *tdp) {

    (void)tdp;

    irq_disable(IRQ_TIM_TS_VECTOR);
}

/**
 * @brief   TIM_TS Break interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(IRQ_TIM_TS_HANDLER) {

    IRQ_PROLOGUE();

    __tim_ts_serve_interrupt(&DRV_TIM_TS);

    IRQ_EPILOGUE();
}

/** @} */
