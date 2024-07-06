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
 * @file    console.h
 * @brief   FreeRTOS Console module header.
 *
 * @addtogroup OS
 * @addtogroup UTILITIES
 * @ingroup OS
 * @addtogroup CONSOLE
 * @ingroup UTILITIES
 * @{
 */

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <console_cfg.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void startConsole(void *pvParameters);
extern void stopConsole(void);

#ifdef __cplusplus
}
#endif

#endif /*_CONSOLE_H_ */
/** @} */
