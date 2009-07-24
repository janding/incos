ARCH ?= i386

TOOLCHAIN_PREFIX = i586-elf-

BUILD_TYPE = $(ARCH)
BUILD_DIR = ./build/$(BUILD_TYPE)

DEPFLAGS := -MMD -MP
INCLUDES := -I include -I include/arch/$(ARCH)

CFLAGS += $(DEPFLAGS) $(INCLUDES) -std=c99 -Wall -fno-exceptions
CXXFLAGS += $(DEPFLAGS) $(INCLUDES) -Wall -fno-exceptions
LDFLAGS += -L$(dir $(shell $(CC) -print-libgcc-file-name)) -lgcc
kernel: LDFLAGS += -Map incos.map

ASFLAGS += -g
CFLAGS += -g
CXXFLAGS += -g

CC := $(TOOLCHAIN_PREFIX)gcc
CXX := $(TOOLCHAIN_PREFIX)g++
LD := $(TOOLCHAIN_PREFIX)ld
AS := $(TOOLCHAIN_PREFIX)as

.phony: all
all: system

.phony: system
system: dirs kernel-$(ARCH) user

include mkfiles/macros.mk
include mkfiles/rules.mk


include kernel/Makefile
include user/Makefile

BINS := $(kernel_BINS) $(user_BINS)
OBJS := $(kernel_OBJS) $(user_OBJS)

DEPS := $(call objs-to-deps,$(OBJS))

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
-include $(DEPS)
endif
endif

DIRS := $(sort $(dir $(OBJS)))
.phony: dirs
dirs: $(DIRS)
$(DIRS): %:
	mkdir -p $@



.phony: clean
clean:
	rm -f $(BINS)
	rm -f $(OBJS)
	rm -f $(DEPS)

.phony: distclean
distclean:
	rm -rf ./build/*
	

include mkfiles/i386emu.mk
