/*
 * can_config.h
 *
 *  Created on: 2024年2月27日
 *      Author: SEEKFREE_Teternal
 */

#ifndef DRIVER_CODE_CAN_CONFIG_H_
#define DRIVER_CODE_CAN_CONFIG_H_

#include <platform.h>
#include <irq.h>
#include <can.h>

#define CAN_DEFAULT_FILTER_ID               0x7F0UL
#define CAN_MESSAGE_SIZE                    8U

extern bool        can_rx_flag;
extern uint16_t    can_remote_id;
extern uint8_t     can_receive_buffer[8];

void can_send_frame (uint16_t id, uint8_t data[8]);
void can_module_init (void);

#endif /* DRIVER_CODE_CAN_CONFIG_H_ */
