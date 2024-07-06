################################################################################
# Project makefile.
################################################################################

# 'Stellar SDK' project values
PROJECTNAME := fireware
SDKID := StellarESDK-1.5.0

# Please Update it if you move your workspace
STELLAR_E_SDK_RELATIVE_PATH := ../sdks/$(SDKID)
TEST_ROOT_DIR := $(STELLAR_E_SDK_RELATIVE_PATH)
PROJECT_COMMON_DIR := $(STELLAR_E_SDK_RELATIVE_PATH)/Projects/SDKTests/CommonBuild
CONFIG_BOARD := Config

include $(PROJECT_COMMON_DIR)/make/test_defs.mk

################################################################################
# Project builds
################################################################################
BUILD_OS_OSAL                  := 1

BUILD_DRIVERS_GPIO             := 1
BUILD_DRIVERS_EXTI             := 1
BUILD_DRIVERS_SARADC           := 1
BUILD_DRIVERS_CORDIC           := 1

BUILD_DRIVERS_DMA              := 1
BUILD_DRIVERS_UART             := 1
BUILD_DRIVERS_SPI              := 1
BUILD_DRIVERS_CAN              := 1

BUILD_DRIVERS_TIM              := 1
BUILD_DRIVERS_HRTIM            := 1
BUILD_DRIVERS_TIM_TS           := 1

################################################################################
# Add project files
################################################################################

# Application name
APP_NAME := $(PROJECTNAME)

# C sources
C_SRCS += \
	src/main.c \
	\
	driver_code/debug_uart.c \
	driver_code/hrtim_pwm.c \
	driver_code/sradc_sample.c \
	driver_code/timts_pit.c \
	driver_code/system_init.c \
	driver_code/spi_config.c \
	driver_code/can_config.c \
	driver_code/motor_exti_config.c \
	driver_code/zf_common_fifo.c \
	\
	device_code/zf_device_tft180.c \
	\
	user_code/zf_common_font.c \
	user_code/ads1256.c \
	user_code/dac1220.c \
	user_code/ad7606.c \
	\
	user_code/soft_fft.c \
	\
	user_code/can_remote.c \
    \
    user_code/eis_measure.c \
    user_code/eis_calculate.c \
    \
    user_code/tft_ui.c \
    user_code/lte.c \
    user_code/lte_uart.c \
    \
#	\
#	cmsis/source/TransformFunctions/arm_cfft_f32.c \
#	cmsis/source/TransformFunctions/arm_cfft_radix8_f32.c \
#	cmsis/source/TransformFunctions/arm_bitreversal2.c \
#	cmsis/source/ComplexMathFunctions/arm_cmplx_mag_f32.c \
#	cmsis/source/FastMathFunctions/arm_atan2_f32.c \
#	cmsis/source/StatisticsFunctions/arm_max_f32.c \
#	cmsis/source/CommonTables/arm_common_tables.c \
#	cmsis/source/CommonTables/arm_const_structs.c \


# Add entries to the files in user_code, as if they were files above
	
# C includes
# PLEASE UPDATE IT FOR GENERATED CODE
# Open when an .gpio file has been added to open this include
# Add src-gen/... when an .Sr5e1_clk file has been added to open this include
# Add cfg/ when any driver config file has been added to open this include
C_INCS += \
	src-gen/ \
	src-gen/sr5e1 \
	\
	driver_code/ \
	device_code/ \
	seekfree_assistant/ \
	user_code/ \
	\
	cmsis/include/ \
	cmsis/include/dsp \
	\

LIBS = -lm

################################################################################
# Include 'Stellar SDK' top level makefile
################################################################################
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/StellarESDK.mk

################################################################################
# Define 'Stellar E SDK' compiling target
################################################################################
all: default-all


