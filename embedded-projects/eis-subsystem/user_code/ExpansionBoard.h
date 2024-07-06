/*
 * ExpansionBoard.h
 *
 *  Created on: 2024年7月4日
 *      Author: Whisp
 */

#ifndef USER_CODE_EXPANSIONBOARD_H_
#define USER_CODE_EXPANSIONBOARD_H_

#include "driver_headfile.h"

#define DS_PIN		PIN_GPIO8_0
#define OE_PIN		PIN_GPIO8_1
#define STCP_PIN	PIN_GPIO8_2
#define SHCP_PIN	PIN_GPIO8_3
#define MR_PIN		PIN_GPIO8_4
#define DET_PIN		PIN_GPIO8_5

uint8_t EB_Init(void);
void EB_Select(uint8_t board, uint8_t index);
void EB_Clear(void);

#endif /* USER_CODE_EXPANSIONBOARD_H_ */
