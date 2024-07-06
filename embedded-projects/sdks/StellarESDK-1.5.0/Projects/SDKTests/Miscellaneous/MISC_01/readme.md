# StellarE SDK MISC_01

## Clockout test

### Description: 
	MISC_01 outputs the clockout. Since
	- MCOSEL = PLL1PHI
	- MCOPRE = 60
	- PLL1 frequency = 300MHz
	the CLKOUT frequency measured to CLKOUT pin has to be 2.5MHz.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Clockout is output on pin GPIOG[12]
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9]  (USER_LED_A)
	2. Clockout is output on pin GPIOG[12]
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
	2. Clockout is output on pin C25 (CON3-B)
-----------------------------------------------------------