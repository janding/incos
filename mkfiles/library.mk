user_LIBS += $(addprefix $(BUILD_DIR)/,$(library_BINS))
user_OBJS += $(call srcs-to-objs, $(library_SRCS))

$(addprefix $(BUILD_DIR)/,$(library_BINS)): $(call srcs-to-objs, $(library_SRCS))
	$(AR) r $@ $?

library_NAME :=
library_BINS :=
library_SRCS :=