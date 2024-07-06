# StellarE SDK RTC_01

## RTC test

### Description:
	RTC_01 shows how to configure and use the Real Time Clock peripheral. The
	RTC is configured to start counting from 11:29:55 AM of 2023/04/07 (Monday)
	and to apply one hour decrement for daylight saving time. A wake-up timer
	is configured to generate an interrupt every 10 seconds. An alarm is
	configured to generate an interrupt at 10:30:00 AM of 2023/04/07 (Monday)
	matching the date and time. The time and date are printed on UART and the
	USER_LED_A is blinked every second. When the wake-up timer is triggered, a
	message is printed on UART. When the alarm is triggered, a message is
	printed on UART and USER_LED_B is lit.
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