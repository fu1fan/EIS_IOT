# StellarE SDK Tests


-----------------------------------------------------------
## Analog tests
-----------------------------------------------------------

### COMP tests
	COMP_01        COMP: simple test

### DAC tests
	DAC_01         DAC: simple test

### SARADC tests
	SARADC_01      SARADC: regular conversion in continuos mode
	SARADC_02      SARADC: regular conversion in single mode with trigger
	SARADC_03      SARADC: regular conversion in single mode + injected conversion with trigger
	SARADC_04      SARADC: regular conversion in continuos mode + injected conversion
	SARADC_05      SARADC: analog watchdog thresholds
	SARADC_06      SARADC: dual mode
	SARADC_07      SARADC: self test

### SDADC tests
	SDADC_01       SDADC: simple test
	SDADC_02       SDADC: timestamp test

### TSENS tests
	TSENS_01       TSENS: simple test

-----------------------------------------------------------
## Benchmarks tests
-----------------------------------------------------------

### COREMARK tests
	COREMARK_01    COREMARK: single core

-----------------------------------------------------------
## Comms tests
-----------------------------------------------------------

### CAN tests
	CAN_01         CAN: simple test
	CAN_02         CAN: DMU test
	CAN_03         CAN: FD frames test

### I2C tests
	I2C_01         I2C: master tx - slave rx, master rx - slave tx, interrupt mode
	I2C_02         I2C: master tx - slave rx, master rx - slave tx, DMA mode

### I2S tests
	I2S_01         I2S: master tx - slave rx

### SENT tests
	SENT_01        SENT: simple test

### SPI tests
	SPI_01         SPI: master tx - slave rx, DMA mode, full duplex
	SPI_02         SPI: master rx - slave tx, DMA mode, full duplex
	SPI_03         SPI: multi-slave, master tx - slaves rx, DMA mode, full duplex
	SPI_04         SPI: master tx - slave rx, interrupt mode, full duplex
	SPI_05         SPI: master rx - slave tx, interrupt mode, full duplex
	SPI_06         SPI: master tx - slave rx, DMA mode, half duplex
	SPI_07         SPI: master rx - slave tx, DMA mode, half duplex
	SPI_08         SPI: master tx - slave rx, interrupt mode, half duplex
	SPI_09         SPI: master rx - slave tx, interrupt mode, half duplex

### UART tests
	UART_01        UART: "Hello World!" test

-----------------------------------------------------------
## Graphics tests
-----------------------------------------------------------

### ILI9341 tests
	ILI9341_01     ILI9341: simple test

-----------------------------------------------------------
## Memories tests
-----------------------------------------------------------

### EED tests
	EED_01         EED: simple test
### FLASH tests
	FLASH_01       FLASH: simple test

-----------------------------------------------------------
## Miscellaneous tests
-----------------------------------------------------------

	MISC_01        MISC: clockout test

-----------------------------------------------------------
## MultiCore tests
-----------------------------------------------------------

	MULTICORE_01   MULTICORE: simple test
	MULTICORE_02   MULTICORE: simple test (single elf)
	MULTICORE_03   MULTICORE: SEV test (single elf)

-----------------------------------------------------------
## OS tests
-----------------------------------------------------------

### FREERTOS tests
	FREERTOS_01    FREERTOS: simple test

-----------------------------------------------------------
## Safety tests
-----------------------------------------------------------

### CEM tests
	CEM_01         CEM: simple test
### CMU tests
	CMU_01         CMU: simple test (SYSCLK check)
	CMU_02         CMU: simple test (SARADC clock check)
	CMU_03         CMU: simple test (IRCOSC clock metering)
### FCCU tests
	FCCU_01        FCCU: simple test

-----------------------------------------------------------
## System tests
-----------------------------------------------------------

### CORDIC tests
	CORDIC_01      CORDIC: simple test

### CRC tests
	CRC_01         CRC: simple test
	CRC_02         CRC: DMA mode

### DMA tests
	DMA_01         DMA: RAM to DTCM
	DMA_02         DMA: RAM to RAM with DCache enabled (cache invalidate)
	DMA_03         DMA: RAM to RAM with DCache enabled (MPU setting)

### EXTI tests
	EXTI_01        EXTI: simple test

### GPIO tests
	GPIO_01        GPIO: LED blinking

### HSEM tests
	HSEM_01        HSEM: simple test

### IWDG tests
	IWDG_01        IWDG: simple test

### MPU tests
	MPU_01         MPU: simple test
	MPU_02         MPU: simple test

### SMPU tests
	SMPU_01        SMPU: simple test

### WWDG tests
	WWDG_01        WWDG: simple test

-----------------------------------------------------------
## Timers tests
-----------------------------------------------------------

### HRTIM tests
	HRTIM_01       HRTIM: simple test

### RTC tests
	RTC_01         RTC: simple test

### TIM tests
	TIM_01         TIM: simple test
	TIM_02         TIM: duty and frequency measurement
	TIM_03         TIM: DMA mode

### TIM_TS tests
	TIM_TS_01      TIM_TS: simple test