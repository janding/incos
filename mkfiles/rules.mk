$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: %.S
	$(AS) $(ASFLAGS) -o $@ $<
