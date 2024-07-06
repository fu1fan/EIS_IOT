# StellarE SDK TIM_03

## TIM test

### Description: 
	TIM_03 generates a PWM waveform on ch1 of TIM1 using the DMA. In
	particular, DMA updates the value of the duty cycle in each period based
	on the values of the DMA source buffer. The DMA source buffer contains
	4 elements that specify the value of the duty cycle in 4 consecutive
	periods. The initial values corresponds to the following duty cycles: 20%,
	40%, 60% and 80%. Moreover, the frequency of the PWM waveform is set to
	4Hz.  A callback related to the ch1 of TIM1 updates the values of the duty
	cycles after their usage for the next DMA transfer cycle. Moreover, the
	test configures the ch2 of TIM1 as input capture via DMA. A DMA callback 
	related to ch2 of TIM1 is defined and will be invoked when the DMA transfer
	on the ch2 is completed. Then the values captured by the ch2 of TIM1 are
	compared with the values transmitted by the ch1 of TIM1. If no error
	occurs, the USER_LED_A is blinked.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Connect GPIOC[15] (PWM out) with GPIOD[1] (ICU in)
#### EVBE3000P Rev.A
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[9] (USER_LED_A)
	2. Connect GPIOI[0] (PWM out) with GPIOI[2] (ICU in)
#### EVBE7000E
	Requirements:
	1. Close jumper JP35 (connect LED6 with GPIOD[14])
	2. Connect C10 (CON3-B, PWM out) with A11 (CON3-A, ICU in)
-----------------------------------------------------------