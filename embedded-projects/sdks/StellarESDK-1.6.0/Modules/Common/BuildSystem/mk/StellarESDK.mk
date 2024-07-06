################################################################################
# StellarESDK top level makefile.
################################################################################

#
# Modules' path 
#
BOOT_PATH                     := Modules/Boot

PLATFORM_BOARD_PATH           := Modules/Platform/Board
PLATFORM_CLOCK_PATH           := Modules/Platform/Clock
PLATFORM_CMSIS_PATH           := Modules/Platform/CMSIS
PLATFORM_IRQ_PATH             := Modules/Platform/IRQ
PLATFORM_MCU_PATH             := Modules/Platform/MCU

DRIVERS_DMA_PATH              := Modules/Drivers/System/DMA
DRIVERS_CORDIC_PATH           := Modules/Drivers/System/CORDIC
DRIVERS_CRC_PATH              := Modules/Drivers/System/CRC
DRIVERS_EXTI_PATH             := Modules/Drivers/System/EXTI
DRIVERS_GPIO_PATH             := Modules/Drivers/System/GPIO
DRIVERS_HSEM_PATH             := Modules/Drivers/System/HSEM
DRIVERS_IWDG_PATH             := Modules/Drivers/System/IWDG
DRIVERS_MPU_PATH              := Modules/Drivers/System/MPU
DRIVERS_PMU_PATH              := Modules/Drivers/System/PMU
DRIVERS_SMPU_PATH             := Modules/Drivers/System/SMPU
DRIVERS_WWDG_PATH             := Modules/Drivers/System/WWDG
DRIVERS_COMP_PATH             := Modules/Drivers/Analog/COMP
DRIVERS_DAC_PATH              := Modules/Drivers/Analog/DAC
DRIVERS_SARADC_PATH           := Modules/Drivers/Analog/SARADC
DRIVERS_SDADC_PATH            := Modules/Drivers/Analog/SDADC
DRIVERS_TSENS_PATH            := Modules/Drivers/Analog/TSENS
DRIVERS_CAN_PATH              := Modules/Drivers/Comms/CAN
DRIVERS_I2C_PATH              := Modules/Drivers/Comms/I2C
DRIVERS_I2S_PATH              := Modules/Drivers/Comms/I2S
DRIVERS_LIN_PATH              := Modules/Drivers/Comms/LIN
DRIVERS_SENT_PATH             := Modules/Drivers/Comms/SENT
DRIVERS_SPI_PATH              := Modules/Drivers/Comms/SPI
DRIVERS_UART_PATH             := Modules/Drivers/Comms/UART
DRIVERS_ILI9341_PATH          := Modules/Drivers/Graphics/ILI9341
DRIVERS_EED_PATH              := Modules/Drivers/Memories/EED
DRIVERS_FLASH_PATH            := Modules/Drivers/Memories/FLASH
DRIVERS_CEM_PATH              := Modules/Drivers/Safety/CEM
DRIVERS_CMU_PATH              := Modules/Drivers/Safety/CMU
DRIVERS_FCCU_PATH             := Modules/Drivers/Safety/FCCU
DRIVERS_DWT_PATH              := Modules/Drivers/Timers/DWT
DRIVERS_HRTIM_PATH            := Modules/Drivers/Timers/HRTIM
DRIVERS_RTC_PATH              := Modules/Drivers/Timers/RTC
DRIVERS_SYSTICK_PATH          := Modules/Drivers/Timers/SYSTICK
DRIVERS_TIM_PATH              := Modules/Drivers/Timers/TIM
DRIVERS_TIM_TS_PATH           := Modules/Drivers/Timers/TIM_TS

OS_OSAL_PATH                  := Modules/OS/OSAL
OS_FREERTOS_PATH              := Modules/OS/FreeRTOS
OS_FREERTOS_CONSOLE_PATH      := Modules/OS/Utilities/FreeRTOS/Console

RUNTIME_IO_PATH               := Modules/Runtime/IO

COMMON_BUILDSYSTEM_PATH       := Modules/Common/BuildSystem

ADDONS_LL_PATH                := Modules/Addons/LL

# Build system verbosity level
ifeq ($(VERBOSE), 0)
AT := @
else
AT :=
endif


FIND_DIR_BY_NAME = $(shell cd $(STELLAR_E_SDK_ROOT_DIR) && find $(1) -type d -name $(2))

