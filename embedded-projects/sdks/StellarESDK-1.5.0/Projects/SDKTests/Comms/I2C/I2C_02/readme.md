# StellarE SDK I2C_02

## I2C test (master tx - slave rx, master rx - slave tx, DMA mode)

### Description:
	I2C_02 transmits data from I2C master to I2C slave and from I2C slave to
	I2C master. The data received is compared with the data transmitted. If no
	error occurs during the data transferring, the USER_LED_A is blinked.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Connect P24[1] (I2C Master SDA) with TP16 (I2C Slave SDA)
	3. Connect P24[2] (I2C Master SCL) with TP15 (I2C Slave SCL)
-----------------------------------------------------------