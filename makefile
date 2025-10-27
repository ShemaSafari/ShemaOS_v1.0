#-*-*-*-*-*Tools*-*-*-*-*-
NASM := nasm
GCC := gcc
QEMU := qemu-system-x86_64

#-*-*-*-*Flags*-*-*-*-*-
CFLAGS := -m32 -ffreestanding -O2 -Wall -Wextra -Werror

#-*-*-*-*-*Directories*-*-*-*-*-
BUILD_DIR := build
BOOT_DIR := boot
KERNEL_DIR := kernel

#-*-*-*-*-*Files*-*-*-*-*-
BOOT_BIN := $(BUILD_DIR)/boot.bin
LOADER_BIN := $(BUILD_DIR)/loader.bin
OS_IMAGE := $(BUILD_DIR)/ShemaOS.img


#-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*Build Rules*-*-*-*-*-*-*-*-*-*-*-*-*-*-
all: $(OS_IMAGE)

$(BUILD_DIR)/%.bin: $(BOOT_DIR)/%.asm
	@mkdir -p $(BUILD_DIR)
	$(NASM) -f bin $< -o $@

$(OS_IMAGE): $(BOOT_BIN) $(LOADER_BIN)
	cat $^ > $@

run: $(OS_IMAGE)
	$(QEMU) -drive format=raw,file=$(OS_IMAGE)

clean:
	@echo files removed
	rm -rf $(BUILD_DIR)
