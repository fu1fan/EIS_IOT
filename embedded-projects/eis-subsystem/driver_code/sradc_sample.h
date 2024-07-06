/*
 * sradc_sample.h
 *
 *  Created on: 2024年2月22日
 *      Author: SEEKFREE_Teternal
 */

#ifndef DRIVER_CODE_SRADC_SAMPLE_H_
#define DRIVER_CODE_SRADC_SAMPLE_H_

#include <gpio.h>
#include <dma.h>

#include <saradc.h>
#include <saradc_private.h>

#define     SRADC_PART1_INDEX               ( DRV_ADC2 )
#define     SRADC_PART2_INDEX               ( DRV_ADC3 )
#define     SRADC_PART3_INDEX               ( DRV_ADC4 )

#define     SRADC_RES_DEFAULT               ( ADC_DATA_RES_12 )

#define     SRADC_SAMPLE_INDEX_MAX          ( 3U )
#define     SRADC_SAMPLE_PART_CHANNEL_MAX   ( 3U )
#define     SRADC_SAMPLE_CHANNEL_MAX        ( 9U )

#define     SRADC_SAMPLE_TIMEROUT_MAX       ( 0x1FF )

typedef enum
{
    // PART 1
    SARDC_MOTOR1_A,
    SARDC_MOTOR1_B,
    SARDC_MOTOR1_C,

    // PART 2
    SARDC_MOTOR2_A,
    SARDC_MOTOR2_B,
    SARDC_MOTOR2_C,

    // PART 3
    SARDC_MOTOR1_SUM,
    SARDC_MOTOR2_SUM,
    SARDC_POWER_SAMPLE,
}sradc_channle_index_enum;

extern uint8_t      sradc_sample_flag;
extern adc_sample_t sradc_sample_buffer[SRADC_SAMPLE_CHANNEL_MAX];

void sradc_sample   (void);
void sradc_init     (void);

#endif /* DRIVER_CODE_SRADC_SAMPLE_H_ */
