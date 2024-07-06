# StellarE SDK SARADC_05

## SARADC test

### Description:
	SARADC_05 shows how to configure SARADC thresholds to trigger the watchdogs
	when the sampled input is lower than 0.64V. WD1 monitors ADC CH1 and WD2
	monitors ADC CH2. Clicking PUSH_BUTTON_0 will short ADC CH1 to ground,
	violating the threshold and toggling the USER_LED_A. Clicking PUSH_BUTTON_1
	will short ADC CH2 to ground, violating the threshold and toggling the
	USER_LED_B.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Connect TP52 (LED_1) with GPIOE[0] (USER_LED_B)
	3. Connect GPIOB[2] (ADC CH1 input pin) to TP53 (PUSH_BUTTON_0)
	4. Connect GPIOB[3] (ADC CH2 input pin) to TP54 (PUSH_BUTTON_1)
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9] (USER_LED_A)
	2. Connect TP52 (LED_1) with GPIOE[10] (USER_LED_B)
	3. Connect GPIOB[2] (ADC CH1 input pin) to TP53 (PUSH_BUTTON_0)
	4. Connect GPIOB[3] (ADC CH2 input pin) to TP54 (PUSH_BUTTON_1)
-----------------------------------------------------------