# StellarE SDK I2S_01

## I2S test (master tx - slave rx)

### Description: 
	I2S_01 transmits data from I2S master to I2S slave. The data received from
	the I2S slave is compared with the data transmitted by the I2S master. If
	no error occurs during the data transferring, the USER_LED_A is blinked.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Connect GPIOG[11] (I2S Master SCK) with GPIOD[10] (I2S Slave SCK)
	3. Connect GPIOG[12] (I2S Master NSS) with GPIOD[13] (I2S Slave NSS)
	4. Connect GPIOG[10] (I2S Master MOSI) with GPIOD[12] (I2S Slave MOSI)
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9] (USER_LED_A)
	2. Connect GPIOG[11] (I2S Master SCK) with GPIOD[10] (I2S Slave SCK)
	3. Connect GPIOG[12] (I2S Master NSS) with GPIOD[13] (I2S Slave NSS)
	4. Connect GPIOF[3] (I2S Master MOSI) with GPIOD[12] (I2S Slave MOSI)
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
	2. Connect D20 (CON3-B, I2S Master SCK) with B7 (CON3-A, I2S Slave SCK)
	3. Connect C25 (CON3-B, I2S Master NSS) with D14 (CON3-B, I2S Slave NSS)
	3. Connect C23 (CON3-B, I2S Master MOSI) with C13 (CON3-B, I2S Slave MOSI)
-----------------------------------------------------------