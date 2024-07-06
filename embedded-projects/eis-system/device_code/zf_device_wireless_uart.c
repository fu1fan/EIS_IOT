/*
 * zf_device_wireless_uart.c
 *
 *  Created on: 2024年2月23日
 *      Author: SEEKFREE_Teternal
 */

#include "zf_common_fifo.h"

#include "zf_device_wireless_uart.h"

static uint8 wireless_uart_buffer[WIRELESS_UART_BUFFER_SIZE];

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无线转串口模块 串口中断回调函数
// 参数说明     void
// 返回参数     void
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static void wireless_uart_callback (uart_driver_t *udp)
{
    udp->rx_buf = (uint8_t *)wireless_uart_buffer;
    udp->rx_len = WIRELESS_UART_BUFFER_SIZE;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无线转串口模块 串口发送驱动
// 参数说明     *buff           要发送的地址
// 返回参数     uint32          剩余发送长度
// 返回参数     void
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static void wireless_uart_write_base (uint8 *buff, uint32 len)
{
    int time_coutn = WIRELESS_UART_TIMEOUT_COUNT;
    while(len)
    {
        WIRLESS_UART_INDEX->uart->TDR = *buff ++;
        while(!(WIRLESS_UART_INDEX->uart->ISR & UART_ISR_TC) && time_coutn --);
        if(time_coutn > 0)
        {
            time_coutn = WIRELESS_UART_TIMEOUT_COUNT;
        }
        else
        {
            break;
        }
        WIRLESS_UART_INDEX->uart->ICR |= UART_ICR_TCCF;
        len --;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无线转串口模块 发送字符串
// 参数说明     *str            要发送的字符串地址
// 返回参数     uint32          剩余发送长度
// 使用示例     wireless_uart_send_string("Believe in yourself.");
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_uart_write_string (const char *str)
{
    uint32  time_count = 0;
    uint32  len = strlen(str);
    while(0 != len)
    {
		if(30 <= len)                                                       // 数据分 30byte 每包发送
		{
			wireless_uart_write_base((uint8 *)str, 30);                     // 发送数据
			str += 30;                                                      // 地址偏移
			len -= 30;                                                      // 数量
			time_count = 0;
		}
		else                                                                // 不足 30byte 的数据一次性发送完毕
		{
			wireless_uart_write_base((uint8 *)str, len);                    // 发送数据
			len = 0;
			break;
		}
    }
    return len;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无线转串口模块 发送数据块
// 参数说明     *buff           发送缓冲区
// 参数说明     len             发送数据长度
// 返回参数     uint32          剩余发送长度
// 使用示例     wireless_uart_send_buffer(buff, 64);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_uart_write_buffer (const uint8 *buff, uint32 len)
{
    uint8   *data = (uint8  *)buff;
    uint32 time_count = 0;
    while(0 != len)
    {
		if(30 <= len)                                                       // 数据分 30byte 每包发送
		{
			wireless_uart_write_base((uint8 *)data, 30);                    // 发送数据
			data += 30;                                                     // 地址偏移
			len -= 30;                                                      // 数量
			time_count = 0;
		}
		else                                                                // 不足 30byte 的数据一次性发送完毕
		{
			wireless_uart_write_base((uint8 *)data, len);                   // 发送数据
			len = 0;
			break;
		}
    }
    return len;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无线转串口模块 读取缓冲
// 参数说明     *buff           接收缓冲区
// 参数说明     len             读取数据长度
// 返回参数     uint32          实际读取数据长度
// 使用示例     wireless_uart_read_buffer(buff, 32);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_uart_read_buffer (uint8 *buff, uint32 len)
{
    uint32 data_len = ((WIRELESS_UART_BUFFER_SIZE - WIRLESS_UART_INDEX->rx_len) > len) ? (len) : (WIRELESS_UART_BUFFER_SIZE - WIRLESS_UART_INDEX->rx_len);
    memcpy(buff, wireless_uart_buffer, data_len);
    if((WIRELESS_UART_BUFFER_SIZE - WIRLESS_UART_INDEX->rx_len) > len)
    {
        for(uint8 i = 0; ((WIRELESS_UART_BUFFER_SIZE - WIRLESS_UART_INDEX->rx_len) - len) > i; i ++)
        {
            wireless_uart_buffer[i] = wireless_uart_buffer[i + data_len];
        }
        WIRLESS_UART_INDEX->rx_buf = (uint8_t *)&wireless_uart_buffer[data_len];
        WIRLESS_UART_INDEX->rx_len = WIRELESS_UART_BUFFER_SIZE - data_len;
        WIRLESS_UART_INDEX->uart->CR1 |= UART_CR1_RXNEIE_RXFNEIE;
    }
    else
    {
        WIRLESS_UART_INDEX->rx_buf = (uint8_t *)wireless_uart_buffer;
        WIRLESS_UART_INDEX->rx_len = WIRELESS_UART_BUFFER_SIZE;
        WIRLESS_UART_INDEX->uart->CR1 |= UART_CR1_RXNEIE_RXFNEIE;
    }
    return data_len;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无线转串口模块 初始化
// 参数说明     void
// 返回参数     void
// 使用示例     wireless_uart_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8_t wireless_uart_init (void)
{
    uint8 get_data[32];

    uart_init(WIRLESS_UART_INDEX);

    uart_set_sbrx_buf(WIRLESS_UART_INDEX, (uint8_t *)wireless_uart_buffer, WIRELESS_UART_BUFFER_SIZE);
    uart_set_rx_drv_mode(WIRLESS_UART_INDEX, UART_RX_DRV_MODE_INT_SYNC);
    uart_set_tx_drv_mode(WIRLESS_UART_INDEX, UART_TX_DRV_MODE_INT_SYNC);
    // uart_set_rx_drv_mode(WIRLESS_UART_INDEX, UART_RX_DRV_MODE_INT_ASYNC);
    // uart_set_tx_drv_mode(WIRLESS_UART_INDEX, UART_TX_DRV_MODE_INT_ASYNC);
    uart_set_baud(WIRLESS_UART_INDEX, WIRLESS_UART_BAUDRATE);
    uart_set_presc(WIRLESS_UART_INDEX, UART_PRESCALER_DIV1);
    uart_set_parity(WIRLESS_UART_INDEX, UART_PARITY_NONE);
    uart_set_over(WIRLESS_UART_INDEX, UART_OVERSAMPLING_16);
    uart_set_sbit(WIRLESS_UART_INDEX, UART_STOPBIT_1);

    uart_set_prio(WIRLESS_UART_INDEX, IRQ_PRIORITY_0);                         // 设置中断的优先级
    uart_set_cb(WIRLESS_UART_INDEX, UART_CB_OPS_RX, wireless_uart_callback);

    uart_start(WIRLESS_UART_INDEX);
    WIRLESS_UART_INDEX->rx_buf = (uint8_t *)wireless_uart_buffer;
    WIRLESS_UART_INDEX->rx_len = WIRELESS_UART_BUFFER_SIZE;
    WIRLESS_UART_INDEX->uart->CR1 |= UART_CR1_RXNEIE_RXFNEIE;
    WIRLESS_UART_INDEX->uart->CR1 &= ~UART_CR1_TCIE;

    wireless_uart_write_base((uint8 *)"AT\r\n", 4);

    osal_delay_millisec(50U);
    while(wireless_uart_read_buffer(get_data, 32));

    return 0;
}
