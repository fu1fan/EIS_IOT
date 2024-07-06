# StellarE SDK MPU_01

## MPU example

### Description: 
	MPU_01 configures 3 MPU regions:
	- Region0: RAM, 128k, R/W
	- Region1: Flash, 512k, R/W
	- Region2: ITCM, 32k, No access
	Since the access permissions of ITCM are configured as no access, when the
	application runs the function user_led_toggling() from the ITCM, a Memory
	Management Unit fault will be generated. The related handler will switch-on
	the USER_LED_B to report the fault, then will disable the MPU. This allows
	to run the function user_led_toggling() from the ITCM, that will blink the
	USER_LED_A.
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