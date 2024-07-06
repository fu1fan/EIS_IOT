################################################################################
# StellarESDK module makefile.
################################################################################

#
# C sources
#
ifeq ($(BUILD_OS_FREERTOS), 1)
MODULE_C_SRCS := \
	src/FreeRTOS_osal.c
else
MODULE_C_SRCS := \
	src/osal.c
endif

#
# Assembly sources
#
MODULE_A_SRCS :=

#
# C include directories
#
MODULE_C_INCS := \
	include

#
# Assembly include directories
#
MODULE_A_INCS :=

#
# Linker script include directories
#
MODULE_LS_INCS :=

#
# C defines
#
MODULE_C_DEFS := \
	BUILD_OS_OSAL

#
# Assembly defines
#
MODULE_A_DEFS :=

#
# C compiler options
#
MODULE_C_OPTS :=

#
# Assembly compiler options
#
MODULE_A_OPTS :=
