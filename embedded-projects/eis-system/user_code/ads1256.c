#include "ads1256.h"

// GPIO
//gpio_port_t port = gpio_iounpack_port(io);
//gpio_pin_t pin   = gpio_iounpack_pin(io);
//GPIO_TypeDef *p  = gpio_dev_get_port_reg_ptr(port);

static gpio_port_t cs_port;
static gpio_pin_t cs_pin;
static GPIO_TypeDef *cs_p;

static gpio_port_t sck_port;
static gpio_pin_t sck_pin;
static GPIO_TypeDef *sck_p;

static gpio_port_t di_port;
static gpio_pin_t di_pin;
static GPIO_TypeDef *di_p;

static gpio_port_t sync_port;
static gpio_pin_t sync_pin;
static GPIO_TypeDef *sync_p;

static gpio_port_t rst_port;
static gpio_pin_t rst_pin;
static GPIO_TypeDef *rst_p;

static gpio_port_t dout_port;
static gpio_pin_t dout_pin;
static GPIO_TypeDef *dout_p;

static gpio_port_t drdy_port;
static gpio_pin_t drdy_pin;
static GPIO_TypeDef *drdy_p;

#define CS_0()  cs_p->BSRR = (uint32_t) (0x10000U << cs_pin);
#define CS_1()  cs_p->BSRR = (uint32_t) (1U << cs_pin);
#define SCK_0() sck_p->BSRR = (uint32_t) (0x10000U << sck_pin);
#define SCK_1() sck_p->BSRR = (uint32_t) (1U << sck_pin);
#define DI_0()	di_p->BSRR = (uint32_t) (0x10000U << di_pin);
#define DI_1()	di_p->BSRR = (uint32_t) (1U << di_pin);
#define SYNC_0() sync_p->BSRR = (uint32_t) (0x10000U << sync_pin);
#define SYNC_1() sync_p->BSRR = (uint32_t) (1U << sync_pin);
#define RST_0()	rst_p->BSRR = (uint32_t) (0x10000U << rst_pin);
#define RST_1()	rst_p->BSRR = (uint32_t) (1U << rst_pin);
#define DO_IS_HIGH() (dout_p->IDR & (1U << dout_pin)) != 0U
#define DRDY_IS_LOW() (drdy_p->IDR & (1U << drdy_pin)) == 0U

static void gpio_reg_init(void){
	cs_port = gpio_iounpack_port(ADS1256_CS_PIN);
	cs_pin   = gpio_iounpack_pin(ADS1256_CS_PIN);
	cs_p  = gpio_dev_get_port_reg_ptr(cs_port);

	sck_port = gpio_iounpack_port(ADS1256_SCK_PIN);
	sck_pin   = gpio_iounpack_pin(ADS1256_SCK_PIN);
	sck_p  = gpio_dev_get_port_reg_ptr(sck_port);

	di_port = gpio_iounpack_port(ADS1256_DIN_PIN);
	di_pin   = gpio_iounpack_pin(ADS1256_DIN_PIN);
	di_p  = gpio_dev_get_port_reg_ptr(di_port);

	sync_port = gpio_iounpack_port(ADS1256_SYNC_PIN);
	sync_pin   = gpio_iounpack_pin(ADS1256_SYNC_PIN);
	sync_p  = gpio_dev_get_port_reg_ptr(sync_port);

	rst_port = gpio_iounpack_port(ADS1256_RESET_PIN);
	rst_pin   = gpio_iounpack_pin(ADS1256_RESET_PIN);
	rst_p  = gpio_dev_get_port_reg_ptr(rst_port);

	dout_port = gpio_iounpack_port(ADS1256_DOUT_PIN);
	dout_pin   = gpio_iounpack_pin(ADS1256_DOUT_PIN);
	dout_p  = gpio_dev_get_port_reg_ptr(dout_port);

	drdy_port = gpio_iounpack_port(ADS1256_DRDY_PIN);
	drdy_pin   = gpio_iounpack_pin(ADS1256_DRDY_PIN);
	drdy_p  = gpio_dev_get_port_reg_ptr(drdy_port);
}

