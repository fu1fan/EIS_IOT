################################################################################
# StellarESDK module makefile.
################################################################################

#
# C sources
#
MODULE_C_SRCS := \
	src/tim_ts.c \
	src/$(CONFIG_DEVICE)/tim_ts_instances.c \
	src/$(CONFIG_DEVICE)/tim_ts_dev.c

#
# Assembly sources
#
MODULE_A_SRCS :=

#
# C include directories
#
MODULE_C_INCS := \
	include \
	include/$(CONFIG_DEVICE)

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
	BUILD_DRIVERS_TIM_TS

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
