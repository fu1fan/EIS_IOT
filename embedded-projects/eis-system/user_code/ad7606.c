/*
 * ad7606.c
 *
 *  Created on: 2024年6月5日
 *      Author: Whisp
 */
#include "ad7606.h"

static gpio_port_t ad_spi_sck_port;
static gpio_pin_t ad_spi_sck_pin;
static GPIO_TypeDef* ad_spi_sck_p;

static gpio_port_t ad_spi_miso_port;
static gpio_pin_t ad_spi_miso_pin;
static GPIO_TypeDef* ad_spi_miso_p;

static gpio_port_t ad_cs_port;
static gpio_pin_t ad_cs_pin;
static GPIO_TypeDef* ad_cs_p;

static gpio_port_t ad_reset_port;
static gpio_pin_t ad_reset_pin;
static GPIO_TypeDef* ad_reset_p;

static gpio_port_t ad_convst_port;
static gpio_pin_t ad_convst_pin;
static GPIO_TypeDef* ad_convst_p;

static gpio_port_t ad_range_port;
static gpio_pin_t ad_range_pin;
static GPIO_TypeDef* ad_range_p;

static gpio_port_t ad_os0_port;
static gpio_pin_t ad_os0_pin;
static GPIO_TypeDef* ad_os0_p;

static gpio_port_t ad_os1_port;
static gpio_pin_t ad_os1_pin;
static GPIO_TypeDef* ad_os1_p;

static gpio_port_t ad_os2_port;
static gpio_pin_t ad_os2_pin;
static GPIO_TypeDef* ad_os2_p;

#define AD_SCK_LOW() ad_spi_sck_p->BSRR = (uint32_t) (0x10000U << ad_spi_sck_pin);
#define AD_SCK_HIGH() ad_spi_sck_p->BSRR = (uint32_t) (1U << ad_spi_sck_pin);

#define AD_MISO_LOW() ad_spi_miso_p->BSRR = (uint32_t) (0x10000U << ad_spi_miso_pin);
#define AD_MISO_HIGH() ad_spi_miso_p->BSRR = (uint32_t) (1U << ad_spi_miso_pin);

#define AD_CS_LOW() ad_cs_p->BSRR = (uint32_t) (0x10000U << ad_cs_pin);
#define AD_CS_HIGH() ad_cs_p->BSRR = (uint32_t) (1U << ad_cs_pin);

#define AD_RESET_LOW() ad_reset_p->BSRR = (uint32_t) (0x10000U << ad_reset_pin);
#define AD_RESET_HIGH() ad_reset_p->BSRR = (uint32_t) (1U << ad_reset_pin);

#define AD_CONVST_LOW() ad_convst_p->BSRR = (uint32_t) (0x10000U << ad_convst_pin);
#define AD_CONVST_HIGH() ad_convst_p->BSRR = (uint32_t) (1U << ad_convst_pin);

#define AD_RANGE_5V() ad_range_p->BSRR = (uint32_t) (0x10000U << ad_range_pin);
#define AD_RANGE_10V() ad_range_p->BSRR = (uint32_t) (1U << ad_range_pin);

#define AD_OS0_0() ad_os0_p->BSRR = (uint32_t) (0x10000U << ad_os0_pin);
#define AD_OS0_1() ad_os0_p->BSRR = (uint32_t) (1U << ad_os0_pin);

#define AD_OS1_0() ad_os1_p->BSRR = (uint32_t) (0x10000U << ad_os1_pin);
#define AD_OS1_1() ad_os1_p->BSRR = (uint32_t) (1U << ad_os1_pin);

#define AD_OS2_0() ad_os2_p->BSRR = (uint32_t) (0x10000U << ad_os2_pin);
#define AD_OS2_1() ad_os2_p->BSRR = (uint32_t) (1U << ad_os2_pin);

#define AD_MISO_IN (ad_spi_miso_p->IDR & (1U << ad_spi_miso_pin)) != 0U

volatile uint16_t sample_num = 0;

void ENABLE_INT(void) {
	timts_interrupt_mode(1);
}

void DISABLE_INT(void) {
	timts_interrupt_mode(0);
}

static void DATA_OUT(void)
{
	gpio_set_pin_moder(AD_SPI_MISO_PIN, GPIO_MODER_OUTPUT);
}

static void DATA_IN(void)
{
	gpio_set_pin_moder(AD_SPI_MISO_PIN, GPIO_MODER_INPUT);
}

