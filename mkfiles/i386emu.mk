IMAGE = /b

.phony: install
install: system image

.phony: image
image: $(addprefix $(IMAGE)/boot/,$(notdir $(kernel_BINS) $(user_BINS)))

$(addprefix $(IMAGE)/boot/,$(notdir $(kernel_BINS) $(user_BINS))): $(IMAGE)/boot/%: $(BUILD_DIR)/%

$(IMAGE)/%:
	cp $< $@
		

.phony: bochs
bochs: install
	cmd /c bochs.bxrc; exit 0


QEMU = /c/Program\ Files/Qemu-0.10.5/qemu.exe
.phony: qemu
qemu: install
	sleep 1
	$(QEMU) -fda b: -serial con:

.phony: qemu-gdb
qemu-gdb: install
	sleep 1
	$(QEMU) -fda b: -serial con: -s &
	gdb -ex "target remote localhost:1234" $(BUILD_DIR)/incos-$(ARCH)-kernel

VBOX = /c/Program\ Files/Sun/xVM\ VirtualBox/VBoxManage.exe
.phony: vbox
vbox: install
	sleep 1
	$(VBOX) startvm testvm