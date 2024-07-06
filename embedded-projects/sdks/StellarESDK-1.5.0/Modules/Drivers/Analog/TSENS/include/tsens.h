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
 * @file    tsens.h
 * @brief   TSENS driver header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup ANALOG
 * @ingroup DRIVERS
 * @addtogroup TSENS
 * @ingroup ANALOG
 * @{
 */

#ifndef _TSENS_H_
#define _TSENS_H_

#include <platform.h>
#include <typedefs.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/* UTest addresses of the TSENS constants */
#define TSENS_P2_ADDRESS                    0x1FF80000
#define TSENS_C2_ADDRESS                    0x1FF80000 + 1UL
#define TSENS_P1_ADDRESS                    0x1FF80000 + 2UL
#define TSENS_C1_ADDRESS                    0x1FF80000 + 3UL

/* Hot and cold temperatures (°C) */
#define TSENS_T1                            150
#define TSENS_T2                            -40

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/**
 * @brief   Initializes a tsens instance.
 *
 * @api
 */
void tsens_init(void);

/**
 * @brief   Calculates the temperature value.
 *
 * @param[in] ctat      complementary proportional to absolute temperature
 *                      value
 * @param[in] ptat      proportional to absolute temperature value
 * @return              Temperature value (°C)
 *
 * @api
 */
float tsens_get_temperature(uint16_t ctat, uint16_t ptat);

#endif /* _TSENS_H_ */

/** @} */
