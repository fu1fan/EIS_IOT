# StellarE SDK FLASH_01

## FLASH test

### Description: 
	FLASH_01 shows how to use the FLASH driver to modify the content of the
	FLASH. In particular, the test verify the block D0_00 (0x08F00000) is
	unlocked, then erase it, verify erasing using the blank check, then program
	to 0 the first 1KB and verify the programming. If the test is correclty
	executed, the USER_LED_A will blink, otherwise the USER_LED_B will blink.
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