//void CS_0(void) {
//	cs_p->BSRR = (uint32_t) (0x10000U << cs_pin);
//}
//
//void CS_1(void) {
//	cs_p->BSRR = (uint32_t) (1U << cs_pin);
//}
//
//void SCK_0(void) {
//	sck_p->BSRR = (uint32_t) (0x10000U << sck_pin);
//}
//
//void SCK_1(void) {
//	sck_p->BSRR = (uint32_t) (1U << sck_pin);
//}
//
//void DI_0(void) {
//	di_p->BSRR = (uint32_t) (0x10000U << di_pin);
//}
//
//void DI_1(void) {
//	di_p->BSRR = (uint32_t) (1U << di_pin);
//}
//
//void SYNC_0(void) {
//	sync_p->BSRR = (uint32_t) (0x10000U << sync_pin);
//}
//
//void SYNC_1(void) {
//	sync_p->BSRR = (uint32_t) (1U << sync_pin);
//}
//
//void RST_0(void) {
//	rst_p->BSRR = (uint32_t) (0x10000U << rst_pin);
//}
//
//void RST_1(void) {
//	rst_p->BSRR = (uint32_t) (1U << rst_pin);
//}
//
//uint8_t DO_IS_HIGH(void) {
//	return (dout_p->IDR & (1U << dout_pin)) != 0U;
//}
//
//uint8_t DRDY_IS_LOW(void) {
//	return (drdy_p->IDR & (1U << drdy_pin)) == 0U;
//}


static uint32_t sampling_num = 0;
static volatile uint32_t sampling_count = 0;
static volatile uint8_t is_sampling = 0;

static uint32_t ads1256_data[MAX_SAMP_NUM] = {0};

const uint8_t s_tabDataRate[ADS1256_DRATE_MAX] =
{
	0xF0,		/* 复位时缺省值 */
	0xE0,
	0xD0,
	0xC0,
	0xB0,
	0xA1,
	0x92,
	0x82,
	0x72,
	0x63,
	0x53,
	0x43,
	0x33,
	0x20,
	0x13,
	0x03
};

void bsp_DelayUS(uint16_t nus)
{
	osal_delay_microsec(nus);
}

void ADS1256_DelaySCLK(void)
{
	uint8_t i = 0;

	/*  for STM32F407 (168M主频)
		取 5 时，实测高电平200ns, 低电平250ns <-- 不稳定
		取 10 以上，可以正常工作， 低电平400ns 高定400ns <--- 稳定
	*/
	i++;
	return;
}


void InitADS1256(void) {
	gpio_reg_init();
	// 初始化GPIO
	SYNC_1();
	RST_1();
	CS_1();
	SCK_0();
	DI_0();

	exti_set_line(ADS1256_EXTI_CHANNEL, EXTI_MODE_INTERRUPT, EXTI_TRIGGER_FALLING);
	exti_set_port(ADS1256_EXTI_CHANNEL, ADS1256_EXTI_PORT);

}

void ADS1256_Send8Bit(uint8_t _data) {
	uint8_t i;

	/* 连续发送多个字节时，需要延迟一下 */
	// ADS1256_DelaySCLK();
	ADS1256_DelaySCLK();

	/*　ADS1256 要求 SCL高电平和低电平持续时间最小 200ns  */
	for(i = 0; i < 8; i++)
	{
		if (_data & 0x80)
		{
			DI_1();
		}
		else
		{
			DI_0();
		}
		SCK_1();
		ADS1256_DelaySCLK();
		_data <<= 1;
		SCK_0();			/* <----  ADS1256 是在SCK下降沿采样DIN数据, 数据必须维持 50nS */
		//ADS1256_DelaySCLK();
	}
}

uint8_t ADS1256_Recive8Bit(void) {
	uint8_t i;
	uint8_t read = 0;

	ADS1256_DelaySCLK();
	/*　ADS1256 要求 SCL高电平和低电平持续时间最小 200ns  */
	for (i = 0; i < 8; i++)
	{
		SCK_1();
		ADS1256_DelaySCLK();
		read = read<<1;
		SCK_0();
		if (DO_IS_HIGH())
		{
			read++;
		}
		//ADS1256_DelaySCLK();
	}
	return read;
}

void ADS1256_WaitDRDY(void) {
	uint32_t i;
	for (i = 0; i < 40000000; i++)
	{
		if (DRDY_IS_LOW())
		{
			break;
		}
	}
	if (i >= 40000000)
	{
		printf("ADS1256_WaitDRDY() Time Out ...\r\n");		/* 调试语句. 用语排错 */
		//USB_SendStr("ADS1256_WaitDRDY() Time Out ...\r\n");		/* 调试语句. 用语排错 */
	}
}

