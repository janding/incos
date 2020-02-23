kernel_SRCS += $(call collect-srcs, $(CURRENT_DIRECTORY))
kernel_BINS += $(BUILD_DIR)/incos-$(ARCH)-kernel

kernel_LDSCRIPT := $(CURRENT_DIRECTORY)kernel.ld

include kernel/arch/$(ARCH)/boot/Makefile
include kernel/arch/$(ARCH)/vm/Makefile
