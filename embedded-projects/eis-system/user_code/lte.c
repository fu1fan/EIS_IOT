/*
 * lte.c
 *
 *  Created on: 2024年7月2日
 *      Author: Whisp
 */

#include "lte.h"
#include "lte_uart.h"
#include "tft_ui.h"

static uint8 uart_buffer[LTE_UART_BUFFER_SIZE];
static uint16_t uart_buffer_len = 0;

uint8_t ip_address[16];

#define LTE_DEBUG
#define CNT96_MAX 5

uint8_t cnt96 = 0; // 给0x60错误计数，如果连续三次则表明需要重新初始化LTE模块

eis_status_t lte_uart_receive(uint16_t timeout){
	// 读取到TIMEOUT为止（自动截断）
	// 如果结尾不为/r/n则报错
	static uint8 temp_buffer[32];
	static uint8_t temp_len;
	static uint16_t receive_time;
	static eis_status_t status;
	status.is_success = 1;
	status.error_code = 0x00;

	receive_time = 0;

	uart_buffer_len = 0;

	while (1) {
		temp_len = lte_uart_read_buffer(temp_buffer, 32);
		if (temp_len) {
			receive_time = 0;	// 重置接收等待时间
			timeout = 200;
			if (uart_buffer_len + temp_len > LTE_UART_BUFFER_SIZE) {
				status.is_success = 0;
				status.error_code = 0x42;
				break;
			}
			memcpy(uart_buffer + uart_buffer_len, temp_buffer, temp_len);
			uart_buffer_len += temp_len;
		} else {
			receive_time++;
			if (receive_time > timeout) {
				// 如果结尾为/r/n则说明接收成功
				if (uart_buffer[uart_buffer_len - 1] == '\n'
						&& uart_buffer[uart_buffer_len - 2] == '\r') {
					uart_buffer[uart_buffer_len] = '\0';
					break;
				} else {
					status.is_success = 0;
					status.error_code = 0x41;
					break;
				}
			}
		}
		osal_delay_millisec(1U);
	}
	return status;
}

void lte_uart_clear(void) {
	lte_uart_read_buffer(uart_buffer, LTE_UART_BUFFER_SIZE);
}

void lte_uart_printf(const char *fmt, ...) {
	va_list args;
	char buffer[512];
	va_start(args, fmt);
	vsnprintf(buffer, 512, fmt, args);
	va_end(args);
	lte_uart_write_string(buffer);
}

eis_status_t lte_module_init(void) {
	eis_status_t status;
	status.is_success = 1;
	status.error_code = 0x00;

	lte_uart_init();
	osal_delay_millisec(50);

	uint8_t retry = 0;
	while(retry < 10){
		lte_uart_printf("AT\r\n");
		status = lte_uart_receive(RECEIVE_TIMEOUT);
		if (!status.is_success && !(status.error_code == 0x41)) {
			return status;
		}
		if (strstr((const char *)uart_buffer, "OK") != NULL) {
			break;
		}
		osal_delay_millisec(500);
		retry++;
	}
	if (retry == 10) {
		status.is_success = 0;
		status.error_code = 0x15;
		return status;
	}

	return status;
}

