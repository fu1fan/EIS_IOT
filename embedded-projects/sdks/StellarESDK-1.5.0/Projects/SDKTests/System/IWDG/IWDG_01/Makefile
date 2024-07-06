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
BUILD_OS_OSAL                  := 1
BUILD_DRIVERS_FCCU             := 1
BUILD_DRIVERS_IWDG             := 1

################################################################################
# Add project files
################################################################################

# Application name
APP_NAME := out

# C sources
C_SRCS += \
	src/main.c

################################################################################
# Include 'Stellar E SDK' top level makefile
################################################################################
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/StellarESDK.mk

################################################################################
# Define 'Stellar E SDK' compiling target
################################################################################
all: default-all