void ADS1256_ResetHard(void)
{
	/* ADS1256数据手册第7页 */
	//RESET_0();			/* 复位 */
	//bsp_DelayUS(5);
	//RESET_1();

	//PWDN_0();			/* 进入掉电 同步*/
	//bsp_DelayUS(2);
	//PWDN_1();			/* 退出掉电 */

	bsp_DelayUS(5);

	//ADS1256_WaitDRDY();	/* 等待 DRDY变为0, 此过程实测: 630us */
}

void ADS1256_DelayDATA(void)
{
	/*
		Delay from last SCLK edge for DIN to first SCLK rising edge for DOUT: RDATA, RDATAC,RREG Commands
		最小 50 个tCLK = 50 * 0.13uS = 6.5uS
	*/
	bsp_DelayUS(4);	/* 最小延迟 6.5uS, 此处取10us */
}

void ADS1256_WriteReg(uint8_t _RegID, uint8_t _RegValue)
{
	CS_0();	/* SPI片选 = 0 */
	ADS1256_Send8Bit(CMD_WREG | _RegID);	/* 写寄存器的命令, 并发送寄存器地址 */
	ADS1256_Send8Bit(0x00);		/* 寄存器个数 - 1, 此处写1个寄存器 */

	ADS1256_Send8Bit(_RegValue);	/* 发送寄存器值 */
	CS_1();	/* SPI片选 = 1 */
}

uint8_t ADS1256_ReadReg(uint8_t _RegID)
{
	uint8_t read;

	CS_0();	/* SPI片选 = 0 */
	ADS1256_Send8Bit(CMD_RREG | _RegID);	/* 写寄存器的命令, 并发送寄存器地址 */
	ADS1256_Send8Bit(0x00);	/* 寄存器个数 - 1, 此处读1个寄存器 */

	ADS1256_DelayDATA();	/* 必须延迟才能读取芯片返回数据 */

	read = ADS1256_Recive8Bit();	/* 读寄存器值 */
	CS_1();	/* SPI片选 = 1 */

	return read;
}

void ADS1256_WriteCmd(uint8_t _cmd)
{
	CS_0();	/* SPI片选 = 0 */
	ADS1256_Send8Bit(_cmd);
	CS_1();	/* SPI片选 = 1 */
}

uint8_t ADS1256_ReadChipID(void)
{
	uint8_t id;

	ADS1256_WaitDRDY();
	id = ADS1256_ReadReg(REG_STATUS);
	return (id >> 4);
}

int32_t ADS1256_ReadData(void)
{
	uint32_t read = 0;

	CS_0();	/* SPI片选 = 0 */

	ADS1256_Send8Bit(CMD_RDATA);	/* 读数据的命令 */

	ADS1256_DelayDATA();	/* 必须延迟才能读取芯片返回数据 */

	/* 读采样结果，3个字节，高字节在前 */
	read = ADS1256_Recive8Bit() << 16;
	read += (ADS1256_Recive8Bit() << 8);
	read += ADS1256_Recive8Bit();

	CS_1();	/* SPI片选 = 1 */

	/* 负数进行扩展。24位有符号数扩展为32位有符号数 */
	if (read & 0x800000)
	{
		read += 0xFF000000;
	}

	return (int32_t)read;
}

