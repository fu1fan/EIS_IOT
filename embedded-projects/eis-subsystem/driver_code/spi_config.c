/*
 * spi_config.c
 *
 *  Created on: 2024年2月26日
 *      Author: SEEKFREE_Teternal
 */

#include "spi_config.h"

uint8 collect = 0;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 读取数组
// 参数说明     *spi_index      SPI 实例指针
// 参数说明     *buffer         数据缓冲区
// 参数说明     len             预读取长度
// 返回参数     void
// 使用示例     
// 备注信息     其实就是转接了 SDK 的接口
//-------------------------------------------------------------------------------------------------------------------
void spi_read_8bit_array (spi_driver_t *spi_index, uint8 *buffer, uint32 len)
{
    uint32 cr2 = spi_index->spi->CR2;
    cr2 &= ~SPI_CR2_DS;
    cr2 |= (0x00000007 << SPI_CR2_DS_Pos);
    spi_index->spi->CR2 = cr2;

    while(len --)
    {
        *((uint8 *)&spi_index->spi->DR) = 0;                                    // 发送数据
        while(!(spi_index->spi->SR & SPI_SR_TXE));                              // 发送为空
        while(!(spi_index->spi->SR & SPI_SR_RXNE));                             // 接收到有效数据
        *buffer ++ = *((uint8 *)&spi_index->spi->DR);                           // 读取数据
    }
    while((spi_index->spi->SR & SPI_SR_BSY));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 发送数组
// 参数说明     *spi_index      SPI 实例指针
// 参数说明     *buffer         数据缓冲区
// 参数说明     len             预发送长度
// 返回参数     void
// 使用示例     
// 备注信息     其实就是转接了 SDK 的接口
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit_array (spi_driver_t *spi_index, uint8 *buffer, uint32 len)
{
    uint32 cr2 = spi_index->spi->CR2;
    cr2 &= ~SPI_CR2_DS;
    cr2 |= (0x00000007 << SPI_CR2_DS_Pos);
    spi_index->spi->CR2 = cr2;

    while(len --)
    {
        *((uint8 *)&spi_index->spi->DR) = *buffer ++;                           // 发送数据
        while(!(spi_index->spi->SR & SPI_SR_TXE));                              // 发送为空
        while(!(spi_index->spi->SR & SPI_SR_RXNE));                             // 接收到有效数据
        collect = *((uint8 *)&spi_index->spi->DR);                              // 读取数据
    }

    while((spi_index->spi->SR & SPI_SR_BSY));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 双向传输数组
// 参数说明     *spi_index      SPI 实例指针
// 参数说明     *tx_buffer      发送数据缓冲区
// 参数说明     *rx_buffer      读取数据缓冲区
// 参数说明     len             预传输长度
// 返回参数     void
// 使用示例     
// 备注信息     其实就是转接了 SDK 的接口
//-------------------------------------------------------------------------------------------------------------------
void spi_transfer_8bit_array (spi_driver_t *spi_index, uint8 *tx_buffer, uint8 *rx_buffer, uint32 len)
{
    uint32 cr2 = spi_index->spi->CR2;
    cr2 &= ~SPI_CR2_DS;
    cr2 |= (0x00000007 << SPI_CR2_DS_Pos);
    spi_index->spi->CR2 = cr2;

    while(len --)
    {
        *((uint8 *)&spi_index->spi->DR) = *tx_buffer ++;                        // 发送数据
        while(!(spi_index->spi->SR & SPI_SR_TXE));                              // 发送为空
        while(!(spi_index->spi->SR & SPI_SR_RXNE));                             // 接收到有效数据
        *rx_buffer ++ = *((uint8 *)&spi_index->spi->DR);                        // 读取数据
    }
    while((spi_index->spi->SR & SPI_SR_BSY));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 读取数组
// 参数说明     *spi_index      SPI 实例指针
// 参数说明     *buffer         数据缓冲区
// 参数说明     len             预读取长度
// 返回参数     void
// 使用示例     
// 备注信息     其实就是转接了 SDK 的接口
//-------------------------------------------------------------------------------------------------------------------
void spi_read_16bit_array (spi_driver_t *spi_index, uint16 *buffer, uint32 len)
{
    uint32 cr2 = spi_index->spi->CR2;
    cr2 &= ~SPI_CR2_DS;
    cr2 |= (0x0000000F << SPI_CR2_DS_Pos);
    spi_index->spi->CR2 = cr2;

    while(len --)
    {
        spi_index->spi->DR = 0;                                                 // 发送数据
        while(!(spi_index->spi->SR & SPI_SR_TXE));                              // 发送为空
        while(!(spi_index->spi->SR & SPI_SR_RXNE));                             // 接收到有效数据
        *buffer ++ = spi_index->spi->DR;                                        // 读取数据
    }
    while((spi_index->spi->SR & SPI_SR_BSY));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 发送数组
// 参数说明     *spi_index      SPI 实例指针
// 参数说明     *buffer         数据缓冲区
// 参数说明     len             预发送长度
// 返回参数     void
// 使用示例     
// 备注信息     其实就是转接了 SDK 的接口
//-------------------------------------------------------------------------------------------------------------------
void spi_write_16bit_array (spi_driver_t *spi_index, uint16 *buffer, uint32 len)
{
    uint32 cr2 = spi_index->spi->CR2;
    cr2 &= ~SPI_CR2_DS;
    cr2 |= (0x0000000F << SPI_CR2_DS_Pos);
    spi_index->spi->CR2 = cr2;

    while(len --)
    {
        spi_index->spi->DR = *buffer ++;                                        // 发送数据
        while(!(spi_index->spi->SR & SPI_SR_TXE));                              // 发送为空
        while(!(spi_index->spi->SR & SPI_SR_RXNE));                             // 接收到有效数据
        collect = spi_index->spi->DR;                                           // 读取数据
    }

    while((spi_index->spi->SR & SPI_SR_BSY));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 双向传输数组
// 参数说明     *spi_index      SPI 实例指针
// 参数说明     *tx_buffer      发送数据缓冲区
// 参数说明     *rx_buffer      读取数据缓冲区
// 参数说明     len             预传输长度
// 返回参数     void
// 使用示例     
// 备注信息     其实就是转接了 SDK 的接口
//-------------------------------------------------------------------------------------------------------------------
void spi_transfer_16bit_array (spi_driver_t *spi_index, uint16 *tx_buffer, uint16 *rx_buffer, uint32 len)
{
    uint32 cr2 = spi_index->spi->CR2;
    cr2 &= ~SPI_CR2_DS;
    cr2 |= (0x0000000F << SPI_CR2_DS_Pos);
    spi_index->spi->CR2 = cr2;

    while(len --)
    {
        spi_index->spi->DR = *tx_buffer ++;                                     // 发送数据
        while(!(spi_index->spi->SR & SPI_SR_TXE));                              // 发送为空
        while(!(spi_index->spi->SR & SPI_SR_RXNE));                             // 接收到有效数据
        *rx_buffer ++ = spi_index->spi->DR;                                     // 读取数据
    }
    while((spi_index->spi->SR & SPI_SR_BSY));
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 阻塞传输配置
// 参数说明     *spi_index      SPI 实例指针
// 参数说明     mode            SPI 模式
// 参数说明     baud_div        时钟分频
// 返回参数     void
// 使用示例     
// 备注信息     其实就是转接了 SDK 的接口
//-------------------------------------------------------------------------------------------------------------------
void spi_config_init (spi_driver_t *spi_index, spi_mode_enum mode, spi_baud_div_enum baud_div)
{
    spi_init        (spi_index);                                                // 初始化实例参数

    spi_set_mode    (spi_index, SPI_MODE_MASTER);                               // 主模式
    spi_set_baud    (spi_index, (spi_baud_t)baud_div);                          // 二分频速率 最快就这

    switch(mode)                                                                // SPI 模式选择 应该不需要注释吧
    {
        case SPI_MODE0:
        {
            spi_set_cpol    (spi_index, SPI_CPOL_0);
            spi_set_cpha    (spi_index, SPI_CPHA_0);
        }break;
        case SPI_MODE1:
        {
            spi_set_cpol    (spi_index, SPI_CPOL_0);
            spi_set_cpha    (spi_index, SPI_CPHA_1);
        }break;
        case SPI_MODE2:
        {
            spi_set_cpol    (spi_index, SPI_CPOL_1);
            spi_set_cpha    (spi_index, SPI_CPHA_0);
        }break;
        case SPI_MODE3:
        {
            spi_set_cpol    (spi_index, SPI_CPOL_1);
            spi_set_cpha    (spi_index, SPI_CPHA_1);
        }break;
    }

    spi_set_fformat (spi_index, SPI_FRAMEFORMAT_MSB);                           // 设置帧格式 MSB
    spi_set_fsize   (spi_index, SPI_FRAMESIZE_8);                               // 设置帧长度 8bit

    // spi_set_drv_mode(spi_index, SPI_DRV_MODE_INT_SYNC);
    // spi_set_prio    (spi_index, IRQ_PRIORITY_2);
    // spi_start       (spi_index);

    uint32 cr1 = 0U;
    uint32 cr2 = 0U;
    spi_dev_clock_enable(spi_index);
    cr1 |=  (((uint32)(spi_index->fformat)  << SPI_CR1_LSBFIRST_Pos) |
            ((uint32)(spi_index->baud)      << SPI_CR1_BR_Pos)       |
            ((uint32)(spi_index->mode)      << SPI_CR1_MSTR_Pos)     |
            ((uint32)(spi_index->cpol)      << SPI_CR1_CPOL_Pos)     |
            ((uint32)(spi_index->cpha)      << SPI_CR1_CPHA_Pos));
    cr2 |= ((uint32)(spi_index->fsize))     << SPI_CR2_DS_Pos;
    cr2 |= SPI_CR2_SSOE;
    cr1 |= SPI_CR1_SPE;

    spi_index->spi->CR2 = cr2 | SPI_CR2_FRXTH;
    spi_index->spi->CR1 = cr1;
    collect = 1;
}
