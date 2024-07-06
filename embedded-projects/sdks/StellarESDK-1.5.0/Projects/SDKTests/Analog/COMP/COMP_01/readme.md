# StellarE SDK COMP_01

## COMP test

### Description: 
	COMP_01 configures a comparator and compares the voltage on input plus
	with the voltage on input minus. The result of comparison is reported on
	compator output pin. In particular, a TIM is configured to generate a PWM
	waveform with a frequency of 2Hz and a duty cycle of 50% that is used as
	input plus. Instead, the value 1/3*(VREH-VREFL) is selected as input minus.
	With this configuration, the output of the comparator will be high for the
	50% of the period of the PWM waveform, i.e. for 250ms. At last, the
	comparator is configured to trigger an interrupt on the rising and falling
	edges of its output. The callback associated to the interrupt will blink
	the USER_LED_B. So, the USER_LED_B will be blinked each 250ms. Moreover,
	also the USER_LED_A is blinked each 250ms. So, if the comparator works
	fine, both user leds will blink simultaneously.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Connect TP52 (LED_1) with GPIOE[0]  (USER_LED_B)
	3. Connect GPIOC[15] (PWM ch1 output) with GPIOB[2] (COMP input plus)
	4. Use GPIOC[5] to sample the compator output
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9]  (USER_LED_A)
	2. Connect TP52 (LED_1) with GPIOE[10] (USER_LED_B)
	3. Connect GPIOI[0] (PWM ch1 output) with GPIOB[2] (COMP input plus)
	4. Use GPIOC[5] to sample the compator output
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
	2. Close jumper JP36 (connect LED7 with GPIOD[15])
	3. Connect C10 (CON3-B, PWM ch1 output) with B2 (CON3-A, COMP input plus)
	4. Use D7 (CON3-B) to sample the compator output
-----------------------------------------------------------