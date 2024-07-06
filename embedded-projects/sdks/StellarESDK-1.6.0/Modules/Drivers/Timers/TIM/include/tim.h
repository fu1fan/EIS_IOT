/****************************************************************************
 *
 * Copyright (c) 2022 STMicroelectronics - All Rights Reserved
 *
 * License terms: STMicroelectronics Proprietary in accordance with licensing
 * terms SLA0098 at www.st.com.
 *
 * THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
 * INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *****************************************************************************/
/**
 * @file    tim.h
 * @brief   TIM driver header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup TIMERS
 * @ingroup DRIVERS
 * @addtogroup TIM
 * @ingroup TIMERS
 * @{
 */

#ifndef _TIM_H_
#define _TIM_H_

#include <dma.h>
#include <typedefs.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    TIM max number of channels
 * @{
 */
#define TIM_MAX_CH_NUM                      6U
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
 * @brief   Type of a TIM driver.
 */
typedef struct tim_driver tim_driver_t;

/**
 * @brief   Type of a TIM callback.
 *
 * @param[in] tdp      pointer to the @p tim_driver_t object
 */
typedef void (*tim_cb_t)(tim_driver_t *tdp);

/**
 * @enum tim_ckd_t
 * Supported clock divisions.
 */
typedef enum {
    TIM_CKD_1,                         /**< Clock division = 1 (tDTS = tTim_ker_clk)     */
    TIM_CKD_2,                         /**< Clock division = 2 (tDTS = 2 * tTim_ker_clk) */
    TIM_CKD_4                          /**< Clock division = 4 (tDTS = 4 * tTim_ker_clk) */
} tim_ckd_t;

/**
 * @brief   Type of a TIM prescaler.
 */
typedef uint16_t tim_presc_t;

/**
 * @enum tim_cms_t
 * Supported center-aligned modes.
 */
typedef enum {
    TIM_CMS_EDGE_ALIGNED = 0,          /**< Edge-aligned mode.                                                                                                                                        */
    TIM_CMS_CENTER_ALIGNED_1 = 1,      /**< Center-aligned mode 1. The counter counts up and down alternatively. Output compare interrupt flags are set only when the counter is counting down.       */
    TIM_CMS_CENTER_ALIGNED_2 = 2,      /**< Center-aligned mode 2. The counter counts up and down alternatively. Output compare interrupt flags are set only when the counter is counting up.         */
    TIM_CMS_CENTER_ALIGNED_3 = 3       /**< Center-aligned mode 3. The counter counts up and down alternatively. Output compare interrupt flags are set both when the counter is counting up or down. */
} tim_cms_t;

/**
 * @enum tim_mode_t
 * Supported channel modes.
 */
typedef enum {
    TIM_MODE_PERIODIC,                 /**< Channel mode = Periodic                                                                                */
    TIM_MODE_PWM_ACTIVE_HIGH,          /**< Channel mode = PWM Active High                                                                         */
    TIM_MODE_PWM_ACTIVE_HIGH_COMP,     /**< Channel mode = PWM Active High + Complementary                                                         */
    TIM_MODE_PWM_ACTIVE_LOW,           /**< Channel mode = PWM Active Low                                                                          */
    TIM_MODE_PWM_ACTIVE_LOW_COMP,      /**< Channel mode = PWM Active Low + Complementary                                                          */
    TIM_MODE_PWM_FROZEN,               /**< Channel mode = Frozen                                                                                  */
    TIM_MODE_PWM_ACTIVE_LEVEL,         /**< Channel mode = Active level on match                                                                   */
    TIM_MODE_PWM_INACTIVE_LEVEL,       /**< Channel mode = Inactive level on match                                                                 */
    TIM_MODE_PWM_COMBINED_MODE1,       /**< Channel mode = Combined PWM mode 1                                                                     */
    TIM_MODE_PWM_COMBINED_MODE2,       /**< Channel mode = Combined PWM mode 2                                                                     */
    TIM_MODE_PWM_ASYMMETRIC_MODE1,     /**< Channel mode = Asymmetric PWM mode 1                                                                   */
    TIM_MODE_PWM_ASYMMETRIC_MODE2,     /**< Channel mode = Asymmetric PWM mode 2                                                                   */
    TIM_MODE_ICU_RISING_EDGE,          /**< Channel mode = ICU Rising Edge                                                                         */
    TIM_MODE_ICU_FALLING_EDGE,         /**< Channel mode = ICU Falling Edge                                                                        */
    TIM_MODE_ICU_BOTH_EDGES,           /**< Channel mode = ICU Both Edges                                                                          */
    TIM_MODE_ICU_RISING_EDGE_IND,      /**< Channel mode = ICU Rising Edge Indirect (CCx input mapped on input y with x=1,2/y=2,1 or x=3,4/y=4,3)  */
    TIM_MODE_ICU_FALLING_EDGE_IND      /**< Channel mode = ICU Falling Edge Indirect (CCx input mapped on input y with x=1,2/y=2,1 or x=3,4/y=4,3) */
} tim_mode_t;

/**
 * @enum tim_trgo_t
 * Supported trigger outputs.
 */
typedef enum {
    TIM_TRGO_RESET = 0,                /*!< UG bit from the TIMx_EGR register is used as trigger output */
    TIM_TRGO_ENABLE = 1,               /*!< Counter Enable signal (CNT_EN) is used as trigger output    */
    TIM_TRGO_UPDATE = 2,               /*!< Update event is used as trigger output                      */
    TIM_TRGO_CC1IF = 3,                /*!< CC1 capture or compare match is used as trigger output      */
    TIM_TRGO_OC1REFC = 4,              /*!< OC1REFC signal is used as trigger output                    */
    TIM_TRGO_OC2REFC = 5,              /*!< OC2REFC signal is used as trigger output                    */
    TIM_TRGO_OC3REFC = 6,              /*!< OC3REFC signal is used as trigger output                    */
    TIM_TRGO_OC4REFC = 7,              /*!< OC4REFC signal is used as trigger output                    */
    TIM_TRGO_ENCODERCLK = 8            /*!< Encoder clock signal is used as trigger output              */
} tim_trgo_t;

/**
 * @enum tim_trgo2_t
 * Supported trigger outputs 2.
 */
typedef enum {
    TIM_TRGO2_RESET = 0,                         /*!< UG bit from the TIMx_EGR register is used as trigger output 2        */
    TIM_TRGO2_ENABLE = 1,                        /*!< Counter Enable signal (CNT_EN) is used as trigger output 2           */
    TIM_TRGO2_UPDATE = 2,                        /*!< Update event is used as trigger output 2                             */
    TIM_TRGO2_CC1IF = 3,                         /*!< CC1 capture or compare match is used as trigger output 2             */
    TIM_TRGO2_OC1REFC = 4,                       /*!< OC1REFC signal is used as trigger output 2                           */
    TIM_TRGO2_OC2REFC = 5,                       /*!< OC2REFC signal is used as trigger output 2                           */
    TIM_TRGO2_OC3REFC = 6,                       /*!< OC3REFC signal is used as trigger output 2                           */
    TIM_TRGO2_OC4REFC = 7,                       /*!< OC4REFC signal is used as trigger output 2                           */
    TIM_TRGO2_OC5REFC = 8,                       /*!< OC5REFC signal is used as trigger output 2                           */
    TIM_TRGO2_OC6REFC = 9,                       /*!< OC6REFC signal is used as trigger output 2                           */
    TIM_TRGO2_OC4REFC_RISINGFALLING = 10,        /*!< OC4REFC rising or falling edges are used as trigger output 2         */
    TIM_TRGO2_OC6REFC_RISINGFALLING = 11,        /*!< OC6REFC rising or falling edges are used as trigger output 2         */
    TIM_TRGO2_OC4REFC_RISING_OC6_RISING = 12,    /*!< OC4REFC or OC6REFC rising edges are used as trigger output 2         */
    TIM_TRGO2_OC4REFC_RISING_OC6_FALLING = 13,   /*!< OC4REFC rising or OC6REFC falling edges are used as trigger output 2 */
    TIM_TRGO2_OC5REFC_RISING_OC6_RISING = 14,    /*!< OC5REFC or OC6REFC rising edges are used as trigger output 2         */
    TIM_TRGO2_OC5REFC_RISING_OC6_FALLING = 15    /*!< OC5REFC rising or OC6REFC falling edges are used as trigger output 2 */
} tim_trgo2_t;

/**
 * @enum tim_sms_t
 * Supported slave mode selections.
 */
typedef enum {
    TIM_SMODE_DISABLED = 0,                      /*!< Slave mode disabled                                                    */
    TIM_SMODE_QUAD_ENCODER_MODE1 = 1,            /*!< Quadrature encoder mode 1, x2 mode                                     */
    TIM_SMODE_QUAD_ENCODER_MODE2 = 2,            /*!< Quadrature encoder mode 2, x2 mode                                     */
    TIM_SMODE_QUAD_ENCODER_MODE3 = 3,            /*!< Quadrature encoder mode 3, x4 mode                                     */
    TIM_SMODE_RESET = 4,                         /*!< Reset mode                                                             */
    TIM_SMODE_GATED = 5,                         /*!< Gated mode                                                             */
    TIM_SMODE_TRIGGER = 6,                       /*!< Trigger mode                                                           */
    TIM_SMODE_EXT_CLOCK_1 = 7,                   /*!< External clock mode 1                                                  */
    TIM_SMODE_COMBO_RESET_TRIGGER = 8,           /*!< Combined reset + trigger mode                                          */
    TIM_SMODE_QUAD_ENCODER_TI1FP1_RISING = 14,   /*!< Quadrature encoder mode, x1 mode, counting on tim_ti1fp1 rising edges  */
    TIM_SMODE_QUAD_ENCODER_TI1FP2_RISING = 15,   /*!< Quadrature encoder mode, x1 mode, counting on tim_ti1fp2 rising edges  */
    TIM_SMODE_QUAD_ENCODER_TI1FP1_FALLING = 16,  /*!< Quadrature encoder mode, x1 mode, counting on tim_ti1fp1 falling edges */
    TIM_SMODE_QUAD_ENCODER_TI1FP2_FALLING = 17,  /*!< Quadrature encoder mode, x1 mode, counting on tim_ti1fp2 falling edges */
} tim_sms_t;

/**
 * @enum tim_trgi_t
 * Supported trigger inputs.
 */
typedef enum {
    TIM_TRGI_ITR0 = 0,       /*!< Internal trigger 0     */
    TIM_TRGI_ITR1 = 1,       /*!< Internal trigger 1     */
    TIM_TRGI_ITR2 = 2,       /*!< Internal trigger 2     */
    TIM_TRGI_ITR3 = 3,       /*!< Internal trigger 3     */
    TIM_TRGI_TI1F_ED = 4,    /*!< Edge detector          */
    TIM_TRGI_TI1FP1 = 5,     /*!< Filtered timer input 1 */
    TIM_TRGI_TI2FP2 = 6,     /*!< Filtered timer input 2 */
    TIM_TRGI_ETRF = 7,       /*!< External trigger input */
    TIM_TRGI_ITR4 = 8,       /*!< Internal trigger 4     */
    TIM_TRGI_ITR5 = 9,       /*!< Internal trigger 5     */
    TIM_TRGI_ITR6 = 10,      /*!< Internal trigger 6     */
    TIM_TRGI_ITR7 = 11,      /*!< Internal trigger 7     */
    TIM_TRGI_ITR8 = 12,      /*!< Internal trigger 8     */
    TIM_TRGI_ITR9 = 13,      /*!< Internal trigger 9     */
    TIM_TRGI_ITR10 = 14      /*!< Internal trigger 10    */
} tim_trgi_t;

/**
 * @enum tim_etps_t
 * Supported external trigger prescalers.
 */
typedef enum {
    TIM_ETPS_DIV1,      /*!< No prescaler                     */
    TIM_ETPS_DIV2,      /*!< ETR input frequency divided by 2 */
    TIM_ETPS_DIV4,      /*!< ETR input frequency divided by 4 */
    TIM_ETPS_DIV8       /*!< ETR input frequency divided by 8 */
} tim_etps_t;

/**
 * @enum tim_etp_t
 * Supported external trigger polarities.
 */
typedef enum {
    TIM_ETPS_NOT_INVERTED,   /*!< ETR not inverted */
    TIM_ETPS_INVERTED        /*!< ETR inverted     */
} tim_etp_t;

/**
 * @enum tim_bkp_t
 * Supported break polarities.
 */
typedef enum {
    TIM_BKP_LOW,   /*!< Break/Break 2 polarity low  */
    TIM_BKP_HIGH   /*!< Break/Break 2 polarity high */
} tim_bkp_t;

/**
 * @enum tim_ti1s_t
 * Supported TIM input 1 selections.
 */
typedef enum {
    TIM_TI1S_TI1,  /*!< tim_ti1_in[15:0] multiplexer output is connected to tim_ti1 input                                                         */
    TIM_TI1S_XOR   /*!< tim_ti1_in[15:0], tim_ti2_in[15:0] and tim_ti3_in[15:0] multiplexers outputs are XORed and connected to the tim_ti1 input */
} tim_ti1s_t;

/**
 * @brief   Type of TIM DMA configuration.
 */
typedef struct {
    /**
     * @brief   TIM DMA stream to be used for operations.
     */
    uint32_t                 dma_stream_id;
    /**
     * @brief   TIM DMA stream bus priority (3..0).
     */
    uint32_t                 dma_stream_bus_prio;
    /**
     * @brief   TIM DMA stream IRQ priority (3..15).
     */
    uint32_t                 dma_stream_irq_prio;
} tim_dma_conf_t;

/**
 * @enum tim_dma_ops_t
 * TIM DMA operation type.
 */
typedef enum {
    TIM_DMA_OPS_UPDATE = 0U,           /**< DMA on update even            */
    TIM_DMA_OPS_CH1 = 1U,              /**< DMA on Capture/compare 1 even */
    TIM_DMA_OPS_CH2 = 2U,              /**< DMA on Capture/compare 2 even */
    TIM_DMA_OPS_CH3 = 3U,              /**< DMA on Capture/compare 3 even */
    TIM_DMA_OPS_CH4 = 4U,              /**< DMA on Capture/compare 4 even */
    TIM_DMA_OPS_MAX                    /**< Number of DMA operations      */
} tim_dma_ops_t;

/**
 * @enum tim_dma_dir_t
 * TIM DMA direction type.
 */
typedef enum {
    TIM_DMA_DIR_P2M,                   /**< DMA peripheral to memory direction */
    TIM_DMA_DIR_M2P,                   /**< DMA memory to peripheral direction */
    TIM_DMA_DIR_M2M                    /**< DMA memory to memory direction     */
} tim_dma_dir_t;

/**
 * @enum tim_dma_size_t
 * TIM DMA size type.
 */
typedef enum {
    TIM_DMA_SIZE_BYTE,                 /**< DMA data size = byte      */
    TIM_DMA_SIZE_HWORD,                /**< DMA data size = half word */
    TIM_DMA_SIZE_WORD                  /**< DMA data size = word      */
} tim_dma_size_t;

/**
 * @enum tim_dma_size_t
 * TIM DMA circular mode type.
 */
typedef enum {
    TIM_DMA_CIRC_DISABLED,             /**< DMA circular mode disabled */
    TIM_DMA_CIRC_ENABLED               /**< DMA circular mode enabled  */
} tim_dma_circ_t;

/**
 * @enum tim_dir_t
 * TIM supported counter directions.
 */
typedef enum {
    TIM_DIR_UPCOUNTER,                /**< Counter used as upcounter   */
    TIM_DIR_DOWNCOUNTER               /**< Counter used as downcounter */
} tim_dir_t;

/**
 * @enum tim_cb_ops_t
 * TIM callback operation type.
 */
typedef enum {
    TIM_CB_OPS_UPDATE,                 /**< Update event                */
    TIM_CB_OPS_CC1,                    /**< Capture/compare 1 event     */
    TIM_CB_OPS_CC2,                    /**< Capture/compare 2 event     */
    TIM_CB_OPS_CC3,                    /**< Capture/compare 3 event     */
    TIM_CB_OPS_CC4,                    /**< Capture/compare 4 event     */
    TIM_CB_OPS_DMA_UPDATE,             /**< Update DMA event            */
    TIM_CB_OPS_DMA_CC1,                /**< Capture/compare 1 DMA event */
    TIM_CB_OPS_DMA_CC2,                /**< Capture/compare 2 DMA event */
    TIM_CB_OPS_DMA_CC3,                /**< Capture/compare 3 DMA event */
    TIM_CB_OPS_DMA_CC4,                /**< Capture/compare 4 DMA event */
    TIM_CB_OPS_BK,                     /**< Break event                 */
    TIM_CB_OPS_BK2,                    /**< Break 2 event               */
    TIM_CB_OPS_COM,                    /**< COM event                   */
    TIM_CB_OPS_MAX                     /**< Number of callbacks         */
} tim_cb_ops_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   From division factor to PSC register value.
 */
#define TIM_DIVIDE_BY(n)    ((uint32_t)((n) - 1U))

/**
 * @name    TIM deadtime settings
 * @{
 */
#define TIM_BDTR_DTG_VALUE(n)               ((uint32_t)(n) << TIM_BDTR_DTG_Pos)
#define TIM_DTR2_DTGF_VALUE(n)              ((uint32_t)(n) << TIM_DTR2_DTGF_Pos)
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Export driver modules.*/
#include <tim_instances.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Enables/Disables one pulse mode.
 * @note    This configuration becomes effective after tim_start.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] enable    one pulse mode status to be configured (true to enable
 *                      the one pulse mode, false to disable one pulse mode).
 * @return              previous one pulse mode status.
 *
 * @api
 */
