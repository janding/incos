user_BINS += $(program_BINS)
user_OBJS += $(call srcs-to-objs, $(program_SRCS))

$(addprefix $(BUILD_DIR)/,$(program_BINS)): $(call srcs-to-objs, $(program_SRCS))
	$(LD) -o $@ $+ $(LDFLAGS)

program_NAME :=
program_BINS :=
program_SRCS :=