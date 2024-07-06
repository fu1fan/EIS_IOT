# StellarE SDK SPI_06

## SPI test (master tx - slave rx, DMA mode, half duplex)

### Description: 
	SPI_06 transmits data from SPI master to SPI slave. The data received from
	the SPI slave is compared with the data transmitted by the SPI master. If
	no error occurs during the data transferring, the USER_LED_A is blinked.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Connect GPIOE[5] (SPI Master MOSI) with GPIOG[9] (SPI Slave MISO)
	3. Connect GPIOE[7] (SPI Master SCK) with GPIOG[11] (SPI Slave SCK)
	4. Connect GPIOE[8] (SPI Master NSS) with GPIOG[12] (SPI Slave NSS)
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9] (USER_LED_A)
	2. Connect GPIOE[2] (SPI Master MOSI) with GPIOF[2] (SPI Slave MISO)
	3. Connect GPIOD[15] (SPI Master SCK) with GPIOG[11] (SPI Slave SCK)
	4. Connect GPIOE[8] (SPI Master NSS) with GPIOG[12] (SPI Slave NSS)
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
	2. Connect B16 (CON3-A, SPI Master MOSI) with C21 (CON3-B, SPI Slave MISO)
	4. Connect D16 (CON3-B, SPI Master SCK) with D20 (CON3-B, SPI Slave SCK)
	5. Connect D29 (CON3-B, SPI Master NSS) with C25 (CON3-B, SPI Slave NSS)
-----------------------------------------------------------