bool tim_enable_opm(tim_driver_t *tdp, bool enable);

/**
 * @brief   Configures TIM clock division.
 * @note    This configuration becomes effective after tim_start.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] ckd       clock division to be configured. It can be one of the
 *                      following values:
 *            @arg @ref TIM_CKD_1
 *            @arg @ref TIM_CKD_2
 *            @arg @ref TIM_CKD_4
 * @return              previous clock division. It can be one of the following
 *                      values:
 *            @arg @ref TIM_CKD_1
 *            @arg @ref TIM_CKD_2
 *            @arg @ref TIM_CKD_4
 *
 * @api
 */
tim_ckd_t tim_set_ckd(tim_driver_t *tdp, tim_ckd_t ckd);

/**
 * @brief   Configures TIM interrupt priority.
 * @note    This configuration becomes effective after tim_start.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] prio      interrupt priority to be configured
 * @return              previous interrupt priority.
 *
 * @api
 */
uint32_t tim_set_prio(tim_driver_t *tdp, uint32_t prio);

/**
 * @brief   Configures TIM prescaler.
 * @note    This configuration becomes effective after tim_start.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] presc     prescaler to be configured
 * @return              previous prescaler.
 *
 * @api
 */
tim_presc_t tim_set_presc(tim_driver_t *tdp, tim_presc_t presc);

