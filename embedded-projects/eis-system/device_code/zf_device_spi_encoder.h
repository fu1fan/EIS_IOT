/*
 * zf_device_spi_encoder.h
 *
 *  Created on: 2024年2月23日
 *      Author: SEEKFREE_Teternal
 */

#ifndef _ZF_DEVICE_SPI_ENCODER_H_
#define _ZF_DEVICE_SPI_ENCODER_H_

#include "zf_common_typedef.h"

#include "spi_config.h"

#define SPI_ENCODER_INDEX       ( &DRV_SPI2 )
#define SPI_ENCODER_MOTOR1_CS   ( ENCODER1_CS_G8 )
#define SPI_ENCODER_MOTOR2_CS   ( ENCODER2_CS_G12 )

typedef enum
{
	SPI_ENCODER_MOTOR1,
	SPI_ENCODER_MOTOR2,
}spi_encoder_channel_enum;

extern float spi_encoder_last_angle[2];
extern float spi_encoder_now_angle[2];

float spi_encoder_get_location  (spi_encoder_channel_enum encoder_ch);
float spi_encoder_get_offset    (spi_encoder_channel_enum encoder_ch);

void spi_encoder_init (void);

#endif
