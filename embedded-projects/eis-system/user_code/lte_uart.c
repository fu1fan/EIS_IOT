/*
 * lte_uart.c
 *
 *  Created on: 2024年7月2日
 *      Author: Whisp
 */


#include "lte_uart.h"

#include "zf_common_fifo.h"

static  uint8           lte_uart_buffer[LTE_UART_BUFFER_SIZE];

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     调试串口模块 串口中断回调函数
// 参数说明     void
// 返回参数     void
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
static void lte_uart_callback (uart_driver_t *udp)
{
    udp->rx_buf = (uint8_t *)lte_uart_buffer;
    udp->rx_len = LTE_UART_BUFFER_SIZE;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     调试串口发送字符串
// 参数说明     *string         字符串指针
// 返回参数     void            无返回值
// 使用示例
// 备注信息     一般来讲用 printf 就行了
//-------------------------------------------------------------------------------------------------------------------
void lte_uart_write_string (const char *string)
{
    int     len_temp    = strlen(string);
    int     time_coutn  = LTE_UART_TIMEOUT_COUNT;
    while(len_temp)
    {
        LTE_UART_INDEX->uart->TDR = *string ++;
        while(!(LTE_UART_INDEX->uart->ISR & UART_ISR_TC) && time_coutn --);
        if(time_coutn > 0)
        {
            time_coutn = LTE_UART_TIMEOUT_COUNT;
        }
        else
        {
            break;
        }
        LTE_UART_INDEX->uart->ICR |= UART_ICR_TCCF;
        len_temp --;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     调试串口发送数据
// 参数说明     *buffer         数据缓冲地址
// 参数说明     len             数据长度
// 返回参数     void            无返回值
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint32 lte_uart_write_buffer (const uint8 *buffer, uint32 len)
{
    int     time_coutn  = LTE_UART_TIMEOUT_COUNT;
    while(len)
    {
        LTE_UART_INDEX->uart->TDR = *buffer ++;
        while(!(LTE_UART_INDEX->uart->ISR & UART_ISR_TC) && time_coutn --);
        if(time_coutn > 0)
        {
            time_coutn = LTE_UART_TIMEOUT_COUNT;
        }
        else
        {
            break;
        }
        LTE_UART_INDEX->uart->ICR |= UART_ICR_TCCF;
        len --;
    }
    return len;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     调试串口 读取缓冲
// 参数说明     *buff           接收缓冲区
// 参数说明     len             读取数据长度
// 返回参数     uint32          实际读取数据长度
// 使用示例     debug_uart_read_buffer(buff, 32);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint32 lte_uart_read_buffer (uint8 *buff, uint32 len)
{
    uint32 data_len = ((LTE_UART_BUFFER_SIZE - LTE_UART_INDEX->rx_len) > len) ? (len) : (LTE_UART_BUFFER_SIZE - LTE_UART_INDEX->rx_len);
    memcpy(buff, lte_uart_buffer, data_len);
    if((LTE_UART_BUFFER_SIZE - LTE_UART_INDEX->rx_len) > len)
    {
        for(uint8 i = 0; ((LTE_UART_BUFFER_SIZE - LTE_UART_INDEX->rx_len) - len) > i; i ++)
        {
            lte_uart_buffer[i] = lte_uart_buffer[i + data_len];
        }
        LTE_UART_INDEX->rx_buf = (uint8_t *)&lte_uart_buffer[data_len];
        LTE_UART_INDEX->rx_len = LTE_UART_BUFFER_SIZE - data_len;
        LTE_UART_INDEX->uart->CR1 |= UART_CR1_RXNEIE_RXFNEIE;
    }
    else
    {
        LTE_UART_INDEX->rx_buf = (uint8_t *)lte_uart_buffer;
        LTE_UART_INDEX->rx_len = LTE_UART_BUFFER_SIZE;
        LTE_UART_INDEX->uart->CR1 |= UART_CR1_RXNEIE_RXFNEIE;
    }
    return data_len;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     调试串口初始化
// 参数说明     void            无参数
// 返回参数     void            无返回值
// 使用示例
// 备注信息     一般来讲用 printf 就行了
//-------------------------------------------------------------------------------------------------------------------
void lte_uart_init (void)
{
    uart_init(LTE_UART_INDEX);                                                // 初始化对应 UART 实例参数配置

    uart_set_sbrx_buf(LTE_UART_INDEX, (uint8_t *)lte_uart_buffer, LTE_UART_BUFFER_SIZE);
    uart_set_rx_drv_mode(LTE_UART_INDEX, UART_RX_DRV_MODE_INT_SYNC);          // 中断同步接收
    uart_set_tx_drv_mode(LTE_UART_INDEX, UART_TX_DRV_MODE_INT_SYNC);          // 发送同步中断
    uart_set_baud(LTE_UART_INDEX, LTE_UART_BAUDRATE);                       // 波特率设置
    uart_set_presc(LTE_UART_INDEX, UART_PRESCALER_DIV1);                      // 这里是分频 应该是时钟
    uart_set_parity(LTE_UART_INDEX, UART_PARITY_NONE);                        // 校验设置 无校验
    uart_set_over(LTE_UART_INDEX, UART_OVERSAMPLING_16);                      // 采样设置
    uart_set_sbit(LTE_UART_INDEX, UART_STOPBIT_1);                            // 停止位 1 位

    uart_set_prio(LTE_UART_INDEX, IRQ_PRIORITY_1);                            // 设置中断的优先级
    uart_set_cb(LTE_UART_INDEX, UART_CB_OPS_RX, lte_uart_callback);

    uart_start(LTE_UART_INDEX);                                               // 启动 UART

    LTE_UART_INDEX->rx_buf = (uint8_t *)lte_uart_buffer;
    LTE_UART_INDEX->rx_len = LTE_UART_BUFFER_SIZE;
    LTE_UART_INDEX->uart->CR1 |= UART_CR1_RXNEIE_RXFNEIE;
    LTE_UART_INDEX->uart->CR1 &= ~UART_CR1_TCIE;
}
