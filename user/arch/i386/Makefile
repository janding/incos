$(BUILD_DIR)/crt0.o: $(CURRENT_DIRECTORY)crt0.S
	$(AS) $(ASFLAGS) -o $@ $<

extra_OBJS += $(BUILD_DIR)/crt0.o

user: $(BUILD_DIR)/crt0.o 
