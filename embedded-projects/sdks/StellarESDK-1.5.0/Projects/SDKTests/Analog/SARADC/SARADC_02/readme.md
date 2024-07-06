# StellarE SDK SARADC_02

## SARADC test

### Description: 
	SARADC_02 runs ADC regular conversions in single mode on 3 ADC channels
	triggered by TIM1 TRGO each 0.5s, prints results of regular conversions on
	UART and blinks USER_LED_A each 500ms.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Use GPIOB[2] as ADC CH1 input pin (ch1 of regular seguence)
	3. Use GPIOB[3] as ADC CH2 input pin (ch2 of regular seguence)
	4. Use GPIOB[4] as ADC CH3 input pin (ch3 of regular seguence)
	5. Close jumpers JP29 and JP30
	6. Connect host serial port to UART via USB_PORT (P26)
	7. Configure host serial port baudrate to 38400
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9] (USER_LED_A)
	2. Use GPIOB[2] as ADC CH1 input pin (ch1 of regular seguence)
	3. Use GPIOB[3] as ADC CH2 input pin (ch2 of regular seguence)
	4. Use GPIOB[4] as ADC CH3 input pin (ch3 of regular seguence)
	5. Open jumpers JP29 and JP30
	6. Connect host serial port to UART via USB_PORT (P26)
	7. Configure host serial port baudrate to 38400
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
	2. Use B2 (CON3-A) as ADC CH1 input pin (ch1 of regular seguence)
	3. Use B3 (CON3-A) as ADC CH2 input pin (ch2 of regular seguence) 
	4. Use C2 (CON3-B) as ADC CH3 input pin (ch3 of regular seguence)
	5. Close jumpers JP23 and JP24
	6. Connect host serial port to UART via USB_PORT (CON4)
	7. Configure host serial port baudrate to 38400
-----------------------------------------------------------