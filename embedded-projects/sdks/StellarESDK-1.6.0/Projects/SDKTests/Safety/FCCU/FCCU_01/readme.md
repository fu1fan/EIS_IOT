# StellarE SDK FCCU_01

## FCCU test

### Description: 
	FCCU_01 shows how to configure and use the FCCU. The application is
	composed by 2 parts: the first part is executed after a power-on reset and
	configure the FCCU to detect 2 faults: the fault 0 is configured with the
	ALARM timeout enabled and with the ALARM timeout IRQ enabled, while fault 1
	is configured without ALARM timeout. Both faults are configued to generate
	a functional reset. Then, after 1.5sec (in which USER_LED_A is blinked),
	both faults are injected. So a functional reset will be generated. After
	the functional reset, the second part of the application will be executed.
	It will check the faults detected by FCCU. Since the callback related to
	the ALARM timeout IRQ of fault 0 clears it, only the fault 1 should be
	latched. If fault 0 is not latched and fault 1 is latched, the application
	will blink USER_LED_B.
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