void ADS1256_CfgADC(ADS1256_GAIN_E _gain, ADS1256_DRATE_E _drate)
{
	ADS1256_StopScan();			/* 暂停CPU中断 */

	ADS1256_ResetHard();		/* 硬件复位 */

	ADS1256_WaitDRDY();

	{
		uint8_t buf[4];		/* 暂存ADS1256 寄存器配置参数，之后连续写4个寄存器 */

		/* 状态寄存器定义
			Bits 7-4 ID3, ID2, ID1, ID0  Factory Programmed Identification Bits (Read Only)

			Bit 3 ORDER: Data Output Bit Order
				0 = Most Significant Bit First (default)
				1 = Least Significant Bit First
			Input data  is always shifted in most significant byte and bit first. Output data is always shifted out most significant
			byte first. The ORDER bit only controls the bit order of the output data within the byte.

			Bit 2 ACAL : Auto-Calibration
				0 = Auto-Calibration Disabled (default)
				1 = Auto-Calibration Enabled
			When Auto-Calibration is enabled, self-calibration begins at the completion of the WREG command that changes
			the PGA (bits 0-2 of ADCON register), DR (bits 7-0 in the DRATE register) or BUFEN (bit 1 in the STATUS register)
			values.

			Bit 1 BUFEN: Analog Input Buffer Enable
				0 = Buffer Disabled (default)
				1 = Buffer Enabled

			Bit 0 DRDY :  Data Ready (Read Only)
				This bit duplicates the state of the DRDY pin.

			ACAL=1使能自校准功能。当 PGA，BUFEEN, DRATE改变时会启动自校准
		*/
		buf[0] = (0 << 3) | (1 << 2) | (0 << 1);//此处(0 << 1)表示关闭BUFFER,改为(1 << 1)表示打开BUFFER
		/*
		打开或关闭BUFFER会影响芯片可采集电压范围.详见ADS1256数据手册第3页
		关闭BUFFER,采集范围：AGND-0.1	~	AVDD+0.1
		打开BUFFER,采集范围：AGND			~	AVDD-2.0
		*/

		buf[1] = 0x08;	/* 高四位0表示AINP接 AIN0,  低四位8表示 AINN 固定接 AINCOM */

		/*	ADCON: A/D Control Register (Address 02h)
			Bit 7 Reserved, always 0 (Read Only)
			Bits 6-5 CLK1, CLK0 : D0/CLKOUT Clock Out Rate Setting
				00 = Clock Out OFF
				01 = Clock Out Frequency = fCLKIN (default)
				10 = Clock Out Frequency = fCLKIN/2
				11 = Clock Out Frequency = fCLKIN/4
				When not using CLKOUT, it is recommended that it be turned off. These bits can only be reset using the RESET pin.

			Bits 4-2 SDCS1, SCDS0: Sensor Detect Current Sources
				00 = Sensor Detect OFF (default)
				01 = Sensor Detect Current = 0.5 μ A
				10 = Sensor Detect Current = 2 μ A
				11 = Sensor Detect Current = 10μ A
				The Sensor Detect Current Sources can be activated to verify  the integrity of an external sensor supplying a signal to the
				ADS1255/6. A shorted sensor produces a very small signal while an open-circuit sensor produces a very large signal.

			Bits 2-0 PGA2, PGA1, PGA0: Programmable Gain Amplifier Setting
				000 = 1 (default)
				001 = 2
				010 = 4
				011 = 8
				100 = 16
				101 = 32
				110 = 64
				111 = 64
		*/
		buf[2] = (0 << 5) | (0 << 3) | (_gain << 0);
		//ADS1256_WriteReg(REG_ADCON, (0 << 5) | (0 << 2) | (GAIN_1 << 0));	/* 选择1;1增益, 输入正负5V */

		/* 因为切换通道和读数据耗时 123uS, 因此扫描中断模式工作时，最大速率 = DRATE_1000SPS */
		buf[3] = s_tabDataRate[_drate];	// DRATE_10SPS;	/* 选择数据输出速率 */
		CS_0();	/* SPI片选 = 0 */
		ADS1256_Send8Bit(CMD_WREG | 0);	/* 写寄存器的命令, 并发送寄存器地址 */
		ADS1256_Send8Bit(0x03);			/* 寄存器个数 - 1, 此处3表示写4个寄存器 */
		ADS1256_Send8Bit(buf[0]);	/* 设置状态寄存器 */
		ADS1256_Send8Bit(buf[1]);	/* 设置输入通道参数 */
		ADS1256_Send8Bit(buf[2]);	/* 设置ADCON控制寄存器，增益 */
		ADS1256_Send8Bit(buf[3]);	/* 设置输出数据速率 */
		CS_1();	/* SPI片选 = 1 */
	}

	bsp_DelayUS(50);
}

void ADS1256_SetChannal(uint8_t _ch)
{
	/*
	Bits 7-4 PSEL3, PSEL2, PSEL1, PSEL0: Positive Input Channel (AINP) Select
		0000 = AIN0 (default)
		0001 = AIN1
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when PSEL3 = 1, PSEL2, PSEL1, PSEL0 are “don’t care”)

		NOTE: When using an ADS1255 make sure to only select the available inputs.

	Bits 3-0 NSEL3, NSEL2, NSEL1, NSEL0: Negative Input Channel (AINN)Select
		0000 = AIN0
		0001 = AIN1 (default)
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when NSEL3 = 1, NSEL2, NSEL1, NSEL0 are “don’t care”)
	*/
	if (_ch > 7)
	{
		return;
	}
	ADS1256_WriteReg(REG_MUX, (_ch << 4) | (1 << 3));	/* Bit3 = 1, AINN 固定接 AINCOM */
}

