/*
 * can_remote.h
 *
 *  Created on: 2024年6月28日
 *      Author: Whisp
 */

#ifndef USER_CODE_CAN_REMOTE_H_
#define USER_CODE_CAN_REMOTE_H_

#define CH0 0x10
#define CH1 0x20
#define CH2 0x40
#define CH3 0x80

#include "driver_headfile.h"

uint8_t can_remote_init(void);
void can_task(void);
void ad9959_reset(void);
void ad9959_set(uint8_t ch_, uint32_t freq_, uint16_t amp_);
uint8_t eb_query(void);
void eb_set(uint8_t board, uint8_t index);
void eb_clear(void);


#endif /* USER_CODE_CAN_REMOTE_H_ */
