# StellarE SDK MULTICORE_01

## Dual core test

### Description:
	MULTICORE_01 shows how to run core2 from core1. It allows to compile an
	application (A1) for core1, that blinks the USER_LED_A each 250ms and an
	application (A2) for core2, that blinks the USER_LED_B each 250ms. The
	application A1 on core1 will start core2.
	A1 is designed to be executed from the core1 flash (0x08000000).
	A2 is designed to be executed from the core2 flash (0x080F0000).
	Binary of A1 must be programmed in core1 flash (0x08000000), while binary
	of A2 must be programmed in core2 flash (0x080F0000).
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