/**
 * @brief   Configures TIM center-aligned mode.
 * @note    This configuration becomes effective after tim_start.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] cms       center-aligned mode to be configured. It can be one of
 *                      the following values:
 *            @arg @ref TIM_CMS_EDGE_ALIGNED
 *            @arg @ref TIM_CMS_CENTER_ALIGNED_1
 *            @arg @ref TIM_CMS_CENTER_ALIGNED_2
 *            @arg @ref TIM_CMS_CENTER_ALIGNED_3
 * @return              previous center-aligned mode. It can be one of the
 *                      following values:
 *            @arg @ref TIM_CMS_EDGE_ALIGNED
 *            @arg @ref TIM_CMS_CENTER_ALIGNED_1
 *            @arg @ref TIM_CMS_CENTER_ALIGNED_2
 *            @arg @ref TIM_CMS_CENTER_ALIGNED_3
 *
 * @api
 */
tim_cms_t tim_set_cms(tim_driver_t *tdp, tim_cms_t cms);

/**
 * @brief   Configures TIM deadtime.
 * @note    This configuration becomes effective after tim_start.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] rdt       rising edge deadtime to be configured
 * @param[in] fdt       falling edge deadtime to be configured
 *
 * @api
 */
void tim_set_dt(tim_driver_t *tdp, uint32_t rdt, uint32_t fdt);

