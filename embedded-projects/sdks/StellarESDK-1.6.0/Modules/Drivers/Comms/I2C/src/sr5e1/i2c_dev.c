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
 * @file    i2c_dev.c
 * @brief   I2C device specific source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup COMMS
 * @ingroup DRIVERS
 * @addtogroup I2C
 * @ingroup COMMS
 * @{
 */

#include <i2c.h>
#include <i2c_private.h>
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
 * @brief   Returns pointer to the I2C register block.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @return              pointer to the I2C register block.
 *
 * @notapi
 */
I2C_TypeDef *i2c_dev_get_reg_ptr(i2c_driver_t *idp) {

    I2C_TypeDef *i2c;

    if (idp == &DRV_I2C1) {
        i2c = I2C1;
    } else {
        i2c = I2C2;
    }

    return i2c;
}

/**
 * @brief   Returns value of I2C clock.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @return              I2C clock.
 *
 * @notapi
 */
uint32_t i2c_dev_get_clock(i2c_driver_t *idp) {

    uint32_t clock;

    if (idp == &DRV_I2C1) {
        clock = CLOCK_I2CCLK;
    } else {
        clock = CLOCK_I2CCLK;
    }

    return clock;
}

/**
 * @brief   Enables I2C clock.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 *
 * @notapi
 */
void i2c_dev_clock_enable(i2c_driver_t *idp) {

    if (idp == &DRV_I2C1) {
        clock_enable_I2C1(true);
    } else {
        clock_enable_I2C2(true);
    }
}

/**
 * @brief   Disables I2C clock.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 *
 * @notapi
 */
void i2c_dev_clock_disable(i2c_driver_t *idp) {

    if (idp == &DRV_I2C1) {
        clock_disable_I2C1();
    } else {
        clock_disable_I2C2();
    }
}

/**
 * @brief   Returns I2C DMA TX trigger.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @return              DMA TX trigger.
 *
 * @notapi
 */
uint32_t i2c_dev_get_dma_tx_trigger(i2c_driver_t *idp) {

    uint32_t dma_tx_trigger;

    if (idp == &DRV_I2C1) {
        dma_tx_trigger = DMAMUX1_I2C1_TX;
    } else {
        dma_tx_trigger = DMAMUX1_I2C2_TX;
    }

    return dma_tx_trigger;
}

/**
 * @brief   Returns I2C DMA RX trigger.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @return              DMA RX trigger.
 *
 * @notapi
 */
uint32_t i2c_dev_get_dma_rx_trigger(i2c_driver_t *idp) {

    uint32_t dma_rx_trigger;

    if (idp == &DRV_I2C1) {
        dma_rx_trigger = DMAMUX1_I2C1_RX;
    } else {
        dma_rx_trigger = DMAMUX1_I2C2_RX;
    }

    return dma_rx_trigger;
}

/**
 * @brief   Returns I2C interrupt vector.
 *
 * @param[in] idp       pointer to a @p i2c_driver_t structure
 * @return              I2C interrupt vector
 *
 * @notapi
 */
IRQn_Type i2c_dev_get_vector(i2c_driver_t *idp) {

    IRQn_Type vector;

    if (idp == &DRV_I2C1) {
        vector = IRQ_I2C1_EV_VECTOR;
    } else {
        vector = IRQ_I2C2_EV_VECTOR;
    }

    return vector;
}

/**
 * @brief   I2C1 event interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(IRQ_I2C1_EV_HANDLER) {

    IRQ_PROLOGUE();

    __i2c_serve_ev_interrupt(&DRV_I2C1);

    IRQ_EPILOGUE();
}

/**
 * @brief   I2C2 event interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(IRQ_I2C2_EV_HANDLER) {

    IRQ_PROLOGUE();

    __i2c_serve_ev_interrupt(&DRV_I2C2);

    IRQ_EPILOGUE();
}

/**
 * @brief   I2C1 error interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(IRQ_I2C1_ER_HANDLER) {

    IRQ_PROLOGUE();

    __i2c_serve_er_interrupt(&DRV_I2C1);

    IRQ_EPILOGUE();
}

/**
 * @brief   I2C2 error interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(IRQ_I2C2_ER_HANDLER) {

    IRQ_PROLOGUE();

    __i2c_serve_er_interrupt(&DRV_I2C2);

    IRQ_EPILOGUE();
}

/** @} */

