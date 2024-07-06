# StellarE SDK DMA_01

## DMA Test

### Description:
	DMA_01 shows how to configure the DMA to transfer a buffer from RAM to DTCM
	(memory to memory) using the indirect address (DMA can access to the the
	DTCM only through its indirect address). The USER_LED_B will be switched on
	when the DMA transfer is completed. If DMA transfer is correctly completed,
	the USER_LED_A will blink every 250ms.

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