/**
 * @brief   Configures TIM channel modes.
 * @note    This configuration becomes effective after tim_start.
 * @note    The TIM channel mode is ignored when the TIM slave mode is one of
 *          the Quadrature Encoder modes:
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_MODE1
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_MODE2
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_MODE3
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_TI1FP1_RISING
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_TI1FP2_RISING
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_TI1FP1_FALLING
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_TI1FP2_FALLING
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] channel   channel ([1:tdp->ch_num]) to be configured
 * @param[in] ch_mode   channel modes to be configured. It can be one of the
 *                      following values:
 *            @arg @ref TIM_MODE_PERIODIC
 *            @arg @ref TIM_MODE_PWM_ACTIVE_HIGH
 *            @arg @ref TIM_MODE_PWM_ACTIVE_HIGH_COMP
 *            @arg @ref TIM_MODE_PWM_ACTIVE_LOW
 *            @arg @ref TIM_MODE_PWM_ACTIVE_LOW_COMP
 *            @arg @ref TIM_MODE_PWM_FROZEN
 *            @arg @ref TIM_MODE_PWM_ACTIVE_LEVEL
 *            @arg @ref TIM_MODE_PWM_INACTIVE_LEVEL
 *            @arg @ref TIM_MODE_PWM_COMBINED_MODE1
 *            @arg @ref TIM_MODE_PWM_COMBINED_MODE2
 *            @arg @ref TIM_MODE_PWM_ASYMMETRIC_MODE1
 *            @arg @ref TIM_MODE_PWM_ASYMMETRIC_MODE2
 *            @arg @ref TIM_MODE_ICU_RISING_EDGE
 *            @arg @ref TIM_MODE_ICU_FALLING_EDGE
 *            @arg @ref TIM_MODE_ICU_BOTH_EDGES
 *            @arg @ref TIM_MODE_ICU_RISING_EDGE_IND
 *            @arg @ref TIM_MODE_ICU_FALLING_EDGE_IND
 * @return              previous channel modes. It can be one of the following
 *                      values:
 *            @arg @ref TIM_MODE_PERIODIC
 *            @arg @ref TIM_MODE_PWM_ACTIVE_HIGH
 *            @arg @ref TIM_MODE_PWM_ACTIVE_HIGH_COMP
 *            @arg @ref TIM_MODE_PWM_ACTIVE_LOW
 *            @arg @ref TIM_MODE_PWM_ACTIVE_LOW_COMP
 *            @arg @ref TIM_MODE_PWM_FROZEN
 *            @arg @ref TIM_MODE_PWM_ACTIVE_LEVEL
 *            @arg @ref TIM_MODE_PWM_INACTIVE_LEVEL
 *            @arg @ref TIM_MODE_PWM_COMBINED_MODE1
 *            @arg @ref TIM_MODE_PWM_COMBINED_MODE2
 *            @arg @ref TIM_MODE_PWM_ASYMMETRIC_MODE1
 *            @arg @ref TIM_MODE_PWM_ASYMMETRIC_MODE2
 *            @arg @ref TIM_MODE_ICU_RISING_EDGE
 *            @arg @ref TIM_MODE_ICU_FALLING_EDGE
 *            @arg @ref TIM_MODE_ICU_BOTH_EDGES
 *            @arg @ref TIM_MODE_ICU_RISING_EDGE_IND
 *            @arg @ref TIM_MODE_ICU_FALLING_EDGE_IND
 *
 * @api
 */
