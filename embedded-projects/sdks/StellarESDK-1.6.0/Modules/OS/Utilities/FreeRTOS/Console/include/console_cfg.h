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
 * @file    console_cfg.h
 * @brief   FreeRTOS Console configuration module header.
 *
 * @addtogroup OS
 * @addtogroup UTILITIES
 * @ingroup OS
 * @addtogroup CONSOLE
 * @ingroup UTILITIES
 * @{
 */

#ifndef _CONSOLE_CFG_H_
#define _CONSOLE_CFG_H_

/* Buffer size in which the console input command is stored */
#define configCOMMAND_INT_MAX_INPUT_SIZE    64U
/* Flag to enable/disable the console prompt */
#define configCLI_DISABLE_CONSOLE_PROMPT    FALSE

/* Console Task Stack */
#define configCONSOLE_STACK_SIZE            (configMINIMAL_STACK_SIZE * 4U)
/* console Task Priority */
#define configCONSOLE_TASK_PRIORITY         (tskIDLE_PRIORITY + 1U)

#endif /*_CONSOLE_CFG_H_ */

/** @} */
