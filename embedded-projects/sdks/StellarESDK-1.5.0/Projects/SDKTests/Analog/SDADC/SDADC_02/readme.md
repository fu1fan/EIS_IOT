# StellarE SDK SDADC_02

## SDADC test

### Description:
	SDADC_02 shows how to configure and use SDADC and TIM_TS peripherals to
	get the timestamp associated to a SDADC acquisition. The test runs a SDADC
	conversion in single ended input mode on AN[0], prints the result and the
	timestamp on UART and blinks USER_LED_A each 500ms.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Use GPIOA[13] as SDADC AN[0] input pin
	3. Close jumpers JP29 and JP30
	4. Connect host serial port to UART via USB_PORT (P26)
	5. Configure host serial port baudrate to 38400
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
	2. Use A25 (CON3-A) as SDADC AN[0] input pin
	3. Close jumpers JP23 and JP24
	4. Connect host serial port to UART via USB_PORT (CON4)
	5. Configure host serial port baudrate to 38400
-----------------------------------------------------------