tim_mode_t tim_set_ch_mode(tim_driver_t *tdp, uint8_t channel,
                           tim_mode_t ch_mode);

/**
 * @brief   Selects TIM trigger outptus.
 * @note    This configuration becomes effective after tim_start.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] trgo      trigger output to be configured. It can be one of the
 *                      following values:
 *            @arg @ref TIM_TRGO_RESET
 *            @arg @ref TIM_TRGO_ENABLE
 *            @arg @ref TIM_TRGO_UPDATE
 *            @arg @ref TIM_TRGO_CC1IF
 *            @arg @ref TIM_TRGO_OC1REFC
 *            @arg @ref TIM_TRGO_OC2REFC
 *            @arg @ref TIM_TRGO_OC3REFC
 *            @arg @ref TIM_TRGO_OC4REFC
 *            @arg @ref TIM_TRGO_ENCODERCLK
 * @return              previous trigger output. It can be one of the following
 *                      values:
 *            @arg @ref TIM_TRGO_RESET
 *            @arg @ref TIM_TRGO_ENABLE
 *            @arg @ref TIM_TRGO_UPDATE
 *            @arg @ref TIM_TRGO_CC1IF
 *            @arg @ref TIM_TRGO_OC1REFC
 *            @arg @ref TIM_TRGO_OC2REFC
 *            @arg @ref TIM_TRGO_OC3REFC
 *            @arg @ref TIM_TRGO_OC4REFC
 *            @arg @ref TIM_TRGO_ENCODERCLK
 *
 * @api
 */
tim_trgo_t tim_set_trgo(tim_driver_t *tdp, tim_trgo_t trgo);

/**
 * @brief   Selects TIM trigger outptus 2.
 * @note    This configuration becomes effective after tim_start.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] trgo2     trigger output 2 to be configured. It can be one of the
 *                      following values:
 *            @arg @ref TIM_TRGO2_RESET
 *            @arg @ref TIM_TRGO2_ENABLE
 *            @arg @ref TIM_TRGO2_UPDATE
 *            @arg @ref TIM_TRGO2_CC1IF
 *            @arg @ref TIM_TRGO2_OC1REFC
 *            @arg @ref TIM_TRGO2_OC2REFC
 *            @arg @ref TIM_TRGO2_OC3REFC
 *            @arg @ref TIM_TRGO2_OC4REFC
 *            @arg @ref TIM_TRGO2_OC5REFC
 *            @arg @ref TIM_TRGO2_OC6REFC
 *            @arg @ref TIM_TRGO2_OC4REFC_RISINGFALLING
 *            @arg @ref TIM_TRGO2_OC6REFC_RISINGFALLING
 *            @arg @ref TIM_TRGO2_OC4REFC_RISING_OC6_RISING
 *            @arg @ref TIM_TRGO2_OC4REFC_RISING_OC6_FALLING
 *            @arg @ref TIM_TRGO2_OC5REFC_RISING_OC6_RISING
 *            @arg @ref TIM_TRGO2_OC5REFC_RISING_OC6_FALLING
 * @return              previous trigger output 2. It can be one of the
 *                      following values:
 *            @arg @ref TIM_TRGO2_RESET
 *            @arg @ref TIM_TRGO2_ENABLE
 *            @arg @ref TIM_TRGO2_UPDATE
 *            @arg @ref TIM_TRGO2_CC1IF
 *            @arg @ref TIM_TRGO2_OC1REFC
 *            @arg @ref TIM_TRGO2_OC2REFC
 *            @arg @ref TIM_TRGO2_OC3REFC
 *            @arg @ref TIM_TRGO2_OC4REFC
 *            @arg @ref TIM_TRGO2_OC5REFC
 *            @arg @ref TIM_TRGO2_OC6REFC
 *            @arg @ref TIM_TRGO2_OC4REFC_RISINGFALLING
 *            @arg @ref TIM_TRGO2_OC6REFC_RISINGFALLING
 *            @arg @ref TIM_TRGO2_OC4REFC_RISING_OC6_RISING
 *            @arg @ref TIM_TRGO2_OC4REFC_RISING_OC6_FALLING
 *            @arg @ref TIM_TRGO2_OC5REFC_RISING_OC6_RISING
 *            @arg @ref TIM_TRGO2_OC5REFC_RISING_OC6_FALLING
 *
 * @api
 */
tim_trgo2_t tim_set_trgo2(tim_driver_t *tdp, tim_trgo2_t trgo2);

