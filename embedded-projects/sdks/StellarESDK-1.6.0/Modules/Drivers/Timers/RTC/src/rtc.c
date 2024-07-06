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
 * @file    rtc.c
 * @brief   RTC driver source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup TIMERS
 * @ingroup DRIVERS
 * @addtogroup RTC
 * @ingroup TIMERS
 * @{
 */

#include <rtc.h>
#include <clock.h>
#include <rtc_private.h>

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief  Waits for registers synchronization.
 * @note   Called to wait the registers synchronization flag.
 *
 * @param[in] \_rdp\_   pointer to a @p rtc_driver_t structure
  *
 * @notapi
 */
#define RTC_WAIT_FOR_SYNC(_rdp_)                  \
    while (REG_READ_FIELD((_rdp_)->rtc->ICSR, RTC_ICSR_RSF) == 0U) {;}

/**
 * @brief  Waits until the ALRAWF flag is clear.
 * @note   Called to wait the registers synchronization flag.
 *
 * @param[in] \_rdp\_   pointer to a @p rtc_driver_t structure
  *
 * @notapi
 */
#define RTC_WAIT_ALRAWF_FLAG(_rdp_)                  \
    while (REG_READ_FIELD((_rdp_)->rtc->ICSR, RTC_ICSR_ALRAWF) == 0U) {;}

/**
 * @brief  Waits until the ALRAWF flag is clear.
 * @note   Called to wait the registers synchronization flag.
 *
 * @param[in] \_rdp\_   pointer to a @p rtc_driver_t structure
  *
 * @notapi
 */
#define RTC_WAIT_WUTWF_FLAG(_rdp_)                  \
    while (REG_READ_FIELD((_rdp_)->rtc->ICSR, RTC_ICSR_WUTWF) == 0U) {;}

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
 * @brief   Enable the RTC configuration registers write protection
 *
 * @param[in] rdp       pointer to an @p rtc_driver_t structure.
 *
 * @notapi
 */
static void rtc_lld_writeprotection_on(rtc_driver_t *rdp) {

    /* Any wrong key will turn on the write protection.*/
    rdp->rtc->WPR = 0xFFU;
}

/**
 * @brief   Disable the RTC configuration registers write protection
 *
 * @param[in] rdp       pointer to an @p rtc_driver_t structure.
 *
 * @notapi
 */
static void rtc_lld_writeprotection_off(rtc_driver_t *rdp) {

    /* Keys to unlock the write protection.*/
    rdp->rtc->WPR = 0xCAU;
    rdp->rtc->WPR = 0x53U;
}

/**
 * @brief   Enters in RTC initialization mode
 * @note    This functionality is write protecred. rtc_lld_writeprotection_off is
 *          required before using this function.
 *
 * @param[in] rdp       pointer to an @p rtc_driver_t structure.
 *
 * @notapi
 */
static void rtc_lld_initmode_on(rtc_driver_t *rdp) {

    /* Check if the Initialization mode is already on.*/
    if (REG_READ_FIELD(rdp->rtc->ICSR, RTC_ICSR_INITF) == 1U) {
        return;
    } else{
        /* Set the Initialization mode */
        REG_SET_FIELD(rdp->rtc->ICSR, RTC_ICSR_INIT, 1U);

        while(REG_READ_FIELD(rdp->rtc->ICSR, RTC_ICSR_INITF) != 1U) {
            /* Wait that the initialization mode is entered. It takes around
               2 RTCCLK clock cycles due to clock synchronization.*/
        }
    }
}

/**
 * @brief   Exits from RTC initialization mode
 *
 * @param[in] rdp       pointer to an @p rtc_driver_t structure.
 *
 * @notapi
 */
static void rtc_lld_initmode_off(rtc_driver_t *rdp) {

    /* Check if the Initialization mode is already off.*/
    if (REG_READ_FIELD(rdp->rtc->ICSR, RTC_ICSR_INITF) == 0U) {
        return;
    } else{
        /* Clear the Initialization mode */
        REG_SET_FIELD(rdp->rtc->ICSR, RTC_ICSR_INIT, 0U);

        while(REG_READ_FIELD(rdp->rtc->ICSR, RTC_ICSR_INITF) != 0U) {
            /* Wait that the initialization mode is cleared.*/
        }
    }
}

/**
 * @brief   Convert a 2 digit decimal value to BCD format
 *
 * @param[in] value     byte to be converted
 * @return              converted byte.
 *
 * @notapi
 */
static uint32_t rtc_lld_bin2bcd(uint32_t value) {

    uint32_t tens = 0U;

    while(value >= 10U) {
        tens++;
        value -= 10U;
    }

    return ((uint32_t)(tens << 4U) | value);
}

/**
 * @brief   Convert a BCD value to 2 digit decimal format
 *
 * @param[in] value     BCD value to be converted
 * @return              converted word.
 *
 * @notapi
 */
static uint8_t rtc_lld_bcd2bin(uint8_t value) {

    uint8_t tens;

    tens = ((value & 0xF0U) >> 4) * 10U;

    return (uint8_t)(tens + (value & 0x0FU));
}

/**
 * @brief   Sets the RTC initialization parameters
 *
 * @note    This function must be called in initialization mode.
 *
 * @param[out] rdp      pointer to an @p rtc_driver_t structure
 *
 * @notapi
 */
static void rtc_lld_set_initialization_parameters(rtc_driver_t *rdp) {

    uint32_t time_reg, date_reg;

    rdp->rtc->PRER = (rdp->prediv_s << RTC_PRER_PREDIV_S_Pos |
                     (rdp->prediv_a << RTC_PRER_PREDIV_A_Pos));

    /* Configure the time register.*/
    if(rdp->out_format == RTC_BIN_FORMAT) {
        /* BIN format requires transformation to BCD before writing.*/
        rdp->time->seconds = (uint8_t)rtc_lld_bin2bcd(rdp->time->seconds);
        rdp->time->minutes = (uint8_t)rtc_lld_bin2bcd(rdp->time->minutes);
        rdp->time->hours   = (uint8_t)rtc_lld_bin2bcd(rdp->time->hours);
    }

    /* Configure the hour format field.*/
    REG_SET_FIELD(rdp->rtc->CR, RTC_CR_FMT, rdp->time->hour_format);

    /* am/pm flag handling in case of 24H format.*/
    if(rdp->time->hour_format == RTC_24H_FORMAT) {
        rdp->time->am_pm = RTC_AM;
    }

    time_reg = ((uint32_t)rdp->time->hours   << RTC_TR_HU_Pos  |
                (uint32_t)rdp->time->minutes << RTC_TR_MNU_Pos |
                (uint32_t)rdp->time->seconds << RTC_TR_SU_Pos  |
                (uint32_t)rdp->time->am_pm   << RTC_TR_PM_Pos);

    REG_SET(rdp->rtc->TR, time_reg);

    /* Configure the date register.*/
    if(rdp->out_format == RTC_BIN_FORMAT){
        /* BIN format requires transformation to BCD before writing.*/
        rdp->date->year  = (uint8_t)rtc_lld_bin2bcd(rdp->date->year);
        rdp->date->month = (uint8_t)rtc_lld_bin2bcd(rdp->date->month);
        rdp->date->date  = (uint8_t)rtc_lld_bin2bcd(rdp->date->date);
    }

    date_reg = ((uint32_t)rdp->date->year  << RTC_DR_YU_Pos |
                (uint32_t)rdp->date->month << RTC_DR_MU_Pos |
                (uint32_t)rdp->date->date  << RTC_DR_DU_Pos |
                (uint32_t)rdp->date->day   << RTC_DR_WDU_Pos);

    REG_SET(rdp->rtc->DR, date_reg);
}

/**
 * @brief   Add or subtract one hour to the calendar in one single operation
 *          without going through the initialization procedure
 *
 * @param[out] rdp      pointer to an @p rtc_driver_t structure.
 *
 * @notapi
 */
static void rtc_lld_set_dst_parameters(rtc_driver_t *rdp) {

    /* Configure the backup flag.*/
    if(rdp->dst_bkp == true) {
        REG_SET_FIELD(rdp->rtc->CR, RTC_CR_BKP, 1U);
    }

    /* Configure the summer time change.*/
    if(rdp->dst_op == RTC_DST_ADD1H) {
        REG_SET_FIELD(rdp->rtc->CR, RTC_CR_ADD1H, 1U);
    }

    /* Configure the winter time change.*/
    if(rdp->dst_op == RTC_DST_SUB1H) {
        REG_SET_FIELD(rdp->rtc->CR, RTC_CR_SUB1H, 1U);
    }
}

/**
 * @brief   Set the alarm parameters
 *
 * @param[out] rdp      pointer to an @p rtc_driver_t structure.
 *
 * @notapi
 */
static void rtc_lld_set_alarm_parameters(rtc_driver_t *rdp) {

    uint32_t alarm_reg, alarm_ssr;
    IRQn_Type vector;
    exti_line_t exti_line;

    /* Configure the alarm register.*/
    if(rdp->is_alarm_en == true) {

        if(rdp->out_format == RTC_BIN_FORMAT) {
            /* BIN format requires transformation to BCD before writing.*/
            rdp->alarm->sub_sec = rtc_lld_bin2bcd(rdp->alarm->sub_sec);

            rdp->alarm->seconds = (uint8_t)rtc_lld_bin2bcd(rdp->alarm->seconds);
            rdp->alarm->minutes = (uint8_t)rtc_lld_bin2bcd(rdp->alarm->minutes);
            rdp->alarm->hours   = (uint8_t)rtc_lld_bin2bcd(rdp->alarm->hours);
            rdp->alarm->date    = (uint8_t)rtc_lld_bin2bcd(rdp->alarm->date);
        }

        /* Configure the alarm registers.*/
        alarm_reg = ((uint32_t)rdp->alarm->match_day  << RTC_ALRMAR_MSK4_Pos  |
                     (uint32_t)rdp->alarm->date_sel   << RTC_ALRMAR_WDSEL_Pos |
                     (uint32_t)rdp->alarm->date       << RTC_ALRMAR_DU_Pos    |
                     (uint32_t)rdp->alarm->match_hour << RTC_ALRMAR_MSK3_Pos  |
                     (uint32_t)rdp->alarm->am_pm      << RTC_ALRMAR_PM_Pos    |
                     (uint32_t)rdp->alarm->hours      << RTC_ALRMAR_HU_Pos    |
                     (uint32_t)rdp->alarm->match_min  << RTC_ALRMAR_MSK2_Pos  |
                     (uint32_t)rdp->alarm->minutes    << RTC_ALRMAR_MNU_Pos   |
                     (uint32_t)rdp->alarm->match_sec  << RTC_ALRMAR_MSK1_Pos  |
                     (uint32_t)rdp->alarm->seconds    << RTC_ALRMAR_SU_Pos);

        alarm_ssr = ((uint32_t)rdp->alarm->match_ss << RTC_ALRMASSR_MAKSS_Pos |
                     (uint32_t)rdp->alarm->sub_sec  << RTC_ALRMASSR_SS_Pos);

        /* Clear alarm interrupt enable and alarm enable flags.*/
        REG_CLEAR_FIELD(rdp->rtc->CR, RTC_CR_ALRAIE);
        REG_CLEAR_FIELD(rdp->rtc->CR, RTC_CR_ALRAE);

        /* Clear alarm flag.*/
        REG_SET_FIELD(rdp->rtc->SCR, RTC_SCR_CALRAF, 1U);

        /* Wait until the alarm write flag is set.*/
        RTC_WAIT_ALRAWF_FLAG(rdp);

        /* Set the alarm configuration registers.*/
        REG_SET(rdp->rtc->ALRMAR, alarm_reg);
        REG_SET(rdp->rtc->ALRMASSR, alarm_ssr);

        /* Enable the alarm.*/
        REG_SET_FIELD(rdp->rtc->CR, RTC_CR_ALRAE, 1U);

        if(rdp->alarm->is_it_en == true) {
            /* If interrupt mode is selected, configure RTC interrupt.*/

            /* Enable alarm interrupt.*/
            REG_SET_FIELD(rdp->rtc->CR, RTC_CR_ALRAIE, 1U);

            vector = rtc_dev_get_alarm_vector(rdp);
            irq_set_priority(vector, rdp->alarm->prio);
            irq_enable(vector);

            /* Get EXTI line related to RTC alarm.*/
            exti_line = rtc_dev_get_exti_alarm_line(rdp);

            /* RTC Alarm Interrupt Configuration: EXTI configuration */
            exti_set_line(exti_line, EXTI_MODE_INTERRUPT, EXTI_TRIGGER_RISING);
        }
    }
}

/**
 * @brief   Set the wake-up timer parameters
 *
 * @param[out] rdp      pointer to an @p rtc_driver_t structure.
 *
 * @notapi
 */
static void rtc_lld_set_wakeup_parameters(rtc_driver_t *rdp) {

    IRQn_Type vector;
    exti_line_t exti_line;

    /* Configure the wakeup timer.*/
    if(rdp->is_wakeup_en == true) {

        /* Disable the wakeup timer.*/
        REG_CLEAR_FIELD(rdp->rtc->CR, RTC_CR_WUTE);

        /* Poll for WUTWF not needed in itialization mode.*/

        /* Set the autoreload value.*/
        REG_SET(rdp->rtc->WUTR, rdp->wakeup->autoreload);

        /* Set the clock source.*/
        REG_SET_FIELD(rdp->rtc->CR, RTC_CR_WUCKSEL, rdp->wakeup->clk_source);

        /* Enable the wakeup timer interrupt.*/
        REG_SET_FIELD(rdp->rtc->CR, RTC_CR_WUTIE, 1U);

        vector = rtc_dev_get_wakeup_vector(rdp);
        irq_set_priority(vector, rdp->wakeup->prio);
        irq_enable(vector);

        /* Get EXTI line related to RTC alarm.*/
        exti_line = rtc_dev_get_exti_wakeup_line(rdp);

        /* RTC Alarm Interrupt Configuration: EXTI configuration */
        exti_set_line(exti_line, EXTI_MODE_INTERRUPT, EXTI_TRIGGER_RISING);

        /* Enable the wakeup timer.*/
        REG_SET_FIELD(rdp->rtc->CR, RTC_CR_WUTE, 1U);
    }
}

/**
 * @brief   Resets RTC parameters.
 *
 * @param[out] rdp      pointer to an @p rtc_driver_t structure
 *
 * @notapi
 */
static void rtc_lld_reset_parameters(rtc_driver_t *rdp) {

    uint8_t i;

    /* Reset driver structure to default values.*/
    rdp->time = NULL;
    rdp->date = NULL;
    rdp->dst_op = RTC_DST_NONE;
    rdp->dst_bkp = false;
    rdp->out_format = RTC_BIN_FORMAT;
    rdp->is_alarm_en = false;
    rdp->alarm = NULL;
    rdp->is_wakeup_en = false;
    rdp->wakeup = NULL;

    for(i = 0; i < (uint8_t)RTC_CB_OPS_MAX; i++) {
        rdp->cb[i] = NULL;
    }
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void rtc_init(rtc_driver_t *rdp) {

    /* Set pointer to CRC register block.*/
    rdp->rtc = rtc_dev_get_reg_ptr(rdp);

    /* Reset RTC parameters.*/
    rtc_lld_reset_parameters(rdp);
}

void rtc_set_clock(rtc_driver_t *rdp, rtc_clk_src_t source,
                   uint32_t prediv_s, uint32_t prediv_a) {

    rdp->clk_src = source;
    rdp->prediv_s = prediv_s;
    rdp->prediv_a = prediv_a;
}

rtc_output_t rtc_set_output_format(rtc_driver_t *rdp, rtc_output_t out_format) {

    rtc_output_t prev_out_format;

    prev_out_format = rdp->out_format;
    rdp->out_format = out_format;

    return prev_out_format;
}

rtc_time_t *rtc_set_time(rtc_driver_t *rdp, rtc_time_t *new_time) {

    rtc_time_t *prev_time;

    prev_time = rdp->time;
    rdp->time = new_time;

    return prev_time;
}

rtc_date_t *rtc_set_date(rtc_driver_t *rdp, rtc_date_t *new_date) {

    rtc_date_t *prev_date;

    prev_date = rdp->date;
    rdp->date = new_date;

    return prev_date;
}

rtc_dst_op_t rtc_set_dst(rtc_driver_t *rdp, rtc_dst_op_t dst_op, bool backup) {

    rtc_dst_op_t prev_op;

    prev_op = rdp->dst_op;
    rdp->dst_op = dst_op;

    rdp->dst_bkp = backup;

    return prev_op;
}

rtc_cb_t rtc_set_cb(rtc_driver_t *rdp, rtc_cb_ops_t cb_op, rtc_cb_t cb) {

    rtc_cb_t prev_cb;

    prev_cb = rdp->cb[cb_op];
    rdp->cb[cb_op] = cb;

    return prev_cb;
}

rtc_alarm_t *rtc_set_alarm(rtc_driver_t *rdp, rtc_alarm_t *new_alarm) {

    rtc_alarm_t *prev_alarm;

    rdp->is_alarm_en = true;

    prev_alarm = rdp->alarm;
    rdp->alarm = new_alarm;

    return prev_alarm;
}

rtc_wakeup_t *rtc_set_wkpu(rtc_driver_t *rdp, rtc_wakeup_t *wut) {

    rtc_wakeup_t *prev_wut;

    rdp->is_wakeup_en = true;

    prev_wut = rdp->wakeup;
    rdp->wakeup = wut;

    return prev_wut;
}

void rtc_start(rtc_driver_t *rdp) {

    /* Enable peripheral clock if not yet enabled.*/
    if(!is_clock_enable_AHB1L(RCC_APB1LENR_RTC)) {
        rtc_dev_clock_enable(rdp);
    }

    /* Reset registers to default values.*/
    rdp->rtc->TR       = 0x00000000;
    rdp->rtc->DR       = 0x00002101;
    rdp->rtc->ICSR     = 0x00000007;
    rdp->rtc->PRER     = 0x007F00FF;
    rdp->rtc->WUTR     = 0x0000FFFF;
    rdp->rtc->CR       = 0x00000000;
    rdp->rtc->WPR      = 0x00000000;
    rdp->rtc->CALR     = 0x00000000;
    rdp->rtc->SHIFTR   = 0x00000000;
    rdp->rtc->ALRMAR   = 0x00000000;
    rdp->rtc->ALRMASSR = 0x00000000;
    rdp->rtc->SCR      = 0x00000000;

    /* Disable write protection.*/
    rtc_lld_writeprotection_off(rdp);

    /* Enter initialization mode.*/
    rtc_lld_initmode_on(rdp);

    /* Apply the initial configuration.*/
    rtc_lld_set_initialization_parameters(rdp);

    /* Exit initialization mode.*/
    rtc_lld_initmode_off(rdp);

    /* Configure the daylight saving time parameters.*/
    rtc_lld_set_dst_parameters(rdp);

    /* Configure the alarm parameters.*/
    rtc_lld_set_alarm_parameters(rdp);

    /* Configure the wake-up timer parameters.*/
    rtc_lld_set_wakeup_parameters(rdp);

    /* Enable write protection.*/
    rtc_lld_writeprotection_on(rdp);

    /* Wait the register synchronization.*/
    RTC_WAIT_FOR_SYNC(rdp);
}

rtc_time_t rtc_get_time(rtc_driver_t *rdp) {

    rtc_time_t curr_time;
    uint32_t time_reg, am_pm;

    curr_time.sub_seconds = REG_READ(rdp->rtc->SSR);
    curr_time.ss_fraction = (uint32_t)(rdp->rtc->PRER & RTC_PRER_PREDIV_S_Msk);

    time_reg = REG_READ(rdp->rtc->TR);

    am_pm = ((time_reg & RTC_TR_PM_Msk) >> RTC_TR_PM_Pos);

    switch (am_pm){
    case 0:
        curr_time.am_pm = RTC_AM;
        break;
    case 1:
        curr_time.am_pm = RTC_PM;
        break;
    default:
        /* default case as required by MISRA.*/
        break;
    }

    curr_time.seconds = (uint8_t)((time_reg & (RTC_TR_ST_Msk | RTC_TR_SU_Msk))
                                            >> RTC_TR_SU_Pos);
    curr_time.minutes = (uint8_t)((time_reg & (RTC_TR_MNT_Msk | RTC_TR_MNU_Msk))
                                            >> RTC_TR_MNU_Pos);
    curr_time.hours   = (uint8_t)((time_reg & (RTC_TR_HT_Msk | RTC_TR_HU_Msk))
                                            >> RTC_TR_HU_Pos);

    if(rdp->out_format == RTC_BIN_FORMAT) {
        /* Required conversion from BCD to BYTE format*/
        curr_time.seconds = rtc_lld_bcd2bin(curr_time.seconds);
        curr_time.minutes = rtc_lld_bcd2bin(curr_time.minutes);
        curr_time.hours   = rtc_lld_bcd2bin(curr_time.hours);
    }

    return curr_time;
}

rtc_date_t rtc_get_date(rtc_driver_t *rdp) {

    rtc_date_t curr_date;
    uint32_t date_reg, day;

    date_reg = REG_READ(rdp->rtc->DR);

    curr_date.year  = (uint8_t)((date_reg & (RTC_DR_YT_Msk | RTC_DR_YU_Msk))
                                          >> RTC_DR_YU_Pos);
    curr_date.month = (uint8_t)((date_reg & (RTC_DR_MT_Msk | RTC_DR_MU_Msk))
                                          >> RTC_DR_MU_Pos);
    curr_date.date  = (uint8_t)((date_reg & (RTC_DR_DT_Msk | RTC_DR_DU_Msk))
                                          >> RTC_DR_DU_Pos);

    day = ((date_reg & RTC_DR_WDU_Msk) >> RTC_DR_WDU_Pos);
    
    switch (day){
    case 1:
        curr_date.day = RTC_MONDAY;
        break;
    case 2:
        curr_date.day = RTC_TUESDAY;
        break;
    case 3:
        curr_date.day = RTC_WEDNESDAY;
        break;
    case 4:
        curr_date.day = RTC_THURSDAY;
        break;
    case 5:
        curr_date.day = RTC_FRIDAY;
        break;
    case 6:
        curr_date.day = RTC_SATURDAY;
        break;
    case 7:
        curr_date.day = RTC_SUNDAY;
        break;
    default:
        /* default case as required by MISRA.*/
        break;
    }

    if(rdp->out_format == RTC_BIN_FORMAT){
        curr_date.year  = rtc_lld_bcd2bin(curr_date.year);
        curr_date.month = rtc_lld_bcd2bin(curr_date.month);
        curr_date.date  = rtc_lld_bcd2bin(curr_date.date);
    }

    return curr_date;
}

bool rtc_get_alarm_event(rtc_driver_t *rdp) {

    uint32_t alarm_flag;

    alarm_flag = REG_READ_FIELD(rdp->rtc->SR, RTC_SR_ALRAF);

    if(alarm_flag == 1U) {
        return true;
    } else {
        return false;
    }
}

bool rtc_get_backup_flag(rtc_driver_t *rdp) {

    uint32_t backup_flag;

    backup_flag = REG_READ_FIELD(rdp->rtc->CR, RTC_CR_BKP);

    if (backup_flag == 1U) {
        return true;
    } else {
        return false;
    }
}

void rtc_stop(rtc_driver_t *rdp) {

    IRQn_Type vector;

    /* Disable interrupts.*/
    if(rdp->alarm->is_it_en == true) {
        vector = rtc_dev_get_alarm_vector(rdp);
        irq_disable(vector);
    }
    if(rdp->is_wakeup_en == true) {
        vector = rtc_dev_get_wakeup_vector(rdp);
        irq_disable(vector);
    }

    /* Disable peripheral clock.*/
    rtc_dev_clock_disable(rdp);
}

void rtc_deinit(rtc_driver_t *rdp) {

    /* Set pointer to RTC register block to NULL.*/
    rdp->rtc = NULL;

    /* Reset RTC parameters.*/
    rtc_lld_reset_parameters(rdp);
}

void __rtc_serve_alarm_interrupt(rtc_driver_t *rdp) {

    /* Invoke the callback.*/
    rdp->cb[RTC_CB_OPS_ALARM](rdp);

    /* clear the interrupt.*/
    REG_SET_FIELD(rdp->rtc->SCR, RTC_SCR_CALRAF, 1U);
}

void __rtc_serve_wkpu_interrupt(rtc_driver_t *rdp) {

    /* Invoke the callback.*/
    rdp->cb[RTC_CB_OPS_WKUP](rdp);

    /* clear the interrupt.*/
    REG_SET_FIELD(rdp->rtc->SCR, RTC_SCR_CWUTF, 1U);
}
/** @} */
