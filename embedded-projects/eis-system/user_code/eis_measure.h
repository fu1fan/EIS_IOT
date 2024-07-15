/*
 * eis_measure.h
 *
 *  Created on: 2024年6月29日
 *      Author: Whisp
 */

#ifndef USER_CODE_EIS_MEASURE_H_
#define USER_CODE_EIS_MEASURE_H_

#include "driver_headfile.h"
#include "eis_calculate.h"

#define VF_RELAY PIN_GPIO5_3
#define CCS_RELAY PIN_GPIO5_2
#define RELAY_DELAY 50

#define MAX_RETRY 5
#define VOLTAGE_CORRECTION_RANGE 1

#define FRQ_CHANGE_WAIT 100
#define VF_DELAY 50

#define DEFAULT_SAMPLE_RATIO 512

void eis_battery_clear(void);
eis_status_t eis_init(void);
eis_status_t eis_battery_select(uint8_t battery_);
eis_status_t eis_measure(void);

eis_status_t eis_get_voltage(float *voltage);

void eis_single_init(void);
eis_status_t eis_single_measure(uint32_t freq_, uint8_t accuracy_);
void eis_single_end(void);
eis_status_t eis_ohmage_measure(float *ohmage, uint32_t *freq);



#endif /* USER_CODE_EIS_MEASURE_H_ */