/**
 * @brief   Sets slave mode selection.
 * @note    This configuration becomes effective after tim_start.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] sms       slave mode selection be configured. It can be one of
 *                      the following values:
 *            @arg @ref TIM_SMODE_DISABLED
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_MODE1
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_MODE2
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_MODE3
 *            @arg @ref TIM_SMODE_RESET
 *            @arg @ref TIM_SMODE_GATED
 *            @arg @ref TIM_SMODE_TRIGGER
 *            @arg @ref TIM_SMODE_EXT_CLOCK_1
 *            @arg @ref TIM_SMODE_COMBO_RESET_TRIGGER
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_TI1FP1_RISING
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_TI1FP2_RISING
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_TI1FP1_FALLING
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_TI1FP2_FALLING
 * @return              previous slave mode selection. It can be one of the
 *                      following values:
 *            @arg @ref TIM_SMODE_DISABLED
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_MODE1
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_MODE2
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_MODE3
 *            @arg @ref TIM_SMODE_RESET
 *            @arg @ref TIM_SMODE_GATED
 *            @arg @ref TIM_SMODE_TRIGGER
 *            @arg @ref TIM_SMODE_EXT_CLOCK_1
 *            @arg @ref TIM_SMODE_COMBO_RESET_TRIGGER
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_TI1FP1_RISING
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_TI1FP2_RISING
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_TI1FP1_FALLING
 *            @arg @ref TIM_SMODE_QUAD_ENCODER_TI1FP2_FALLING
 *
 * @api
 */
tim_sms_t tim_set_sms(tim_driver_t *tdp, tim_sms_t sms);

/**
 * @brief   Selects trigger input.
 * @note    This configuration becomes effective after tim_start.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] trgi      trigger input be configured. It can be one of the
 *                      following values:
 *            @arg @ref TIM_TRGI_ITR0
 *            @arg @ref TIM_TRGI_ITR1
 *            @arg @ref TIM_TRGI_ITR2
 *            @arg @ref TIM_TRGI_ITR3
 *            @arg @ref TIM_TRGI_TI1F_ED
 *            @arg @ref TIM_TRGI_TI1FP1
 *            @arg @ref TIM_TRGI_TI2FP2
 *            @arg @ref TIM_TRGI_ETRF
 *            @arg @ref TIM_TRGI_ITR4
 *            @arg @ref TIM_TRGI_ITR5
 *            @arg @ref TIM_TRGI_ITR6
 *            @arg @ref TIM_TRGI_ITR7
 *            @arg @ref TIM_TRGI_ITR8
 *            @arg @ref TIM_TRGI_ITR9
 *            @arg @ref TIM_TRGI_ITR10
 * @return              previous trigger input. It can be one of the following
 *                      values:
 *            @arg @ref TIM_TRGI_ITR0
 *            @arg @ref TIM_TRGI_ITR1
 *            @arg @ref TIM_TRGI_ITR2
 *            @arg @ref TIM_TRGI_ITR3
 *            @arg @ref TIM_TRGI_TI1F_ED
 *            @arg @ref TIM_TRGI_TI1FP1
 *            @arg @ref TIM_TRGI_TI2FP2
 *            @arg @ref TIM_TRGI_ETRF
 *            @arg @ref TIM_TRGI_ITR4
 *            @arg @ref TIM_TRGI_ITR5
 *            @arg @ref TIM_TRGI_ITR6
 *            @arg @ref TIM_TRGI_ITR7
 *            @arg @ref TIM_TRGI_ITR8
 *            @arg @ref TIM_TRGI_ITR9
 *            @arg @ref TIM_TRGI_ITR10
 *
 * @api
 */
tim_trgi_t tim_set_trgi(tim_driver_t *tdp, tim_trgi_t trgi);

/**
 * @brief   Configures external trigger filter.
 * @note    This configuration becomes effective after tim_start.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] etf       external trigger filter be configured. It can be one of
 *                      the values in the range [0:15]
 * @return              previous external trigger filter. It can be one of the
 *                      the values in the range [0:15].
 *
 * @api
 */
uint8_t tim_set_etf(tim_driver_t *tdp, uint8_t etf);

/**
 * @brief   Configures external trigger prescaler.
 * @note    This configuration becomes effective after tim_start.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] etps      external trigger prescaler to be configured. It can be
 *                      one of the following values:
 *            @arg @ref TIM_ETPS_DIV1
 *            @arg @ref TIM_ETPS_DIV2
 *            @arg @ref TIM_ETPS_DIV4
 *            @arg @ref TIM_ETPS_DIV8
 * @return              previous external trigger prescaler. It can be one of
 *                      the following values:
 *            @arg @ref TIM_ETPS_DIV1
 *            @arg @ref TIM_ETPS_DIV2
 *            @arg @ref TIM_ETPS_DIV4
 *            @arg @ref TIM_ETPS_DIV8
 *
 * @api
 */
tim_etps_t tim_set_etps(tim_driver_t *tdp, tim_etps_t etps);

/**
 * @brief   Configures external trigger polarity.
 * @note    This configuration becomes effective after tim_start.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] etps      external trigger polarity to be configured. It can be
 *                      one of the following values:
 *            @arg @ref TIM_ETPS_NOT_INVERTED
 *            @arg @ref TIM_ETPS_INVERTED
 * @return              previous external trigger polarity. It can be one of
 *                      the following values:
 *            @arg @ref TIM_ETPS_NOT_INVERTED
 *            @arg @ref TIM_ETPS_INVERTED
 *
 * @api
 */
tim_etp_t tim_set_etp(tim_driver_t *tdp, tim_etp_t etp);

/**
 * @brief   Enables break event and set break polarity.
 * @note    This configuration becomes effective after tim_start.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] polarity  break polarity. It can be one of the following values:
 *            @arg @ref TIM_BKP_LOW
 *            @arg @ref TIM_BKP_HIGH
 *
 * @api
 */
void tim_set_bk(tim_driver_t *tdp, tim_bkp_t polarity);

/**
 * @brief   Enables break 2 event and set break 2 polarity.
 * @note    This configuration becomes effective after tim_start.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] polarity  break 2 polarity. It can be one of the following
 *                      values:
 *            @arg @ref TIM_BKP_LOW
 *            @arg @ref TIM_BKP_HIGH
 *
 * @api
 */
void tim_set_bk2(tim_driver_t *tdp, tim_bkp_t polarity);

