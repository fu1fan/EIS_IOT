/*
 * lte.h
 *
 *  Created on: 2024年7月2日
 *      Author: Whisp
 */

#ifndef USER_CODE_LTE_H_
#define USER_CODE_LTE_H_

#include "driver_headfile.h"

#define RECEIVE_TIMEOUT 1000

extern uint8_t ip_address[16];

typedef struct{
	uint16_t code;
	uint32_t size;
	char *content;
	uint32_t max_size;
}http_response_t;

eis_status_t lte_module_init(void);
eis_status_t lte_celluar_init(void);
eis_status_t lte_connect_server(void);

eis_status_t lte_http_get(const char *url, http_response_t *response, uint16_t timeout, uint8_t retry);
eis_status_t lte_http_post(const char *url, const uint8_t *content, uint32_t content_size, http_response_t *response, uint16_t timeout, uint8_t retry);
void response_purify(http_response_t *response);

#endif /* USER_CODE_LTE_H_ */
