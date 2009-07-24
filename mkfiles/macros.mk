CURRENT_MAKEFILE = $(lastword $(MAKEFILE_LIST))
CURRENT_DIRECTORY = $(dir $(CURRENT_MAKEFILE))

collect-srcs = $(wildcard $(addprefix $(1), *.c *.S))
srcs-to-objs = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(basename $(1))))
objs-to-deps = $(addsuffix .d, $(basename $(1)))
