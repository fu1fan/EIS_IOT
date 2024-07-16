/*
 * ExpansionBoard.c
 *
 *  Created on: 2024年7月4日
 *      Author: Whisp
 */

#include "ExpansionBoard.h"

static uint8_t board_count = 0;

void _74HC595_Init(void){
	gpio_write_pin(DS_PIN, 0);
	gpio_write_pin(OE_PIN, 0);
	gpio_write_pin(STCP_PIN, 0);
	gpio_write_pin(SHCP_PIN, 0);
	gpio_write_pin(MR_PIN, 0);
	osal_delay_millisec(1U);
	gpio_write_pin(MR_PIN, 1);
}

void _74HC595_Apply(void) {
	gpio_write_pin(STCP_PIN, 0);
	osal_delay_millisec(1U);
	gpio_write_pin(STCP_PIN, 1);
}

void _74HC595_Write(uint8_t data, uint8_t apply) {
	uint8_t i;
	for (i = 0; i < 8; i++) {
		gpio_write_pin(DS_PIN, data & 0x80);
		gpio_write_pin(SHCP_PIN, 0);
		osal_delay_millisec(1U);
		gpio_write_pin(SHCP_PIN, 1);
		osal_delay_millisec(1U);
		data <<= 1;
	}
	if(apply) {
		_74HC595_Apply();
	}
}

void _74HC595_Reset(void) {
	gpio_write_pin(MR_PIN, 0);
	osal_delay_millisec(1U);
	gpio_write_pin(MR_PIN, 1);
}

void _74HC595_Enable(void) {
	gpio_write_pin(OE_PIN, 0);
}

void _74HC595_Disable(void) {
	gpio_write_pin(OE_PIN, 1);
}

void _74HC595_Clear(void) {
	_74HC595_Reset();
	for (int i = 0; i < board_count; i++) {
		_74HC595_Write(0x00, 0);
	}
	_74HC595_Apply();
}

uint8_t EB_Init(void) {
	_74HC595_Init();
	board_count = 0;
	uint8_t i = 0;
	while(board_count < 4){
		_74HC595_Reset();
		_74HC595_Write(0b01100000, 0);
		for(i=0; i<board_count; i++){
			_74HC595_Write(0x00, 0);
		}
		_74HC595_Apply();
		osal_delay_millisec(50U);
		if (gpio_read_pin(DET_PIN)) {
			board_count++;
		}
		else{
			break;
		}
	}
	EB_Clear();
	return board_count;
}

void EB_Select(uint8_t board, uint8_t index) {
	_74HC595_Clear();
	// 除了被选的board，其他board寄存器全写0

	for (uint8_t re = 0; re < 3; re++) {
		uint8_t write_index = board_count - board - 1;
			uint8_t i = 0;
			for (i = 0; i < board_count; i++) {
				if (i == write_index) {
					_74HC595_Write(0x01 << index | 0b01000000, 0);
				} else {
					_74HC595_Write(0x00, 0);
				}
			}
			_74HC595_Apply();
	}
}

void EB_Clear(void) {
	_74HC595_Clear();
	_74HC595_Clear();
	_74HC595_Clear();
}
