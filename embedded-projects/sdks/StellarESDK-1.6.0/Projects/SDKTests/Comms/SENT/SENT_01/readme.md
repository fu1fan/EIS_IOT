# StellarE SDK SENT_01

## SENT test

### Description:
	SENT_01 shows how to configure and use the SENT driver to receive frames of
	6 nibbles. A SENT sensor is emulated internally and sends data on a GPIO,
	while the SENT driver reads data and prints the result on UART.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect GPIOF[3] (SENT sensor TX) to GPIOC[15] (SENT driver RX)
	2. Close jumpers JP29 and JP30
	3. Connect host serial port to UART via USB_PORT (P26)
	4. Configure host serial port baudrate to 38400
#### EVBE3000P Rev.A
	Requirements:
	1. Connect GPIOF[3] (SENT sensor TX) to GPIOI[0] (SENT driver RX)
	2. Open jumpers JP29 and JP30
	3. Connect host serial port to UART via USB_PORT (P26)
	4. Configure host serial port baudrate to 38400
#### EVBE7000E
	Requirements:
	1. Connect B20 (CON3-A, SENT sensor TX) to C10 (CON3-B, SENT driver RX)
	2. Close jumpers JP23 and JP24
	3. Connect host serial port to UART via USB_PORT (CON4)
	4. Configure host serial port baudrate to 38400
-----------------------------------------------------------