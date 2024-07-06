/*
 * debug_uart.h
 *
 *  Created on: 2024年2月20日
 *      Author: SEEKFREE_Teternal
 */

#ifndef DRIVER_CODE_DEBUG_UART_H_
#define DRIVER_CODE_DEBUG_UART_H_

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#include <platform.h>
#include <typedefs.h>

#include <irq.h>

#include <gpio.h>
#include <uart.h>
#include <uart_private.h>

#include "zf_common_typedef.h"

#define DEBUG_UART_INDEX            ( &DRV_UART3 )
#define DEBUG_UART_BAUDRATE         ( UART_BAUDRATE_460800 )
#define DEBUG_UART_BUFFER_SIZE      ( 128 )
#define DEBUG_UART_TIMEOUT_COUNT    ( 0x000FFFFF )

void    debug_uart_write_string (const char *string);
uint32  debug_uart_write_buffer (const uint8 *buffer, uint32 len);
uint32  debug_uart_read_buffer  (uint8 *buff, uint32 len);
void    debug_uart_init         (void);

#endif /* DRIVER_CODE_DEBUG_UART_H_ */
