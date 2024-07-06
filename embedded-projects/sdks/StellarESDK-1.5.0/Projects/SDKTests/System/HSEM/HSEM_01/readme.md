# StellarE SDK HSEM_01

## Hardware semaphore test

### Description:
	HSEM_01 shows how to configure and use the hardware semaphores. It allows
	to compile an application (A1) for core1 and an application (A2) for core2.
	A1 locks the semaphore 30, then run the core 2, blinks USER_LED_A each
	250ms for 4 times, then unlockes the semaphore 30. The semaphore module is
	configured to generate an interrupt when the semaphore 30 is unlocked. The
	related callback will decrease the toggling frequency of USER_LED_A (from
	250ms to 500ms). A2 waits till the semaphore 30 is unlocked, then will 
	blink the USER_LED_B each 500ms.
	So, the normal behaviuor is that the USER_LED_A will blink for 2 seconds
	each 250ms, then will blink each 500ms. Instead, the USER_LED_B will start
	to blink after 2 seconds each 500ms. 
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