/**
 * @brief   Configures TIM input 1 selection.
 * @note    This configuration becomes effective after tim_start.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] ti1s      TIM input 1 selection to be configured. It can be one
 *                      of the following values:
 *            @arg @ref TIM_TI1S_TI1
 *            @arg @ref TIM_TI1S_XOR
 * @return              previous TIM input 1 selection. It can be one of the
 *                      following values:
 *            @arg @ref TIM_TI1S_TI1
 *            @arg @ref TIM_TI1S_XOR
 *
 * @api
 */
tim_ti1s_t tim_set_ti1_selection(tim_driver_t *tdp, tim_ti1s_t ti1s);

/**
 * @brief   Enables/Disables capture/compare preloaded control.
 * @note    This configuration becomes effective after tim_start.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] enable    capture/compare preloaded control enable flag ('true'
 *                      means the preload control is enabled, 'false'
 *                      otherwise)
 * @return              previous capture/compare preloaded control enable flag
 *                      value.
 *
 * @api
 */
bool tim_enable_preload_control(tim_driver_t *tdp, bool enable);

/**
 * @brief   Configures TIM DMA.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] dma_op    operation for which to set the DMA. It can be one of
 *                      the following values:
 *            @arg @ref TIM_DMA_OPS_CH1
 *            @arg @ref TIM_DMA_OPS_CH2
 *            @arg @ref TIM_DMA_OPS_CH3
 *            @arg @ref TIM_DMA_OPS_CH4
 *            @arg @ref TIM_DMA_OPS_UPDATE
 * @param[in] dma_conf  pointer to DMA configuration to be configured
 *
 * @api
 */
void tim_set_dma_conf(tim_driver_t *tdp, tim_dma_ops_t dma_op,
                      tim_dma_conf_t *dma_conf);

/**
 * @brief   Configures pointers to TIM callback functions.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure
 * @param[in] cb_op     callback operation type. It can be one of the following
 *                      values:
 *            @arg @ref TIM_CB_OPS_UPDATE
 *            @arg @ref TIM_CB_OPS_CC1
 *            @arg @ref TIM_CB_OPS_CC2
 *            @arg @ref TIM_CB_OPS_CC3
 *            @arg @ref TIM_CB_OPS_CC4
 *            @arg @ref TIM_CB_OPS_DMA_UPDATE
 *            @arg @ref TIM_CB_OPS_DMA_CC1
 *            @arg @ref TIM_CB_OPS_DMA_CC2
 *            @arg @ref TIM_CB_OPS_DMA_CC3
 *            @arg @ref TIM_CB_OPS_DMA_CC4
 *            @arg @ref TIM_CB_OPS_BK
 *            @arg @ref TIM_CB_OPS_BK2
 *            @arg @ref TIM_CB_OPS_COM
 * @param[in] cb        pointer to TIM callback function
 * @return              pointer to previuos TIM callback function.
 *
 * @api
 */
tim_cb_t tim_set_cb(tim_driver_t *tdp, tim_cb_ops_t cb_op, tim_cb_t cb);

/**
 * @brief   Initializes an TIM instance.
 *
 * @param[out] tdp      pointer to a @p tim_driver_t structure to be
 *                      initialized
 *
 * @api
 */
void tim_init(tim_driver_t *tdp);

/**
 * @brief   Returns the value of the TIM clock.
 * @note    It must be invoked after tim_init, otherwise it returns 0.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @return              the value of TIM clock [Hz]
 *
 * @api
 */
uint32_t tim_get_clock(tim_driver_t *tdp);

/**
 * @brief   Starts a TIM instance.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @api
 */
void tim_start(tim_driver_t *tdp);

/**
 * @brief   Enables TIM instance.
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @api
 */
void tim_enable(tim_driver_t *tdp);

/**
 * @brief   Disables TIM instance.
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @api
 */
void tim_disable(tim_driver_t *tdp);

/**
 * @brief   Enables the update callback.
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @api
 */
void tim_enable_update_callback(tim_driver_t *tdp);

/**
 * @brief   Disables the update callback.
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @api
 */
void tim_disable_update_callback(tim_driver_t *tdp);

/**
 * @brief   Enables a channel callback.
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] channel   channel number ([1:tdp->ch_num] && <= 4).
 *
 * @api
 */
void tim_enable_channel_callback(tim_driver_t *tdp, uint8_t channel);

/**
 * @brief   Disables a channel callback.
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] channel   channel number ([1:tdp->ch_num] && <= 4).
 *
 * @api
 */
void tim_disable_channel_callback(tim_driver_t *tdp, uint8_t channel);

/**
 * @brief   Enables the break/break 2 event callback.
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @api
 */
void tim_enable_break_callback(tim_driver_t *tdp);

/**
 * @brief   Disables the break/ break 2 event callback.
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @api
 */
void tim_disable_break_callback(tim_driver_t *tdp);

/**
 * @brief   Enables the COM event callback.
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @api
 */
void tim_enable_com_callback(tim_driver_t *tdp);

/**
 * @brief   Disables the COM event callback.
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @api
 */
void tim_disable_com_callback(tim_driver_t *tdp);

/**
 * @brief   Generates a capture/compare control update.
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @api
 */
void tim_generate_com(tim_driver_t *tdp);

/**
 * @brief   Enables the update DMA request (input capture mode).
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] data      pointer to the DMA buffer in which to write data
 * @param[in] size      DMA size
 *
 * @api
 */
void tim_enable_update_dma_ic(tim_driver_t *tdp, uint16_t *data, uint32_t size);

/**
 * @brief   Enables the update DMA request (output compare mode).
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] data      pointer to the DMA buffer from which to read data
 * @param[in] size      DMA size
 *
 * @api
 */
void tim_enable_update_dma_oc(tim_driver_t *tdp, uint16_t *data, uint32_t size);

/**
 * @brief   Disables the update DMA request (input capture mode).
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @api
 */
void tim_disable_update_dma_ic(tim_driver_t *tdp);

/**
 * @brief   Disables the update DMA request (output compare mode).
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @api
 */