# Find sources
ifeq ($(BUILD_BOOT), 1)
MODULE_FILE_MK := $(BOOT_PATH)/mk/boot.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_PLATFORM_BOARD), 1)
MODULE_FILE_MK := $(PLATFORM_BOARD_PATH)/mk/board.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_PLATFORM_CLOCK), 1)
MODULE_FILE_MK := $(PLATFORM_CLOCK_PATH)/mk/clock.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_PLATFORM_CMSIS), 1)
MODULE_FILE_MK := $(PLATFORM_CMSIS_PATH)/mk/cmsis.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_PLATFORM_IRQ), 1)
MODULE_FILE_MK := $(PLATFORM_IRQ_PATH)/mk/irq.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_PLATFORM_MCU), 1)
MODULE_FILE_MK := $(PLATFORM_MCU_PATH)/mk/mcu.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_DMA), 1)
MODULE_FILE_MK := $(DRIVERS_DMA_PATH)/mk/dma.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_CORDIC), 1)
MODULE_FILE_MK := $(DRIVERS_CORDIC_PATH)/mk/cordic.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_CRC), 1)
MODULE_FILE_MK := $(DRIVERS_CRC_PATH)/mk/crc.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_EXTI), 1)
MODULE_FILE_MK := $(DRIVERS_EXTI_PATH)/mk/exti.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_GPIO), 1)
MODULE_FILE_MK := $(DRIVERS_GPIO_PATH)/mk/gpio.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_HSEM), 1)
MODULE_FILE_MK := $(DRIVERS_HSEM_PATH)/mk/hsem.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_IWDG), 1)
MODULE_FILE_MK := $(DRIVERS_IWDG_PATH)/mk/iwdg.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_MPU), 1)
MODULE_FILE_MK := $(DRIVERS_MPU_PATH)/mk/mpu.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_PMU), 1)
MODULE_FILE_MK := $(DRIVERS_PMU_PATH)/mk/pmu.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_SMPU), 1)
MODULE_FILE_MK := $(DRIVERS_SMPU_PATH)/mk/smpu.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_WWDG), 1)
MODULE_FILE_MK := $(DRIVERS_WWDG_PATH)/mk/wwdg.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_COMP), 1)
MODULE_FILE_MK := $(DRIVERS_COMP_PATH)/mk/comp.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_DAC), 1)
MODULE_FILE_MK := $(DRIVERS_DAC_PATH)/mk/dac.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_SARADC), 1)
MODULE_FILE_MK := $(DRIVERS_SARADC_PATH)/mk/saradc.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_SDADC), 1)
MODULE_FILE_MK := $(DRIVERS_SDADC_PATH)/mk/sdadc.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_TSENS), 1)
MODULE_FILE_MK := $(DRIVERS_TSENS_PATH)/mk/tsens.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_CAN), 1)
MODULE_FILE_MK := $(DRIVERS_CAN_PATH)/mk/can.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_I2C), 1)
MODULE_FILE_MK := $(DRIVERS_I2C_PATH)/mk/i2c.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_I2S), 1)
MODULE_FILE_MK := $(DRIVERS_I2S_PATH)/mk/i2s.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_LIN), 1)
MODULE_FILE_MK := $(DRIVERS_LIN_PATH)/mk/lin.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_SENT), 1)
MODULE_FILE_MK := $(DRIVERS_SENT_PATH)/mk/sent.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_SPI), 1)
MODULE_FILE_MK := $(DRIVERS_SPI_PATH)/mk/spi.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_UART), 1)
MODULE_FILE_MK := $(DRIVERS_UART_PATH)/mk/uart.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_ILI9341), 1)
MODULE_FILE_MK := $(DRIVERS_ILI9341_PATH)/mk/ili9341.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_EED), 1)
MODULE_FILE_MK := $(DRIVERS_EED_PATH)/mk/eed.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_FLASH), 1)
MODULE_FILE_MK := $(DRIVERS_FLASH_PATH)/mk/flash.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_CEM), 1)
MODULE_FILE_MK := $(DRIVERS_CEM_PATH)/mk/cem.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_CMU), 1)
MODULE_FILE_MK := $(DRIVERS_CMU_PATH)/mk/cmu.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_FCCU), 1)
MODULE_FILE_MK := $(DRIVERS_FCCU_PATH)/mk/fccu.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_DWT), 1)
MODULE_FILE_MK := $(DRIVERS_DWT_PATH)/mk/dwt.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_HRTIM), 1)
MODULE_FILE_MK := $(DRIVERS_HRTIM_PATH)/mk/hrtim.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_RTC), 1)
MODULE_FILE_MK := $(DRIVERS_RTC_PATH)/mk/rtc.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

