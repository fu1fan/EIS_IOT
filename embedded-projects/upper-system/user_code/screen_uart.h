/*
 * screen_uart.h
 *
 *  Created on: 2024年7月2日
 *      Author: Whisp
 */

#ifndef USER_CODE_SCREEN_UART_H_
#define USER_CODE_SCREEN_UART_H_

#include <uart.h>

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
#include <uart_private.h>

#include "zf_common_typedef.h"

#define SCREEN_UART_INDEX            ( &DRV_UART2 )
#define SCREEN_UART_BAUDRATE         ( UART_BAUDRATE_921600 )
#define SCREEN_UART_BUFFER_SIZE      ( 1024 )
#define SCREEN_UART_TIMEOUT_COUNT    ( 0x000FFFFF )

void    screen_uart_write_string (const char *string);
uint32  screen_uart_write_buffer (const uint8 *buffer, uint32 len);
uint32  screen_uart_read_buffer  (uint8 *buff, uint32 len);
void    screen_uart_init         (void);

#endif /* USER_CODE_SCREEN_UART_H_ */
