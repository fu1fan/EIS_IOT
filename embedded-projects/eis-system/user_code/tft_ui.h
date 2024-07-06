/*
 * tft_ui.h
 *
 *  Created on: 2024年7月1日
 *      Author: Whisp
 */

#ifndef USER_CODE_TFT_UI_H_
#define USER_CODE_TFT_UI_H_

#include "driver_headfile.h"

void ui_init(void);
void ui_menu(uint8_t index);
void ui_console_mode(void);
void ui_console_printf(const char *fmt, ...);

#endif /* USER_CODE_TFT_UI_H_ */
