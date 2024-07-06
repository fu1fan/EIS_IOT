################################################################################
# StellarESDK module makefile.
################################################################################

# Currently FLASH driver only supports GCC and GHS compilers
#ifneq ($(TOOLCHAIN), $(filter $(TOOLCHAIN), ARM GHS))
#$(error FLASH driver does not support this compiler)
#endif

#
# C sources
#
MODULE_C_SRCS := \
	src/flash.c \
	ssd/$(CONFIG_DEVICE)/src/GetLock.c \
	ssd/$(CONFIG_DEVICE)/src/SetLock.c \
	ssd/$(CONFIG_DEVICE)/src/FlashCheckStatus.c \
	ssd/$(CONFIG_DEVICE)/src/FlashProgram.c \
	ssd/$(CONFIG_DEVICE)/src/ProgramVerify.c \
	ssd/$(CONFIG_DEVICE)/src/FlashInit.c \
	ssd/$(CONFIG_DEVICE)/src/FlashErase.c \
	ssd/$(CONFIG_DEVICE)/src/BlankCheck.c

#
# Assembly sources
#
MODULE_A_SRCS :=

#
# C include directories
#
MODULE_C_INCS := \
	include \
	ssd/$(CONFIG_DEVICE)/inc

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
	BUILD_DRIVERS_FLASH

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

