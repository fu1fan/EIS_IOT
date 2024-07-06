# StellarE SDK SMPU_01

## SMPU Test

### Description:
	SMPU_01 configures 3 SMPU regions:
	- Region 0 (0x00000000 to 0x2403804F) with R/W access to all masters.
	- Region 1 (0x24038050 to 0x2403805F) with R access to all masters.
	- Region 2 (0x24038060 to 0xFFFFFFFF) with R/W access to all masters.
	After the configuration of the peripheral, a write access is done in region
	1 which is write protected, causing an hardfault. In the hardfault handler
	USER_LED_B will be toggled, and the access rights to region 1 will be
	updated to grant R/W access. Exiting the hardfault condition, the same
	write access will be done again without causing an hardfault. Furthermore,
	USER_LED_A will blink every 500ms.

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