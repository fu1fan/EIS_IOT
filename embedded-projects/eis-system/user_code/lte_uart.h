/*
 * lte_uart.h
 *
 *  Created on: 2024年7月2日
 *      Author: Whisp
 */

#ifndef USER_CODE_LTE_UART_H_
#define USER_CODE_LTE_UART_H_

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

#define LTE_UART_INDEX            ( &DRV_UART2 )
#define LTE_UART_BAUDRATE         ( UART_BAUDRATE_115200 )
#define LTE_UART_BUFFER_SIZE      ( 128 )
#define LTE_UART_TIMEOUT_COUNT    ( 0x000FFFFF )

void    lte_uart_write_string (const char *string);
uint32  lte_uart_write_buffer (const uint8 *buffer, uint32 len);
uint32  lte_uart_read_buffer  (uint8 *buff, uint32 len);
void    lte_uart_init         (void);

#endif /* USER_CODE_LTE_UART_H_ */
