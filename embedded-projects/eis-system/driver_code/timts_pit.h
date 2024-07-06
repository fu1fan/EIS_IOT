/*
 * timts_pit.h
 *
 *  Created on: 2024年2月23日
 *      Author: SEEKFREE_Teternal
 */

#ifndef DRIVER_CODE_TIMTS_PIT_H_
#define DRIVER_CODE_TIMTS_PIT_H_

#include <tim_ts.h>

extern void (*timts_interrupt_callback)(void);

void timts_set_callback (void (*callback)(tim_ts_driver_t *tdp));
void timts_interrupt_mode (uint8_t state);
void timts_pit_ms_init  (uint16_t period_ms);
void timts_pit_us_init  (uint16_t period_us);
void timts_pit_freq_init  (uint32_t period_freq);

#endif /* DRIVER_CODE_TIMTS_PIT_H_ */
