/*
 * can_remote.c
 *
 *  Created on: 2024年6月28日
 *      Author: Whisp
 */
#include "can_remote.h"

#define TIMEOUT 200

uint8_t can_remote_init(void) {
	can_remote_id = 0x7F0;
	can_module_init();

	can_receive_buffer[0] = 0x00;

	// 等待rx_flag变为1
	uint16_t time = 0;
	can_rx_flag = 0;
	while (!can_rx_flag) {
		osal_delay_millisec(1U);
		time++;
		if (time > TIMEOUT) {
			return 0;
		}
	}
	can_rx_flag = 0;
	if (can_receive_buffer[0] == 0xFF) {
		return 1;
	}
	return 0;
}

void can_task(void){
	if (can_rx_flag) {
		can_rx_flag = 0;


	}
}

void ad9959_reset(void){
	can_receive_buffer[0] = 0x01;
	can_receive_buffer[1] = 0x00;
	can_receive_buffer[2] = 0x00;
	can_receive_buffer[3] = 0x00;
	can_receive_buffer[4] = 0x00;
	can_receive_buffer[5] = 0x00;
	can_receive_buffer[6] = 0x00;
	can_receive_buffer[7] = 0x00;

	can_send_frame(can_remote_id, can_receive_buffer);
	osal_delay_millisec(25U);
}

void ad9959_set(uint8_t ch_, uint32_t freq_, uint16_t amp_){
	can_receive_buffer[0] = 0x01;
	can_receive_buffer[1] = ch_;
	can_receive_buffer[2] = amp_ >> 8;
	can_receive_buffer[3] = amp_;
	can_receive_buffer[4] = freq_ >> 24;
	can_receive_buffer[5] = freq_ >> 16;
	can_receive_buffer[6] = freq_ >> 8;
	can_receive_buffer[7] = freq_;

	can_send_frame(can_remote_id, can_receive_buffer);
	osal_delay_millisec(25U);
 }

uint8_t eb_query() {
	can_receive_buffer[0] = 0x02;
	can_receive_buffer[1] = 0x00;

	can_rx_flag = 0;
	can_send_frame(can_remote_id, can_receive_buffer);

	uint16_t time = 0;

	while (!can_rx_flag) {
		osal_delay_millisec(1U);
		time++;
		if (time > TIMEOUT) {
			return 0;
		}
	}
	can_rx_flag = 0;
	return can_receive_buffer[1];
}

void eb_set(uint8_t board, uint8_t index) {
	can_receive_buffer[0] = 0x02;
	can_receive_buffer[1] = 0x01;
	can_receive_buffer[2] = board;
	can_receive_buffer[3] = index;

	can_send_frame(can_remote_id, can_receive_buffer);
	osal_delay_millisec(25U);
}

void eb_clear() {
	can_receive_buffer[0] = 0x02;
	can_receive_buffer[1] = 0x02;

	can_send_frame(can_remote_id, can_receive_buffer);
	osal_delay_millisec(25U);
}


