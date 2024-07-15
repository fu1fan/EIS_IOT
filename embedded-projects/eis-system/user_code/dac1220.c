/*
 * dac1220.c
 *
 *  Created on: 2024年6月4日
 *      Author: Whisp
 */


#include "dac1220.h"

//#define SCLK_0() sclk_p->BSRR = (uint32_t) (0x10000U << sclk_pin);
#define SCLK_0() gpio_write_pin(DAC_SCLK_PIN, 0);
//#define SCLK_1() sclk_p->BSRR = (uint32_t) (1U << sclk_pin);
#define SCLK_1() gpio_write_pin(DAC_SCLK_PIN, 1);

//#define SDIO_0()  sdio_p->BSRR = (uint32_t) (0x10000U << sdio_pin);
#define SDIO_0()  gpio_write_pin(DAC_SDIO_PIN, 0);
//#define SDIO_1()  sdio_p->BSRR = (uint32_t) (1U << sdio_pin);
#define SDIO_1()  gpio_write_pin(DAC_SDIO_PIN, 1);

//#define CS_0()  cs_p->BSRR = (uint32_t) (0x10000U << cs_pin);
#define CS_0()  gpio_write_pin(DAC_CS_PIN, 0);
//#define CS_1()  cs_p->BSRR = (uint32_t) (1U << cs_pin);
#define CS_1()  gpio_write_pin(DAC_CS_PIN, 1);

void delay_us(uint32_t us) {
	osal_delay_microsec(us);
}

void delay_ms(uint32_t ms) {
	osal_delay_millisec(ms);
}

static void SDA_OUT(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin = DAC_SDA_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(DAC_SDA_PORT, &GPIO_InitStructure);
	gpio_set_pin_moder(DAC_SDIO_PIN, GPIO_MODER_OUTPUT);
}

static void SDA_IN(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//  GPIO_InitStructure.GPIO_Pin = DAC_SDA_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(DAC_SDA_PORT, &GPIO_InitStructure);
	gpio_set_pin_moder(DAC_SDIO_PIN, GPIO_MODER_INPUT);
}

void DAC1220_IO_Init(void)
{
	SCLK_1();
	//while (1);
	SDA_IN();
	SDA_OUT();
	SDIO_0();

//	delay_ms(50);
//	SDIO_0();
	CS_0();

}

void DAC1220_Write_Byte(uint8_t out_data)
{
	uint8_t index;
	SDA_OUT();
	SCLK_0();
	SDIO_0();
	for(index=0;index<8;index++)
	{
		delay_us(5);
		SCLK_1();
		if((out_data&0x80)==0x80)
		{
			SDIO_1();
		}
		else
		{
			SDIO_0();
		}
		out_data=out_data<<1;		//数据移位
		delay_us(5);
		SCLK_0();		//锁存数据
	}
}
uint8_t DAC1220_Read_Byte(void)
{
	uint8_t data=0,index=0;
	SDA_IN();
	SCLK_0();
	for(index=0;index<8;index++)
	{
		delay_us(5);
		SCLK_1();
		data<<=1;
		if (gpio_read_pin(DAC_SDIO_PIN) == 1)
			data++;
		delay_us(5);
		SCLK_0();		//锁存数据
	}
	delay_us(5);
	return data;
}


/*复位DA*/
void DAC1220_Reset(void)
{
	CS_0();
	SCLK_0();
	delay_us(10);

//****//
//SCLK复位模式
//DAC1220没有专用的复位引脚。
//相反，它包含一个等待特殊模式出现在SCLK上的电路，并在检测到特殊模式时触发内部硬件复位线。
//此模式称为SCLK重置模式。
//这种模式与SCLK上通常出现的时钟模式有很大的不同，在正常工作时不太可能被意外检测到。CS低有效
//详见DAC1220.PDF:页11：Table 5. Reset Timing Characteristics
	SCLK_1();
	delay_us(600*TXIN);
	SCLK_0();
	delay_us(20*TXIN);
	SCLK_1();
	delay_us(1100*TXIN);
	SCLK_0();
	delay_us(20*TXIN);
	SCLK_1();
	delay_us(2100*TXIN);
	SCLK_0();
//****//
	delay_us(10);
}

void DAC1220_Init(void)
{
	uint8_t command;			//缓存命令字节
	DAC1220_IO_Init();
	DAC1220_Reset();
	DAC1220_Self_Calibration();//自校准
	command=0x04;
	DAC1220_Write_Byte(command);
	delay_us(10);
	DAC1220_Write_Byte(0x00);
	delay_us(10);
	command=0x05;
	DAC1220_Write_Byte(command);
	delay_us(10);
	DAC1220_Write_Byte(0xA0);	//配置成正常20位模式，(直接二进制)Straight binary
	delay_us(10);

}

void DAC1220_WDAT(uint32_t dat)
{
	uint8_t command;

	if(dat > 1048575)
		dat = 1048575;
	dat = dat<<4;

	delay_us(10);
	command=0x00;		//DIR高字节
	DAC1220_Write_Byte(command);
	delay_us(10);
	command=(dat>>16)&0X00FF;			//写DIR高位数据
	DAC1220_Write_Byte(command);
	delay_us(10);

	command=0x01;		//写中位数据
	DAC1220_Write_Byte(command);
	delay_us(10);
	command=(dat>>8)&0X00FF;
	DAC1220_Write_Byte(command);
	delay_us(10);

	command=0x02;		//写低位数据
	DAC1220_Write_Byte(command);
	delay_us(10);
	command=(dat>>0)&0X00FF;
	DAC1220_Write_Byte(command);
	delay_us(10);

}

void DAC1220_Self_Calibration(void) //自校准
{
	DAC1220_Write_Byte(0x04);//写命令 寄存器高字节
	delay_us(10);
	DAC1220_Write_Byte(0x00);//校准期间输出将断开
	delay_us(10);
	DAC1220_Write_Byte(0x05);//写命令 寄存器低字节
	delay_us(10);
	DAC1220_Write_Byte(0xA1);	//配置成20位模式，(直接二进制)Straight binary  校准模式
	delay_ms(600); //等待校准

	DAC1220_Write_Byte(0x85);//读命令 寄存器低字节
	delay_us(10);
	while((DAC1220_Read_Byte()&0x03)!=0) //检测校准是否完成
	{
		delay_ms(500);
	}

}
void DAC1220VolWrite(float v)
{
	uint32_t dacData;
	dacData=mapfloat(v,-9.9738, 9.9725, 0, 1048575);
	DAC1220_WDAT(dacData);
}


/*
************************************************************
*	函数名称：	mapfloat
*
*	函数功能：	将一个区间内的值，映射到另一个区间
*
*	入口参数：	x输入值、
*						in_min输入最小值，
*						in_max输入最大值，
*						out_min输出最小值，
*						out_max输出最大值。
*	返回参数：	映射后的值
*	说明：
************************************************************/
uint32_t mapfloat(float x, float in_min, float in_max, uint32_t out_min, uint32_t out_max)
{
	uint32_t out;
	out=(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

	return out;
}














