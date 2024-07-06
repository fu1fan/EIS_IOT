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
 * @file    cmu.c
 * @brief   CMU driver source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SAFETY
 * @ingroup DRIVERS
 * @addtogroup CMU
 * @ingroup SAFETY
 * @{
 */

#include <cmu.h>
#include <clock.h>
#include <cmu_private.h>

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
 * @brief  Waits for Frequency measure to end.
 * @note   Called to wait for end of frequency calculation.
 *
 * @param[in] \_cdp\_   pointer to a @p cmu_driver_t structure
  *
 * @notapi
 */
#define CMU_WAIT_FOR_FREQUENCY_MEASURE(_cdp_)                                   \
    while (REG_READ_FIELD((_cdp_)->cmu->CMU_CSR, CMU_CMU_CSR_SFM) == 1U) {;} \

/**
 * @brief   Sets the CMU parameters
 *
 * @param[out] cdp      pointer to a @p cmu_driver_t structure
 *
 * @notapi
 */
static void cmu_lld_set_parameters(cmu_driver_t *cdp) {

    /* Set high frequency reference.*/
    cdp->cmu->CMU_HFREFR = (uint32_t)(cdp->hfref << CMU_CMU_HFREFR_HFREF_Pos);

    /* Set low frequency reference.*/
    cdp->cmu->CMU_LFREFR = (uint32_t)(cdp->lfref << CMU_CMU_LFREFR_LFREF_Pos);

    /* Clock metering related configuration, only available for CMU0.*/
    if(cdp == &DRV_CMU0){
        /* Set frequency measure clock selection.*/
        cdp->cmu->CMU_CSR |= (uint32_t)((uint32_t)cdp->freq_meter_cfg.cksel1
                                        << CMU_CMU_CSR_CKSEL1_Pos);

        /* Set CLKMT0_RMN division factor.*/
        cdp->cmu->CMU_CSR |= (uint32_t)((uint32_t)cdp->freq_meter_cfg.rcdiv
                                        << CMU_CMU_CSR_RCDIV_Pos);

        /* Set measurement duration*/
        cdp->cmu->CMU_MDR |= (uint32_t)(cdp->freq_meter_cfg.duration
                                        << CMU_CMU_MDR_MD_Pos);
    }
}

/**
 * @brief   Resets the CMU parameters to the default values
 * 
 * @param[out] cdp      pointer to a @p cmu_driver_t structure
 *
 * @notapi
 */
static void cmu_lld_reset_parameters(cmu_driver_t *cdp) {

    /* Reset driver structure to the default values.*/
    cdp->hfref = 4095;
    cdp->lfref = 0;
    cdp->freq_meter_cfg.cksel1 = CMU_CLKMT0_RMN;
    cdp->freq_meter_cfg.rcdiv = CMU_RCDIV_1;
    cdp->freq_meter_cfg.duration = 0;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void cmu_init(cmu_driver_t *cdp) {

    /* Set pointer to CMU register block.*/
    cdp->cmu = cmu_dev_get_reg_ptr(cdp);

    /* Reset CMU parameters.*/
    cmu_lld_reset_parameters(cdp);
}

cmu_clk_sel_t cmu_set_clk_source(cmu_driver_t *cdp, cmu_clk_sel_t clk_src) {

    cmu_clk_sel_t old_clk_src;

    /* Clock metering related, only available for CMU0.*/
    if(cdp == &DRV_CMU0){

        old_clk_src = cdp->freq_meter_cfg.cksel1;

        cdp->freq_meter_cfg.cksel1 = clk_src;

        return old_clk_src;
    }

    return clk_src;
}

uint16_t cmu_set_high_freq_ref(cmu_driver_t *cdp, uint16_t hfref) {

    uint16_t old_hfref;

    old_hfref = cdp->hfref;
    cdp->hfref = hfref;

    return old_hfref;
}

uint16_t cmu_set_low_freq_ref(cmu_driver_t *cdp, uint16_t lfref) {

    uint16_t old_lfref;

    old_lfref = cdp->lfref;
    cdp->lfref = lfref;

    return old_lfref;
}

cmu_clk_sel_t cmu_set_freq_meter_clock(cmu_driver_t *cdp, cmu_clk_sel_t cksel) {

    cmu_clk_sel_t old_cksel;

    old_cksel = cdp->freq_meter_cfg.cksel1;
    cdp->freq_meter_cfg.cksel1 = cksel;

    return old_cksel;
}

cmu_rcdiv_t cmu_set_division_factor(cmu_driver_t *cdp, cmu_rcdiv_t rcdiv) {

    cmu_rcdiv_t old_rcdiv;

    old_rcdiv = cdp->freq_meter_cfg.rcdiv;
    cdp->freq_meter_cfg.rcdiv = rcdiv;

    return old_rcdiv;
}

uint32_t cmu_set_measurement_duration(cmu_driver_t *cdp, uint32_t duration) {

    uint32_t old_duration;

    old_duration = cdp->freq_meter_cfg.duration;
    cdp->freq_meter_cfg.duration = duration;

    return old_duration;
}

void cmu_start(cmu_driver_t *cdp) {

    cmu_dev_clock_enable(cdp);

    /* Clear the interrupt flags.*/
    cdp->cmu->CMU_ISR = 0xFU;

    /* Set CMU parameters.*/
    cmu_lld_set_parameters(cdp);

    /* Enable CMU instance.*/
    REG_SET_FIELD(cdp->cmu->CMU_CSR, CMU_CMU_CSR_CME, 1U);
}

float cmu_get_monitored_frequency(cmu_driver_t *cdp) {

    uint32_t fdr;
    float freq = 0.0F;

    /* Clock metering related, only available for CMU0.*/
    if(cdp == &DRV_CMU0){

        /* Start frequency measure.*/
        REG_SET_FIELD(cdp->cmu->CMU_CSR, CMU_CMU_CSR_SFM, 1U);

        /* Wait for measure completition.*/
        CMU_WAIT_FOR_FREQUENCY_MEASURE(cdp);

        /* Get frequency display value.*/
        fdr = cdp->cmu->CMU_FDR;

        /* Convert to frequency value.*/
        freq = (((float)cdp->freq_meter_cfg.duration * (float)CLOCK_CFG_XOSCCLK) /
                 (float)fdr);
    }

    return freq;
}

cmu_int_t cmu_get_event(cmu_driver_t *cdp) {

    /* Check FHHI interrupt flag.*/
    if(REG_READ_FIELD(cdp->cmu->CMU_ISR, CMU_CMU_ISR_FHHI) == 1U){
        return CMU_INT_FHHI;
    }
    /* Check FLLI interrupt flag.*/
    if(REG_READ_FIELD(cdp->cmu->CMU_ISR, CMU_CMU_ISR_FLLI) == 1U){
        return CMU_INT_FLLI;
    }
    /* Check FLCI interrupt flag.*/
    if(REG_READ_FIELD(cdp->cmu->CMU_ISR, CMU_CMU_ISR_FLCI) == 1U){
        return CMU_INT_FLCI;
    }
    /* Check OLRI interrupt flag.*/
    if(REG_READ_FIELD(cdp->cmu->CMU_ISR, CMU_CMU_ISR_OLRI) == 1U){
        return CMU_INT_OLRI;
    }

    /* No interrupt occurred.*/
    return CMU_INT_NONE;
}

void cmu_clear_event(cmu_driver_t *cdp, cmu_int_t event) {

    switch (event){
    case CMU_INT_OLRI:
        REG_SET_FIELD(cdp->cmu->CMU_ISR, CMU_CMU_ISR_OLRI, 1U);
        break;
    case CMU_INT_FLCI:
        REG_SET_FIELD(cdp->cmu->CMU_ISR, CMU_CMU_ISR_FLCI, 1U);
        break;
    case CMU_INT_FLLI:
        REG_SET_FIELD(cdp->cmu->CMU_ISR, CMU_CMU_ISR_FLLI, 1U);
        break;
    case CMU_INT_FHHI:
        REG_SET_FIELD(cdp->cmu->CMU_ISR, CMU_CMU_ISR_FHHI, 1U);
        break;
    case CMU_INT_NONE:
        break;
    default:
        /* Default case as required by MISRA.*/
        break;
    }
}

void cmu_stop(cmu_driver_t *cdp) {

    /* Disable CMU instance.*/
    REG_SET_FIELD(cdp->cmu->CMU_CSR, CMU_CMU_CSR_CME, 0U);

    /* Clear the interrupt flags.*/
    cdp->cmu->CMU_ISR = 0xFU;

    /* Disable peripheral clock.*/
    cmu_dev_clock_disable(cdp);
}

void cmu_deinit(cmu_driver_t *cdp) {

    /* Set pointer to CMU peripheral to NULL.*/
    cdp->cmu = NULL;

    /* Reset RTC parameters.*/
    cmu_lld_reset_parameters(cdp);
}

/** @} */
