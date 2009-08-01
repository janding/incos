kernel_SRCS += $(call collect-srcs, $(CURRENT_DIRECTORY))
kernel_BINS += $(BUILD_DIR)/incos-$(ARCH)-kernel

kernel_LDSCRIPT := $(CURRENT_DIRECTORY)kernel.ld

kernel_COMBINE_SRCS = $(filter-out %vm_boot.c %multiboot.c,$(filter %.c,$(kernel_SRCS)))
kernel_NO_COMBINE_SRCS = $(filter-out %c,$(kernel_SRCS)) $(filter %vm_boot.c %multiboot.c,$(kernel_SRCS))
kernel_COMBINE_OBJ = incos-$(ARCH)-kernel.o

include kernel/arch/$(ARCH)/boot/Makefile
include kernel/arch/$(ARCH)/vm/Makefile
