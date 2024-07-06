# StellarE SDK EED_01

## EED test

### Description: 
	EED_01 shows how to configure and use the EEPROM Emulation Driver. It
	performs a lot of operations (read/write/delete) on the emulated EEPROM.
	The information related to the operation on-going is printed on the UART.
	If the led USER_LED_B blinks, an error occurred, otherwise, when all the
	operation are completed, the led USER_LED_A will blink.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Connect TP52 (LED_1) with GPIOE[0]  (USER_LED_B)
	3. Close jumpers JP29 and JP30
	4. Connect host serial port to UART via USB_PORT (P26)
	5. Configure host serial port baudrate to 38400
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9]  (USER_LED_A)
	2. Connect TP52 (LED_1) with GPIOE[10] (USER_LED_B)
	3. Open jumpers JP29 and JP30
	4. Connect host serial port to UART via USB_PORT (P26)
	5. Configure host serial port baudrate to 38400
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
	2. Close jumper JP36 (connect LED7 with GPIOD[15])
	3. Close jumpers JP23 and JP24
	4. Connect host serial port to UART via USB_PORT (CON4)
	5. Configure host serial port baudrate to 38400
-----------------------------------------------------------