eis_status_t lte_celluar_init(void){
	eis_status_t status;
	status.is_success = 1;
	status.error_code = 0x00;

	// SIM卡检查
//	lte_uart_printf("AT+CPIN?\r\n");
//	status = lte_uart_receive(RECEIVE_TIMEOUT);
//	if (!status.is_success) {
//		return status;
//	}
//	if (strstr((const char*) uart_buffer, "READY") == NULL) {
//		status.is_success = 0;
//		status.error_code = 0x51;
//		return status;
//	}
	uint8_t retry = 0;
	while(retry<10){
		lte_uart_printf("AT+CPIN?\r\n");
		status = lte_uart_receive(RECEIVE_TIMEOUT);
		if (!status.is_success && !(status.error_code == 0x41)) {
			return status;
		}
		if (strstr((const char*) uart_buffer, "READY") != NULL) {
			break;
		}
		osal_delay_millisec(500);
		retry++;
	}
	if (retry == 10) {
		status.is_success = 0;
		status.error_code = 0x51;
		return status;
	}

	// 查询信号
	retry = 0;
	while(1){
		lte_uart_printf("AT+CSQ?\r\n");
		status = lte_uart_receive(RECEIVE_TIMEOUT);
		if (!status.is_success) {
			return status;
		}
		if (strstr((const char*) uart_buffer, "OK") == NULL) {
			status.is_success = 0;
			status.error_code = 0x52;
			return status;
		}
		char *p = strstr((const char*) uart_buffer, "+CSQ: ");
		if (p == NULL) {
			status.is_success = 0;
			status.error_code = 0x53;
			return status;
		}
		p += 5;
		char *q = strstr(p, ",");
		*q = '\0';
		uint8_t csq = atoi(p);
		if (csq < 10) {
			if(retry<10){
				osal_delay_millisec(2000);
				retry++;
			}
			else{
				status.is_success = 0;
				status.error_code = 0x54;
				return status;
			}
		}
		else {
			break;
		}
	}


	// 查询网络注册状态
	lte_uart_printf("AT+CGREG?\r\n");
	status = lte_uart_receive(RECEIVE_TIMEOUT);
	if (!status.is_success) {
		return status;
	}
	if (strstr((const char*) uart_buffer, "OK") == NULL) {
		status.is_success = 0;
		status.error_code = 0x55;
		return status;
	}

	//  查询IP地址
	osal_delay_millisec(500);
	lte_uart_printf("AT+MIPCALL=1\r\n");
	status = lte_uart_receive(RECEIVE_TIMEOUT);
	if (!status.is_success) {
		return status;
	}
	if (strstr((const char*) uart_buffer, "ERROR")) {
		lte_uart_printf("AT+MIPCALL?\r\n");
		status = lte_uart_receive(RECEIVE_TIMEOUT);
	}
	if (strstr((const char*) uart_buffer, "OK") == NULL) {
		status.is_success = 0;
		status.error_code = 0x56;
		return status;
	}
	char *p1 = strstr((const char*) uart_buffer, "+MIPCALL: ");
	if (p1 == NULL) {
		status.is_success = 0;
		status.error_code = 0x56;
		return status;
	}
	p1 += 10;
	if(strstr((const char*) uart_buffer, "+MIPCALL: 1,")!=NULL){
		p1 += 2;
	}
	char *q1 = strstr(p1, "\r\n");
	*q1 = '\0';
	strcpy((char*) ip_address, p1);
	// 如果ip地址为0或者0\r则说明获取失败
	if (strcmp((char*) ip_address, "0") == 0
			|| strcmp((char*) ip_address, "0\r") == 0) {
		status.is_success = 0;
		status.error_code = 0x56;
		return status;
	}

	return status;
}

