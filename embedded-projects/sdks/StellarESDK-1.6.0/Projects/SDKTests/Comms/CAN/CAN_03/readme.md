# StellarE SDK CAN_03

## CAN test

### Description: 
	CAN_03 transmits CAN FD frames from CAN TX to CAN RX. The frames are
	transmitted via TX FIFO and received via RX FIFO1. A new transmission is
	run each 500ms. If the trasnmission is completed, the USER_LED_A is
	blinked. The frame received from the CAN RX is compared with the frame
	transmitted by the CAN TX. If no error occurs during the data transferring,
	the USER_LED_B is blinked. So, if the communication between the CAN TX and
	the CAN RX is correctly executed, USER_LED_A and USER_LED_B will blink
	simultaniously each 500ms.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Connect TP52 (LED_1) with GPIOE[0]  (USER_LED_B)
	3. Connect CAN connector J8 with CAN connector J9 using a CAN cable or
	   connect pins CAN_L (pin 2 of J8) and CAN_H (pin 7 of J8) of CAN TX
	   with pins CAN_L (pin 2 of J9) and CAN_H of CAN RX (pin 7 of J9)
	4. Open jumpers JP19, J24
	5. Close jumpers JP20, JP23, JP25, JP28
	6. Open jumpers JP21 and JP22, and cross connect pin 1 of JP21 with pin 2
	   of JP22 and pin 2 of JP21 with pin 1 of JP22.
	7. Open jumpers JP26 and JP27, and cross connect pin 1 of JP26 with pin 2
	   of JP27 and pin 2 of JP26 with pin 1 of JP27.
#### EVBE7000P Rev.B or later
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Connect TP52 (LED_1) with GPIOE[0]  (USER_LED_B)
	3. Connect CAN connector J8 with CAN connector J9 using a CAN cable or
	   connect pins CAN_L (pin 2 of J8) and CAN_H (pin 7 of J8) of CAN TX
	   with pins CAN_L (pin 2 of J9) and CAN_H of CAN RX (pin 7 of J9)
	4. Open jumpers JP19, J24
	5. Close jumpers JP20, JP21, JP22, JP23, JP25, JP26, JP27, JP28
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9]  (USER_LED_A)
	2. Connect TP52 (LED_1) with GPIOE[10] (USER_LED_B)
	3. Connect CAN connector J8 with CAN connector J9 using a CAN cable or
	   connect pins CAN_L (pin 2 of J8) and CAN_H (pin 7 of J8) of CAN TX
	   with pins CAN_L (pin 2 of J9) and CAN_H of CAN RX (pin 7 of J9)
	4. Open jumpers JP19, J24
	5. Close jumpers JP20, JP21, JP22, JP23, JP25, JP26, JP27, JP28
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
	2. Close jumper JP36 (connect LED7 with GPIOD[15])
	3. Connect CAN connector CON6 with CAN connector CON7 using a CAN cable or
	   connect pins CAN_L (pin 2 of CON6) and CAN_H (pin 7 of CON6) of CAN TX
	   with pins CAN_L (pin 2 of CON7) and CAN_H of CAN RX (pin 7 of CON7)
	4. Close jumpers JP15, JP16 on position 2-3
	5. Close jumpers JP13, JP14, JP17, JP18
-----------------------------------------------------------