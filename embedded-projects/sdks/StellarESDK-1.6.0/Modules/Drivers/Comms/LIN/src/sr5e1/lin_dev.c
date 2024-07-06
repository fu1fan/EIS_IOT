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
 * @file    lin_dev.c
 * @brief   LIN device specific source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup LIN
 * @ingroup SYSTEM
 * @{
 */

#include <lin.h>
#include <lin_private.h>
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
 * @brief   Returns pointer to the LIN register block.
 *
 * @param[in] ldp       pointer to a @p lin_driver_t structure
 * @return              pointer to the LIN register block.
 *
 * @notapi
 */
UART_TypeDef *lin_dev_get_reg_ptr(lin_driver_t *ldp) {

    UART_TypeDef *lin;

    if (ldp == &DRV_LIN1) {
        lin = UART1;
    } else if (ldp == &DRV_LIN2) {
        lin = UART2;
    } else {
        lin = UART3;
    }

    return lin;
}

/**
 * @brief   Returns value of LIN clock.
 *
 * @param[in] ldp       pointer to a @p lin_driver_t structure
 * @return              LIN clock.
 *
 * @notapi
 */
uint32_t lin_dev_get_clock(lin_driver_t *ldp) {

    uint32_t clock;

    if (ldp == &DRV_LIN1) {
        clock = CLOCK_UART1CLK;
    } else if (ldp == &DRV_LIN2) {
        clock = CLOCK_UART2CLK;
    } else {
        clock = CLOCK_UART3CLK;
    }

    return clock;
}

/**
 * @brief   Enables LIN clock.
 *
 * @param[in] ldp       pointer to a @p lin_driver_t structure
 *
 * @notapi
 */
void lin_dev_clock_enable(lin_driver_t *ldp) {

    if (ldp == &DRV_LIN1) {
        clock_enable_UART1(true);
    } else if (ldp == &DRV_LIN2) {
        clock_enable_UART2(true);
    } else {
        clock_enable_UART3(true);
    }
}

/**
 * @brief   Disables LIN clock.
 *
 * @param[in] ldp       pointer to a @p lin_driver_t structure
 *
 * @notapi
 */
void lin_dev_clock_disable(lin_driver_t *ldp) {

    if (ldp == &DRV_LIN1) {
        clock_disable_UART1();
    } else if (ldp == &DRV_LIN2) {
        clock_disable_UART2();
    } else {
        clock_disable_UART3();
    }
}

/**
 * @brief   Returns LIN interrupt vector.
 *
 * @param[in] ldp       pointer to a @p lin_driver_t structure
 * @return              LIN interrupt vector
 *
 * @notapi
 */
IRQn_Type lin_dev_get_vector(lin_driver_t *ldp) {

    IRQn_Type vector;

    if (ldp == &DRV_LIN1) {
        vector = IRQ_UART1_VECTOR;
    } else if (ldp == &DRV_LIN2) {
        vector = IRQ_UART2_VECTOR;
    } else {
        vector = IRQ_UART3_VECTOR;
    }

    return vector;
}

/**
 * @brief   LIN1 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(IRQ_UART1_HANDLER) {

    IRQ_PROLOGUE();

    __lin_serve_interrupt(&DRV_LIN1);

    IRQ_EPILOGUE();
}

/**
 * @brief   LIN2 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(IRQ_UART2_HANDLER) {

    IRQ_PROLOGUE();

    __lin_serve_interrupt(&DRV_LIN2);

    IRQ_EPILOGUE();
}

/**
 * @brief  LIN3 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(IRQ_UART3_HANDLER) {

    IRQ_PROLOGUE();

    __lin_serve_interrupt(&DRV_LIN3);

    IRQ_EPILOGUE();
}

/** @} */
