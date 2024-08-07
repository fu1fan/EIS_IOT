/*
 * can_config.c
 *
 *  Created on: 2024年2月27日
 *      Author: SEEKFREE_Teternal
 */

#include <string.h>
#include "can_config.h"

bool        can_rx_flag = 0;
uint16_t    can_remote_id = 0;
uint8_t     can_receive_buffer[8];

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 接收中断回调
// 参数说明     *cdp            CAN 实例
// 参数说明     frame           CAN 帧 实例
// 参数说明     event           触发事件
// 返回参数     void            无返回值
// 使用示例     
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void can_receive_cb (can_driver_t *cdp, can_frame_t frame, can_evt_t event)
{
    (void)cdp;
    (void)frame;
    (void)event;

//    if ((CAN_CB_RX == event) &&
//        (CAN_ID_STD == frame.TYPE) &&
//        (CAN_DEFAULT_FILTER_ID == frame.ID))
    if ((CAN_CB_RX == event) &&
        (CAN_ID_STD == frame.TYPE))
    {
        can_remote_id = frame.ID;
        memcpy(can_receive_buffer, frame.data8, 8);
        can_rx_flag = 1;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 错误中断回调
// 参数说明     *cdp            CAN 实例
// 参数说明     psr             参数
// 返回参数     void            无返回值
// 使用示例     
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
//static void can_error_cb(can_driver_t *cdp, uint32_t psr) {
//
//    (void)cdp;
//    (void)psr;
//    while(1);
//}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 发送标准帧
// 参数说明     id              目标 ID
// 参数说明     data[8]         数据
// 返回参数     void            无返回值
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void can_send_frame (uint16_t id, uint8_t data[8])
{
    can_frame_t frame;
    frame.TYPE = CAN_ID_STD;                                                    // 标准 ID
    frame.DLC = CAN_MESSAGE_SIZE;                                               // 默认数据长
    frame.OPERATION = CAN_OPERATION_NORMAL;                                     // 正常模式
    frame.ID = id;
    frame.data8[0] = data[0];
    frame.data8[1] = data[1];
    frame.data8[2] = data[2];
    frame.data8[3] = data[3];
    frame.data8[4] = data[4];
    frame.data8[5] = data[5];
    frame.data8[6] = data[6];
    frame.data8[7] = data[7];
    can_transmit(&DRV_CAN1, CAN_TX_ANY_BUFFER, &frame);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 发送标准帧
// 参数说明     void            无参数
// 返回参数     void            无返回值
// 使用示例     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void can_module_init (void)
{
    can_init(&DRV_CAN1);                                                        // 初始化实例配置

    can_set_irq_line    (&DRV_CAN1, CAN_IRQ_LINE0);                             // 设置中断线
    can_set_irq_mask    (&DRV_CAN1, CAN_IRQ_ALL_ENABLED);                       // 启用所有中断触发
    can_set_loopback    (&DRV_CAN1, CAN_NO_LOOPBACK);                           // 禁止回环

    // can_set_fd_mode     (&DRV_CAN1, true);                                      // FD 模式
    // can_set_fd_brs_mode (&DRV_CAN1, true);                                      // FD 灵活波特率模式
    can_set_fd_mode     (&DRV_CAN1, false);                                     // FD 模式
    can_set_datasize    (&DRV_CAN1, CAN_DATASIZE_8BYTE);                        // 数据包长
    can_set_baudrate    (&DRV_CAN1, CAN_BAUDRATE_1MBIT);                        // 波特率设置
    can_set_dmu_mode    (&DRV_CAN1, false);                                     // DMU 模式

    can_set_filter      (&DRV_CAN1, CAN_DEFAULT_FILTER_ID, 0,                   // 设置过滤
                             CAN_STD_FILTER_TYPE, CAN_FILTER_CLASSIC,
                             CAN_FEC_RX_BUF);

    can_set_buffers     (&DRV_CAN1, 8, 8, 1, 1, 1);                             // BUFF 参数设置

    can_set_rx_polling_mode (&DRV_CAN1, false);                                 // 接收轮询模式
    can_set_tx_mode         (&DRV_CAN1, CAN_TX_MODE_DEDICATED_BUFF);            // 发送模式

    can_set_prio(&DRV_CAN1, IRQ_PRIORITY_4);
    can_set_cb              (&DRV_CAN1, can_receive_cb);                        // 设置中断回调
    // can_set_err_cb          (&DRV_CAN1, can_error_cb);                          // 设置错误中断回调

    can_start(&DRV_CAN1);                                                       // 启用参数配置 启用模块

}
