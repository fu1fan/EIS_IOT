# StellarE SDK EXTI_01

## EXTI test

### Description: 
	EXTI_01 configures a pin as external interrupt to generate an interrupt on
	both edges (rising edge and falling edge) of the input signal. Moreover, it
	configure a TIM to generate a PWM waveform with a frequency of 2Hz and a
	duty cycle of 50% that is used as input of the EXTI pin. With this
	configuration, the input signal on the EXTI pin will present an new edge
	each 250ms. So, the EXTI module will generate an interrupt each 250ms. The
	callback associated to the interrupt will blink the USER_LED_B. So, the
	USER_LED_B will be blinked each 250ms. Moreover, also the USER_LED_A is
	blinked each 250ms. So, if the EXTI works fine, both user leds will blink
	simultaneously.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Connect TP52 (LED_1) with GPIOE[0]  (USER_LED_B)
	3. Connect GPIOC[15] (PWM output) with GPIOB[1] (EXTI input)
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9]  (USER_LED_A)
	2. Connect TP52 (LED_1) with GPIOE[10] (USER_LED_B)
	3. Connect GPIOI[0] (PWM output) with GPIOB[1] (EXTI input)
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
	2. Close jumper JP36 (connect LED7 with GPIOD[15])
	3. Connect C10 (CON3-B, PWM output) with A10 (CON3-A, EXTI input)
-----------------------------------------------------------