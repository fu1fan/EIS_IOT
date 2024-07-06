# StellarE SDK LIN_01

## LIN test (master tx - slave rx, master rx - slave tx)

### Description: 
	LIN_01 shows how to transmit data from LIN master to LIN slave and how to
	transmit data from LIN slave to LIN master. In particular, two different ID
	are used: for ID = 0x5 the LIN slave will receive 4bytes from the LIN
	master, while for ID = 0xA the slave will transmit 4bytes to the master.
	The data received is compared with the data transmitted. If no error occurs
	during the data transferring, the USER_LED_A is blinked.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Open jumpers JP29 and JP30
	3. Connect GPIOH[14] (LIN master RX) with GPIOF[3] (LIN slave TX)
	4. Connect GPIOH[15] (LIN master TX) with GPIOF[2] (LIN slave RX)
-----------------------------------------------------------