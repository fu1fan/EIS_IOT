# StellarE SDK MPU_02

## MPU example

### Description: 
	MPU_02 configures 3 MPU regions:
	- Region0: RAM, 128k, R/W
	- Region1: Flash, 512k, R/W
	- Region2: ITCM, 32k, Access from privileged software only
	Since the access permissions of ITCM are configured only in privileged
	mode, when the application runs the function user_led_toggling() from the
	ITCM, a Memory Management Unit fault will be generated if the core is not
	in privileged mode. In particular, after the boot the core is in privileged
	mode, so the function user_led_toggling() is executed without fault and the
	USER_LED_A is switched on. After 500ms the core is moved in unprivileged
	mode, then the function user_led_toggling() is invoked again. A Memory
	Management Unit fault is generated. The related handler switches on the
	USER_LED_B and disables the MPU. Then, after 500ms, the application moves
	back the core in privileged mode using a supervisor call instruction and
	the MPU is re-enabled. Since the core is again in privileged mode, the next
	calls to the function user_led_toggling() will not generate any fault. So,
	if the test is correctly executed, the USER_LED_A is switched on, then,
	after 500ms, the USER_LED_B is switched on, and finally, after 500ms, the
	USER_LED_A is toggled each 250ms.
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