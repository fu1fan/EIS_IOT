# StellarE SDK HRTIM_01

## HRTIM test

### Description: 
	HRTIM_01 generates a couple of PWM signals using the output 1 and output 2
	of the TimerF of HRTIM1 with the following configuration:
	- frequency = 2KHz (period = 500us)
	- duty cycle = 50%
	- rising edge duty cycle = 25us
	- falling edge duty cycle = 25us
	A callback related to the Master of HRTIM1 is configured to blink the led
	USER_LED_A each 200 periods (100ms), while a callback related to the TimerF
	of HRTIM1 is configured to blink the led USER_LED_A each 200 periods (100ms).
	So, both user leds will blink simultaneously each 100ms. Moreover, both
	outputs of TimerF can me measured using an oscilloscope.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Connect TP52 (LED_1) with GPIOE[0]  (USER_LED_B)
	3. Use GPIOD[8] as output1 of TimerF
	4. Use GPIOD[7] as output1 of TimerF
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9]  (USER_LED_A)
	2. Connect TP52 (LED_1) with GPIOE[10] (USER_LED_B)
	3. Use GPIOD[8] as output1 of TimerF
	4. Use GPIOD[7] as output1 of TimerF
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
	2. Close jumper JP36 (connect LED7 with GPIOD[15])
	3. Open jumper JP21
	4. Use C24 (CON3-B) as output1 of TimerF
	5. Use D17 (CON3-B) as output1 of TimerF
-----------------------------------------------------------