# SYSTICK modlue is automatically included when OSAL module is used
ifeq ($(BUILD_OS_OSAL), 1)
ifeq ($(BUILD_OS_FREERTOS), 0)
MODULE_FILE_MK := $(DRIVERS_SYSTICK_PATH)/mk/systick.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif
endif

ifeq ($(BUILD_DRIVERS_TIM), 1)
MODULE_FILE_MK := $(DRIVERS_TIM_PATH)/mk/tim.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_DRIVERS_TIM_TS), 1)
MODULE_FILE_MK := $(DRIVERS_TIM_TS_PATH)/mk/tim_ts.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_OS_OSAL), 1)
MODULE_FILE_MK := $(OS_OSAL_PATH)/mk/osal.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_OS_FREERTOS), 1)
MODULE_FILE_MK := $(OS_FREERTOS_PATH)/mk/freertos.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_OS_FREERTOS_CONSOLE), 1)
MODULE_FILE_MK := $(OS_FREERTOS_CONSOLE_PATH)/mk/console.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_RUNTIME_IO), 1)
MODULE_FILE_MK := $(RUNTIME_IO_PATH)/mk/io.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif

ifeq ($(BUILD_ADDONS_LL), 1)
MODULE_FILE_MK := $(ADDONS_LL_PATH)/mk/ll.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/module.mk
endif


C_SRCS_ABS := $(patsubst $(STELLAR_E_SDK_RELATIVE_PATH)/%, %, $(C_SRCS))
A_SRCS_ABS := $(patsubst $(STELLAR_E_SDK_RELATIVE_PATH)/%, %, $(A_SRCS))
A_SRCS_ABS_1 := $(filter %.S,$(A_SRCS_ABS))
A_SRCS_ABS_2 := $(filter %.s,$(A_SRCS_ABS))


C_OBJS = $(C_SRCS_ABS:%.c=$(TARGET_BUILD_DIR)/%.o)
A_OBJS = $(A_SRCS_ABS_1:%.S=$(TARGET_BUILD_DIR)/%.o)
A_OBJS += $(A_SRCS_ABS_2:%.s=$(TARGET_BUILD_DIR)/%.o)

C_DEPS = $(C_SRCS_ABS:%.c=$(TARGET_BUILD_DIR)/%.d)
A_DEPS = $(A_SRCS_ABS_1:%.S=$(TARGET_BUILD_DIR)/%.d)
A_DEPS += $(A_SRCS_ABS_2:%.s=$(TARGET_BUILD_DIR)/%.d)


################################################################################
# Toolchain section
################################################################################
ifneq (PATH_SEPARATOR,)
export PATH_SEPARATOR := :
endif

ifeq ($(TOOLCHAIN), ARM)
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/toolchain_arm_m7.mk
else ifeq ($(TOOLCHAIN), HIGHTEC)
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/toolchain_hightec_m7.mk
else ifeq ($(TOOLCHAIN), ARMCLANG)
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/toolchain_armclang_m7.mk
else ifeq ($(TOOLCHAIN), GHS)
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/toolchain_ghs_m7.mk
else ifeq ($(TOOLCHAIN), IAR)
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/toolchain_iar_m7.mk
else
$(error Wrong toolchain selection $(TOOLCHAIN))
endif


################################################################################
# Device Settings.
################################################################################

SUPPORTED_DEVICE := sr5e1

ifneq ($(CONFIG_DEVICE), $(filter $(CONFIG_DEVICE),$(SUPPORTED_DEVICE)))
$(error DEVICE <$(CONFIG_DEVICE)> NOT SUPPORTED)
endif

################################################################################
# Toolchain settings
################################################################################

ifneq (STELLAR_E_SDK_TOOLCHAIN_PATH,)
export PATH := $(STELLAR_E_SDK_TOOLCHAIN_PATH)$(PATH_SEPARATOR)${PATH}
endif

################################################################################
# Targets section
################################################################################
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/targets.mk
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/help.mk

################################################################################
# Clang and Misra section
################################################################################
-include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/clang.mk
-include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/misra.mk

################################################################################
# Packaging section
################################################################################
-include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/pack.mk

################################################################################
# Documentation section
################################################################################
-include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/docs.mk

################################################################################
# release note generation section
################################################################################
-include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/releaseNote.mk
