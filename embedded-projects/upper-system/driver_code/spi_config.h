/*
 * spi_config.h
 *
 *  Created on: 2024年2月26日
 *      Author: SEEKFREE_Teternal
 */

#ifndef DRIVER_CODE_SPI_CONFIG_H_
#define DRIVER_CODE_SPI_CONFIG_H_

#include <config.h>

#include <gpio.h>
#include <spi.h>
#include <spi_private.h>

#include "zf_common_typedef.h"

typedef enum
{
    SPI_MODE0,
    SPI_MODE1,
    SPI_MODE2,
    SPI_MODE3,
}spi_mode_enum;

typedef enum
{
    SPI_BAUD_DIV2,
    SPI_BAUD_DIV4,
    SPI_BAUD_DIV8,
    SPI_BAUD_DIV16,
    SPI_BAUD_DIV32,
    SPI_BAUD_DIV64,
    SPI_BAUD_DIV128,
    SPI_BAUD_DIV256,
}spi_baud_div_enum;

void spi_read_8bit_array (spi_driver_t *spi_index, uint8 *buffer, uint32 len);
void spi_write_8bit_array (spi_driver_t *spi_index, uint8 *buffer, uint32 len);
void spi_transfer_8bit_array (spi_driver_t *spi_index, uint8 *tx_buffer, uint8 *rx_buffer, uint32 len);

void spi_read_16bit_array (spi_driver_t *spi_index, uint16 *buffer, uint32 len);
void spi_write_16bit_array (spi_driver_t *spi_index, uint16 *buffer, uint32 len);
void spi_transfer_16bit_array (spi_driver_t *spi_index, uint16 *tx_buffer, uint16 *rx_buffer, uint32 len);

void spi_config_init (spi_driver_t *spi_index, spi_mode_enum mode, spi_baud_div_enum baud_div);

#endif /* DRIVER_CODE_SPI_CONFIG_H_ */
