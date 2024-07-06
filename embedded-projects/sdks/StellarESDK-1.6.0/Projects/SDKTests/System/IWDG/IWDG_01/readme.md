# StellarE SDK IWDG_01

## IWDG test

### Description: 
	IWDG_01 shows how to configure and use the Indipendent Watchdog driver. In
	particular, since
	- IRCOSC = 16MHz
	- prescaler = 256
	- counter = 0xFFF
	- window = 0x7FF
	the watchdog timeout is 65.5ms while the time-window is 32.8ms. This means
	the watchdog will down-count from 65.5ms to 0 and the counter refresh will
	be allowed starting from 32.8ms, i.e. after (65.5 - 32.8) = 32.7ms by the
	enable of the watchdog. So, if the counter is refreshed each 35ms, no reset
	will be generated and led USER_LED_A will be blinked. Instead, if a reset
	is wrongly generated, on the restart the led USER_LED_B will be blinked.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Connect TP52 (LED_1) with GPIOE[0]  (USER_LED_B)
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9]  (USER_LED_A)
	2. Connect TP52 (LED_1) with GPIOE[10] (USER_LED_B)
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
	2. Close jumper JP36 (connect LED7 with GPIOD[15])
-----------------------------------------------------------