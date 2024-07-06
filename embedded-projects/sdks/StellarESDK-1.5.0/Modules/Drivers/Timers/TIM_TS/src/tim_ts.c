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
 * @file    tim_ts.c
 * @brief   TIM_TS driver source file.
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
 * @brief   Resets TIM_TS parameters.
 *
 * @param[out] tdp      pointer to an @p tim_ts_driver_t structure to
 *                      be reset
 *
 * @notapi
 */
static void tim_ts_lld_reset_paramters(tim_ts_driver_t *tdp) {

    tdp->is_dithering_en   = false;
    tdp->is_uif_remap_en   = false;
    tdp->is_arpre_en       = false;
    tdp->is_opm_en         = false;
    tdp->counter_ofuf_only = false;
    tdp->is_uev_en         = false;
    tdp->master_mode_sel   = TIM_TS_MMS_RESET;
    tdp->presc             = 0x0U;
    tdp->ar_value          = 0xFFFFFFFFU;
    tdp->is_update_int_en  = false;
    tdp->prio              = 0x0U;
    tdp->cb                = NULL;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void tim_ts_init(tim_ts_driver_t *tdp) {

    /* Set pointer to TIM_TS register block.*/
    tdp->tim_ts = tim_ts_dev_get_reg_ptr(tdp);

    /* Reset TIM parameters.*/
    tim_ts_lld_reset_paramters(tdp);
}

bool tim_ts_enable_dithering(tim_ts_driver_t *tdp, bool flag) {

    bool prev_conf;

    prev_conf = tdp->is_dithering_en;
    tdp->is_dithering_en = flag;

    return prev_conf;
}

bool tim_ts_enable_uif_remapping(tim_ts_driver_t *tdp, bool flag) {

    bool prev_conf;

    prev_conf = tdp->is_uif_remap_en;
    tdp->is_uif_remap_en = flag;

    return prev_conf;
}

bool tim_ts_enable_autoreload_preload(tim_ts_driver_t *tdp, bool flag) {

    bool prev_conf;

    prev_conf = tdp->is_arpre_en;
    tdp->is_arpre_en = flag;

    return prev_conf;
}

bool tim_ts_enable_one_pulse_mode(tim_ts_driver_t *tdp, bool flag) {

    bool prev_conf;

    prev_conf = tdp->is_opm_en;
    tdp->is_opm_en = flag;

    return prev_conf;
}

bool tim_ts_set_source_ofuf_only(tim_ts_driver_t *tdp, bool flag) {

    bool prev_conf;

    prev_conf = tdp->counter_ofuf_only;
    tdp->counter_ofuf_only = flag;

    return prev_conf;
}

bool tim_ts_enable_update_event_generation(tim_ts_driver_t *tdp, bool flag) {

    bool prev_conf;

    prev_conf = tdp->is_uev_en;
    tdp->is_uev_en = flag;

    return prev_conf;
}

tim_ts_mms_t tim_ts_set_master_mode_selection(tim_ts_driver_t *tdp,
                                              tim_ts_mms_t mmode) {

    tim_ts_mms_t prev_conf;

    prev_conf = tdp->master_mode_sel;
    tdp->master_mode_sel = mmode;

    return prev_conf;
}

uint32_t tim_ts_set_autoreload(tim_ts_driver_t *tdp, uint32_t ar) {

    uint32_t prev_ar;

    prev_ar = tdp->ar_value;
    tdp->ar_value = ar;

    return prev_ar;
}

uint16_t tim_ts_set_prescaler(tim_ts_driver_t *tdp, uint16_t presc) {

    uint16_t prev_presc;

    prev_presc = tdp->presc;
    tdp->presc = presc;

    /* Set prescaler also on-fly.*/
    REG_SET_FIELD(tdp->tim_ts->PSC, TIM_PSC_PSC, presc);

    return prev_presc;
}

uint32_t tim_ts_set_prio(tim_ts_driver_t *tdp, uint32_t prio) {

    uint32_t prev_prio;

    tdp->is_update_int_en = true;

    prev_prio = tdp->prio;
    tdp->prio = prio;

    return prev_prio;
}

tim_ts_cb_t tim_ts_set_cb(tim_ts_driver_t *tdp, tim_ts_cb_t cb) {

    tim_ts_cb_t prev_cb;

    prev_cb = tdp->cb;
    tdp->cb = cb;

    return prev_cb;
}

void tim_ts_start(tim_ts_driver_t *tdp) {

    IRQn_Type vector;

    /* Enable clock.*/
    tim_ts_dev_clock_enable(tdp);

    /* Configure the dithering.*/
    if(tdp->is_dithering_en == true) {
        REG_SET_FIELD(tdp->tim_ts->CR1, TIM_CR1_DITHEN, 1U);
    }

    /* Configure the UIF remapping.*/
    if(tdp->is_uif_remap_en == true) {
        REG_SET_FIELD(tdp->tim_ts->CR1, TIM_CR1_UIFREMAP, 1U);
    }

    /* Configure the auto-reload preload.*/
    if(tdp->is_arpre_en == true) {
        REG_SET_FIELD(tdp->tim_ts->CR1, TIM_CR1_ARPE, 1U);
    }

    /* Configure the one pulse mode.*/
    if(tdp->is_opm_en == true) {
        REG_SET_FIELD(tdp->tim_ts->CR1, TIM_CR1_OPM, 1U);
    }

    /* Configure the update request source.*/
    if(tdp->counter_ofuf_only == true) {
        REG_SET_FIELD(tdp->tim_ts->CR1, TIM_CR1_URS, 1U);
    }

    /* Configure the update event generation.*/
    if(tdp->is_uev_en == false) {
        REG_SET_FIELD(tdp->tim_ts->CR1, TIM_CR1_UDIS, 1U);
    }

    /* Configure the master mode selection.*/
    REG_SET_FIELD(tdp->tim_ts->CR2, TIM_CR2_MMS2, tdp->master_mode_sel);

    /* Configure the prescaler.*/
    REG_SET_FIELD(tdp->tim_ts->PSC, TIM_PSC_PSC, tdp->presc);

    /* Configure the auto-reload value.*/
    REG_SET_FIELD(tdp->tim_ts->ARR, TIM_ARR_ARR, tdp->ar_value);

    if(tdp->is_update_int_en == true) {
        /* Enable the update interrupt event.*/
        REG_SET_FIELD(tdp->tim_ts->DIER, TIM_DIER_UIE, 1U);
        vector = tim_ts_dev_get_vector(tdp);
        irq_set_priority(vector, tdp->prio);
        irq_enable(vector);
    }

    /* Generate an update event to upload the auto-reload value.*/
    REG_SET_FIELD(tdp->tim_ts->EGR, TIM_EGR_UG, 1U);

    /* Enable the peripheral.*/
    REG_SET_FIELD(tdp->tim_ts->CR1, TIM_CR1_CEN, 1U);
}

uint32_t tim_ts_get_counter(tim_ts_driver_t *tdp) {

    if(tdp->is_uif_remap_en == true){
        return REG_READ(tdp->tim_ts->CNT);
    } else {
        return (REG_READ(tdp->tim_ts->CNT) & 0x7FFFFFFFU);
    }
}

void tim_ts_stop(tim_ts_driver_t *tdp) {

    /* Disable interrupts.*/
    tim_ts_dev_disable_irq(tdp);

    /* Disable clock.*/
    tim_ts_dev_clock_disable(tdp);
}

void tim_ts_deinit(tim_ts_driver_t *tdp) {

    /* Set pointer to TIM register block to NULL.*/
    tdp->tim_ts = NULL;

    /* Reset TIM parameters.*/
    tim_ts_lld_reset_paramters(tdp);
}

void __tim_ts_serve_interrupt(tim_ts_driver_t *tdp) {

    /* clear the interrupt.*/
    REG_SET_FIELD(tdp->tim_ts->SR, TIM_SR_UIF, 0U);

    /* Invoke the callback.*/
    tdp->cb(tdp);
}

/** @} */
