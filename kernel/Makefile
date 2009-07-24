kernel_SRCS := $(call collect-srcs, $(CURRENT_DIRECTORY))
kernel_BINS := 
kernel_OBJS = $(call srcs-to-objs, $(kernel_SRCS))

include kernel/arch/$(ARCH)/Makefile
include kernel/libc/Makefile
include kernel/vm/Makefile

kernel-$(ARCH): $(BUILD_DIR)/incos-$(ARCH)-kernel

kernel_BINS += $(BUILD_DIR)/incos-$(ARCH)-kernel

$(BUILD_DIR)/incos-$(ARCH)-kernel: $(kernel_OBJS)
	$(LD) -T $(kernel_LDSCRIPT) -o $@ $+ $(LDFLAGS) 
