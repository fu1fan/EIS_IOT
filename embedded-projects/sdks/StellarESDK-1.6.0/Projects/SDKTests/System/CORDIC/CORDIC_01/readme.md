# StellarE SDK CORDIC_01

## CORDIC test

### Description: 
	CORDIC_01 shows how to configure CORDIC to calculate the sine and cosine
	of an array of angles. The CORDIC is configured to use the DMA. The CORDIC
	calculation is repeted each 250ms. When a calculation is completed, the led
	USER_LED_A is blinked and the result of the calculations is printed on the
	UART.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Close jumpers JP29 and JP30
	3. Connect host serial port to UART via USB_PORT (P26)
	4. Configure host serial port baudrate to 38400
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9] (USER_LED_A)
	2. Open jumpers JP29 and JP30
	3. Connect host serial port to UART via USB_PORT (P26)
	4. Configure host serial port baudrate to 38400
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
	2. Close jumpers JP23 and JP24
	3. Connect host serial port to UART via USB_PORT (CON4)
	4. Configure host serial port baudrate to 38400
-----------------------------------------------------------