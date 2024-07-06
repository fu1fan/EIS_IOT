################################################################################
# Project makefile.
################################################################################

# 'Stellar E SDK' project CommonBuild dir
STELLAR_E_SDK_RELATIVE_PATH := ../../../../..
PROJECT_COMMON_DIR := $(STELLAR_E_SDK_RELATIVE_PATH)/Projects/SDKTests/CommonBuild

include $(PROJECT_COMMON_DIR)/make/test_defs.mk

################################################################################
# Project builds
################################################################################
BUILD_DRIVERS_HSEM             := 1

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

################################################################################
# Include 'Stellar E SDK' top level makefile
################################################################################
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/StellarESDK.mk

################################################################################
# Define 'Stellar E SDK' compiling target
################################################################################
all:
	$(AT)$(MAKE) CONFIG_TARGET_CORE=core1 CONFIG_TARGET_MEMORY=nvm default-all
	$(AT)$(MAKE) CONFIG_TARGET_CORE=core2 CONFIG_TARGET_MEMORY=nvm default-all
