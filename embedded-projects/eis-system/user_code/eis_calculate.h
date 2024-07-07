/*
 * eis_calculate.h
 *
 *  Created on: 2024年6月29日
 *      Author: Whisp
 */

#ifndef USER_CODE_EIS_CALCULATE_H_
#define USER_CODE_EIS_CALCULATE_H_

#include "driver_headfile.h"

#define FACTOR (250 * 1.62 * 1.3)

struct eis_node_t{
	float real;
	float imag;
	uint32_t freq;
	struct eis_node_t *next;
};

struct eis_data_t{
	struct eis_node_t *head;
	struct eis_node_t *tail;
	size_t size;
};

extern struct eis_data_t eis_result;

void eis_result_clear(void);
void eis_calculate_init(void);
void eis_add_point(uint32_t sample_rate);
void eis_remove_tail(void);
void eis_single_point_process(void);

#endif /* USER_CODE_EIS_CALCULATE_H_ */
