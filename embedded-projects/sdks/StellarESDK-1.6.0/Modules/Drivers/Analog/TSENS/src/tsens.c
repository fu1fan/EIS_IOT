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
 * @file    tsens.c
 * @brief   TSENS driver source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup ANALOG
 * @ingroup DRIVERS
 * @addtogroup TSENS
 * @ingroup ANALOG
 * @{
 */

#include "tsens.h"

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

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void tsens_init(void) {

}

float tsens_get_temperature(uint16_t ctat, uint16_t ptat) {

    float temperature;
    float num, den;
    int16_t p1, p2, c1, c2;
    int16_t px, cx;
    int32_t a, b;

    /* get calibration constants from UTEST area */
    p1 = *((int16_t *)TSENS_P1_ADDRESS);
    p2 = *((int16_t *)TSENS_P2_ADDRESS);
    c1 = *((int16_t *)TSENS_C1_ADDRESS);
    c2 = *((int16_t *)TSENS_C2_ADDRESS);

    /* get current CTAT and PTAT values from SARADC channels */
    cx = (int16_t)ctat;
    px = (int16_t)ptat;

    /* temperature formula implementation */
    a = ((((int32_t)px) * c2) - (((int32_t)p2) * cx));
    b = ((((int32_t)cx) * p1) - (((int32_t)px) * c1));

    num = ((float)a) * ((float)TSENS_T1 - (float)TSENS_T2);
    den = ((float)a + (float)b);

    temperature = (float)TSENS_T2 + ((num / den));

    return temperature;
}
