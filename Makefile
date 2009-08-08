ARCH ?= i386

TOOLCHAIN_PREFIX = i586-elf-

BUILD_TYPE = $(ARCH)
BUILD_DIR = ./build/$(BUILD_TYPE)

DEPFLAGS := -MMD -MP
INCLUDES := -I include -I include/arch/$(ARCH)

CFLAGS += -std=c99 -Wall -fno-exceptions -O3 -fomit-frame-pointer -fcaller-saves
CPPFLAGS += $(DEPFLAGS) $(INCLUDES)
LDFLAGS += -L$(dir $(shell $(CC) -print-libgcc-file-name)) -lgcc
kernel: LDFLAGS += -Map incos.map

CC := $(TOOLCHAIN_PREFIX)gcc
CXX := $(TOOLCHAIN_PREFIX)g++
LD := $(TOOLCHAIN_PREFIX)ld
AS := $(TOOLCHAIN_PREFIX)as
AR := $(TOOLCHAIN_PREFIX)ar

.phony: all
all: system

.phony: system
system: dirs kernel-$(ARCH) user

include mkfiles/macros.mk
include mkfiles/rules.mk


include kernel/Makefile
include user/Makefile

BINS := $(kernel_BINS) $(user_BINS) $(user_LIBS)
OBJS := $(kernel_OBJS) $(user_OBJS) $(extra_OBJS)

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
	
-include mkfiles/i386emu.mk
