################################################################################
# StellarESDK module makefile.
################################################################################

#
# C sources
#
MODULE_C_SRCS := \
	src/croutine.c \
	src/event_groups.c \
	src/list.c \
	src/queue.c \
	src/stream_buffer.c \
	src/tasks.c \
	src/timers.c \
	src/FreeRTOS_CLI.c \
	portable/MemMang/heap_4.c

ifeq ($(TOOLCHAIN), $(filter $(TOOLCHAIN),ARM HIGHTEC))
MODULE_C_SRCS += \
	portable/GCC/ARM_CM4F/port.c
else
ifeq ($(TOOLCHAIN), GHS)
MODULE_C_SRCS += \
	portable/GHS/ARM_CM4F/port.c
else
MODULE_C_SRCS += \
	portable/IAR/ARM_CM4F/port.c
endif
endif

#
# Assembly sources
#
ifeq ($(TOOLCHAIN), GHS)
MODULE_A_SRCS := \
	portable/GHS/ARM_CM4F/portasm.s
else
ifeq ($(TOOLCHAIN), IAR)
MODULE_A_SRCS := \
	portable/IAR/ARM_CM4F/portasm.s
endif
endif

#
# C include directories
#
ifeq ($(TOOLCHAIN), $(filter $(TOOLCHAIN),ARM HIGHTEC))
MODULE_C_INCS := \
	include \
	portable/GCC/ARM_CM4F
else
ifeq ($(TOOLCHAIN), GHS)
MODULE_C_INCS := \
	include \
	portable/GHS/ARM_CM4F
else
MODULE_C_INCS := \
	include \
	portable/IAR/ARM_CM4F
endif
endif

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
	BUILD_OS_FREERTOS

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
