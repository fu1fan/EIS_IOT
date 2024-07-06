# StellarE SDK TIM_04

## TIM break event test

### Description: 
	TIM_04 shows how to configure and use the TIM break events. In particular,
	the TIM1 is configured to generate a PWM waveform. A callback related to
	the ch1 of TIM1 is defined and configured to blink the led USER_LED_A at
	the end of each duty cycle of the PWM waveform. Instead, the break event is
	configured to be active high, while the related callback will toggle the
	USER_LED_B. So, if the break input pin is connected to a push button, when
	the button is pressed, the PWM waveform will be stopped and the break event
	callback will toggle the USER_LED_B.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Connect TP52 (LED_1) with GPIOE[0]  (USER_LED_B)
	3. Connect GPIOD[2] (BKIN pin) with PB_1 (PUSH BUTTON 1)
-----------------------------------------------------------