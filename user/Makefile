program_NAME :=
program_BINS :=
program_SRCS :=
program_LIBS :=

library_NAME :=
library_BINS :=
library_SRCS :=

user_BINS :=
user_OBJS :=
user_LIBS :=

include user/arch/$(ARCH)/Makefile
include user/hello/Makefile
include user/libuser/Makefile

.phony: user
user: $(user_BINS) $(user_LIBS)
user: CPPFLAGS := -Iuser/include $(CPPFLAGS)
