################################################################################
# Project makefile.
################################################################################

# 'Stellar E SDK' project CommonBuild dir
STELLAR_E_SDK_RELATIVE_PATH := ../../../..
PROJECT_COMMON_DIR := $(STELLAR_E_SDK_RELATIVE_PATH)/Projects/SDKTests/CommonBuild

include $(PROJECT_COMMON_DIR)/make/test_defs.mk

################################################################################
# Project builds
################################################################################
BUILD_DRIVERS_EXTI             := 1

################################################################################
# Add project files
################################################################################

# Application name
APP_NAME := out

# C sources
ifeq ($(CONFIG_TARGET_CORE), core1)
C_SRCS += \
	src/main_core1.c
else
C_SRCS += \
	src/main_core2.c
endif

ifneq ($(TOOLCHAIN), IAR)
# Assemby sources
ifeq ($(CONFIG_TARGET_CORE), core1)
A_SRCS += \
	src/core2_bin.S
endif

# Assembly defines
A_DEFS += \
	CORE2_BIN_FILE=\"build/$(CONFIG_DEVICE)/$(CONFIG_BOARD)/core2/$(TARGET_BUILD)/$(APP_NAME).bin\"
else
ifeq ($(CONFIG_TARGET_CORE), core1)
# Linker options
L_OPTS += \
	--image_input=build/$(CONFIG_DEVICE)/$(CONFIG_BOARD)/core2/$(TARGET_BUILD)/$(APP_NAME).bin,core2_bin_start,core2_bin,1
endif
endif

################################################################################
# Linker file
################################################################################

# Linker file
LINKER_SCRIPT_SOURCE := ./rsc/$(LINKER_SCRIPT_NAME).E

################################################################################
# Include 'Stellar E SDK' top level makefile
################################################################################
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/StellarESDK.mk

ifneq ($(TOOLCHAIN), IAR)
ifeq ($(CONFIG_TARGET_CORE), core2)
pre-build::
	-$(AT)$(RM) build/$(CONFIG_DEVICE)/$(CONFIG_BOARD)/core1/$(TARGET_BUILD)/src/core2_bin.o
endif
endif

################################################################################
# Define 'Stellar E SDK' compiling target
################################################################################
all:
	$(AT)$(MAKE) CONFIG_TARGET_CORE=core2 CONFIG_TARGET_MEMORY=nvm default-all
	$(AT)$(MAKE) CONFIG_TARGET_CORE=core1 CONFIG_TARGET_MEMORY=nvm default-all

