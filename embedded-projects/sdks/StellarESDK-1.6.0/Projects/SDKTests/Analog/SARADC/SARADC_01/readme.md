# StellarE SDK SARADC_01

## SARADC test

### Description: 
	SARADC_01 runs an ADC regular conversion in continuos mode on a single
	channel. Each 5 conversions, it stops the regular conversions, calculates
	the average and prints the results on UART. Moreover, it blinks USER_LED_A
	each 500ms.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Use GPIOB[2] as ADC input pin
	3. Close jumpers JP29 and JP30
	4. Connect host serial port to UART via USB_PORT (P26)
	5. Configure host serial port baudrate to 38400
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9] (USER_LED_A)
	2. Use GPIOB[2] as ADC input pin
	3. Open jumpers JP29 and JP30
	4. Connect host serial port to UART via USB_PORT (P26)
	5. Configure host serial port baudrate to 38400
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
	2. Use B2 (CON3-A) as ADC input pin
	3. Close jumpers JP23 and JP24
	4. Connect host serial port to UART via USB_PORT (CON4)
	5. Configure host serial port baudrate to 38400
-----------------------------------------------------------