# StellarE SDK SARADC_06

## SARADC test

### Description:
	SARADC_06 runs ADC regular conversions in dual mode on two channels on ADC1
	(master) and ADC2 (slave). The converted data is then read from the common
	data register and printed on UART. Furthermore ADC injected conversionson
	on 2 ADC channels are done each 500ms, the results are printed on UART
	and USER_LED_A blinks each 500ms.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Use GPIOB[2] as ADC1 CH1 input pin (ch1 of regular seguence)
	3. Use GPIOB[3] as ADC1 CH2 input pin (ch2 of regular seguence)
	4. Use GPIOB[4] as ADC1 CH3 input pin (ch1 of injected seguence)
	5. Use GPIOB[5] as ADC1 CH4 input pin (ch2 of injected seguence)
	6. Use GPIOB[7] as ADC2 CH1 input pin (ch2 of regular seguence)
	7. Use GPIOB[8] as ADC2 CH2 input pin (ch1 of regular seguence)
	8. Close jumpers JP29 and JP30
	9. Connect host serial port to UART via USB_PORT (P26)
	10. Configure host serial port baudrate to 38400
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9] (USER_LED_A)
	2. Use GPIOB[2] as ADC1 CH1 input pin (ch1 of regular seguence)
	3. Use GPIOB[3] as ADC1 CH2 input pin (ch2 of regular seguence)
	4. Use GPIOB[4] as ADC1 CH3 input pin (ch1 of injected seguence)
	5. Use GPIOB[5] as ADC1 CH4 input pin (ch2 of injected seguence)
	6. Use GPIOB[7] as ADC2 CH1 input pin (ch2 of regular seguence)
	7. Use GPIOB[8] as ADC2 CH2 input pin (ch1 of regular seguence)
	8. Open jumpers JP29 and JP30
	9. Connect host serial port to UART via USB_PORT (P26)
	10. Configure host serial port baudrate to 38400
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
	2. Use B2 (CON3-A) as ADC1 CH1 input pin (ch1 of regular seguence)
	3. Use B3 (CON3-A) as ADC1 CH2 input pin (ch2 of regular seguence) 
	4. Use C2 (CON3-B) as ADC1 CH3 input pin (ch1 of injected seguence)
	5. Use C3 (CON3-B) as ADC1 CH4 input pin (ch2 of injected seguence)
	6. Use D3 (CON3-B) as ADC2 CH1 input pin (ch2 of regular seguence)
	7. Use B4 (CON3-A) as ADC2 CH2 input pin (ch1 of regular seguence)
	8. Close jumpers JP23 and JP24
	9. Connect host serial port to UART via USB_PORT (CON4)
	10. Configure host serial port baudrate to 38400
-----------------------------------------------------------