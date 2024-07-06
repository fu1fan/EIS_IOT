################################################################################
# Project makefile.
################################################################################

# 'Stellar E SDK' project common dir
STELLAR_E_SDK_RELATIVE_PATH := ../../../../..
PROJECT_COMMON_DIR := $(STELLAR_E_SDK_RELATIVE_PATH)/Projects/SDKTests/CommonBuild

include $(PROJECT_COMMON_DIR)/make/test_defs.mk

################################################################################
# Project builds
################################################################################
BUILD_DRIVERS_DMA              := 1
BUILD_DRIVERS_UART             := 1
BUILD_RUNTIME_IO               := 1

################################################################################
# Add project files
################################################################################

# Application name
APP_NAME := out

# C sources
C_SRCS += \
	src/main.c \
	src/benchmarks.c \
	src/coremark/core_list_join.c \
	src/coremark/core_main.c \
	src/coremark/core_matrix.c \
	src/coremark/core_state.c \
	src/coremark/core_util.c

# C includes
C_INCS += \
	include \
	include/coremark \

# C options
C_OPTS += \
	-DCORE_DEBUG=0 \
	-DCOMPILER_REQUIRES_SORT_RETURN=0

# C options (optimazation for performances improvement)
ifeq ($(TOOLCHAIN), ARM)
C_OPTS += \
	-funroll-loops \
	-finline-limit=1000
endif

################################################################################
# Include 'Stellar E SDK' top level makefile
################################################################################
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/StellarESDK.mk

################################################################################
# Define 'Stellar E SDK' compiling target
################################################################################
all: default-all
