# StellarE SDK SPI_03

## SPI test (multi-slave communication, DMA mode, full duplex)

### Description: 
	SPI_03 shows a SPI multi-slave communication. The same data is trasmitted
	from SPI master to 2 SPI slaves. The master uses GPIO pins to manage the
	chip select lines for each slave. The data received from the slaves is
	compared with the data transmitted by the SPI master. If no error occurs
	during the data transferring, the USER_LED_A is blinked.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Connect GPIOE[5] (SPI Master MOSI) with GPIOG[10] (SPI Slave1 MOSI)
	3. Connect GPIOE[6] (SPI Master MISO) with GPIOG[9] (SPI Slave1 MISO)
	4. Connect GPIOE[7] (SPI Master SCK) with GPIOG[11] (SPI Slave1 SCK)
	5. Connect GPIOE[9] (SPI Master NSS GPIO) with GPIOG[12] (SPI Slave1 NSS)
	6. Connect P17[2] (SPI Master MOSI) with GPIOG[7] (SPI Slave2 MOSI)
	7. Connect P17[1] (SPI Master MISO) with GPIOG[6] (SPI Slave2 MISO)
	8. Connect P17[3] (SPI Master SCK) with GPIOG[5] (SPI Slave2 SCK)
	9. Connect P17[4] (SPI Master NSS GPIO) with GPIOG[8] (SPI Slave2 NSS)
-----------------------------------------------------------