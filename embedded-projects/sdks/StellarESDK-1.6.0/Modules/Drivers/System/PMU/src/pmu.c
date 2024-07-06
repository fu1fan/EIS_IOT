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
 * @file    pmu.c
 * @brief   PMU driver source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup PMU
 * @ingroup SYSTEM
 * @{
 */

#include <pmu.h>
#include <pmu_private.h>

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

static const pmu_vd_descriptor_t pmu_vd_descriptors[PMU_VD_NUM] = {
    {PMU_VD_LVD119_C,      24U},
    {PMU_VD_LVD119_FL,     26U},
    {PMU_VD_LVD119_DD,     27U},
    {PMU_VD_LVD119_RC,     29U},
    {PMU_VD_LVD119_PLL0,   30U},
    {PMU_VD_LVD119_PLL1,   31U},
    {PMU_VD_HVD140_C,      16U},
    {PMU_VD_UVD145_RC,     29U},
    {PMU_VD_LVD290_DACCMP, 16U},
    {PMU_VD_LVD290_OSC,    17U},
    {PMU_VD_UVD380_AS,     18U},
    {PMU_VD_UVD380_DACCMP, 19U},
    {PMU_VD_LVD290_C,      24U},
    {PMU_VD_LVD290_FL,     25U},
    {PMU_VD_LVD290_IO1,    27U},
    {PMU_VD_LVD290_IO0,    28U},
    {PMU_VD_LVD290_AS,     29U},
    {PMU_VD_LVD290_AD,     30U},
};

/*===========================================================================*/
/* Local function prototypes                                                 */
/*===========================================================================*/

/**
 * @brief   Returns the position in the registers Reset Event Enable, Reset
 *          Event Select, Interrupt Enable and FCCU Event Enable of the bit
 *          related to the specified PMU voltage detect.
 *
 * @param[in] \_reg_en\_     Reset Event Enable register related to the PMU
 *                           voltage detect to be set
 * @param[in] \_reg_sel\_    Reset Event Select register related to the PMU
 *                           voltage detect to be set
 * @param[in] \_vd\_         PMU voltage detect to be set
 *
 * @notapi
 */

#define PMU_GET_POS(_vd_)                                           \
    (pmu_vd_descriptors[(_vd_)].pos)

/**
 * @brief   Sets Reset Event Enable bit and Reset Event Select bit for a
 *          specified PMU voltage detect.
 *
 * @param[in] \_reg_en\_     Reset Event Enable register related to the PMU
 *                           voltage detect to be set
 * @param[in] \_reg_sel\_    Reset Event Select register related to the PMU
 *                           voltage detect to be set
 * @param[in] \_res_type\_   reset type
 * @param[in] \_vd\_         PMU voltage detect to be set
 *
 * @notapi
 */
#define PMU_SET_REE_RES(_reg_en_, _reg_sel_, _res_type_, _vd_)      \
    switch(_res_type_) {                                            \
        case PMU_RES_DESTRUCTIVE_RESET:                             \
            (_reg_en_)  |= 1UL << (PMU_GET_POS(_vd_));              \
            (_reg_sel_) &= ~(1UL << (PMU_GET_POS(_vd_)));           \
            break;                                                  \
        case PMU_RES_FUNCTIONAL_RESET:                              \
            (_reg_en_)  |= 1UL << (PMU_GET_POS(_vd_));              \
            (_reg_sel_) |= 1UL << (PMU_GET_POS(_vd_));              \
            break;                                                  \
        case PMU_RES_NO_RESET:                                      \
        default:                                                    \
            (_reg_en_)  &= ~(1UL << (PMU_GET_POS(_vd_)));           \
            (_reg_sel_) &= ~(1UL << (PMU_GET_POS(_vd_)));           \
            break;                                                  \
    }

/**
 * @brief   Clears Reset Event Enable bit and Reset Event Select bit for a
 *          specified PMU voltage detect.
 *
 * @param[in] \_reg_en\_     Reset Event Enable register related to the PMU
 *                           voltage detect to be cleared
 * @param[in] \_reg_sel\_    Reset Event Select register related to the PMU
 *                           voltage detect to be cleared
 * @param[in] \_vd\_         PMU voltage detect to be cleared
 *
 * @notapi
 */
#define PMU_CLEAR_REE_RES(_reg_en_, _reg_sel_, _vd_)                \
    (_reg_en_)  &= ~(1UL << (PMU_GET_POS(_vd_)));                   \
    (_reg_sel_) &= ~(1UL << (PMU_GET_POS(_vd_)));

/**
 * @brief   Sets Interrupt Enable bit for a specified PMU voltage detect.
 *
 * @param[in] \_reg\_        Interrupt Enable register related to the PMU
 *                           voltage detect to be set
 * @param[in] \_int_en\_     interrupt enable flag
 * @param[in] \_vd\_         PMU voltage detect to be set
 *
 * @notapi
 */
#define PMU_SET_IE(_reg_, _int_en_, _vd_)                           \
    if ((_int_en_) == true) {                                       \
        (_reg_) |= 1UL << (PMU_GET_POS(_vd_));                      \
    } else {                                                        \
        (_reg_) &= ~(1UL << (PMU_GET_POS(_vd_)));                   \
    }

/**
 * @brief   Clears Interrupt Enable bit for a specified PMU voltage detect.
 *
 * @param[in] \_reg\_        Interrupt Enable register related to the PMU
 *                           voltage detect to be cleared
 * @param[in] \_vd\_         PMU voltage detect to be cleared
 *
 * @notapi
 */
#define PMU_CLEAR_IE(_reg_, _vd_)                                   \
    (_reg_) &= ~(1UL << (PMU_GET_POS(_vd_)));

/**
 * @brief   Sets FCCU Event Enable bit for a specified PMU voltage detect.
 *
 * @param[in] \_reg\_        FCCU Event Enable register related to the PMU
 *                           voltage detect to be set
 * @param[in] \_fccu_en\_    FCCU enable flag
 * @param[in] \_vd\_         PMU voltage detect to be set
 *
 * @notapi
 */
#define PMU_SET_FEE(_reg_, _fccu_en_, _vd_)                         \
    if ((_fccu_en_) == true) {                                      \
        (_reg_) |= 1UL << (PMU_GET_POS(_vd_));                      \
    } else {                                                        \
        (_reg_) &= ~(1UL << (PMU_GET_POS(_vd_)));                   \
    }

/**
 * @brief   Clears FCCU Event Enable bit for a specified PMU voltage detect.
 *
 * @param[in] \_reg\_        FCCU Event Enable register related to the PMU
 *                           voltage detect to be cleared
 * @param[in] \_vd\_         PMU voltage detect to be cleared
 *
 * @notapi
 */
#define PMU_CLEAR_FEE(_reg_, _vd_)                                  \
    (_reg_) &= ~(1UL << (PMU_GET_POS(_vd_)));

/**
 * @brief   Returns the event pending status for a specified PMU voltage detect.
 *
 * @param[in] \_reg\_        Event Pending register related to a specified PMU
 *                           voltage detect
 * @param[in] \_vd\_         PMU voltage detect for which to get the event
 *                           pending status
 *
 * @notapi
 */
#define PMU_GET_EP(_reg_, _vd_)                                     \
    ((uint8_t)(0x1U & ((_reg_) >> (PMU_GET_POS(_vd_)))))

/**
 * @brief   Clears the event pending status for a specified PMU voltage detect.
 *
 * @param[in] \_reg\_        Event Pending register related to a specified PMU
 *                           voltage detect
 * @param[in] \_vd\_         PMU voltage detect for which to clear the event
 *                           pending status
 *
 * @notapi
 */
#define PMU_CLEAR_EP(_reg_, _vd_)                                   \
    ((_reg_) |= (0x1UL << (PMU_GET_POS(_vd_))))

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Resets PMU parameters.
 *
 * @param[out] pdp      pointer to an @p pmu_driver_t structure
 *
 * @notapi
 */
static void pmu_lld_reset_paramters(pmu_driver_t *pdp) {

    uint8_t i;

    pdp->prio      = IRQ_MIN_PRIORITY;
    pdp->is_int_en = false;

    for (i = 0U; i < PMU_VD_NUM; i++) {
        pdp->res_type[i] = PMU_RES_NO_RESET;
        pdp->int_en[i]   = false;
        pdp->fccu_en[i]  = false;
    }
}

/**
 * @brief   Returns PMU voltage detect event pending status.
 *
 * @param[out] pdp      pointer to a @p pmu_driver_t structure
 * @param[in] vd        voltage detect for which to return the event pending
 *                      status. It can be one of the following values:
 *            @arg @ref PMU_VD_LVD119_C
 *            @arg @ref PMU_VD_LVD119_FL
 *            @arg @ref PMU_VD_LVD119_DD
 *            @arg @ref PMU_VD_LVD119_RC
 *            @arg @ref PMU_VD_LVD119_PLL0
 *            @arg @ref PMU_VD_LVD119_PLL1
 *            @arg @ref PMU_VD_HVD140_C
 *            @arg @ref PMU_VD_UVD145_RC
 *            @arg @ref PMU_VD_LVD290_DACCMP
 *            @arg @ref PMU_VD_LVD290_OSC
 *            @arg @ref PMU_VD_UVD380_AS
 *            @arg @ref PMU_VD_UVD380_DACCMP
 *            @arg @ref PMU_VD_LVD290_C
 *            @arg @ref PMU_VD_LVD290_FL
 *            @arg @ref PMU_VD_LVD290_IO1
 *            @arg @ref PMU_VD_LVD290_IO0
 *            @arg @ref PMU_VD_LVD290_AS
 *            @arg @ref PMU_VD_LVD290_AD
 * @return              PMU voltage detect event pending status ("0" means
 *                      event has not been detected, "1" otherwise).
 *
 * @notapi
 */
static uint8_t pmu_lld_get_status(pmu_driver_t *pdp, pmu_vd_t vd) {

    /* Get voltage detect event pending status.*/
    switch(vd) {
        case PMU_VD_LVD119_C:
        case PMU_VD_LVD119_FL:
        case PMU_VD_LVD119_DD:
        case PMU_VD_LVD119_RC:
        case PMU_VD_LVD119_PLL0:
        case PMU_VD_LVD119_PLL1:
            return PMU_GET_EP(pdp->pmu->EPR_LV0, vd);
        case PMU_VD_HVD140_C:
        case PMU_VD_UVD145_RC:
            return PMU_GET_EP(pdp->pmu->EPR_LV1, vd);
        case PMU_VD_LVD290_DACCMP:
        case PMU_VD_LVD290_OSC:
        case PMU_VD_UVD380_AS:
        case PMU_VD_UVD380_DACCMP:
        case PMU_VD_LVD290_C:
        case PMU_VD_LVD290_FL:
        case PMU_VD_LVD290_IO1:
        case PMU_VD_LVD290_IO0:
        case PMU_VD_LVD290_AS:
        case PMU_VD_LVD290_AD:
            return PMU_GET_EP(pdp->pmu->EPR_HV0, vd);
        default:
            return 0U;
    }
}

/**
 * @brief   Clears the event pending status for a specified PMU voltage detect.
 *
 * @param[out] pdp      pointer to an @p pmu_driver_t structure
 * @param[in] vd        PMU voltage detect for which to clear the event pending
 *                      status. It can be one of the following values:
 *            @arg @ref PMU_VD_LVD119_C
 *            @arg @ref PMU_VD_LVD119_FL
 *            @arg @ref PMU_VD_LVD119_DD
 *            @arg @ref PMU_VD_LVD119_RC
 *            @arg @ref PMU_VD_LVD119_PLL0
 *            @arg @ref PMU_VD_LVD119_PLL1
 *            @arg @ref PMU_VD_HVD140_C
 *            @arg @ref PMU_VD_UVD145_RC
 *            @arg @ref PMU_VD_LVD290_DACCMP
 *            @arg @ref PMU_VD_LVD290_OSC
 *            @arg @ref PMU_VD_UVD380_AS
 *            @arg @ref PMU_VD_UVD380_DACCMP
 *            @arg @ref PMU_VD_LVD290_C
 *            @arg @ref PMU_VD_LVD290_FL
 *            @arg @ref PMU_VD_LVD290_IO1
 *            @arg @ref PMU_VD_LVD290_IO0
 *            @arg @ref PMU_VD_LVD290_AS
 *            @arg @ref PMU_VD_LVD290_AD
 *
 * @notapi
 */
static void pmu_lld_clear_status(pmu_driver_t *pdp, pmu_vd_t vd) {

    /* Clear voltage detect event pending status.*/
    switch(vd) {
        case PMU_VD_LVD119_C:
        case PMU_VD_LVD119_FL:
        case PMU_VD_LVD119_DD:
        case PMU_VD_LVD119_RC:
        case PMU_VD_LVD119_PLL0:
        case PMU_VD_LVD119_PLL1:
            PMU_CLEAR_EP(pdp->pmu->EPR_LV0, vd);
            break;
        case PMU_VD_HVD140_C:
        case PMU_VD_UVD145_RC:
            PMU_CLEAR_EP(pdp->pmu->EPR_LV1, vd);
            break;
        case PMU_VD_LVD290_DACCMP:
        case PMU_VD_LVD290_OSC:
        case PMU_VD_UVD380_AS:
        case PMU_VD_UVD380_DACCMP:
        case PMU_VD_LVD290_C:
        case PMU_VD_LVD290_FL:
        case PMU_VD_LVD290_IO1:
        case PMU_VD_LVD290_IO0:
        case PMU_VD_LVD290_AS:
        case PMU_VD_LVD290_AD:
            PMU_CLEAR_EP(pdp->pmu->EPR_HV0, vd);
            break;
        default:
            /* Dummy comment as required by MISRA.*/
            break;
    }
}

/**
 * @brief   Resets PMU registers.
 *
 * @param[out] pdp      pointer to an @p pmu_driver_t structure
 *
 * @notapi
 */
static void pmu_lld_reg_reset(pmu_driver_t *pdp) {

    uint8_t i;

    for (i = 0U; i < PMU_VD_NUM; i++) {
        /* Clears event pending status of all voltate detects.*/
        pmu_lld_clear_status(pdp, pmu_vd_descriptors[i].vd);

        /* Clear voltage detect settings.*/
        switch(i) {
            case (uint8_t)(PMU_VD_LVD119_C):
            case (uint8_t)(PMU_VD_LVD119_FL):
            case (uint8_t)(PMU_VD_LVD119_DD):
            case (uint8_t)(PMU_VD_LVD119_RC):
            case (uint8_t)(PMU_VD_LVD119_PLL0):
            case (uint8_t)(PMU_VD_LVD119_PLL1):
                PMU_CLEAR_REE_RES(pdp->pmu->REE_LV0, pdp->pmu->RES_LV0, i);
                PMU_CLEAR_IE(pdp->pmu->IE_LV0, i);
                PMU_CLEAR_FEE(pdp->pmu->FEE_LV0, i);
                break;
            case (uint8_t)(PMU_VD_HVD140_C):
            case (uint8_t)(PMU_VD_UVD145_RC):
                PMU_CLEAR_REE_RES(pdp->pmu->REE_LV1, pdp->pmu->RES_LV1, i);
                PMU_CLEAR_IE(pdp->pmu->IE_LV1, i);
                PMU_CLEAR_FEE(pdp->pmu->FEE_LV1, i);
                break;
            case (uint8_t)(PMU_VD_LVD290_DACCMP):
            case (uint8_t)(PMU_VD_LVD290_OSC):
            case (uint8_t)(PMU_VD_UVD380_AS):
            case (uint8_t)(PMU_VD_UVD380_DACCMP):
            case (uint8_t)(PMU_VD_LVD290_C):
            case (uint8_t)(PMU_VD_LVD290_FL):
            case (uint8_t)(PMU_VD_LVD290_IO1):
            case (uint8_t)(PMU_VD_LVD290_IO0):
            case (uint8_t)(PMU_VD_LVD290_AS):
            case (uint8_t)(PMU_VD_LVD290_AD):
                PMU_CLEAR_REE_RES(pdp->pmu->REE_HV0, pdp->pmu->RES_HV0, i);
                PMU_CLEAR_IE(pdp->pmu->IE_HV0, i);
                PMU_CLEAR_FEE(pdp->pmu->FEE_HV0, i);
                break;
            default:
                /* Dummy comment as required by MISRA.*/
                break;
        }
    }
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void pmu_init(pmu_driver_t *pdp) {

    /* Set pointer to PMU register block.*/
    pdp->pmu = pmu_dev_get_reg_ptr(pdp);

    /* Reset PMU parameters.*/
    pmu_lld_reset_paramters(pdp);
}

uint32_t pmu_set_prio(pmu_driver_t *pdp, uint32_t prio) {

    uint32_t prev_prio;

    prev_prio = pdp->prio;
    pdp->prio = prio;

    return prev_prio;
}

pmu_reset_t pmu_set_reset(pmu_driver_t *pdp, pmu_vd_t vd, pmu_reset_t res_type) {

    pmu_reset_t prev_res_type;

    prev_res_type = pdp->res_type[vd];
    pdp->res_type[vd] = res_type;

    return prev_res_type;
}

bool pmu_enable_interrupt(pmu_driver_t *pdp, pmu_vd_t vd, bool enable) {

    bool prev_int_en;

    prev_int_en = pdp->int_en[vd];
    pdp->int_en[vd] = enable;

    return prev_int_en;
}

bool pmu_enable_fccu(pmu_driver_t *pdp, pmu_vd_t vd, bool enable) {

    bool prev_fccu_en;

    prev_fccu_en = pdp->fccu_en[vd];
    pdp->fccu_en[vd] = enable;

    return prev_fccu_en;
}

pmu_cb_t pmu_set_cb(pmu_driver_t *pdp, pmu_vd_t vd, pmu_cb_t cb) {

    pmu_cb_t prev_cb;

    prev_cb = pdp->cb[vd];
    pdp->cb[vd] = cb;

    return prev_cb;
}

void pmu_start(pmu_driver_t *pdp) {

    uint8_t i;
    IRQn_Type vector;

    /* Reset PMU registers.*/
    pmu_lld_reg_reset(pdp);

    /* Check if interrupt is enabled at least for one of the voltage detects.*/
    pdp->is_int_en = false;
    for (i = 0U; i < PMU_VD_NUM; i++) {
        if (pdp->int_en[i] == true) {
            pdp->is_int_en = true;
            break;
        }
    }

    /* If interrupt is enabled at least for one of the voltage detects,
       set bit IE_EN that permits interrupt enabled bits to be written.*/
    if (pdp->is_int_en == true) {
        pdp->pmu->IE_G |= PMC_DIG_IE_G_IE_EN;

        /* Enable PMU interrup.*/
        vector = pmu_dev_get_vector(pdp);
        irq_set_priority(vector, pdp->prio);
        irq_enable(vector);
    } else {
        pdp->pmu->IE_G &= ~PMC_DIG_IE_G_IE_EN;
    }

    /* Set voltage detect settings.*/
    for (i = 0U; i < PMU_VD_NUM; i++) {
        switch(i) {
            case (uint8_t)(PMU_VD_LVD119_C):
            case (uint8_t)(PMU_VD_LVD119_FL):
            case (uint8_t)(PMU_VD_LVD119_DD):
            case (uint8_t)(PMU_VD_LVD119_RC):
            case (uint8_t)(PMU_VD_LVD119_PLL0):
            case (uint8_t)(PMU_VD_LVD119_PLL1):
                PMU_SET_REE_RES(pdp->pmu->REE_LV0, pdp->pmu->RES_LV0, pdp->res_type[i], i);
                PMU_SET_IE(pdp->pmu->IE_LV0, pdp->int_en[i], i);
                PMU_SET_FEE(pdp->pmu->FEE_LV0, pdp->fccu_en[i], i);
                break;
            case (uint8_t)(PMU_VD_HVD140_C):
            case (uint8_t)(PMU_VD_UVD145_RC):
                PMU_SET_REE_RES(pdp->pmu->REE_LV1, pdp->pmu->RES_LV1, pdp->res_type[i], i);
                PMU_SET_IE(pdp->pmu->IE_LV1, pdp->int_en[i], i);
                PMU_SET_FEE(pdp->pmu->FEE_LV1, pdp->fccu_en[i], i);
                break;
            case (uint8_t)(PMU_VD_LVD290_DACCMP):
            case (uint8_t)(PMU_VD_LVD290_OSC):
            case (uint8_t)(PMU_VD_UVD380_AS):
            case (uint8_t)(PMU_VD_UVD380_DACCMP):
            case (uint8_t)(PMU_VD_LVD290_C):
            case (uint8_t)(PMU_VD_LVD290_FL):
            case (uint8_t)(PMU_VD_LVD290_IO1):
            case (uint8_t)(PMU_VD_LVD290_IO0):
            case (uint8_t)(PMU_VD_LVD290_AS):
            case (uint8_t)(PMU_VD_LVD290_AD):
                PMU_SET_REE_RES(pdp->pmu->REE_HV0, pdp->pmu->RES_HV0, pdp->res_type[i], i);
                PMU_SET_IE(pdp->pmu->IE_HV0, pdp->int_en[i], i);
                PMU_SET_FEE(pdp->pmu->FEE_HV0, pdp->fccu_en[i], i);
                break;
            default:
                /* Dummy comment as required by MISRA.*/
                break;
        }
    }
}

bool pmu_get_status(pmu_driver_t *pdp, pmu_vd_t vd) {

    if (pmu_lld_get_status(pdp, vd) == 0U) {
        return false;
    } else {
        return true;
    }
}

void pmu_clear_status(pmu_driver_t *pdp, pmu_vd_t vd) {

    pmu_lld_clear_status(pdp, vd);
}

void pmu_stop(pmu_driver_t *pdp) {

    IRQn_Type vector;

    /* If enabled, disable PMU interrupt.*/
    if (pdp->is_int_en == true) {
        vector = pmu_dev_get_vector(pdp);
        irq_disable(vector);
    }

    /* Reset PMU registers.*/
    pmu_lld_reg_reset(pdp);
}

void pmu_deinit(pmu_driver_t *pdp) {

    /* Reset pointer to PMU register block.*/
    pdp->pmu = NULL;

    /* Reset PMU parameters.*/
    pmu_lld_reset_paramters(pdp);
}

void __pmu_serve_interrupt(pmu_driver_t *pdp) {

    uint8_t i;
    uint8_t status;

    for (i = 0U; i < PMU_VD_NUM; i++) {
        status = pmu_lld_get_status(pdp, pmu_vd_descriptors[i].vd);
        if ((pdp->int_en[i] == true) && (status == 1U)) {
            if (pdp->cb[i] != NULL) {
                pdp->cb[i](pdp);
            }
            pmu_lld_clear_status(pdp, pmu_vd_descriptors[i].vd);
        }
    }
}

/** @} */
