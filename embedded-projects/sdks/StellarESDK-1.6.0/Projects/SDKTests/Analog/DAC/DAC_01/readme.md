# StellarE SDK DAC_01

## DAC test

### Description: 
	DAC_01 shows how to configure DAC to generate on its 2 channels a couple
	of signals with a variable amplitude. The value of amplitude of channel 1
	is updated each 100ms. Instead the value of amplitude of channel 2 is
	updated through DMA triggered by a TIM. The USER_LED_A will blink each 300ms.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Use GPIOB[1] as BADC CH1 output pin
	3. Use GPIOC[5] as BADC CH2 output pin
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9] (USER_LED_A)
	2. Use GPIOB[1] as BADC CH1 output pin
	3. Use GPIOC[5] as BADC CH2 output pin
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
	2. Use A10 (CON3-A) as BADC CH1 output pin
	3. Use D7 (CON3-B) as BADC CH2 output pin
-----------------------------------------------------------