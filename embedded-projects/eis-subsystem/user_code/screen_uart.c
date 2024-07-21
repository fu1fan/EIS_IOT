/*
 * screen_uart.c
 *
 *  Created on: 2024年7月2日
 *      Author: Whisp
 */


#include "screen_uart.h"

#include "zf_common_fifo.h"

static  uint8           screen_uart_buffer[SCREEN_UART_BUFFER_SIZE];

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     调试串口模块 串口中断回调函数
// 参数说明     void
// 返回参数     void
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
static void screen_uart_callback (uart_driver_t *udp)
{
    udp->rx_buf = (uint8_t *)screen_uart_buffer;
    udp->rx_len = SCREEN_UART_BUFFER_SIZE;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     调试串口发送字符串
// 参数说明     *string         字符串指针
// 返回参数     void            无返回值
// 使用示例
// 备注信息     一般来讲用 printf 就行了
//-------------------------------------------------------------------------------------------------------------------
void screen_uart_write_string (const char *string)
{
    int     len_temp    = strlen(string);
    int     time_coutn  = SCREEN_UART_TIMEOUT_COUNT;
    while(len_temp)
    {
        SCREEN_UART_INDEX->uart->TDR = *string ++;
        while(!(SCREEN_UART_INDEX->uart->ISR & UART_ISR_TC) && time_coutn --);
        if(time_coutn > 0)
        {
            time_coutn = SCREEN_UART_TIMEOUT_COUNT;
        }
        else
        {
            break;
        }
        SCREEN_UART_INDEX->uart->ICR |= UART_ICR_TCCF;
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
uint32 screen_uart_write_buffer (const uint8 *buffer, uint32 len)
{
    int     time_coutn  = SCREEN_UART_TIMEOUT_COUNT;
    while(len)
    {
        SCREEN_UART_INDEX->uart->TDR = *buffer ++;
        while(!(SCREEN_UART_INDEX->uart->ISR & UART_ISR_TC) && time_coutn --);
        if(time_coutn > 0)
        {
            time_coutn = SCREEN_UART_TIMEOUT_COUNT;
        }
        else
        {
            break;
        }
        SCREEN_UART_INDEX->uart->ICR |= UART_ICR_TCCF;
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
uint32 screen_uart_read_buffer (uint8 *buff, uint32 len)
{
    uint32 data_len = ((SCREEN_UART_BUFFER_SIZE - SCREEN_UART_INDEX->rx_len) > len) ? (len) : (SCREEN_UART_BUFFER_SIZE - SCREEN_UART_INDEX->rx_len);
    memcpy(buff, screen_uart_buffer, data_len);
    if((SCREEN_UART_BUFFER_SIZE - SCREEN_UART_INDEX->rx_len) > len)
    {
        for(uint8 i = 0; ((SCREEN_UART_BUFFER_SIZE - SCREEN_UART_INDEX->rx_len) - len) > i; i ++)
        {
            screen_uart_buffer[i] = screen_uart_buffer[i + data_len];
        }
        SCREEN_UART_INDEX->rx_buf = (uint8_t *)&screen_uart_buffer[data_len];
        SCREEN_UART_INDEX->rx_len = SCREEN_UART_BUFFER_SIZE - data_len;
        SCREEN_UART_INDEX->uart->CR1 |= UART_CR1_RXNEIE_RXFNEIE;
    }
    else
    {
        SCREEN_UART_INDEX->rx_buf = (uint8_t *)screen_uart_buffer;
        SCREEN_UART_INDEX->rx_len = SCREEN_UART_BUFFER_SIZE;
        SCREEN_UART_INDEX->uart->CR1 |= UART_CR1_RXNEIE_RXFNEIE;
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
void screen_uart_init (void)
{
    uart_init(SCREEN_UART_INDEX);                                                // 初始化对应 UART 实例参数配置

    uart_set_sbrx_buf(SCREEN_UART_INDEX, (uint8_t *)screen_uart_buffer, SCREEN_UART_BUFFER_SIZE);
    uart_set_rx_drv_mode(SCREEN_UART_INDEX, UART_RX_DRV_MODE_INT_SYNC);          // 中断同步接收
    uart_set_tx_drv_mode(SCREEN_UART_INDEX, UART_TX_DRV_MODE_INT_SYNC);          // 发送同步中断
    uart_set_baud(SCREEN_UART_INDEX, SCREEN_UART_BAUDRATE);                       // 波特率设置
    uart_set_presc(SCREEN_UART_INDEX, UART_PRESCALER_DIV1);                      // 这里是分频 应该是时钟
    uart_set_parity(SCREEN_UART_INDEX, UART_PARITY_NONE);                        // 校验设置 无校验
    uart_set_over(SCREEN_UART_INDEX, UART_OVERSAMPLING_16);                      // 采样设置
    uart_set_sbit(SCREEN_UART_INDEX, UART_STOPBIT_1);                            // 停止位 1 位

    uart_set_prio(SCREEN_UART_INDEX, IRQ_PRIORITY_1);                            // 设置中断的优先级
    uart_set_cb(SCREEN_UART_INDEX, UART_CB_OPS_RX, screen_uart_callback);

    uart_start(SCREEN_UART_INDEX);                                               // 启动 UART

    SCREEN_UART_INDEX->rx_buf = (uint8_t *)screen_uart_buffer;
    SCREEN_UART_INDEX->rx_len = SCREEN_UART_BUFFER_SIZE;
    SCREEN_UART_INDEX->uart->CR1 |= UART_CR1_RXNEIE_RXFNEIE;
    SCREEN_UART_INDEX->uart->CR1 &= ~UART_CR1_TCIE;
}

void screen_uart_printf(const char *format, ...){
	char buffer[SCREEN_UART_BUFFER_SIZE];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, SCREEN_UART_BUFFER_SIZE, format, args);
	va_end(args);
	screen_uart_write_string(buffer);
}