void tim_disable_update_dma_oc(tim_driver_t *tdp);

/**
 * @brief   Configures a TIM channel to trigger a DMA request.
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] channel   TIM channel ([1:tdp->ch_num] && <= 4) used to trigger
 *                      the DMA request
 * @param[in] dma_dir   DMA direction. It can be one of the following values:
 *            @arg @ref TIM_DMA_DIR_P2M
 *            @arg @ref TIM_DMA_DIR_M2P
 *            @arg @ref TIM_DMA_DIR_M2M
 * @param[in] dma_size  DMA size. It can be one of the following values:
 *            @arg @ref TIM_DMA_SIZE_BYTE
 *            @arg @ref TIM_DMA_SIZE_HWORD
 *            @arg @ref TIM_DMA_SIZE_WORD
 * @param[in] dma_circ  DMA circular mode. It can be one of the following
 *                      values:
 *            @arg @ref TIM_DMA_CIRC_DISABLED
 *            @arg @ref TIM_DMA_CIRC_ENABLED
 * @param[in] src       DMA source address
 * @param[in] size      DMA size
 * @param[in] src       DMA destination address
 *
 * @api
 */
void tim_enable_channel_dma(tim_driver_t *tdp, uint8_t channel,
                            tim_dma_dir_t dma_dir, tim_dma_size_t dma_size,
                            tim_dma_circ_t dma_circ, uint32_t src,
                            uint32_t size, uint32_t dst);
/**
 * @brief   Disables DMA request tiggered by the TIM channel.
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] channel   TIM channel ([1:tdp->ch_num] && <= 4) for which to
 *                      disable the DMA request
 *
 * @api
 */
void tim_disable_channel_dma(tim_driver_t *tdp, uint8_t channel);

/**
 * @brief   Enables the channel capture/compare DMA request (input capture).
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] channel   TIM channel ([1:tdp->ch_num] && <= 4) for which to
 *                      enable the capture/compare request
 * @param[in] data      pointer to the DMA buffer in which to write the data
 * @param[in] size      DMA size
 *
 * @api
 */
void tim_enable_channel_dma_ic(tim_driver_t *tdp, uint8_t channel,
                               uint16_t *data, uint32_t size);

/**
 * @brief   Enables the channel capture/compare DMA request (output compare).
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] channel   TIM channel ([1:tdp->ch_num] && <= 4) for which to
 *                      enable the capture/compare request
 * @param[in] data      pointer to the DMA buffer from which to read the data
 * @param[in] size      DMA size
 *
 * @api
 */
void tim_enable_channel_dma_oc(tim_driver_t *tdp, uint8_t channel,
                               uint16_t *data, uint32_t size);

/**
 * @brief   Disables the channel capture/compare DMA request (input capture).
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] channel   TIM channel ([1:tdp->ch_num] && <= 4) for which to
 *                      disable the capture/compare request
 *
 * @api
 */
void tim_disable_channel_dma_ic(tim_driver_t *tdp, uint8_t channel);

/**
 * @brief   Disables the channel capture/compare DMA request (output compare).
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] channel   TIM channel ([1:tdp->ch_num] && <= 4) for which to
 *                      disable the capture/compare request
 *
 * @api
 */
void tim_disable_channel_dma_oc(tim_driver_t *tdp, uint8_t channel);

/**
 * @brief   Changes the timer interval.
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] interval  the interval as number of timer clock pulses
 *
 * @api
 */
void tim_set_interval(tim_driver_t *tdp, uint32_t interval);

/**
 * @brief   Returns the current interval.
 * @note    It must be invoked after tim_start, otherwise it returns 0.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @return              the interval as number of timer clock pulses.
 *
 * @api
 */
uint32_t tim_get_interval(tim_driver_t *tdp);

/**
 * @brief   Changes a comparator value.
 * @note    It must be invoked after tim_start, otherwise it doesn't not take
 *          effect.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] channel   channel number ([1:tdp->ch_num]).
 * @param[in] cmpval    the value to be compared with the counter
 *
 * @api
 */
void tim_set_comparator(tim_driver_t *tdp, uint8_t channel, uint32_t cmpval);

/**
 * @brief   Returns a comparator value.
 * @note    It must be invoked after tim_start, otherwise it returns 0.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] channel   channel number ([1:tdp->ch_num]).
 * @return              the current comparator value.
 *
 * @api
 */
uint32_t tim_get_comparator(tim_driver_t *tdp, uint8_t channel);

/**
 * @brief   Returns counter direction.
 * @note    It must be invoked after tim_start, otherwise it returns default
 *          counter direction.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @return              the current counter direction.
 *
 * @api
 */
tim_dir_t tim_get_direction(tim_driver_t *tdp);

/**
 * @brief   Returns counter value.
 * @note    It must be invoked after tim_start, otherwise it returns 0.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @return              the current counter value.
 *
 * @api
 */
uint16_t tim_get_counter(tim_driver_t *tdp);

/**
 * @brief   Set application private data pointer.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @param[in] priv      pointer to application private data
 *
 * @api
 */
void tim_set_private(tim_driver_t *tdp, void *priv);

/**
 * @brief   Get application private data pointer.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 * @return              the aplication private data pointer.
 *
 * @api
 */
void *tim_get_private(tim_driver_t *tdp);

/**
 * @brief   Stops a TIM instance.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @api
 */
void tim_stop(tim_driver_t *tdp);

/**
 * @brief   De-initializes a TIM instance.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure to
 *                      be de-initialized
 *
 * @api
 */
void tim_deinit(tim_driver_t *tdp);

/**
 * @brief   Serves a TIM interrupt.
 * @note    Not an API, used internally.
 *
 * @param[in] tdp       pointer to a @p tim_driver_t structure
 *
 * @isr
 */
void __tim_serve_interrupt(tim_driver_t *tdp);

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _TIM_H_ */

/** @} */
