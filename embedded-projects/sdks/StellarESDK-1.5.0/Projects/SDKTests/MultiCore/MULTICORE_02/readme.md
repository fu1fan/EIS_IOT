# StellarE SDK MULTICORE_02

## Dual core test (single elf)

### Description:
	MULTICORE_02 shows how to create a unique elf file containing both binaries
	of core1 and core2. The project must be compiled for flash. In particular,
	it allows to compile both applications for core1 (A1) and for core2 (A2).
	A1 blinks the USER_LED_A each 250ms and is compiled to be executed from the
	core1 flash (0x08000000). A2 blinks the USER_LED_B each 250ms and is
	compiled to be executed from the core2 flash (0x080F0000). The application
	A1 on core1 will start core2. To include in the elf file of core1 also the
	binary of A2, a custom version of the linker files (in folder rsc) are
	used. They include for core1 a new section core2_bin. The start address of
	core2_bin is exactly the core2 flash start address (0x080F0000), i.e. the
	address for which A2 has been compiled. The build system of this project is
	designed to build A2 first, then A1. In this way, during the linking of A1,
	the binary of A2 (already generated) will be placed in the section
	core2_bin, so the elf file of core1 will include both binaries. Download on
	the target only the elf of core1.
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