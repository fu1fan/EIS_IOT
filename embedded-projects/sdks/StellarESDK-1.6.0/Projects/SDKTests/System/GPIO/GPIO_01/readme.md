# StellarE SDK GPIO_01

## Blinking LED

### Description: 
	GPIO_01 blinks USER_LED_A each 250ms.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9] (USER_LED_A)
#### EVBE7000E
	Requirements:
	1. Close jumpers JP35 (connect LED6 with GPIOD[14])
#### EVBE3000D
	Requirements:
	None. USER_LED_A (D12) already connected.
-----------------------------------------------------------