static void gpio_reg_init(void) {
	ad_spi_sck_port = gpio_iounpack_port(AD_SPI_SCK_PIN);
	ad_spi_sck_pin = gpio_iounpack_pin(AD_SPI_SCK_PIN);
	ad_spi_sck_p = gpio_dev_get_port_reg_ptr(ad_spi_sck_port);

	ad_spi_miso_port = gpio_iounpack_port(AD_SPI_MISO_PIN);
	ad_spi_miso_pin = gpio_iounpack_pin(AD_SPI_MISO_PIN);
	ad_spi_miso_p = gpio_dev_get_port_reg_ptr(ad_spi_miso_port);

	ad_cs_port = gpio_iounpack_port(AD_CS_PIN);
	ad_cs_pin = gpio_iounpack_pin(AD_CS_PIN);
	ad_cs_p = gpio_dev_get_port_reg_ptr(ad_cs_port);

	ad_reset_port = gpio_iounpack_port(AD_RESET_PIN);
	ad_reset_pin = gpio_iounpack_pin(AD_RESET_PIN);
	ad_reset_p = gpio_dev_get_port_reg_ptr(ad_reset_port);

	ad_convst_port = gpio_iounpack_port(AD_CONVST_PIN);
	ad_convst_pin = gpio_iounpack_pin(AD_CONVST_PIN);
	ad_convst_p = gpio_dev_get_port_reg_ptr(ad_convst_port);

	ad_range_port = gpio_iounpack_port(AD_RANGE_PIN);
	ad_range_pin = gpio_iounpack_pin(AD_RANGE_PIN);
	ad_range_p = gpio_dev_get_port_reg_ptr(ad_range_port);

	ad_os0_port = gpio_iounpack_port(AD_OS0_PIN);
	ad_os0_pin = gpio_iounpack_pin(AD_OS0_PIN);
	ad_os0_p = gpio_dev_get_port_reg_ptr(ad_os0_port);

	ad_os1_port = gpio_iounpack_port(AD_OS1_PIN);
	ad_os1_pin = gpio_iounpack_pin(AD_OS1_PIN);
	ad_os1_p = gpio_dev_get_port_reg_ptr(ad_os1_port);

	ad_os2_port = gpio_iounpack_port(AD_OS2_PIN);
	ad_os2_pin = gpio_iounpack_pin(AD_OS2_PIN);
	ad_os2_p = gpio_dev_get_port_reg_ptr(ad_os2_port);
}

FIFO_T	g_tAD;
int16_t g_tAD_single_buffer[8];
volatile uint8_t is_sampling = 0;


uint16_t ad7606_ReadBytes(void);
void ad7606_StartConv(void);

/*
*********************************************************************************************************
*	函 数 名: ad7606_IRQSrc
*	功能说明: 定时调用本函数，用于读取AD转换器数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
inline static void ad7606_IRQSrc(tim_ts_driver_t *tdp)
{
	(void)tdp;
	if (!is_sampling) {
		return;
	}
	// uint8_t i;
	uint16_t usReadValue;

	// TIM_ClearFlag(TIM4, TIM_FLAG_Update);

	/* 读取数据
	示波器监测，CS低电平持续时间 35us
	*/
	AD_CS_LOW();
	usReadValue=ad7606_ReadBytes();
	g_tAD.usBuf_0[g_tAD.usCount] = usReadValue;
	usReadValue=ad7606_ReadBytes();
	g_tAD.usBuf_1[g_tAD.usCount] = usReadValue;
	g_tAD.usCount++;
	if (g_tAD.usCount >= sample_num){
		ad7606_StopRecord();
	}
//	for (i = 0; i < CH_NUM; i++)
//	{
//		usReadValue = ad7606_ReadBytes();
//		if (g_tAD.usWrite < FIFO_SIZE)
//		{
//			g_tAD.usBuf[g_tAD.usWrite] = usReadValue;
//			++g_tAD.usWrite;
//		}
//	}

	AD_CS_HIGH();
//	g_tAD.usWrite = 0;
	ad7606_StartConv();
}

void bsp_TIM4_Configuration(void);

/*
*********************************************************************************************************
*	函 数 名: bsp_InitAD7606
*	功能说明: 初始化AD7606 SPI口线
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitAD7606(void)
{
	gpio_reg_init();

	DATA_OUT();

	/* 设置过采样模式 */
	ad7606_SetOS(0);

	/* 设置GPIO的初始状态 */
	ad7606_Reset();				/* 硬件复位复AD7606 */

	AD_CONVST_HIGH();			/* CONVST脚设置为高电平 */

	bsp_TIM4_Configuration();	/* 配置TIM2定时中断 */

	AD_CONVST_HIGH();
	osal_delay_millisec(50);
}

/*
*********************************************************************************************************
*	函 数 名: ad7606_Reset
*	功能说明: 硬件复位AD7606
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_Reset(void)
{
	/* AD7606是高电平复位，要求最小脉宽50ns */

	AD_RESET_LOW();

	AD_RESET_HIGH();
	AD_RESET_HIGH();
	AD_RESET_HIGH();
	AD_RESET_HIGH();

	AD_RESET_LOW();
}

