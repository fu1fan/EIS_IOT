# StellarE SDK CMU_01

## CMU test

### Description:
	CMU_01 shows how to configure and use the CMU1 peripheral to use the clock
	supervisor. It configures the FCCU and CMU1 peripherals to monitor the
	Sysclk. Since the Sysclk will violate the high reference treshold, a fault
	on FCCU channel 16 alarm timeout will be triggered and USER_LED_B will be
	lit. Furthermore the USER_LED_A will blink every 500ms.

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