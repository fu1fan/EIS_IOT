/*
 * zf_device_ips200.c
 *
 *  Created on: 2024年2月26日
 *      Author: SEEKFREE_Teternal
 */

#include "zf_device_ips200.h"

#define     func_abs(x)         ((x) >= 0 ? (x): -(x))

static uint16                   ips200_pencolor         = IPS200_DEFAULT_PENCOLOR;
static uint16                   ips200_bgcolor          = IPS200_DEFAULT_BGCOLOR;

static ips200_dir_enum          ips200_display_dir      = IPS200_DEFAULT_DISPLAY_DIR;
static ips200_font_size_enum    ips200_display_font     = IPS200_DEFAULT_DISPLAY_FONT;

static uint16                   ips200_x_max            = 240;
static uint16                   ips200_y_max            = 320;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 写命令
// 参数说明     command         命令
// 返回参数     void
// 使用示例     ips200_write_command(0x2a);
// 备注信息     内部调用 用户无需关心
//-------------------------------------------------------------------------------------------------------------------
static void ips200_write_command (uint8_t command)
{
    IPS200_DC(0);
    spi_write_8bit_array(LCD_SPI_INDEX, &command, 1);
    IPS200_DC(1);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 向液晶屏写 8bit 数据
// 参数说明     dat             数据
// 返回参数     void
// 使用示例     ips200_write_8bit_data(0x0C);
// 备注信息     内部调用 用户无需关心
//-------------------------------------------------------------------------------------------------------------------
static void ips200_write_8bit_data (uint8 dat)
{
    spi_write_8bit_array(LCD_SPI_INDEX, &dat, 1);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 向液晶屏写 8bit 数据
// 参数说明     dat             数据
// 返回参数     void
// 使用示例     ips200_write_8bit_data(0x0C);
// 备注信息     内部调用 用户无需关心
//-------------------------------------------------------------------------------------------------------------------
static void ips200_write_8bit_data_array (uint8 *dat, uint32 len)
{
    spi_write_8bit_array(LCD_SPI_INDEX, dat, len);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 向液晶屏写 16bit 数据
// 参数说明     dat             数据
// 返回参数     void
// 使用示例     ips200_write_16bit_data(x1);
// 备注信息     内部调用 用户无需关心
//-------------------------------------------------------------------------------------------------------------------
static void ips200_write_16bit_data (uint16 dat)
{
    uint8 data[2] = {(dat >> 8) & 0xFF, dat & 0x00FF};
    spi_write_8bit_array(LCD_SPI_INDEX, (uint8_t *)data, 2);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 向液晶屏写 16bit 数据
// 参数说明     dat             数据
// 返回参数     void
// 使用示例     ips200_write_16bit_data(x1);
// 备注信息     内部调用 用户无需关心
//-------------------------------------------------------------------------------------------------------------------
static void ips200_write_16bit_data_array (uint16 *dat, uint32 len)
{
    // uint8 data_buffer[64];
    // while(len)
    // {
    //     if(32 <= len)
    //     {
    //         for(uint32 len_temp = 0; 32 > len_temp; len_temp ++)
    //         {
    //             data_buffer[len_temp * 2 + 0] = (*dat >> 8);
    //             data_buffer[len_temp * 2 + 1] = (*dat) & 0xFF;
    //             dat ++;
    //         }
    //         spi_write_8bit_array(LCD_SPI_INDEX, data_buffer, 64);
    //         len -= 32;
    //     }
    //     else
    //     {
    //         for(uint32 len_temp = 0; len > len_temp; len_temp ++)
    //         {
    //             data_buffer[len_temp * 2 + 0] = (*dat >> 8);
    //             data_buffer[len_temp * 2 + 1] = (*dat) & 0xFF;
    //             dat ++;
    //         }
    //         spi_write_8bit_array(LCD_SPI_INDEX, data_buffer, len * 2);
    //         len -= len;
    //     }
    // }
    spi_write_16bit_array(LCD_SPI_INDEX, dat, len);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     设置显示区域
// 参数说明     x1              起始x轴坐标
// 参数说明     y1              起始y轴坐标
// 参数说明     x2              结束x轴坐标
// 参数说明     y2              结束y轴坐标
// 返回参数     void
// 使用示例     ips200_set_region(0, 0, ips200_x_max - 1, ips200_y_max - 1);
// 备注信息     内部调用 用户无需关心
//-------------------------------------------------------------------------------------------------------------------
static void ips200_set_region (uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
    ips200_write_command(0x2a);
    ips200_write_16bit_data(x1);
    ips200_write_16bit_data(x2);
    
    ips200_write_command(0x2b);
    ips200_write_16bit_data(y1);
    ips200_write_16bit_data(y2);
    
    ips200_write_command(0x2c);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示DEBUG信息初始化
// 参数说明     void
// 返回参数     void
// 使用示例     ips200_debug_init();
// 备注信息     内部使用 用户无需关心
//-------------------------------------------------------------------------------------------------------------------
static void ips200_debug_init (void)
{
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 清屏函数
// 参数说明     void
// 返回参数     void
// 使用示例     ips200_clear();
// 备注信息     将屏幕清空成背景颜色
//-------------------------------------------------------------------------------------------------------------------
void ips200_clear (void)
{
    uint16 color_buffer[ips200_x_max];
    uint16 i = 0, j = 0;

    IPS200_CS(0);
    ips200_set_region(0, 0, ips200_x_max - 1, ips200_y_max - 1);
    for(i = 0; i < ips200_x_max; i ++)
    {
        color_buffer[i] = ips200_bgcolor;
    }
    for (j = 0; j < ips200_y_max; j ++)
    {
        ips200_write_16bit_data_array(color_buffer, ips200_x_max);
    }
    IPS200_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 屏幕填充函数
// 参数说明     color           颜色格式 RGB565 或者可以使用 zf_common_font.h 内 rgb565_color_enum 枚举值或者自行写入
// 返回参数     void
// 使用示例     ips200_full(RGB565_BLACK);
// 备注信息     将屏幕填充成指定颜色
//-------------------------------------------------------------------------------------------------------------------
void ips200_full (const uint16 color)
{
    uint16 color_buffer[ips200_x_max];
    uint16 i = 0, j = 0;

    IPS200_CS(0);
    ips200_set_region(0, 0, ips200_x_max - 1, ips200_y_max - 1);
    for(i = 0; i < ips200_x_max; i ++)
    {
        color_buffer[i] = color;
    }
    for (j = 0; j < ips200_y_max; j ++)
    {
        ips200_write_16bit_data_array(color_buffer, ips200_x_max);
    }
    IPS200_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     设置显示方向
// 参数说明     dir             显示方向  参照 zf_device_ips200.h 内 ips200_dir_enum 枚举体定义
// 返回参数     void
// 使用示例     ips200_set_dir(IPS200_PORTAIT);
// 备注信息     这个函数只有在初始化屏幕之前调用才生效
//-------------------------------------------------------------------------------------------------------------------
void ips200_set_dir (ips200_dir_enum dir)
{
    ips200_display_dir = dir;
    switch(ips200_display_dir)
    {
        case IPS200_PORTAIT:
        case IPS200_PORTAIT_180:
        {
            ips200_x_max = 240;
            ips200_y_max = 320;
        }break;
        case IPS200_CROSSWISE:
        case IPS200_CROSSWISE_180:
        {
            ips200_x_max = 320;
            ips200_y_max = 240;
        }break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     设置显示字体
// 参数说明     dir             显示方向  参照 zf_device_ips200.h 内 ips200_font_size_enum 枚举体定义
// 返回参数     void
// 使用示例     ips200_set_font(IPS200_8x16_FONT);
// 备注信息     字体可以随时自由设置 设置后生效 后续显示就是新的字体大小
//-------------------------------------------------------------------------------------------------------------------
void ips200_set_font (ips200_font_size_enum font)
{
    ips200_display_font = font;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     设置显示颜色
// 参数说明     pen             颜色格式 RGB565 或者可以使用 zf_common_font.h 内 rgb565_color_enum 枚举值或者自行写入
// 参数说明     bgcolor         颜色格式 RGB565 或者可以使用 zf_common_font.h 内 rgb565_color_enum 枚举值或者自行写入
// 返回参数     void
// 使用示例     ips200_set_color(RGB565_RED, RGB565_GRAY);
// 备注信息     字体颜色和背景颜色也可以随时自由设置 设置后生效
//-------------------------------------------------------------------------------------------------------------------
void ips200_set_color (const uint16 pen, const uint16 bgcolor)
{
    ips200_pencolor = pen;
    ips200_bgcolor = bgcolor;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 画点
// 参数说明     x               坐标x方向的起点 [0, ips200_x_max-1]
// 参数说明     y               坐标y方向的起点 [0, ips200_y_max-1]
// 参数说明     color           颜色格式 RGB565 或者可以使用 zf_common_font.h 内 rgb565_color_enum 枚举值或者自行写入
// 返回参数     void
// 使用示例     ips200_draw_point(0, 0, RGB565_RED);            //坐标0,0画一个红色的点
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void ips200_draw_point (uint16 x, uint16 y, const uint16 color)
{
    IPS200_CS(0);
    ips200_set_region(x, y, x, y);
    ips200_write_16bit_data(color);
    IPS200_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 画线
// 参数说明     x_start         坐标x方向的起点 [0, ips200_x_max-1]
// 参数说明     y_start         坐标y方向的起点 [0, ips200_y_max-1]
// 参数说明     x_end           坐标x方向的终点 [0, ips200_x_max-1]
// 参数说明     y_end           坐标y方向的终点 [0, ips200_y_max-1]
// 参数说明     color           颜色格式 RGB565 或者可以使用 zf_common_font.h 内 rgb565_color_enum 枚举值或者自行写入
// 返回参数     void
// 使用示例     ips200_draw_line(0, 0, 10, 10, RGB565_RED);     // 坐标 0,0 到 10,10 画一条红色的线
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void ips200_draw_line (uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color)
{
    int16 x_dir = (x_start < x_end ? 1 : -1);
    int16 y_dir = (y_start < y_end ? 1 : -1);
    float temp_rate = 0;
    float temp_b = 0;

    do
    {
        if(x_start != x_end)
        {
            temp_rate = (float)(y_start - y_end) / (float)(x_start - x_end);
            temp_b = (float)y_start - (float)x_start * temp_rate;
        }
        else
        {
            while(y_start != y_end)
            {
                ips200_draw_point(x_start, y_start, color);
                y_start += y_dir;
            }
            ips200_draw_point(x_start, y_start, color);
            break;
        }
        if(func_abs(y_start - y_end) > func_abs(x_start - x_end))
        {
            while(y_start != y_end)
            {
                ips200_draw_point(x_start, y_start, color);
                y_start += y_dir;
                x_start = (int16)(((float)y_start - temp_b) / temp_rate);
            }
            ips200_draw_point(x_start, y_start, color);
        }
        else
        {
            while(x_start != x_end)
            {
                ips200_draw_point(x_start, y_start, color);
                x_start += x_dir;
                y_start = (int16)((float)x_start * temp_rate + temp_b);
            }
            ips200_draw_point(x_start, y_start, color);
        }
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示字符
// 参数说明     x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// 参数说明     dat             需要显示的字符
// 返回参数     void
// 使用示例     ips200_show_char(0, 0, 'x');                     // 坐标0,0写一个字符x
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_char (uint16 x, uint16 y, const char dat)
{
    uint8 i = 0, j = 0;

    IPS200_CS(0);
    switch(ips200_display_font)
    {
        case IPS200_6X8_FONT:
        {
            uint16 display_buffer[6*8];
            ips200_set_region(x, y, x + 5, y + 7);
            for(i = 0; 6 > i; i ++)
            {
                // 减 32 因为是取模是从空格开始取得 空格在 ascii 中序号是 32
                uint8 temp_top = ascii_font_6x8[dat - 32][i];
                for(j = 0; 8 > j; j ++)
                {
                    if(temp_top & 0x01)
                    {
                        display_buffer[i + j * 6] = (ips200_pencolor);
                    }
                    else
                    {
                        display_buffer[i + j * 6] = (ips200_bgcolor);
                    }
                    temp_top >>= 1;
                }
            }
            ips200_write_16bit_data_array(display_buffer, 6*8);
        }break;
        case IPS200_8X16_FONT:
        {
            uint16 display_buffer[8*16];
            ips200_set_region(x, y, x + 7, y + 15);
            for(i = 0; 8 > i; i ++)
            {
                uint8 temp_top = ascii_font_8x16[dat - 32][i];
                uint8 temp_bottom = ascii_font_8x16[dat - 32][i + 8];
                for(j = 0; 8 > j; j ++)
                {
                    if(temp_top & 0x01)
                    {
                        display_buffer[i + j * 8] = (ips200_pencolor);
                    }
                    else
                    {
                        display_buffer[i + j * 8] = (ips200_bgcolor);
                    }
                    temp_top >>= 1;
                }
                for(j = 0; 8 > j; j ++)
                {
                    if(temp_bottom & 0x01)
                    {
                        display_buffer[i + j * 8 + 4 * 16] = (ips200_pencolor);
                    }
                    else
                    {
                        display_buffer[i + j * 8 + 4 * 16] = (ips200_bgcolor);
                    }
                    temp_bottom >>= 1;
                }
            }
            ips200_write_16bit_data_array(display_buffer, 8 * 16);
        }break;
        case IPS200_16X16_FONT:
        {
            // 暂不支持
        }break;
    }
    IPS200_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示字符串
// 参数说明     x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// 参数说明     dat             需要显示的字符串
// 返回参数     void
// 使用示例     ips200_show_string(0, 0, "seekfree");
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_string (uint16 x, uint16 y, const char dat[])
{
    uint16 j = 0;
    while('\0' != dat[j])
    {
        switch(ips200_display_font)
        {
            case IPS200_6X8_FONT:   ips200_show_char(x + 6 * j, y, dat[j]); break;
            case IPS200_8X16_FONT:  ips200_show_char(x + 8 * j, y, dat[j]); break;
            case IPS200_16X16_FONT: break;                                      // 暂不支持
        }
        j ++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示32位有符号 (去除整数部分无效的0)
// 参数说明     x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// 参数说明     dat             需要显示的变量 数据类型 int32
// 参数说明     num             需要显示的位数 最高10位  不包含正负号
// 返回参数     void
// 使用示例     ips200_show_int(0, 0, x, 3);                    // x 可以为 int32 int16 int8 类型
// 备注信息     负数会显示一个 ‘-’号
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_int (uint16 x, uint16 y, const int32 dat, uint8 num)
{
    int32 dat_temp = dat;
    int32 offset = 1;
    char data_buffer[12];

    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num+1);

    // 用来计算余数显示 123 显示 2 位则应该显示 23
    if(10 > num)
    {
        for(; 0 < num; num --)
        {
            offset *= 10;
        }
        dat_temp %= offset;
    }
    func_int_to_str(data_buffer, dat_temp);
    ips200_show_string(x, y, (const char *)&data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示32位无符号 (去除整数部分无效的0)
// 参数说明     x               坐标x方向的起点 参数范围 [0, ips114_x_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, ips114_y_max-1]
// 参数说明     dat             需要显示的变量 数据类型 uint32
// 参数说明     num             需要显示的位数 最高10位  不包含正负号
// 返回参数     void
// 使用示例     ips200_show_uint(0, 0, x, 3);                   // x 可以为 uint32 uint16 uint8 类型
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_uint (uint16 x, uint16 y, const uint32 dat, uint8 num)
{
    uint32 dat_temp = dat;
    int32 offset = 1;
    char data_buffer[12];
    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num);

    // 用来计算余数显示 123 显示 2 位则应该显示 23
    if(10 > num)
    {
        for(; 0 < num; num --)
        {
            offset *= 10;
        }
        dat_temp %= offset;
    }
    func_uint_to_str(data_buffer, dat_temp);
    ips200_show_string(x, y, (const char *)&data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示浮点数(去除整数部分无效的0)
// 参数说明     x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// 参数说明     dat             需要显示的变量 数据类型 double
// 参数说明     num             整数位显示长度   最高8位  
// 参数说明     pointnum        小数位显示长度   最高6位
// 返回参数     void
// 使用示例     ips200_show_float(0, 0, x, 2, 3);               // 显示浮点数   整数显示2位   小数显示三位
// 备注信息     特别注意当发现小数部分显示的值与你写入的值不一样的时候，
//              可能是由于浮点数精度丢失问题导致的，这并不是显示函数的问题，
//              有关问题的详情，请自行百度学习   浮点数精度丢失问题。
//              负数会显示一个 ‘-’号
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_float (uint16 x, uint16 y, const double dat, uint8 num, uint8 pointnum)
{
    double dat_temp = dat;
    double offset = 1.0;
    char data_buffer[17];
    memset(data_buffer, 0, 17);
    memset(data_buffer, ' ', num+pointnum+2);

    // 用来计算余数显示 123 显示 2 位则应该显示 23
    for(; 0 < num; num --)
    {
        offset *= 10;
    }
    dat_temp = dat_temp - ((int)dat_temp / (int)offset) * offset;
    func_double_to_str(data_buffer, dat_temp, pointnum);
    ips200_show_string(x, y, data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示二值图像 数据每八个点组成一个字节数据
// 参数说明     x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// 参数说明     *image          图像数组指针
// 参数说明     width           图像实际宽度
// 参数说明     height          图像实际高度
// 参数说明     dis_width       图像显示宽度 参数范围 [0, ips200_x_max]
// 参数说明     dis_height      图像显示高度 参数范围 [0, ips200_y_max]
// 返回参数     void
// 使用示例     ips200_show_binary_image(0, 0, ov7725_image_binary[0], OV7725_W, OV7725_H, OV7725_W, OV7725_H);
// 备注信息     用于显示小钻风的未解压的压缩二值化图像
//              这个函数不可以用来直接显示总钻风的未压缩的二值化图像
//              这个函数不可以用来直接显示总钻风的未压缩的二值化图像
//              这个函数不可以用来直接显示总钻风的未压缩的二值化图像
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_binary_image (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
{
    uint32 i = 0, j = 0;
    uint8 temp = 0;
    uint32 width_index = 0;
    uint16 data_buffer[dis_width];
    const uint8 *image_temp;

    IPS200_CS(0);
    ips200_set_region(x, y, x + dis_width - 1, y + dis_height - 1);             // 设置显示区域

    for(j = 0; j < dis_height; j ++)
    {
        image_temp = image + j * height / dis_height * width / 8;               // 直接对 image 操作会 Hardfault 暂时不知道为什么
        for(i = 0; i < dis_width; i ++)
        {
            width_index = i * width / dis_width;
            temp = *(image_temp + width_index / 8);                             // 读取像素点
            if(0x80 & (temp << (width_index % 8)))
            {
                data_buffer[i] = (RGB565_WHITE);
            }
            else
            {
                data_buffer[i] = (RGB565_BLACK);
            }
        }
        ips200_write_16bit_data_array(data_buffer, dis_width);
    }
    IPS200_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示 8bit 灰度图像 带二值化阈值
// 参数说明     x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// 参数说明     *image          图像数组指针
// 参数说明     width           图像实际宽度
// 参数说明     height          图像实际高度
// 参数说明     dis_width       图像显示宽度 参数范围 [0, ips200_x_max]
// 参数说明     dis_height      图像显示高度 参数范围 [0, ips200_y_max]
// 参数说明     threshold       二值化显示阈值 0-不开启二值化
// 返回参数     void
// 使用示例     ips200_show_gray_image(0, 0, mt9v03x_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
// 备注信息     用于显示总钻风的图像
//              如果要显示二值化图像 直接修改最后一个参数为需要的二值化阈值即可
//              如果要显示二值化图像 直接修改最后一个参数为需要的二值化阈值即可
//              如果要显示二值化图像 直接修改最后一个参数为需要的二值化阈值即可
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_gray_image (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 threshold)
{
    uint32 i = 0, j = 0;
    uint16 color = 0,temp = 0;
    uint16 data_buffer[dis_width];
    const uint8 *image_temp;

    IPS200_CS(0);
    ips200_set_region(x, y, x + dis_width - 1, y + dis_height - 1);             // 设置显示区域

    for(j = 0; j < dis_height; j ++)
    {
        image_temp = image + j * height / dis_height * width;                   // 直接对 image 操作会 Hardfault 暂时不知道为什么
        for(i = 0; i < dis_width; i ++)
        {
            temp = *(image_temp + i * width / dis_width);                       // 读取像素点
            if(threshold == 0)
            {
                color = (0x001f & ((temp) >> 3)) << 11;
                color = color | (((0x003f) & ((temp) >> 2)) << 5);
                color = color | (0x001f & ((temp) >> 3));
                data_buffer[i] = (color >> 8) | (color << 8);
            }
            else if(temp < threshold)
            {
                data_buffer[i] = (RGB565_WHITE);
            }
            else
            {
                data_buffer[i] = (RGB565_BLACK);
            }
        }
        ips200_write_16bit_data_array(data_buffer, dis_width);
    }
    IPS200_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示 RGB565 彩色图像
// 参数说明     x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// 参数说明     *image          图像数组指针
// 参数说明     width           图像实际宽度
// 参数说明     height          图像实际高度
// 参数说明     dis_width       图像显示宽度 参数范围 [0, ips200_x_max]
// 参数说明     dis_height      图像显示高度 参数范围 [0, ips200_y_max]
// 参数说明     color_mode      色彩模式 0-低位在前 1-高位在前
// 返回参数     void
// 使用示例     ips200_show_rgb565_image(0, 0, scc8660_image[0], SCC8660_W, SCC8660_H, SCC8660_W, SCC8660_H, 1);
// 备注信息     用于显示凌瞳的 RGB565 的图像
//              如果要显示低位在前的其他 RGB565 图像 修改最后一个参数即可
//              如果要显示低位在前的其他 RGB565 图像 修改最后一个参数即可
//              如果要显示低位在前的其他 RGB565 图像 修改最后一个参数即可
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_rgb565_image (uint16 x, uint16 y, const uint16 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 color_mode)
{
    uint32 i = 0, j = 0;
    uint16 data_buffer[dis_width];
    const uint16 *image_temp;

    IPS200_CS(0);
    ips200_set_region(x, y, x + dis_width - 1, y + dis_height - 1);             // 设置显示区域

    for(j = 0; j < dis_height; j ++)
    {
        image_temp = image + j * height / dis_height * width;                   // 直接对 image 操作会 Hardfault 暂时不知道为什么
        for(i = 0; i < dis_width; i ++)
        {
            data_buffer[i] = *(image_temp + i * width / dis_width);             // 读取像素点
        }
        if(color_mode)
        {
            ips200_write_8bit_data_array((uint8 *)data_buffer, dis_width * 2);
        }
        else
        {
            ips200_write_16bit_data_array(data_buffer, dis_width);
        }
    }
    IPS200_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IPS200 显示波形
// 参数说明     x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// 参数说明     *wave           波形数组指针
// 参数说明     width           波形实际宽度
// 参数说明     value_max       波形实际最大值
// 参数说明     dis_width       波形显示宽度 参数范围 [0, ips200_x_max]
// 参数说明     dis_value_max   波形显示最大值 参数范围 [0, ips200_y_max]
// 返回参数     void
// 使用示例     ips200_show_wave(0, 0, data, 128, 64, 64, 32);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_wave (uint16 x, uint16 y, const uint16 *wave, uint16 width, uint16 value_max, uint16 dis_width, uint16 dis_value_max)
{
    uint32 i = 0, j = 0;
    uint32 width_index = 0, value_max_index = 0;
    uint16 data_buffer[dis_width];

    IPS200_CS(0);
    ips200_set_region(x, y, x + dis_width - 1, y + dis_value_max - 1);          // 设置显示区域
    for(j = 0; j < dis_value_max; j ++)
    {
        for(i = 0; i < dis_width; i ++)
        {
            data_buffer[i] = ips200_bgcolor; 
        }
        ips200_write_16bit_data_array(data_buffer, dis_width);
    }
    IPS200_CS(1);

    for(i = 0; i < dis_width; i ++)
    {
        width_index = i * width / dis_width;
        value_max_index = *(wave + width_index) * (dis_value_max - 1) / value_max;
        ips200_draw_point(i + x, (dis_value_max - 1) - value_max_index + y, ips200_pencolor);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     汉字显示
// 参数说明     x               坐标x方向的起点 参数范围 [0, ips200_x_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, ips200_y_max-1]
// 参数说明     size            取模的时候设置的汉字字体大小 也就是一个汉字占用的点阵长宽为多少个点 取模的时候需要长宽是一样的
// 参数说明     *chinese_buffer 需要显示的汉字数组
// 参数说明     number          需要显示多少位
// 参数说明     color           颜色格式 RGB565 或者可以使用 zf_common_font.h 内 rgb565_color_enum 枚举值或者自行写入
// 返回参数     void
// 使用示例     ips200_show_chinese(0, 0, 16, chinese_test[0], 4, RGB565_RED);//显示font文件里面的 示例
// 备注信息     使用PCtoLCD2002软件取模           阴码、逐行式、顺向   16*16
//-------------------------------------------------------------------------------------------------------------------
void ips200_show_chinese (uint16 x, uint16 y, uint8 size, const uint8 *chinese_buffer, uint8 number, const uint16 color)
{
    int i = 0, j = 0, k = 0; 
    uint8 temp = 0, temp1 = 0, temp2 = 0;
    const uint8 *p_data = chinese_buffer;
    
    temp2 = size / 8;
    
    IPS200_CS(0);
    ips200_set_region(x, y, number * size - 1 + x, y + size - 1);
    
    for(i = 0; i < size; i ++)
    {
        temp1 = number;
        p_data = chinese_buffer + i * temp2;
        while(temp1 --)
        {
            for(k = 0; k < temp2; k ++)
            {
                for(j = 8; 0 < j; j --)
                {
                    temp = (*p_data >> (j - 1)) & 0x01;
                    if(temp)
                    {
                        ips200_write_16bit_data(color);
                    }
                    else
                    {
                        ips200_write_16bit_data(ips200_bgcolor);
                    }
                }
                p_data ++;
            }
            p_data = p_data - temp2 + temp2 * size;
        }   
    }
    IPS200_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     2寸 IPS液晶初始化
// 参数说明     type_select     两寸屏接口类型 IPS200_TYPE_SPI 为 SPI 接口串口两寸屏 IPS200_TYPE_PARALLEL8 为 8080 协议八位并口两寸屏
// 返回参数     void
// 使用示例     ips200_init(IPS200_TYPE_PARALLEL8);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void ips200_init (void)
{
    IPS200_CS(1);

    ips200_set_dir(ips200_display_dir);
    ips200_set_color(ips200_pencolor, ips200_bgcolor);
    
    spi_config_init(LCD_SPI_INDEX, SPI_MODE0, SPI_BAUD_DIV2);

    tim_init        (&DRV_TIM4);
    tim_set_presc   (&DRV_TIM4, TIM_DIVIDE_BY(tim_get_clock(&DRV_TIM4) / 5000000U));
    tim_set_ch_mode (&DRV_TIM4, 2U, TIM_MODE_PWM_ACTIVE_HIGH);
    tim_start       (&DRV_TIM4);

    tim_set_interval    (&DRV_TIM4, 100U);/* 5MHz/100 = 50kHz. */
    tim_set_comparator  (&DRV_TIM4, 2U, 50U * IPS200_BLK_DUTY / 100);
    tim_enable          (&DRV_TIM4);

//    IPS200_BL(1);
    IPS200_RST(0);  
    osal_delay_millisec(5);
    IPS200_RST(1);      
    osal_delay_millisec(120);

    IPS200_CS(0);
    ips200_write_command(0x11);
    osal_delay_millisec(120);

    ips200_write_command(0x36);
    switch(ips200_display_dir)
    {
        case IPS200_PORTAIT:        ips200_write_8bit_data(0x00);   break;
        case IPS200_PORTAIT_180:    ips200_write_8bit_data(0xC0);   break;
        case IPS200_CROSSWISE:      ips200_write_8bit_data(0x70);   break;
        case IPS200_CROSSWISE_180:  ips200_write_8bit_data(0xA0);   break;
    }

    ips200_write_command(0x3A);
    ips200_write_8bit_data(0x05);
    
    ips200_write_command(0xB2);
    ips200_write_8bit_data(0x0C);
    ips200_write_8bit_data(0x0C);
    ips200_write_8bit_data(0x00);
    ips200_write_8bit_data(0x33);
    ips200_write_8bit_data(0x33);

    ips200_write_command(0xB7);
    ips200_write_8bit_data(0x35);

    ips200_write_command(0xBB);
    ips200_write_8bit_data(0x29);                                               // 32 Vcom=1.35V

    ips200_write_command(0xC2);
    ips200_write_8bit_data(0x01);

    ips200_write_command(0xC3);
    ips200_write_8bit_data(0x19);                                               // GVDD=4.8V 

    ips200_write_command(0xC4);
    ips200_write_8bit_data(0x20);                                               // VDV, 0x20:0v

    ips200_write_command(0xC5);
    ips200_write_8bit_data(0x1A);                                               // VCOM Offset Set

    ips200_write_command(0xC6);
    ips200_write_8bit_data(0x01F);                                              // 0x0F:60Hz

    ips200_write_command(0xD0);
    ips200_write_8bit_data(0xA4);
    ips200_write_8bit_data(0xA1);
                
    ips200_write_command(0xE0);
    ips200_write_8bit_data(0xD0);
    ips200_write_8bit_data(0x08);
    ips200_write_8bit_data(0x0E);
    ips200_write_8bit_data(0x09);
    ips200_write_8bit_data(0x09);
    ips200_write_8bit_data(0x05);
    ips200_write_8bit_data(0x31);
    ips200_write_8bit_data(0x33);
    ips200_write_8bit_data(0x48);
    ips200_write_8bit_data(0x17);
    ips200_write_8bit_data(0x14);
    ips200_write_8bit_data(0x15);
    ips200_write_8bit_data(0x31);
    ips200_write_8bit_data(0x34);

    ips200_write_command(0xE1);  
    ips200_write_8bit_data(0xD0);
    ips200_write_8bit_data(0x08);
    ips200_write_8bit_data(0x0E);
    ips200_write_8bit_data(0x09);
    ips200_write_8bit_data(0x09);
    ips200_write_8bit_data(0x15);
    ips200_write_8bit_data(0x31);
    ips200_write_8bit_data(0x33);
    ips200_write_8bit_data(0x48);
    ips200_write_8bit_data(0x17);
    ips200_write_8bit_data(0x14);
    ips200_write_8bit_data(0x15);
    ips200_write_8bit_data(0x31);
    ips200_write_8bit_data(0x34);

    ips200_write_command(0x21);
    
    ips200_write_command(0x29);
    IPS200_CS(1);

    ips200_clear();                                                             // 初始化为白屏
    ips200_debug_init();
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     整形数字转字符串 数据范围是 [-32768,32767]
// 参数说明     *str            字符串指针
// 参数说明     number          传入的数据
// 返回参数     void
// 使用示例     func_int_to_str(data_buffer, -300);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void func_int_to_str (char *str, int32 number)
{
    uint8 data_temp[16];                                                        // 缓冲区
    uint8 bit = 0;                                                              // 数字位数
    int32 number_temp = 0;

    do
    {
        if(NULL == str)
        {
            break;
        }

        if(0 > number)                                                          // 负数
        {
            *str ++ = '-';
            number = -number;
        }
        else if(0 == number)                                                    // 或者这是个 0
        {
            *str = '0';
            break;
        }

        while(0 != number)                                                      // 循环直到数值归零
        {
            number_temp = number % 10;
            data_temp[bit ++] = func_abs(number_temp);                          // 倒序将数值提取出来
            number /= 10;                                                       // 削减被提取的个位数
        }
        while(0 != bit)                                                         // 提取的数字个数递减处理
        {
            *str ++ = (data_temp[bit - 1] + 0x30);                              // 将数字从倒序数组中倒序取出 变成正序放入字符串
            bit --;
        }
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     整形数字转字符串 数据范围是 [0,65535]
// 参数说明     *str            字符串指针
// 参数说明     number          传入的数据
// 返回参数     void
// 使用示例     func_uint_to_str(data_buffer, 300);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void func_uint_to_str (char *str, uint32 number)
{
    int8 data_temp[16];                                                         // 缓冲区
    uint8 bit = 0;                                                              // 数字位数

    do
    {
        if(NULL == str)
        {
            break;
        }

        if(0 == number)                                                         // 这是个 0
        {
            *str = '0';
            break;
        }

        while(0 != number)                                                      // 循环直到数值归零
        {
            data_temp[bit ++] = (number % 10);                                  // 倒序将数值提取出来
            number /= 10;                                                       // 削减被提取的个位数
        }
        while(0 != bit)                                                         // 提取的数字个数递减处理
        {
            *str ++ = (data_temp[bit - 1] + 0x30);                              // 将数字从倒序数组中倒序取出 变成正序放入字符串
            bit --;
        }
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     浮点数字转字符串
// 参数说明     *str            字符串指针
// 参数说明     number          传入的数据
// 参数说明     point_bit       小数点精度
// 返回参数     void
// 使用示例     func_double_to_str(data_buffer, 3.1415, 2);                     // 结果输出 data_buffer = "3.14"
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void func_double_to_str (char *str, double number, uint8 point_bit)
{
    int data_int = 0;                                                           // 整数部分
    int data_float = 0.0;                                                       // 小数部分
    int data_temp[12];                                                          // 整数字符缓冲
    int data_temp_point[9];                                                     // 小数字符缓冲
    uint8 bit = point_bit;                                                      // 转换精度位数

    do
    {
        if(NULL == str)
        {
            break;
        }

        // 提取整数部分
        data_int = (int)number;                                                 // 直接强制转换为 int
        if(0 > number)                                                          // 判断源数据是正数还是负数
        {
            *str ++ = '-';
        }
        else if(0.0 == number)                                                  // 如果是个 0
        {
            *str ++ = '0';
            *str ++ = '.';
            *str = '0';
            break;
        }

        // 提取小数部分
        number = number - data_int;                                             // 减去整数部分即可
        while(bit --)
        {
            number = number * 10;                                               // 将需要的小数位数提取到整数部分
        }
        data_float = (int)number;                                               // 获取这部分数值

        // 整数部分转为字符串
        bit = 0;
        do
        {
            data_temp[bit ++] = data_int % 10;                                  // 将整数部分倒序写入字符缓冲区
            data_int /= 10;
        }while(0 != data_int);
        while(0 != bit)
        {
            *str ++ = (func_abs(data_temp[bit - 1]) + 0x30);                    // 再倒序将倒序的数值写入字符串 得到正序数值
            bit --;
        }

        // 小数部分转为字符串
        if(0 != point_bit)
        {
            bit = 0;
            *str ++ = '.';
            if(0 == data_float)
            {
                *str = '0';
            }
            else
            {
                while(0 != point_bit)                                           // 判断有效位数
                {
                    data_temp_point[bit ++] = data_float % 10;                  // 倒序写入字符缓冲区
                    data_float /= 10;
                    point_bit --;                                                
                }
                while(0 != bit)
                {
                    *str ++ = (func_abs(data_temp_point[bit - 1]) + 0x30);      // 再倒序将倒序的数值写入字符串 得到正序数值
                    bit --;
                }
            }
        }
    }while(0);
}

const uint8 ascii_font_8x16[][16]=
{
    // <阴码> <列行式> <逆向> <8*16>
    // <纵向 8bit 为一个数据模型> <横向取模>

    // 字模数据储存格式为：{byte1, byte2,....byte16}

    // 字模数据与像素对应关系
    // byte 1      2      3      4      5      6      7      8
    // bit  [  0]  [  8]  [ 16]  [ 24]  [ 32]  [ 40]  [ 48]  [ 56]
    // bit  [  1]  [  9]  [ 17]  [ 25]  [ 33]  [ 41]  [ 49]  [ 57]
    // bit  [  2]  [ 10]  [ 18]  [ 26]  [ 34]  [ 42]  [ 50]  [ 58]
    // bit  [  3]  [ 11]  [ 19]  [ 37]  [ 35]  [ 43]  [ 51]  [ 59]
    // bit  [  4]  [ 12]  [ 20]  [ 38]  [ 36]  [ 44]  [ 52]  [ 60]
    // bit  [  5]  [ 13]  [ 21]  [ 39]  [ 37]  [ 45]  [ 53]  [ 61]
    // bit  [  6]  [ 14]  [ 22]  [ 30]  [ 38]  [ 46]  [ 54]  [ 62]
    // bit  [  7]  [ 15]  [ 23]  [ 31]  [ 39]  [ 47]  [ 55]  [ 63]

    // byte 9      10     11     12     13     14     15     16
    // bit  [ 64]  [ 72]  [ 80]  [ 88]  [ 96]  [104]  [112]  [120]
    // bit  [ 65]  [ 73]  [ 81]  [ 89]  [ 97]  [105]  [113]  [121]
    // bit  [ 66]  [ 74]  [ 82]  [ 90]  [ 98]  [106]  [114]  [122]
    // bit  [ 67]  [ 75]  [ 83]  [ 91]  [ 99]  [107]  [115]  [123]
    // bit  [ 68]  [ 76]  [ 84]  [ 92]  [100]  [108]  [116]  [124]
    // bit  [ 69]  [ 77]  [ 85]  [ 93]  [101]  [109]  [117]  [125]
    // bit  [ 70]  [ 78]  [ 86]  [ 94]  [102]  [110]  [118]  [126]
    // bit  [ 71]  [ 79]  [ 87]  [ 95]  [103]  [111]  [119]  [127]

    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  //   0
    {0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00},  // ! 1
    {0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // " 2
    {0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00},  // # 3
    {0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00,0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00},  // $ 4
    {0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00,0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00},  // % 5
    {0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10},  // & 6
    {0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // ' 7
    {0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00},  // ( 8
    {0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00},  // ) 9
    {0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00},  // * 10
    {0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00},  // + 11
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00},  // , 12
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01},  // - 13
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00},  // . 14
    {0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00},  // / 15
    {0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00},  // 0 16
    {0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},  // 1 17
    {0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00},  // 2 18
    {0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00},  // 3 19
    {0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00},  // 4 20
    {0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00},  // 5 21
    {0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00},  // 6 22
    {0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00},  // 7 23
    {0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00},  // 8 24
    {0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00},  // 9 25
    {0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00},  // : 26
    {0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00},  // ; 27
    {0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00},  // < 28
    {0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00},  // = 29
    {0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00},  // > 30
    {0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00},  // ? 31
    {0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00},  // @ 32
    {0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20},  // A 33
    {0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00},  // B 34
    {0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00},  // C 35
    {0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00},  // D 36
    {0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00},  // E 37
    {0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00},  // F 38
    {0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00},  // G 39
    {0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20},  // H 40
    {0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},  // I 41
    {0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00},  // J 42
    {0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00},  // K 43
    {0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00},  // L 44
    {0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00},  // M 45
    {0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00},  // N 46
    {0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00},  // O 47
    {0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00},  // P 48
    {0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00},  // Q 49
    {0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20},  // R 50
    {0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00},  // S 51
    {0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},  // T 52
    {0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00},  // U 53
    {0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00},  // V 54
    {0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00},  // W 55
    {0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20},  // X 56
    {0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},  // Y 57
    {0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00},  // Z 58
    {0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00},  // [ 59
    {0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00},  // \ 60
    {0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00},  // ] 61
    {0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // ^ 62
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80},  // _ 63
    {0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // ` 64
    {0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20},  // a 65
    {0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00},  // b 66
    {0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00},  // c 67
    {0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20},  // d 68
    {0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00},  // e 69
    {0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},  // f 70
    {0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00},  // g 71
    {0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20},  // h 72
    {0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},  // i 73
    {0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00},  // j 74
    {0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00},  // k 75
    {0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},  // l 76
    {0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F},  // m 77
    {0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20},  // n 78
    {0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00},  // o 79
    {0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00},  // p 80
    {0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80},  // q 81
    {0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00},  // r 82
    {0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00},  // s 83
    {0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00},  // t 84
    {0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20},  // u 85
    {0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00},  // v 86
    {0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00},  // w 87
    {0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00},  // x 88
    {0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00},  // y 89
    {0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00},  // z 90
    {0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40},  // { 91
    {0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00},  // | 92
    {0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00,0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00},  // } 93
    {0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}   // ~ 94
};

const uint8 ascii_font_6x8[][6] =
{
    // <阴码> <列行式> <逆向> <6*8>
    // <纵向 8bit 为一个数据模型> <横向取模>

    // 字模数据储存格式为：{byte1, byte2,....byte6}

    // 字模数据与像素对应关系
    // byte 1      2      3      4      5      6    
    // bit  [  0]  [  8]  [ 16]  [ 24]  [ 32]  [ 40]
    // bit  [  1]  [  9]  [ 17]  [ 25]  [ 33]  [ 41]
    // bit  [  2]  [ 10]  [ 18]  [ 26]  [ 34]  [ 42]
    // bit  [  3]  [ 11]  [ 19]  [ 37]  [ 35]  [ 43]
    // bit  [  4]  [ 12]  [ 20]  [ 38]  [ 36]  [ 44]
    // bit  [  5]  [ 13]  [ 21]  [ 39]  [ 37]  [ 45]
    // bit  [  6]  [ 14]  [ 22]  [ 30]  [ 38]  [ 46]
    // bit  [  7]  [ 15]  [ 23]  [ 31]  [ 39]  [ 47]


    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },     //    0
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },     // !  1
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },     // "  2
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },     // #  3
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },     // $  4
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },     // %  5
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },     // &  6
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },     // '  7
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },     // (  8
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },     // )  9
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },     // * 10
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },     // + 11
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },     // , 12
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },     // - 13
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },     // . 14
    { 0x40, 0x20, 0x10, 0x08, 0x04, 0x02 },     // / 15
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },     // 0 16
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },     // 1 17
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },     // 2 18
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },     // 3 19
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },     // 4 20
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },     // 5 21
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },     // 6 22
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },     // 7 23
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },     // 8 24
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },     // 9 25
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },     // : 26
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },     // ; 27
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },     // < 28
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },     // = 29
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },     // > 30
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },     // ? 31
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },     // @ 32
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },     // A 33
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },     // B 34
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },     // C 35
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },     // D 36
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },     // E 37
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },     // F 38
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },     // G 39
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },     // H 40
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },     // I 41
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },     // J 42
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },     // K 43
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },     // L 44
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },     // M 45
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },     // N 46
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },     // O 47
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },     // P 48
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },     // Q 49
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },     // R 50
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },     // S 51
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },     // T 52
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },     // U 53
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },     // V 54
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },     // W 55
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },     // X 56
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },     // Y 57
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },     // Z 58
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },     // [ 59
    { 0x02, 0x04, 0x08, 0x10, 0x20, 0x40 },     // \ 60 
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },     // ] 61
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },     // ^ 62
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },     // _ 63
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },     // ` 64
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },     // a 65
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },     // b 66
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },     // c 67
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },     // d 68
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },     // e 69
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },     // f 70
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },     // g 71
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },     // h 72
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },     // i 73
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },     // j 74
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },     // k 75
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },     // l 76
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },     // m 77
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },     // n 78
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },     // o 79
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },     // p 80
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },     // q 81
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },     // r 82
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },     // s 83
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },     // t 84
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },     // u 85
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },     // v 86
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },     // w 87
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },     // x 88
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },     // y 89
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },     // z 90
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }      // horiz lines
};
