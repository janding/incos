user_BINS += $(addprefix $(BUILD_DIR)/,$(program_BINS))
user_OBJS += $(call srcs-to-objs, $(program_SRCS))

$(addprefix $(BUILD_DIR)/,$(program_BINS)): $(call srcs-to-objs, $(program_SRCS)) $(addprefix $(BUILD_DIR)/,$(program_LIBS))
	$(LD) -Tuser/arch/$(ARCH)/user.ld -o $@ $+ $(LDFLAGS)

program_NAME :=
program_BINS :=
program_SRCS :=
program_LIBS :=
