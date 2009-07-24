IMAGE = /b

.phony: install
install: system image

.phony: image
image: $(IMAGE)/boot/incos-$(ARCH)-kernel $(addprefix $(IMAGE)/boot/,$(user_BINS))

$(IMAGE)/boot/incos-$(ARCH)-kernel $(addprefix $(IMAGE)/boot/,$(user_BINS)): $(IMAGE)/boot/%: $(BUILD_DIR)/%

$(IMAGE)/%:
	cp $< $@
		

.phony: bochs
bochs: install
	cmd /c bochs.bxrc; exit 0


QEMU = /c/Program\ Files/Qemu/qemu.exe
.phony: qemu
qemu: install
	sleep 1
	$(QEMU) -fda b: -serial con:

.phony: qemu-gdb
qemu-gdb: install
	sleep 1
	$(QEMU) -fda b: -serial con: -s &
	gdb -ex "target remote localhost:1234" $(BUILD_DIR)/incos-$(ARCH)-kernel
