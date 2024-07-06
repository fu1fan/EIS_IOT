# StellarE SDK TSENS_01

## TSENS test

### Description: 
	TSENS_01 runs an ADC regular conversion in single mode on a PTAT and CTAT
	channels (ADC1_CH13, ADC1_CH14). For each conversion, it calculates the 
	temperature and prints the results on UART. Moreover, it blinks USER_LED_A
	each 500ms.
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