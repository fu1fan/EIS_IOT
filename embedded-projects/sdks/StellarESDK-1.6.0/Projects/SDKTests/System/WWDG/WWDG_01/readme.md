# StellarE SDK WWDG_01

## WWDG test

### Description: 
	WWDG_01 shows how to configure and use the System Window Watchdog driver.
	In particular, since
	- PCLK1 = 75MHz (set as default in Clock module)
	- prescaler = 128
	- counter = 0x7F
	- window = 0x5F
	the watchdog timeout is 859ms while the time-window is 669ms. This means
	the watchdog will down-count from 859ms to 451ms (0x40) and the counter
	refresh will be allowed starting from 669ms, i.e. after (859 - 669) = 190ms
	by the enable of the watchdog. So, if the counter is refreshed each 230ms,
	no reset will be generated and led USER_LED_A will be blinked. Instead, if
	a reset is wrongly generated, on the restart the led USER_LED_B will be
	blinked.
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