eis_status_t _lte_http_get(const char *url, http_response_t *response, uint32_t timeout){
	eis_status_t status;
	status.is_success = 1;
	status.error_code = 0x00;

	if(timeout<10000){
		timeout = 10000;
	}

	lte_uart_printf("AT+HTTPSET=\"URL\",\"%s\"\r\n", url);
	status = lte_uart_receive(RECEIVE_TIMEOUT);
	if (!status.is_success) {
		return status;
	}
	if (strstr((const char*) uart_buffer, "OK") == NULL) {
		status.is_success = 0;
		status.error_code = 0x60;
#ifdef LTE_DEBUG
		ui_console_printf("get debug point 0");
#endif
		return status;
	}
	lte_uart_printf("AT+HTTPACT=0,%d\r\n", timeout/1000);
	status = lte_uart_receive(RECEIVE_TIMEOUT);
	// 如果+HTTP: 存在，说明已经完成请求
	char *p = strstr((const char*) uart_buffer, "AT+HTTPACT=0");
	if (p == NULL) {
		status.is_success = 0;
		status.error_code = 0x60;
#ifdef LTE_DEBUG
		ui_console_printf("get debug point 1");
#endif
		return status;
	}
	p += 15;
	if (strstr(p, "+HTTP") == NULL) {
		if (strstr(p, "OK") == NULL) {
			status.is_success = 0;
			status.error_code = 0x60;
#ifdef LTE_DEBUG
			ui_console_printf("get debug point 2");
#endif
			return status;
		}
		status = lte_uart_receive(timeout - RECEIVE_TIMEOUT);
		if (!status.is_success) {
			return status;
		}
	}
	if (strstr((const char*) uart_buffer, "+HTTP") == NULL) {
		status.is_success = 0;
		status.error_code = 0x60;
#ifdef LTE_DEBUG
		ui_console_printf("get debug point 3");
#endif
		return status;
	}
	//+HTTPRES: 0,200,10499
	p = strstr((const char*) uart_buffer, "+HTTPRES: ");
	if (p == NULL) {
		status.is_success = 0;
		status.error_code = 0x60;
#ifdef LTE_DEBUG
		ui_console_printf("get debug point 4");
#endif
		return status;
	}
	p = strstr(p, ",");
	char *q = strstr(++p, ",");
	*q = '\0';
	response->code = (uint16_t)atoi(p);
	p = q + 1;
	q = strstr(p, "\r\n");
	*q = '\0';
	uint32_t len = atoi(p);
	if (len > response->max_size) {
		status.is_success = 0;
		status.error_code = 0x61;
		return status;
	}
	uint16_t size_once = LTE_UART_BUFFER_SIZE - 64;
	uint16_t size;
	uint32_t index = 0;
	while(len > 0){
		size = len > size_once ? size_once : len;
        lte_uart_printf("AT+HTTPREAD=%d,%d\r\n", index, size);

        status = lte_uart_receive(RECEIVE_TIMEOUT);
		if (!status.is_success) {
			return status;
		}
		p = strstr((const char*) uart_buffer, "\r\n");
		if (p == NULL) {
			status.is_success = 0;
			status.error_code = 0x62;
			return status;
		}
		p++;
		p = strstr(p, "\r\n");
		p+=2;
		q = strstr(p, "\r\nOK\r\n");
		if (q == NULL) {
			status.is_success = 0;
			status.error_code = 0x62;
			return status;
		}
		*q = '\0';

		strcpy(response->content + index, p);

		len -= size;
		index += size;
	}
	*(response->content + index)= '\0';
	response->size = index;

	return status;
}

eis_status_t _lte_http_post(const char *url, const uint8_t *content, uint32_t content_size, http_response_t *response, uint32_t timeout){
	eis_status_t status;
	status.is_success = 1;
	status.error_code = 0x00;

	if(timeout<10000){
		timeout = 10000;
	}

	lte_uart_printf("AT+HTTPSET=\"URL\",\"%s\"\r\n", url);
	status = lte_uart_receive(RECEIVE_TIMEOUT);
	if (!status.is_success) {
		return status;
	}
	if (strstr((const char*) uart_buffer, "OK") == NULL) {
		status.is_success = 0;
		status.error_code = 0x60;
#ifdef LTE_DEBUG
		ui_console_printf("post debug point 0");
#endif
		return status;
	}
	lte_uart_printf("AT+HTTPDATA=%d\r\n", content_size);
	status = lte_uart_receive(RECEIVE_TIMEOUT);
	if (!status.is_success) {
		return status;
	}
	if (strstr((const char*) uart_buffer, "AT+HTTPDATA") == NULL) {
		status.is_success = 0;
		status.error_code = 0x60;
#ifdef LTE_DEBUG
		ui_console_printf("post debug point 0.5");
#endif
	}
	// 每次发送64byte
	while (content_size > 64) {
		lte_uart_write_buffer(content, 64);
		content += 64;
		content_size -= 64;
		osal_delay_millisec(10U);
	}
	lte_uart_write_buffer(content, content_size);
	osal_delay_millisec(100U);
	lte_uart_clear();

	lte_uart_printf("AT+HTTPACT=1,%d\r\n", timeout/1000); // TODO: 优化等待时间
	status = lte_uart_receive(RECEIVE_TIMEOUT);
	if (!status.is_success) {
		return status;
	}
	// 如果+HTTP: 存在，说明已经完成请求
	char *p = strstr((const char*) uart_buffer, "AT+HTTPACT=1");
	if (p == NULL) {
		status.is_success = 0;
		status.error_code = 0x60;
#ifdef LTE_DEBUG
		ui_console_printf("post debug point 1");
#endif
		return status;
	}
	p += 15;
	if (strstr(p, "+HTTP") == NULL) {
		if (strstr(p, "OK") == NULL) {
			status.is_success = 0;
			status.error_code = 0x60;
#ifdef LTE_DEBUG
			ui_console_printf("post debug point 2");
#endif
			return status;
		}
		status = lte_uart_receive(timeout - RECEIVE_TIMEOUT);
		if (!status.is_success) {
			return status;
		}
	}
	if (strstr((const char*) uart_buffer, "+HTTP") == NULL) {
		status.is_success = 0;
		status.error_code = 0x60;
#ifdef LTE_DEBUG
		ui_console_printf("post debug point 3");
#endif
		return status;
	}
	//+HTTPRES: 0,200,10499
	p = strstr((const char*) uart_buffer, "+HTTPRES: ");
	if (p == NULL) {
		status.is_success = 0;
		status.error_code = 0x60;
#ifdef LTE_DEBUG
		ui_console_printf("post debug point 4");
#endif
		return status;
	}
	p = strstr(p, ",");
	char *q = strstr(++p, ",");
	*q = '\0';
	response->code = (uint16_t)atoi(p);
	p = q + 1;
	q = strstr(p, "\r\n");
	*q = '\0';
	uint32_t len = atoi(p);
	if (len > response->max_size) {
		status.is_success = 0;
		status.error_code = 0x61;
		return status;
	}
	uint16_t size_once = LTE_UART_BUFFER_SIZE - 64;
	uint16_t size;
	uint32_t index = 0;
	while(len > 0){
		size = len > size_once ? size_once : len;
        lte_uart_printf("AT+HTTPREAD=%d,%d\r\n", index, size);

        status = lte_uart_receive(RECEIVE_TIMEOUT);
		if (!status.is_success) {
			return status;
		}
		p = strstr((const char*) uart_buffer, "\r\n");
		if (p == NULL) {
			status.is_success = 0;
			status.error_code = 0x62;
			return status;
		}
		p++;
		p = strstr(p, "\r\n");
		p+=2;
		q = strstr(p, "\r\nOK\r\n");
		if (q == NULL) {
			status.is_success = 0;
			status.error_code = 0x62;
			return status;
		}
		*q = '\0';

		strcpy(response->content + index, p);

		len -= size;
		index += size;
	}
	*(response->content + index)= '\0';
	response->size = index;

	return status;
}

