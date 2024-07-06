/*
 * zf_device_spi_encoder.c
 *
 *  Created on: 2024年2月23日
 *      Author: SEEKFREE_Teternal
 */

#include "zf_device_spi_encoder.h"

float spi_encoder_last_angle[2]  = {0};                                         // 当前位置定义 static:静态声明，仅此文件可用此变量
float spi_encoder_now_angle[2]   = {0};                                         // 当前位置定义 static:静态声明，仅此文件可用此变量

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     绝对值编码器 获取位置的绝对值
// 参数说明     encoder_ch      选择的编码器通道
// 返回参数     float           当前角度绝对值
// 使用示例     spi_encoder_get_location();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
float spi_encoder_get_location (spi_encoder_channel_enum encoder_ch)
{
    uint8   get_data[3]     = {0x83, 0x00, 0x00};
    uint16  now_location    = 0;

    spi_encoder_last_angle[encoder_ch] = spi_encoder_now_angle[encoder_ch];                 // 更新数据前保存上一次的位置

    gpio_clear_pin((encoder_ch == SPI_ENCODER_MOTOR1 ? SPI_ENCODER_MOTOR1_CS : SPI_ENCODER_MOTOR2_CS));

    spi_transfer_8bit_array(SPI_ENCODER_INDEX, get_data, get_data, 3);

    gpio_set_pin((encoder_ch == SPI_ENCODER_MOTOR1 ? SPI_ENCODER_MOTOR1_CS : SPI_ENCODER_MOTOR2_CS));

    now_location = (((uint16)get_data[1] << 8) |  (uint16)get_data[2]) >> 2;

    spi_encoder_now_angle[encoder_ch] = (float)((uint32)now_location * 360) / 16384.0f;

    return spi_encoder_now_angle[encoder_ch];
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     绝对值编码器 获取相较上次位置的偏移值
// 参数说明     encoder_ch      选择的编码器通道
// 返回参数     float           偏移角度值
// 使用示例     spi_encoder_get_offset();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
float spi_encoder_get_offset (spi_encoder_channel_enum encoder_ch)
{
    float result_data = (spi_encoder_now_angle[encoder_ch] >= spi_encoder_last_angle[encoder_ch])
        ? (spi_encoder_now_angle[encoder_ch] - spi_encoder_last_angle[encoder_ch])
        : (spi_encoder_last_angle[encoder_ch] - spi_encoder_now_angle[encoder_ch]);
    if(180.0f < result_data)
    {
        result_data = (180.0f < spi_encoder_now_angle[encoder_ch])
            ? (spi_encoder_now_angle[encoder_ch] - 360.0f - spi_encoder_last_angle[encoder_ch])
            : (spi_encoder_now_angle[encoder_ch] + 360.0f - spi_encoder_last_angle[encoder_ch]);
    }
    else
    {
        result_data = (spi_encoder_now_angle[encoder_ch] - spi_encoder_last_angle[encoder_ch]);
    }
    return result_data;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     绝对值编码器 初始化
// 参数说明     void
// 返回参数     void
// 使用示例     spi_encoder_init();
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void spi_encoder_init (void)
{
    spi_config_init(SPI_ENCODER_INDEX, SPI_MODE3, SPI_BAUD_DIV4);
    gpio_set_pin(SPI_ENCODER_MOTOR1_CS);
    gpio_set_pin(SPI_ENCODER_MOTOR2_CS);
}
