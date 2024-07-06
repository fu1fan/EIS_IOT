# StellarE SDK CEM_01

## CEM test

### Description: 
	CEM_01 shows how to configure and use the CEM module in conjunction with
	the FCCU module. The application is composed by 2 parts: the first part is
	executed after a power-on reset and configure the FCCU to detect the fault
	34. In particular, the fault 34 is configured with the ALARM timeout and 
	with the ALARM timeout IRQ enabled and with a functional reset as reaction.
	Moreover, the CEM module related to the FCCU fault channel 34 is configured
	with its input fault channel 0 enabled. After 1.5sec (in which USER_LED_A
	is blinked), a fault is injected on the CEM input channel 0 previously
	enabled. This will generate a functional reset. After the functional reset,
	the second part of the application will be executed. It will check that the
	FCCU has correctly detected a fault on its channel 34. If so, USER_LED_B
	will be blinked.
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