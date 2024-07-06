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
 * @file    smpu.c
 * @brief   SMPU driver source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup SMPU
 * @ingroup SYSTEM
 * @{
 */

#include <smpu.h>
#include <smpu_private.h>

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
 * @brief   Sets the SMPU parameters
 * 
 * @param[out] sdp      pointer to a @p smpu_driver_t structure
 *
 * @notapi
 */
static void smpu_lld_set_parameters(smpu_driver_t *sdp) {

    uint8_t i, reg_num;
    uint32_t word3;

    /* Get number of available regions.*/
    reg_num = (uint8_t)REG_READ_FIELD(sdp->smpu->CESR1, SMPU_CESR1_NRGD) * 4U;

    /* Disable SMPU module to grant access to the registers.*/
    REG_SET_FIELD(sdp->smpu->CESR0, SMPU_CESR0_GVLD, 0U);

    for(i = 0U; i < reg_num; i++) {

        /* Check if the region is configured.*/
        if(sdp->is_region_en[i] == true) {

            /* Reset word3.*/
            word3 = 0x0U;

            /* Set cache inhibit flag.*/
            if(sdp->region_cfg[i].cacheability == false) {
                word3 |= 0x1UL << SMPU_REGION_RGDN_WORD3_CI_Pos;
            }

            /* Set read-only flag.*/
            if(sdp->region_cfg[i].read_only == true) {
                word3 |= SMPU_REGION_RGDN_WORD3_RO;
            }

            /* Set validity bit for the region descriptor.*/
            word3 |= SMPU_REGION_RGDN_WORD3_VLD;

            /* Set start address.*/
            REG_SET(sdp->smpu->REGION[i].RGDN_WORD0,
                    sdp->region_cfg[i].start_addr);

            /* Set end address.*/
            REG_SET(sdp->smpu->REGION[i].RGDN_WORD1,
                    sdp->region_cfg[i].end_addr);

            /* Set bus master access rights.*/
            REG_SET(sdp->smpu->REGION[i].RGDN_WORD2_FMT0,
                    sdp->region_cfg[i].accs_rights);

            /* Set word3 parameters.*/
            REG_SET(sdp->smpu->REGION[i].RGDN_WORD3, word3);
        }
    }

    /* Enable SMPU module.*/
    REG_SET_FIELD(sdp->smpu->CESR0, SMPU_CESR0_GVLD, 1U);
}

/**
 * @brief   Resets the SMPU parameters
 * 
 * @param[out] sdp      pointer to a @p smpu_driver_t structure
 *
 * @notapi
 */
static void smpu_lld_reset_parameters(smpu_driver_t *sdp) {

    uint8_t i;

    /* Reset all the region descriptors.*/
    for(i = 0; i < SMPU_REGIONS_NUM; i++){
        sdp->is_region_en[i] = false;

        sdp->region_cfg[i].start_addr   = 0x0U;
        sdp->region_cfg[i].end_addr     = 0x0U;
        sdp->region_cfg[i].accs_rights  = 0x0U;
        sdp->region_cfg[i].cacheability = true;
        sdp->region_cfg[i].read_only    = false;
    }
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void smpu_init(smpu_driver_t *sdp) {

    /* Set pointer to SMPU register block.*/
    sdp->smpu = smpu_dev_get_reg_ptr(sdp);

    /* Reset SMPU parameters.*/
    smpu_lld_reset_parameters(sdp);
}

bool smpu_enable_region(smpu_driver_t *sdp, smpu_region_t region, bool enable) {

    bool prev_state;

    prev_state = sdp->is_region_en[region];
    sdp->is_region_en[region] = enable;

    return prev_state;
}

uint32_t smpu_set_start_address(smpu_driver_t *sdp, smpu_region_t region,
                                uint32_t address) {

    uint32_t old_address;

    old_address = sdp->region_cfg[region].start_addr;
    sdp->region_cfg[region].start_addr = address;

    return old_address;
}

uint32_t smpu_set_end_address(smpu_driver_t *sdp, smpu_region_t region,
                              uint32_t address) {

    uint32_t old_address;

    old_address = sdp->region_cfg[region].end_addr;
    sdp->region_cfg[region].end_addr = address;

    return old_address;
}

uint32_t smpu_set_rights(smpu_driver_t *sdp, smpu_region_t region, 
                         smpu_master_t master, smpu_access_right_t rights) {

    uint32_t old_rights;

    old_rights = sdp->region_cfg[region].accs_rights;
    sdp->region_cfg[region].accs_rights |= ((uint32_t)(rights) <<
                                           ((uint32_t)(master) * 2U));
    return old_rights;
}

bool smpu_set_cacheability(smpu_driver_t *sdp, smpu_region_t region,
                           bool enable) {

    bool old_flag;

    old_flag = sdp->region_cfg[region].cacheability;
    sdp->region_cfg[region].cacheability = enable;

    return old_flag;
}

bool smpu_set_read_only(smpu_driver_t *sdp, smpu_region_t region, bool enable) {

    bool old_flag;

    old_flag = sdp->region_cfg[region].read_only;
    sdp->region_cfg[region].read_only = enable;

    return old_flag;
}

void smpu_remove_region(smpu_driver_t *sdp, smpu_region_t region) {

    /* Reset the region on driver level.*/
    sdp->is_region_en[region]            = false;
    sdp->region_cfg[region].start_addr   = 0x0U;
    sdp->region_cfg[region].end_addr     = 0x0U;
    sdp->region_cfg[region].accs_rights  = 0x0U;
    sdp->region_cfg[region].cacheability = true;
    sdp->region_cfg[region].read_only    = false;
}

void smpu_start(smpu_driver_t *sdp) {

    /* Enable peripheral clock.*/
    smpu_dev_clock_enable(sdp);

    /* Set SMPU parameters.*/
    smpu_lld_set_parameters(sdp);
}

smpu_hw_info_t smpu_get_hardware_info(smpu_driver_t *sdp) {

    smpu_hw_info_t hw_info;

    /* Get hardware revision level.*/
    hw_info.rev_level = (uint8_t)REG_READ_FIELD(sdp->smpu->CESR0,
                                                SMPU_CESR0_HRL);

    /* Get number of region descriptors available.*/
    hw_info.rgd_num = (uint8_t)REG_READ_FIELD(sdp->smpu->CESR1,
                                              SMPU_CESR1_NRGD) * 4U;

    return hw_info;
}

smpu_error_info_t smpu_get_error(smpu_driver_t *sdp) {

    smpu_error_info_t err_info;
    uint8_t rw_error, i, eattr;
    uint16_t merr, movr;
    uint32_t regions;

    /* Find the bus master that generated the error.*/
    merr = (uint16_t)REG_READ_FIELD(sdp->smpu->CESR0, SMPU_CESR0_MERR);

    switch (merr) {
    case 1U:
        err_info.master = SMPU_ERR_MSTR_CORE_2;
        break;
    case 2U:
        err_info.master = SMPU_ERR_MSTR_CORE_HSM;
        break;
    case 8U:
        err_info.master = SMPU_ERR_MSTR_DMA2;
        break;
    case 256U:
        err_info.master = SMPU_ERR_MSTR_CORE_1;
        break;
    case 2048U:
        err_info.master = SMPU_ERR_MSTR_DMA1;
        break;
    case 8192U:
        err_info.master = SMPU_ERR_MSTR_DEBUG;
        break;
    default:
        err_info.master = SMPU_NO_MSTR_ERR;
        break;
    }

    /* Detect master overrun.*/
    movr = (uint16_t)REG_READ_FIELD(sdp->smpu->CESR1, SMPU_CESR1_MEOVR);

    switch (movr) {
    case 1U:
        err_info.overrun = SMPU_ERR_MSTR_CORE_2;
        break;
    case 4U:
        err_info.overrun = SMPU_ERR_MSTR_CORE_HSM;
        break;
    case 16U:
        err_info.overrun = SMPU_ERR_MSTR_DMA2;
        break;
    case 256U:
        err_info.overrun = SMPU_ERR_MSTR_CORE_1;
        break;
    case 2048U:
        err_info.overrun = SMPU_ERR_MSTR_DMA1;
        break;
    case 8192U:
        err_info.overrun = SMPU_ERR_MSTR_DEBUG;
        break;
    default:
        err_info.overrun = SMPU_NO_MSTR_ERR;
        break;
    }

    /* Get the error address*/
    err_info.address = sdp->smpu->ERROR[err_info.master].EAR;

    /* Get the error access control detail.*/
    regions = REG_READ_FIELD(sdp->smpu->ERROR[err_info.master].EDR,
                             SMPU_ERROR_EDR_EACD);

    for(i = 0U; i < SMPU_REGIONS_NUM; i++) {
        /* Check if i-th bit is 1.*/
        if(((regions >> i) % 2U) == 1U) {
            err_info.region[(SMPU_REGIONS_NUM - i) - 1U] = true;
        } else {
            err_info.region[(SMPU_REGIONS_NUM - i) - 1U] = false;
        }
    }

    /* Get the error attributes.*/
    eattr = (uint8_t)REG_READ_FIELD(sdp->smpu->ERROR[err_info.master].EDR,
                                    SMPU_ERROR_EDR_EATTR);

    switch (eattr)
    {
    case 0U:
        err_info.attribute = SMPU_ERR_UM_IA;
        break;
    case 1U:
        err_info.attribute = SMPU_ERR_UM_DA;
        break;
    case 2U:
        err_info.attribute = SMPU_ERR_SM_IA;
        break;
    case 3U:
        err_info.attribute = SMPU_ERR_SM_DA;
        break;
    default:
        /* Default case as required by MISRA.*/
        break;
    }

    /* Get the read/write error detail.*/
    rw_error = (uint8_t)REG_READ_FIELD(sdp->smpu->ERROR[err_info.master].EDR,
                                       SMPU_ERROR_EDR_ERW);

    if(rw_error == 0U) {
        err_info.rw_access = SMPU_READ_ACCESS;
    } else {
        err_info.rw_access = SMPU_WRITE_ACCESS;
    }

    return err_info;
}

void smpu_clear_errors(smpu_driver_t *sdp) {

    REG_SET_FIELD(sdp->smpu->CESR0, SMPU_CESR0_MERR, 0xFFFFU);
}

void smpu_stop(smpu_driver_t *sdp) {

    /* Disable peripheral clock.*/
    smpu_dev_clock_disable(sdp);
}

void smpu_deinit(smpu_driver_t *sdp) {

    /* Set pointer to SMPU register block to NULL.*/
    sdp->smpu = NULL;

    /* Reset SMPU parameters.*/
    smpu_lld_reset_parameters(sdp);
}

/** @} */