eis_status_t lte_http_get(const char *url, http_response_t *response, uint32_t timeout, uint8_t retry){
	eis_status_t status;
	uint8_t self_cnt96 = 0;
	for (uint8_t i = 0; i < retry; i++) {
		status = _lte_http_get(url, response, timeout);
		if (status.is_success) {
			cnt96 = 0;
			break;
		}
		if (status.error_code == 0x60) {
			cnt96++;
			self_cnt96++;
			if (cnt96 >= CNT96_MAX) {
				if (self_cnt96 == 255) {
					status.error_code = 0x64;
					return status;
				}
				status = lte_module_init();
				if (!status.is_success) {
					status.error_code = 0x64;
					return status;
				}
				status = lte_celluar_init();
				if (!status.is_success) {
					status.error_code = 0x64;
					return status;
				}
				cnt96 = 0;
				i -= self_cnt96;
				self_cnt96 = 255;
			}
		}
	}
	return status;
}

eis_status_t lte_http_post(const char *url, const uint8_t *content, uint32_t content_size, http_response_t *response, uint32_t timeout, uint8_t retry){
	eis_status_t status;
	uint8_t self_cnt96 = 0;

	for (uint8_t i = 0; i < retry; i++) {
		status = _lte_http_post(url, content, content_size, response, timeout);
		if (status.is_success) {
			cnt96 = 0;
			break;
		}
		if (status.error_code == 0x60) {
			cnt96++;
			self_cnt96++;
			if (cnt96 >= CNT96_MAX) {
				if (self_cnt96 == 255) {
					status.error_code = 0x64;
					return status;
				}
				status = lte_module_init();
				if (!status.is_success) {
					status.error_code = 0x64;
					return status;
				}
				status = lte_celluar_init();
				if (!status.is_success) {
					status.error_code = 0x64;
					return status;
				}
				cnt96 = 0;
				i -= self_cnt96;
				self_cnt96 = 255;
			}
		} else {
			osal_delay_millisec(500);
		}
	}
	return status;
}

void response_purify(http_response_t *response){
	// 删除http响应头
	char *p = strstr(response->content, "\r\n\r\n");
	if (p == NULL) {
		return;
	}
	p += 4;
	uint32_t len = response->size - (p - response->content);
	memmove(response->content, p, len);
	response->content[len] = '\0';
}