/*
*********************************************************************************************************
*	函 数 名: ad7606_SetOS
*	功能说明: 设置过采样模式（数字滤波，硬件求平均值)
*	形    参：_ucMode : 0-6  0表示无过采样，1表示2倍，2表示4倍，3表示8倍，4表示16倍
*				5表示32倍，6表示64倍
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_SetOS(uint8_t _ucMode)
{
	if (_ucMode == 1)
	{
		AD_OS2_0();
		AD_OS1_0();
		AD_OS0_1();
	}
	else if (_ucMode == 2)
	{
		AD_OS2_0();
		AD_OS1_1();
		AD_OS0_0();
	}
	else if (_ucMode == 3)
	{
		AD_OS2_0();
		AD_OS1_1();
		AD_OS0_1();
	}
	else if (_ucMode == 4)
	{
		AD_OS2_1();
		AD_OS1_0();
		AD_OS0_0();
	}
	else if (_ucMode == 5)
	{
		AD_OS2_1();
		AD_OS1_0();
		AD_OS0_1();
	}
	else if (_ucMode == 6)
	{
		AD_OS2_1();
		AD_OS1_1();
		AD_OS0_0();
	}
	else	/* 按0处理 */
	{
		AD_OS2_0();
		AD_OS1_0();
		AD_OS0_0();
	}
}

/*
*********************************************************************************************************
*	函 数 名: ad7606_StartConv
*	功能说明: 启动AD7606的ADC转换
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_StartConv(void)
{
	/* 上升沿开始转换，低电平持续时间至少25ns  */
	AD_CONVST_LOW();
	AD_CONVST_LOW();	/* 连续执行2次，低电平约50ns */

	AD_CONVST_HIGH();
}

/*
*********************************************************************************************************
*	函 数 名: bsp_TIM4_Configuration
*	功能说明: 配置TIM4定时器
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_TIM4_Configuration(void)
{
//	NVIC_InitTypeDef NVIC_InitStructure;
//
//	/* TIM2 clock enable */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//
//	/* Configure the NVIC Preemption Priority Bits[配置优先级组] */
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
//
//	/* Enable the TIM2 gloabal Interrupt [允许TIM2全局中断]*/
//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	timts_set_callback(ad7606_IRQSrc);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SET_TIM4_FREQ
*	功能说明: 设置TIM4定时器频率
*	形    参：_ulFreq : 采样频率，单位Hz，
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SET_TIM4_FREQ(uint32_t _ulFreq)
{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	uint16_t usPrescaler;
//	uint16_t usPeriod;
//
//	TIM_DeInit(TIM4);	/* 复位TIM2定时器 */
//
//	/* TIM2 configuration
//		TIM2CLK = 36 MHz
//	*/
//	if (_ulFreq == 0)
//	{
//		return;		/* 采样频率为0，停止采样 */
//	}
//	else if (_ulFreq <= 100)   /* 采样频率小于100Hz */
//	{
//		usPrescaler = 36000;		/* TM2CLK = 72 000 000/36000 = 2000 */
//		usPeriod = 2000 / _ulFreq;
//	}
//	else if (_ulFreq <= 200000)	/* 采样频率 ：100Hz - 200kHz */
//	{
//		usPrescaler = 36 - 1;		/* TM2CLK = 36 000 000/36 = 2 000 000 */
//		usPeriod = 2000000 / _ulFreq;
//	}
//	else	/* 采样频率大于 200kHz */
//	{
//		return;
//	}
//
//	TIM_TimeBaseStructure.TIM_Period = usPeriod - 1; 		/* 计数周期 */
//	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;	/* 分频系数 */
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; 		/* */
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数方向向上计数
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//
//	/* Clear TIM2 update pending flag[清除TIM2溢出中断标志] */
//	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
//
//	TIM_SetCounter(TIM4, 0);
//
//	/* Enable TIM2 Update interrupt [TIM2溢出中断允许]*/
//	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
//
//	/* TIM2 enable counter [允许tim2计数]*/
//	TIM_Cmd(TIM4, ENABLE);
	timts_pit_freq_init(_ulFreq);
}

