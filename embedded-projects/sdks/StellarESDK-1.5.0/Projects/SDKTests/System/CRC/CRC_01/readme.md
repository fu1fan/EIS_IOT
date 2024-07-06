# StellarE SDK CRC_01

## CRC test

### Description: 
	CRC_01 shows how to configure and use the Cyclic Redundancy Check
	peripheral. After configuring the peripheral, the CRC checksum of a 39 
	elements buffer is calculated in two ways: a one shot test passing the
	whole buffer and a two shots test passing the buffer in two times. If the
	result corresponds to the expected value, the USER_LED_A will blink  every
	500ms.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9]  (USER_LED_A)
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
-----------------------------------------------------------