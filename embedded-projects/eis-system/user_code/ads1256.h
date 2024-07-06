#pragma once
#include "driver_headfile.h"

// 设置
#define MAX_SAMP_NUM 4096

// 参考电压
#define ADS1256_VREF 2.50925
#define ADS1256_FIX_FACTOR

// 引脚定义
#define ADS1256_DRDY_PIN PIN_GPIO8_1
#define ADS1256_CS_PIN PIN_GPIO8_3
#define ADS1256_RESET_PIN PIN_GPIO8_5
#define ADS1256_SYNC_PIN PIN_GPIO0_9
#define ADS1256_SCK_PIN PIN_GPIO8_0
#define ADS1256_DIN_PIN PIN_GPIO8_2
#define ADS1256_DOUT_PIN PIN_GPIO8_4

// EXTI通道
#define ADS1256_EXTI_CHANNEL EXTI_LINE_1
#define ADS1256_EXTI_PORT GPIO_PORT_I


/* 寄存器定义： Table 23. Register Map --- ADS1256数据手册第30页 */
enum
{
	/* 寄存器地址， 后面是复位后缺省值 */
	REG_STATUS = 0,	// x1H
	REG_MUX    = 1, // 01H
	REG_ADCON  = 2, // 20H
	REG_DRATE  = 3, // F0H
	REG_IO     = 4, // E0H
	REG_OFC0   = 5, // xxH
	REG_OFC1   = 6, // xxH
	REG_OFC2   = 7, // xxH
	REG_FSC0   = 8, // xxH
	REG_FSC1   = 9, // xxH
	REG_FSC2   = 10, // xxH
};

/* 命令定义： TTable 24. Command Definitions --- ADS1256数据手册第34页 */
enum
{
	CMD_WAKEUP  = 0x00,	// Completes SYNC and Exits Standby Mode 0000  0000 (00h)
	CMD_RDATA   = 0x01, // Read Data 0000  0001 (01h)
	CMD_RDATAC  = 0x03, // Read Data Continuously 0000   0011 (03h)
	CMD_SDATAC  = 0x0F, // Stop Read Data Continuously 0000   1111 (0Fh)
	CMD_RREG    = 0x10, // Read from REG rrr 0001 rrrr (1xh)
	CMD_WREG    = 0x50, // Write to REG rrr 0101 rrrr (5xh)
	CMD_SELFCAL = 0xF0, // Offset and Gain Self-Calibration 1111    0000 (F0h)
	CMD_SELFOCAL= 0xF1, // Offset Self-Calibration 1111    0001 (F1h)
	CMD_SELFGCAL= 0xF2, // Gain Self-Calibration 1111    0010 (F2h)
	CMD_SYSOCAL = 0xF3, // System Offset Calibration 1111   0011 (F3h)
	CMD_SYSGCAL = 0xF4, // System Gain Calibration 1111    0100 (F4h)
	CMD_SYNC    = 0xFC, // Synchronize the A/D Conversion 1111   1100 (FCh)
	CMD_STANDBY = 0xFD, // Begin Standby Mode 1111   1101 (FDh)
	CMD_RESET   = 0xFE, // Reset to Power-Up Values 1111   1110 (FEh)
};

/* 增益选项 */
typedef enum
{
	ADS1256_GAIN_1			= (0),	/* 增益1（缺省） */
	ADS1256_GAIN_2			= (1),	/* 增益2 */
	ADS1256_GAIN_4			= (2),	/* 增益4 */
	ADS1256_GAIN_8			= (3),	/* 增益8 */
	ADS1256_GAIN_16			= (4),	/* 增益16 */
	ADS1256_GAIN_32			= (5),	/* 增益32 */
	ADS1256_GAIN_64			= (6),	/* 增益64 */
}ADS1256_GAIN_E;

/* 采样速率选项 */
/* 数据转换率选择
	11110000 = 30,000SPS (default)
	11100000 = 15,000SPS
	11010000 = 7,500SPS
	11000000 = 3,750SPS
	10110000 = 2,000SPS
	10100001 = 1,000SPS
	10010010 = 500SPS
	10000010 = 100SPS
	01110010 = 60SPS
	01100011 = 50SPS
	01010011 = 30SPS
	01000011 = 25SPS
	00110011 = 15SPS
	00100011 = 10SPS
	00010011 = 5SPS
	00000011 = 2.5SPS
*/
typedef enum
{
	ADS1256_30000SPS = 0,
	ADS1256_15000SPS,
	ADS1256_7500SPS,
	ADS1256_3750SPS,
	ADS1256_2000SPS,
	ADS1256_1000SPS,
	ADS1256_500SPS,
	ADS1256_100SPS,
	ADS1256_60SPS,
	ADS1256_50SPS,
	ADS1256_30SPS,
	ADS1256_25SPS,
	ADS1256_15SPS,
	ADS1256_10SPS,
	ADS1256_5SPS,
	ADS1256_2d5SPS,

	ADS1256_DRATE_MAX
}ADS1256_DRATE_E;

#define ADS1256_DRAE_COUNT = 15;

//void ADS1256_Send8Bit(uint8_t _data);
//uint8_t ADS1256_Recive8Bit(void);
//void ADS1256_WaitDRDY(void);
//void ADS1256_ResetHard(void);
//void ADS1256_DelaySCLK(void);
//void ADS1256_DelayDATA(void);
//void ADS1256_WriteReg(uint8_t _RegID, uint8_t _RegValue);
//uint8_t ADS1256_ReadReg(uint8_t _RegID);
//void ADS1256_WriteCmd(uint8_t _cmd);
//uint8_t ADS1256_ReadChipID(void);
//int32_t ADS1256_ReadData(void);

void ADS1256_SetChannal(uint8_t _ch);
void ADS1256_SetDiffChannal(uint8_t _ch);

void InitADS1256(void);
void ADS1256_CfgADC(ADS1256_GAIN_E _gain, ADS1256_DRATE_E _drate);
uint8_t ADS1256_ReadChipID(void);
void ADS1256_StartScan(uint8_t _ch, uint32_t _sampling_num);
void ADS1256_StopScan(void);
int32_t ADS1256_GetAdc(uint8_t _ch);

void ADS1256_WaitSample(void);

uint32_t* ADS1256_GetData(void);

void EXTI_IRQHandler(void);

float ADS1256_ReadPin(ADS1256_GAIN_E _gain, uint8_t _ch);
