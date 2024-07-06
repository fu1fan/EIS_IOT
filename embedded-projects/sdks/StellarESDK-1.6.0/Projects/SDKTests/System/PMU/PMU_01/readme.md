# StellarE SDK PMU_01

## PMU simple test

### Description: 
	PMU_01 shows how to configure and use the Power Management Unit. In
	particular, the PMU is configured to generate an interrupt when LVD290_AS
	detects a SAR ADC supply < 2.90V. The SAR ADC supply (VDD_HV_SAR) is
	connected with a GPIO that is normally set to VDD and that goes low each
	1s. When SAR ADC supply goes low, a PMU interrupt is generated that toggles
	the USER_LED_B and set high the SAR ADC supply again. The USER_LED_A is
	blinked each 250ms. So, the normal behaviuor is that the USER_LED_A will
	blink each 250ms while the USER_LED_B will blink each 1s.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Connect TP52 (LED_1) with GPIOE[0]  (USER_LED_B)
	3. Disconnect jumper between pins 7 and 8 of JP1
	4. Connect pin 8 (VDD_HV_SAR) of JP1 with GPIOA[0]
-----------------------------------------------------------