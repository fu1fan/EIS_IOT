/*
 * debug_uart.c
 *
 *  Created on: 2024年2月20日
 *      Author: SEEKFREE_Teternal
 */

#include "debug_uart.h"

#include "zf_common_fifo.h"

static  uint8           debug_uart_buffer[DEBUG_UART_BUFFER_SIZE];

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     调试串口模块 串口中断回调函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static void debug_uart_callback (uart_driver_t *udp)
{
    udp->rx_buf = (uint8_t *)debug_uart_buffer;
    udp->rx_len = DEBUG_UART_BUFFER_SIZE;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     调试串口发送字符串
// 参数说明     *string         字符串指针
// 返回参数     void            无返回值
// 使用示例     
// 备注信息     一般来讲用 printf 就行了
//-------------------------------------------------------------------------------------------------------------------
void debug_uart_write_string (const char *string)
{
    int     len_temp    = strlen(string);
    int     time_coutn  = DEBUG_UART_TIMEOUT_COUNT;
    while(len_temp)
    {
        DEBUG_UART_INDEX->uart->TDR = *string ++;
        while(!(DEBUG_UART_INDEX->uart->ISR & UART_ISR_TC) && time_coutn --);
        if(time_coutn > 0)
        {
            time_coutn = DEBUG_UART_TIMEOUT_COUNT;
        }
        else
        {
            break;
        }
        DEBUG_UART_INDEX->uart->ICR |= UART_ICR_TCCF;
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
uint32 debug_uart_write_buffer (const uint8 *buffer, uint32 len)
{
    int     time_coutn  = DEBUG_UART_TIMEOUT_COUNT;
    while(len)
    {
        DEBUG_UART_INDEX->uart->TDR = *buffer ++;
        while(!(DEBUG_UART_INDEX->uart->ISR & UART_ISR_TC) && time_coutn --);
        if(time_coutn > 0)
        {
            time_coutn = DEBUG_UART_TIMEOUT_COUNT;
        }
        else
        {
            break;
        }
        DEBUG_UART_INDEX->uart->ICR |= UART_ICR_TCCF;
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
uint32 debug_uart_read_buffer (uint8 *buff, uint32 len)
{
    uint32 data_len = ((DEBUG_UART_BUFFER_SIZE - DEBUG_UART_INDEX->rx_len) > len) ? (len) : (DEBUG_UART_BUFFER_SIZE - DEBUG_UART_INDEX->rx_len);
    memcpy(buff, debug_uart_buffer, data_len);
    if((DEBUG_UART_BUFFER_SIZE - DEBUG_UART_INDEX->rx_len) > len)
    {
        for(uint8 i = 0; ((DEBUG_UART_BUFFER_SIZE - DEBUG_UART_INDEX->rx_len) - len) > i; i ++)
        {
            debug_uart_buffer[i] = debug_uart_buffer[i + data_len];
        }
        DEBUG_UART_INDEX->rx_buf = (uint8_t *)&debug_uart_buffer[data_len];
        DEBUG_UART_INDEX->rx_len = DEBUG_UART_BUFFER_SIZE - data_len;
        DEBUG_UART_INDEX->uart->CR1 |= UART_CR1_RXNEIE_RXFNEIE;
    }
    else
    {
        DEBUG_UART_INDEX->rx_buf = (uint8_t *)debug_uart_buffer;
        DEBUG_UART_INDEX->rx_len = DEBUG_UART_BUFFER_SIZE;
        DEBUG_UART_INDEX->uart->CR1 |= UART_CR1_RXNEIE_RXFNEIE;
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
void debug_uart_init (void)
{
    uart_init(DEBUG_UART_INDEX);                                                // 初始化对应 UART 实例参数配置

    uart_set_sbrx_buf(DEBUG_UART_INDEX, (uint8_t *)debug_uart_buffer, DEBUG_UART_BUFFER_SIZE);
    uart_set_rx_drv_mode(DEBUG_UART_INDEX, UART_RX_DRV_MODE_INT_SYNC);          // 中断同步接收
    uart_set_tx_drv_mode(DEBUG_UART_INDEX, UART_TX_DRV_MODE_INT_SYNC);          // 发送同步中断
    uart_set_baud(DEBUG_UART_INDEX, DEBUG_UART_BAUDRATE);                       // 波特率设置
    uart_set_presc(DEBUG_UART_INDEX, UART_PRESCALER_DIV1);                      // 这里是分频 应该是时钟
    uart_set_parity(DEBUG_UART_INDEX, UART_PARITY_NONE);                        // 校验设置 无校验
    uart_set_over(DEBUG_UART_INDEX, UART_OVERSAMPLING_16);                      // 采样设置
    uart_set_sbit(DEBUG_UART_INDEX, UART_STOPBIT_1);                            // 停止位 1 位

    uart_set_prio(DEBUG_UART_INDEX, IRQ_PRIORITY_1);                            // 设置中断的优先级
    uart_set_cb(DEBUG_UART_INDEX, UART_CB_OPS_RX, debug_uart_callback);

    uart_start(DEBUG_UART_INDEX);                                               // 启动 UART

    DEBUG_UART_INDEX->rx_buf = (uint8_t *)debug_uart_buffer;
    DEBUG_UART_INDEX->rx_len = DEBUG_UART_BUFFER_SIZE;
    DEBUG_UART_INDEX->uart->CR1 |= UART_CR1_RXNEIE_RXFNEIE;
    DEBUG_UART_INDEX->uart->CR1 &= ~UART_CR1_TCIE;
}


int _write(int fd, uint8_t *buf, int size)
{
    (void)fd;
    int     len_temp    = size;
    int     time_coutn  = DEBUG_UART_TIMEOUT_COUNT;
    while(len_temp)
    {
        DEBUG_UART_INDEX->uart->TDR = *buf ++;
        while(!(DEBUG_UART_INDEX->uart->ISR & UART_ISR_TC) && time_coutn --);
        if(time_coutn > 0)
        {
            time_coutn = DEBUG_UART_TIMEOUT_COUNT;
        }
        else
        {
            break;
        }
        DEBUG_UART_INDEX->uart->ICR |= UART_ICR_TCCF;
        len_temp --;
    }
    return size - len_temp;
}

#if !defined(__ICCARM__)
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
/**
 * @brief   Increment location of program break.
 * @details This function allows to increase the program break. The result is
 *          to allocate memory to the process.
 *
 * @param[in] increment     number of bytes to increment
 * @return                  previuos program break.
 *
 * @special
 */
void *_sbrk(intptr_t increment)
{
    /*
     * Using external symbols:
     *   __heap_base__ : is placed at the beginning of the heap chunk
     *   __heap_end__  : is placed at the end of available heap chunk
     *
     * For definition of __heap_base__ and __heap_end__,
     * see linker script file.
     *
     * Limitation:
     *   multiple heap chunks not handled
     *   re-entrance (multi-threading) not handled
     */
    extern uint8_t __heap_base__;
    extern uint8_t __heap_end__;

    static uint8_t *heap_base = &__heap_base__;
    uint8_t *prev_head_base;

    prev_head_base = heap_base;
    if ((heap_base + increment) > (uint8_t*) &__heap_end__) {
        /* not enough memory, set errno and return -1 */
        errno = ENOMEM;
        return (void *) -1;
    }

    /* enough memory, increment heap base */
    heap_base += increment;

    /* return prior head base */
    return prev_head_base;
}
#endif
