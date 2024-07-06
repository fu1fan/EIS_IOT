#pragma once
#include "driver_headfile.h"


#define DAC_SCLK_PIN	PIN_GPIO4_4
#define DAC_SDIO_PIN    PIN_GPIO4_3
#define DAC_CS_PIN    	PIN_GPIO4_1

#define FXIN	2.4576//MHz
#define TXIN	0.41//uS   1/FXIN

void DAC1220_IO_Init(void);
void DAC1220_Init(void);
void DAC1220_Write_Byte(uint8_t out_data);//写
uint8_t DAC1220_Read_Byte(void);//读
void DAC1220_WDAT(uint32_t dat);

void DAC1220_Reset(void);//复位
void DAC1220_Self_Calibration(void); //自校准

uint32_t mapfloat(float x, float in_min, float in_max, uint32_t out_min, uint32_t out_max);
void DAC1220VolWrite(float v);

//DOUT-AD7190  SDI-HMC1044   c9
//DIN-AD7190  SCK-HMC1044    c10
//SCLK-AD7190  SEN-HMC1044  c11
