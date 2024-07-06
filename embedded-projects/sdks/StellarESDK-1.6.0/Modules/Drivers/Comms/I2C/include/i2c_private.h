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
 * @file    i2c_private.h
 * @brief   I2C driver private header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup COMMS
 * @ingroup DRIVERS
 * @addtogroup I2C
 * @ingroup COMMS
 * @{
 */

#ifndef _I2C_PRIVATE_H_
#define _I2C_PRIVATE_H_

#include <dma.h>
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
 * @brief   I2C driver structure.
 * @note    This structure is internal to the I2C driver.
 */
struct i2c_driver {
    /**
     * @brief   Pointer to I2C registers block.
     */
    I2C_TypeDef              *i2c;
    /**
     * @brief   I2C clock value.
     */
    uint32_t                 clock;
    /**
     * @brief   I2C mode (master tx/rx or slave tx/rx).
     */
    i2c_mode_t               mode;
    /**
     * @brief   Pointer to transfer data buffer.
     */
    uint8_t                  *data;
    /**
     * @brief   Transfer size.
     */
    uint16_t                 size;
    /**
     * @brief   Number of bytes to transfer.
     */
    uint16_t                 count;
    /**
     * @brief  Transfer in progress flag.
     */
    volatile i2c_op_sts_t    op_busy;
    /**
     * @brief   I2C interrupt priority.
     */
    uint32_t                 prio;
    /**
     * @brief   Driver mode (synchronous or asynchronous).
     */
    i2c_drv_mode_t           drv_mode;
    /**
     * @brief   DMA mode (enabled or disabled).
     */
    bool                     is_dma_en;
    /**
     * @brief   DMA configuration.
     */
    i2c_dma_conf_t           dma_conf;
    /**
     * @brief   Allocated RX DMA descriptor.
     */
    const dma_descriptor_t   *dma_rx_descriptor;
    /**
     * @brief   Allocated TX DMA descriptor.
     */
    const dma_descriptor_t   *dma_tx_descriptor;
    /**
     * @brief   I2C timing value.
     */
    uint32_t                 timing;
    /**
     * @brief   I2C addressing mode (7-bit or 10-bit).
     */
    i2c_oa1_mode_t           oa1_mode;
    /**
     * @brief   I2C own address 1.
     */
    uint16_t                 oa1;
    /**
     * @brief   I2C own addres 2 enable flag.
     */
    bool                     is_oa2_en;
    /**
     * @brief   I2C own address 2.
     */
    uint32_t                 oa2;
    /**
     * @brief   I2C own address 2 mask.
     */
    i2c_oa2_mask_t           oa2_mask;
    /**
     * @brief   I2C general call enable flag.
     */
    bool                     is_gc_en;
    /**
     * @brief   Clock no-stretching enable flag.
     */
    bool                     is_no_stretch_en;
    /**
     * @brief   Pointers to I2C callback functions.
     * @note    Set to @p NULL if not required.
     */
    i2c_cb_t                 cb[I2C_CB_OPS_MAX];
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

IRQ_HANDLER(IRQ_I2C1_EV_HANDLER);
IRQ_HANDLER(IRQ_I2C2_EV_HANDLER);
IRQ_HANDLER(IRQ_I2C1_ER_HANDLER);
IRQ_HANDLER(IRQ_I2C2_ER_HANDLER);

I2C_TypeDef *i2c_dev_get_reg_ptr(i2c_driver_t *idp);
uint32_t i2c_dev_get_clock(i2c_driver_t *idp);
void i2c_dev_clock_enable(i2c_driver_t *idp);
void i2c_dev_clock_disable(i2c_driver_t *idp);
uint32_t i2c_dev_get_dma_tx_trigger(i2c_driver_t *idp);
uint32_t i2c_dev_get_dma_rx_trigger(i2c_driver_t *idp);
IRQn_Type i2c_dev_get_vector(i2c_driver_t *idp);

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _I2C_PRIVATE_H_ */

/** @} */

