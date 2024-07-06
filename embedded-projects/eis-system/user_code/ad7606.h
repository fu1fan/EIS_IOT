/*
 * ad7606.h
 *
 *  Created on: 2024年6月5日
 *      Author: Whisp
 */

#ifndef USER_CODE_AD7606_H_
#define USER_CODE_AD7606_H_

/* 每个样本2字节，采集通道 */
#define CH_NUM			8				/* 采集2通道 */
#define FIFO_SIZE		1*1024*2		/* 大小不要超过48K (CPU内部RAM 只有64K) */

#define AD_SPI_SCK_PIN 		PIN_GPIO3_12
#define AD_SPI_MISO_PIN		PIN_GPIO3_14
#define AD_CS_PIN			PIN_GPIO3_15
#define AD_RESET_PIN		PIN_GPIO3_13
#define AD_CONVST_PIN		PIN_GPIO3_10
#define AD_RANGE_PIN		PIN_GPIO3_7
#define AD_OS0_PIN			PIN_GPIO4_8
#define AD_OS1_PIN			PIN_GPIO3_9
#define AD_OS2_PIN			PIN_GPIO3_8

#include "driver_headfile.h"

/* AD数据采集缓冲区 */
typedef struct
{
	uint16_t usCount;
	int16_t usBuf_0[FIFO_SIZE];
	int16_t usBuf_1[FIFO_SIZE];
}FIFO_T;

/* 供外部调用的函数声明 */
void ENABLE_INT(void);// 使能中断
void DISABLE_INT(void);// 禁止中断
void ad7606_Reset(void);
void ad7606_SetOS(uint8_t _ucMode);
void bsp_SET_TIM2_FREQ(uint32_t _ulFreq);
void bsp_InitAD7606(void);
void ad7606_StartRecord(uint32_t _ulFreq, uint16_t _count);
void ad7606_StopRecord(void);
void ad7606_WaitforRecord(void);
void ad7606_SingleRecord(void);


extern FIFO_T  g_tAD;
extern int16_t g_tAD_single_buffer[8];

#endif /* USER_CODE_AD7606_H_ */