//SPI写数据
//向触摸屏IC写入1byte数据
//num:要写入的数据
void SPI_SendData(uint16_t data)
{
	DATA_OUT();
	uint8_t count=0;
	AD_SCK_LOW();	//下降沿有效
	for(count=0;count<16;count++)
	{
		if(data&0x8000){
			AD_MISO_LOW();
		}
		else{
			AD_MISO_HIGH();
		}
		data<<=1;
		AD_SCK_LOW();
		AD_SCK_HIGH();		//上升沿有效
	}
}
//SPI读数据
//从触摸屏IC读取adc值
//CMD:指令
//返回值:读到的数据
//u16 SPI_ReceiveData(void)
//{
//	u8 count=0;
//	u16 Num=0;
//	AD_SCK_LOW();		//先拉低时钟
//	for(count=0;count<16;count++)//读出16位数据,只有高12位有效
//	{
//		Num<<=1;
//		AD_SCK_LOW();	//下降沿有效
//		AD_CSK_HIGH();
//
//		if(AD_MISO_IN)Num++;
//	}
//	return(Num);
//}
uint16_t SPI_ReceiveData(void)
{
	DATA_IN();
	uint8_t count=0;
	uint16_t Num=0;
	AD_SCK_HIGH();
	for(count=0;count<16;count++)//读出16位数据
	{
		Num<<=1;
		AD_SCK_LOW();	//下降沿有效
		if(AD_MISO_IN)Num++;
		AD_SCK_HIGH();
	}
	return(Num);
}
/*
*********************************************************************************************************
*	函 数 名: ad7606_ReadBytes
*	功能说明: 读取AD7606的采样结果
*	形    参：
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t ad7606_ReadBytes(void)
{
  uint16_t usData = 0;

////  /* Wait until the transmit buffer is empty */
////  while (SPI_I2S_GetFlagStatus(AD_SPI, SPI_I2S_FLAG_TXE) == RESET)
////  {
////  }

//  /* Send the byte */
//  SPI_I2S_SendData(AD_SPI, 0xFFFF);
////	SPI_SendData(0xFFFF);

////  /* Wait until a data is received */
////  while (SPI_I2S_GetFlagStatus(AD_SPI, SPI_I2S_FLAG_RXNE) == RESET)
////  {
////  }

  /* Get the received data */
////  usData = SPI_I2S_ReceiveData(AD_SPI);
	usData = SPI_ReceiveData();

  /* Return the shifted data */
  return usData;
}

/*
*********************************************************************************************************
*	函 数 名: ad7606_StartRecord
*	功能说明: 开始采集
*	形    参：_ulFreq : 采样频率, 单位 HZ
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_StartRecord(uint32_t _ulFreq, uint16_t _count)
{
	//ad7606_Reset();	/* 复位硬件 */

	if (is_sampling) {
		return;
	}
	is_sampling = 1;

	ad7606_StartConv();				/* 启动采样，避免第1组数据全0的问题 */

	AD_CS_LOW();
	ad7606_ReadBytes();
	ad7606_ReadBytes();
	ad7606_ReadBytes();
	ad7606_ReadBytes();
	ad7606_ReadBytes();
	ad7606_ReadBytes();
	ad7606_ReadBytes();
	ad7606_ReadBytes();
	AD_CS_HIGH();

	ad7606_StartConv();

	sample_num = _count;
	g_tAD.usCount = 0;				/* 必须在开启TIM2之前清0 */

	bsp_TIM4_Configuration();		/* 配置TIM2定时中断 */

	bsp_SET_TIM4_FREQ(_ulFreq);		/* 设置采样频率, 并使能TIM2定时采样中断 */

	ENABLE_INT();
}

/*
*********************************************************************************************************
*	函 数 名: ad7606_StopRecord
*	功能说明: 停止采集
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_StopRecord(void)
{
	//TIM_Cmd(TIM4, DISABLE);
	DISABLE_INT();
	is_sampling = 0;
}

void ad7606_WaitforRecord(void) {
	while (is_sampling) {
		osal_delay_millisec(1);
	}
}

void ad7606_SingleRecord(void) {
	ad7606_Reset(); /* 复位硬件 */
	ad7606_StartConv();

	ad7606_StartConv();				/* 启动采样，避免第1组数据全0的问题 */
	AD_CS_LOW();
	ad7606_ReadBytes();
	ad7606_ReadBytes();
	ad7606_ReadBytes();
	ad7606_ReadBytes();
	ad7606_ReadBytes();
	ad7606_ReadBytes();
	ad7606_ReadBytes();
	ad7606_ReadBytes();
	AD_CS_HIGH();

	ad7606_StartConv();
	osal_delay_millisec(1);

	AD_CS_LOW();
	g_tAD_single_buffer[0] = ad7606_ReadBytes();
	g_tAD_single_buffer[1] = ad7606_ReadBytes();
	g_tAD_single_buffer[2] = ad7606_ReadBytes();
	g_tAD_single_buffer[3] = ad7606_ReadBytes();
	g_tAD_single_buffer[4] = ad7606_ReadBytes();
	g_tAD_single_buffer[5] = ad7606_ReadBytes();
	g_tAD_single_buffer[6] = ad7606_ReadBytes();
	g_tAD_single_buffer[7] = ad7606_ReadBytes();
	AD_CS_HIGH();
}

