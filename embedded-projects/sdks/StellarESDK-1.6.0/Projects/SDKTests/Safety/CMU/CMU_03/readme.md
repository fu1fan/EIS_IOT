# StellarE SDK CMU_03

## CMU test

### Description:
	CMU_03 shows how to configure and use the CMU0 peripheral to use the
	frequency meter. It configures the FCCU and CMU0 peripherals to monitor the
	frequency of the 16MHz IRCOSC. If the frequency is accurate, the USER_LED_B
	will be lit. Furthermore the USER_LED_A will blink every 500ms.

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