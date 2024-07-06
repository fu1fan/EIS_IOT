# StellarE SDK TIM_TS_01

## TIM_TS test

### Description:
	TIM_TS_01 shows how to configure and use the TIM_TS to generate an interrupt
	on an update event. Configuring the prescaler to 65535 and the auto-reload
	register to 2289 an update event is generated with a frequency of 1Hz,
	blinking the USER_LED_A each second.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9]  (USER_LED_A)
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
-----------------------------------------------------------