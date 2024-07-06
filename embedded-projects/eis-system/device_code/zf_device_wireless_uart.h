/*
 * zf_device_wireless_uart.h
 *
 *  Created on: 2024年2月26日
 *      Author: SEEKFREE_Teternal
 */

#ifndef DEVICE_CODE_ZF_DEVICE_WIRELESS_UART_H_
#define DEVICE_CODE_ZF_DEVICE_WIRELESS_UART_H_

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <config.h>

#include <gpio.h>
#include <uart.h>
#include <uart_private.h>

#include "zf_common_typedef.h"

#define WIRLESS_UART_INDEX              ( &DRV_UART2 )
#define WIRLESS_UART_BAUDRATE           ( UART_BAUDRATE_115200 )

#define WIRELESS_UART_BUFFER_SIZE       ( 128   )
#define WIRELESS_UART_TIMEOUT_COUNT     ( 0x000FFFFF )

uint32  wireless_uart_write_string      (const char *str);
uint32  wireless_uart_write_buffer      (const uint8 *buff, uint32 len);

uint32  wireless_uart_read_buffer       (uint8 *buff, uint32 len);

uint8   wireless_uart_init              (void);

#endif /* DEVICE_CODE_ZF_DEVICE_WIRELESS_UART_H_ */
