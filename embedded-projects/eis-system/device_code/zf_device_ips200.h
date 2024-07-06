/*
 * zf_device_ips200.h
 *
 *  Created on: 2024年2月26日
 *      Author: SEEKFREE_Teternal
 */

#ifndef DEVICE_CODE_ZF_DEVICE_IPS200_H_
#define DEVICE_CODE_ZF_DEVICE_IPS200_H_

#include <config.h>
#include <tim.h>

#include "zf_common_typedef.h"
#include "spi_config.h"

#define LCD_SPI_INDEX           ( &DRV_SPI4 )

//-------常用颜色----------
typedef enum
{
    RGB565_WHITE    = (0xFFFF),                                                 // 白色
    RGB565_BLACK    = (0x0000),                                                 // 黑色
    RGB565_BLUE     = (0x001F),                                                 // 蓝色
    RGB565_PURPLE   = (0xF81F),                                                 // 紫色
    RGB565_PINK     = (0xFE19),                                                 // 粉色
    RGB565_RED      = (0xF800),                                                 // 红色
    RGB565_MAGENTA  = (0xF81F),                                                 // 品红
    RGB565_GREEN    = (0x07E0),                                                 // 绿色
    RGB565_CYAN     = (0x07FF),                                                 // 青色
    RGB565_YELLOW   = (0xFFE0),                                                 // 黄色
    RGB565_BROWN    = (0xBC40),                                                 // 棕色
    RGB565_GRAY     = (0x8430),                                                 // 灰色

    RGB565_39C5BB   = (0x3616),
    RGB565_66CCFF   = (0x665F),
}rgb565_color_enum;

#define IPS200_DEFAULT_DISPLAY_DIR      ( IPS200_PORTAIT   )                    // 默认的显示方向
#define IPS200_DEFAULT_PENCOLOR         ( RGB565_YELLOW    )                    // 默认的画笔颜色
#define IPS200_DEFAULT_BGCOLOR          ( RGB565_GRAY      )                    // 默认的背景颜色
#define IPS200_DEFAULT_DISPLAY_FONT     ( IPS200_6X8_FONT  )                    // 默认的字体模式

#define IPS200_RST_PIN                  ( LCD_RST_G6 )
#define IPS200_DC_PIN                   ( LCD_DC_H1 )
#define IPS200_CS_PIN                   ( LCD_CS_H12 )

#define IPS200_RST(x)   ((x) ? (gpio_set_pin(IPS200_RST_PIN)) : (gpio_clear_pin(IPS200_RST_PIN)))
#define IPS200_DC(x)    ((x) ? (gpio_set_pin(IPS200_DC_PIN))  : (gpio_clear_pin(IPS200_DC_PIN)))
#define IPS200_CS(x)    ((x) ? (gpio_set_pin(IPS200_CS_PIN))  : (gpio_clear_pin(IPS200_CS_PIN)))

#define IPS200_BLK_DUTY ( 50 )                                                  // 背光亮度调节 0-100

typedef enum
{
    IPS200_PORTAIT                      = 0,                                    // 竖屏模式
    IPS200_PORTAIT_180                  = 1,                                    // 竖屏模式  旋转180
    IPS200_CROSSWISE                    = 2,                                    // 横屏模式
    IPS200_CROSSWISE_180                = 3,                                    // 横屏模式  旋转180
}ips200_dir_enum;

typedef enum
{
    IPS200_6X8_FONT                     = 0,                                    // 6x8      字体
    IPS200_8X16_FONT                    = 1,                                    // 8x16     字体
    IPS200_16X16_FONT                   = 2,                                    // 16x16    字体 目前不支持
}ips200_font_size_enum;

extern const uint8 ascii_font_6x8[][6];
extern const uint8 ascii_font_8x16[][16];

void    ips200_clear                (void);
void    ips200_full                 (const uint16 color);
void    ips200_set_dir              (ips200_dir_enum dir);
void    ips200_set_font             (ips200_font_size_enum font);
void    ips200_set_color            (const uint16 pen, const uint16 bgcolor);
void    ips200_draw_point           (uint16 x, uint16 y, const uint16 color);
void    ips200_draw_line            (uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color);

void    ips200_show_char            (uint16 x, uint16 y, const char dat);
void    ips200_show_string          (uint16 x, uint16 y, const char dat[]);
void    ips200_show_int             (uint16 x, uint16 y, const int32 dat, uint8 num);
void    ips200_show_uint            (uint16 x, uint16 y, const uint32 dat, uint8 num);
void    ips200_show_float           (uint16 x, uint16 y, const double dat, uint8 num, uint8 pointnum);

void    ips200_show_binary_image    (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);
void    ips200_show_gray_image      (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 threshold);
void    ips200_show_rgb565_image    (uint16 x, uint16 y, const uint16 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 color_mode);

void    ips200_show_wave            (uint16 x, uint16 y, const uint16 *wave, uint16 width, uint16 value_max, uint16 dis_width, uint16 dis_value_max);
void    ips200_show_chinese         (uint16 x, uint16 y, uint8 size, const uint8 *chinese_buffer, uint8 number, const uint16 color);

void    ips200_init                 (void);

void    func_int_to_str             (char *str, int32 number);
void    func_uint_to_str            (char *str, uint32 number);
void    func_double_to_str          (char *str, double number, uint8 point_bit);

#endif /* DEVICE_CODE_ZF_DEVICE_IPS200_H_ */
