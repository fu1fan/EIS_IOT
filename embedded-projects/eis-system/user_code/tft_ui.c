/*
 * tft_ui.c
 *
 *  Created on: 2024年7月1日
 *      Author: Whisp
 */
#include "tft_ui.h"

#include "zf_device_tft180.h"
#include "stdio.h"

static uint8_t console_line = 0;

void ui_init(){
    tft180_init();
    tft180_full(RGB565_BLACK);
}

void ui_console_mode() {
	console_line = 0;
	tft180_full(RGB565_BLACK);
	tft180_set_font(TFT180_6X8_FONT);
	tft180_set_color(RGB565_WHITE, RGB565_BLACK);
}

void ui_console_printf(const char *fmt, ...) {
	if (console_line >= 160 / 8) {
		console_line = 0;
		tft180_full(RGB565_BLACK);
	}
	va_list args;
	va_start(args, fmt);
	char buf[128];
	vsprintf(buf, fmt, args);
	va_end(args);

	tft180_show_string(0, console_line * 8, buf);

	console_line++;
}

void ui_console_up() {
	console_line--;
}

void ui_background(void){

}

