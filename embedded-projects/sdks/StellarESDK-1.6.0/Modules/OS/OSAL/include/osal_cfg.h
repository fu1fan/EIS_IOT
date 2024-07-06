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
 * @file    osal_cfg.h
 * @brief   Operating System Abstraction Layer module configuration file.
 *
 * @addtogroup OS
 * @addtogroup OSAL
 * @ingroup OS
 * @{
 */

#ifndef _OSAL_CFG_H_
#define _OSAL_CFG_H_

/**
 * @name    OSAL Interrupt Frequency
 * @{
 */
#define OSAL_CFG_IRQ_PER_SEC                OSAL_IRQ_PER_SEC_1000
/** @} */

/**
 * @name    OSAL Interrupt Priority
 * @{
 */
#define OSAL_CFG_IRQ_PRIO                   (4UL)
/** @} */

/** 
 * @name    OSAL Interrupt Hook Enable Switch
 * @{
 */

#define OSAL_CFG_USE_IRQ_HOOK               FALSE
/** @} */

#endif /* _OSAL_CFG_H_ */