void ADS1256_SetDiffChannal(uint8_t _ch)
{
	/*
	Bits 7-4 PSEL3, PSEL2, PSEL1, PSEL0: Positive Input Channel (AINP) Select
		0000 = AIN0 (default)
		0001 = AIN1
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when PSEL3 = 1, PSEL2, PSEL1, PSEL0 are “don’t care”)

		NOTE: When using an ADS1255 make sure to only select the available inputs.

	Bits 3-0 NSEL3, NSEL2, NSEL1, NSEL0: Negative Input Channel (AINN)Select
		0000 = AIN0
		0001 = AIN1 (default)
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when NSEL3 = 1, NSEL2, NSEL1, NSEL0 are “don’t care”)
	*/
	if (_ch == 0)
	{
		ADS1256_WriteReg(REG_MUX, (0 << 4) | 1);	/* 差分输入 AIN0， AIN1 */
	}
	else if (_ch == 1)
	{
		ADS1256_WriteReg(REG_MUX, (2 << 4) | 3);	/* 差分输入 AIN2， AIN3 */
	}
	else if (_ch == 2)
	{
		ADS1256_WriteReg(REG_MUX, (4 << 4) | 5);	/* 差分输入 AIN4， AIN5 */
	}
	else if (_ch == 3)
	{
		ADS1256_WriteReg(REG_MUX, (6 << 4) | 7);	/* 差分输入 AIN6， AIN7 */
	}
}

void ADS1256_StartScan(uint8_t _ch, uint32_t _sampling_num){
	sampling_num = _sampling_num;
	sampling_count = 0;
	for (uint32_t i = 0; i < MAX_SAMP_NUM; i++) {
		ads1256_data[i] = 0;
	}
	ADS1256_SetChannal(_ch);
	ADS1256_WriteCmd(CMD_SYNC);
	bsp_DelayUS(5);
	ADS1256_WriteCmd(CMD_WAKEUP);
	bsp_DelayUS(25);
	is_sampling = 1;
	exti_set_irq(ADS1256_EXTI_CHANNEL, IRQ_MIN_PRIORITY, EXTI_IRQHandler);
}

void ADS1256_StopScan(void){
	is_sampling = 0;
	exti_set_line(ADS1256_EXTI_CHANNEL, EXTI_MODE_NONE,
	EXTI_TRIGGER_NONE); // Disable the EXTI line
}

void ADS1256_ISR(void){
	if(is_sampling){
	if (1) {
	//if (sampling_count < sampling_num) {
//		ADS1256_WriteCmd(CMD_SYNC);
//		bsp_DelayUS(5);
//		ADS1256_WriteCmd(CMD_WAKEUP);
//		bsp_DelayUS(10);
		ads1256_data[sampling_count] = ADS1256_ReadData();
		//sampling_count++;
	} else {
		is_sampling = 0;
		exti_set_irq(ADS1256_EXTI_CHANNEL, IRQ_MIN_PRIORITY, NULL);
	}}
}

void ADS1256_WaitSample(void) {
	while (is_sampling);
}

void EXTI_IRQHandler(void){
	exti_clear_pending(ADS1256_EXTI_CHANNEL);
	if (is_sampling) {
		ADS1256_ISR();
	}
	exti_clear_pending(ADS1256_EXTI_CHANNEL);
//	if(exti_is_pending(ADS1256_EXTI_CHANNEL)){
//		exti_clear_pending(ADS1256_EXTI_CHANNEL);
//		if (is_sampling) {
//			ADS1256_ISR();
//		}
//		exti_clear_pending(ADS1256_EXTI_CHANNEL);
//	}
}

uint32_t* ADS1256_GetData(void) {
	return ads1256_data;
}

float ADS1256_ReadPin(ADS1256_GAIN_E _gain, uint8_t _ch){
	if (is_sampling){
		ADS1256_StopScan();
	}
	ADS1256_CfgADC(_gain, ADS1256_30SPS);
	ADS1256_SetChannal(_ch);
	ADS1256_WriteCmd(CMD_SYNC);
	bsp_DelayUS(5);
	ADS1256_WriteCmd(CMD_WAKEUP);
	bsp_DelayUS(25);
	for (uint8_t i = 0; i < 2; i++) {	// 等待两轮后数据稳定，实际不影响，可以注释掉这个函数
		while (DRDY_IS_LOW());
		while (!DRDY_IS_LOW());
	}
	uint32_t data = ADS1256_ReadData();
//	return ((float)data * ADS1256_VREF) / (8388608.0 * ADS1256_VREF / 5 - 1);
	return ((float)data * 2*ADS1256_VREF) / (0x7FFFFF) / 4174200 * 4194303;
}
