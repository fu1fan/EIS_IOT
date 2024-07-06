# StellarE SDK MULTICORE_03

## Dual core SEV test

### Description:
	MULTICORE_03 shows how to generate an interrupt on core1 using the Send
	Event instruction on core2. The project is designed to create a unique elf
	file containing both binaries of core1 and core2. In particular, the
	application for core1 (A1) is compiled to be executed from core1 flash
	(0x08000000), while the application for core2 (A2) is compiled to be
	executed from core2 flash (0x080F0000). The application A1 on core1 will
	start core2, than will wait for the interrupt Core2_SEV (Core2 Send Event
	interrupt to Core1 through EXTI line 45). The application A2, after 1 sec,
	will issue a Send Event instruction, than will blink the USER_LED_B each
	250ms. When the application A1 receives the interrupt Core2_SEV generated
	by the Send Event instruction on core2, it will start to blink the
	USER_LED_A each 250ms. So, if no errors occur, both leds will blink each
	250ms. Download on the target only the elf of core1.
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