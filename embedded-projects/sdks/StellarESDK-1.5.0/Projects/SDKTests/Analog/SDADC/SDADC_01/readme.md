# StellarE SDK SDADC_01

## SDADC test

### Description: 
	SDADC_01 runs an SDADC conversion in single ended input mode on AN[0],
	prints the result on UART and blinks USER_LED_A each 500ms. It configures
	also the watchdog high and low thresholds. So, an interrupt will be
	generated if the input voltage is lower than the low threshold or is higher
	than the high threshold.
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