/*
 * sradc_sample.c
 *
 *  Created on: 2024年2月22日
 *      Author: SEEKFREE_Teternal
 */

#include "sradc_sample.h"

#define SRADC_SAMPLE_CHANNEL_LIST       {   ADC_CH_IN1, ADC_CH_IN2, ADC_CH_IN3, \
                                            ADC_CH_IN1, ADC_CH_IN2, ADC_CH_IN3, \
                                            ADC_CH_IN1, ADC_CH_IN2, ADC_CH_IN3 }
#define SRADC_SAMPLE_INDEX_LIST         {   &SRADC_PART1_INDEX, &SRADC_PART2_INDEX, &SRADC_PART3_INDEX }

uint8_t         sradc_sample_flag;
adc_sample_t    sradc_sample_buffer[SRADC_SAMPLE_CHANNEL_MAX];
adc_driver_t    *sradc_index_list[SRADC_SAMPLE_INDEX_MAX]   = SRADC_SAMPLE_INDEX_LIST;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SRADC 数据采样 超时会退出
// 参数说明     void
// 返回参数     void
// 使用示例     sradc_sample();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void sradc_sample (void)
{
    uint8_t state_temp = 0;
    int16_t timeout_counter = SRADC_SAMPLE_TIMEROUT_MAX;

    for(uint8_t i = 0; i < SRADC_SAMPLE_INDEX_MAX; i ++)
    {
        adc_start_regular(sradc_index_list[i]);                                 // 启动所有的转换序列
    }
    while(timeout_counter --)                                                   // 超时退出
    {
        if(sradc_index_list[0]->adc->ISR & ADC_ISR_EOS)                         // 等待转换完成标志
        {
            state_temp |= 0x01;
        }
        if(sradc_index_list[1]->adc->ISR & ADC_ISR_EOS)                         // 等待转换完成标志
        {
            state_temp |= 0x02;
        }
        if(sradc_index_list[2]->adc->ISR & ADC_ISR_EOS)                         // 等待转换完成标志
        {
            state_temp |= 0x04;
        }
        if(0x07 == state_temp)                                                  // 三个模块都完成转换即可退出
        {
            sradc_sample_flag |= 0x01;
            break;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SRADC 采样引脚配置与参数设置
// 参数说明     void
// 返回参数     void
// 使用示例     sradc_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void sradc_init (void)
{
    adc_dma_conf_t  adc_dma_conf                                = {DMA_STREAM_ID_ANY, DMA_PRIORITY_MAX, IRQ_PRIORITY_10};
    uint8_t         sradc_channel_list[SRADC_SAMPLE_CHANNEL_MAX]= SRADC_SAMPLE_CHANNEL_LIST;

    SYSCFG->ADC_BIASEN = SYSCFG_ADC_BIASEN_GLOBAL_BIAS_EN;                      // 它需要 SYSCFG 启用基准时钟

    for(uint8_t i = 0; i < SRADC_SAMPLE_INDEX_MAX; i ++)                        // 实际上开发板用到的 ADC 引脚分布在三个模块上
    {
        adc_init(sradc_index_list[i]);                                          // 初始化对应示例参数

        adc_set_data_res(sradc_index_list[i], SRADC_RES_DEFAULT);               // 精度设置
        adc_set_dma_conf(sradc_index_list[i], &adc_dma_conf);                   // 配置 DMA 数据搬运
        adc_set_reg_mode(sradc_index_list[i], ADC_REG_SINGLE);                  // 单次序列转换模式 或许可以开启循环模式 让它自动采集刷新 不过会占用总线

        adc_set_seq(                                                            // 配置序列转换
            sradc_index_list[i],                                                // 对应 SRADC 模块
            ADC_REG_CONV,                                                       // 转换
            SRADC_SAMPLE_PART_CHANNEL_MAX,                                      // 该模块的通道数
            &sradc_channel_list[i * SRADC_SAMPLE_PART_CHANNEL_MAX],             // 通道列表
            &sradc_sample_buffer[i * SRADC_SAMPLE_PART_CHANNEL_MAX]);           // 数据缓冲区
        for(uint8_t j = 0; j < SRADC_SAMPLE_PART_CHANNEL_MAX; j ++)
        {
            adc_set_smp_time(                                                   // 配置采样的时钟
                sradc_index_list[i],                                            // 对应 SRADC 模块
                sradc_channel_list[i * SRADC_SAMPLE_PART_CHANNEL_MAX + j],      // 对应通道
                ADC_SMP_6P5);                                                   // 配置个较短的采样提高速率
        }

        adc_start(sradc_index_list[i]);                                         // 启动模块
    }
}
