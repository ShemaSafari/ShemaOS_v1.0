#-*-*-*-*-*Tools*-*-*-*-*-
NASM := nasm
GCC := gcc
OBJCOPY := objcopy
QEMU := qemu-system-x86_64

##-*-*-*-*Get absolute path to this Makefile's directory*-*-*-*-*-
MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

#-*-*-*-*-*Directories*-*-*-*-*-
BUILD_DIR    :=   $(MAKEFILE_DIR)/../build
BOOT_DIR     :=   $(MAKEFILE_DIR)/../boot
KERNEL_DIR   :=   $(MAKEFILE_DIR)/../kernel
INCLUDE_DIR  :=   $(MAKEFILE_DIR)/../kernel/include
IDT_DIR 	 :=   $(MAKEFILE_DIR)/../kernel/idt	# all idt files are found here

#-*-*-*-*Flags*-*-*-*-*-
CFLAGS := -m32 -ffreestanding -O2 -Wall -Wextra -Werror -fno-leading-underscore -I$(INCLUDE_DIR)
LDFLAGS = -m32 -T $(MAKEFILE_DIR)/Linker.ld -nostdlib -ffreestanding

#-*-*-*-*-*Files*-*-*-*-*-
BOOT_BIN 		:= 	$(BUILD_DIR)/boot.bin
LOADER_BIN 		:= 	$(BUILD_DIR)/loader.bin
KERNEL_ENTRY    := 	$(BUILD_DIR)/kernel_entry.o
KERNEL_C 		:= 	$(BUILD_DIR)/kernel_c.o
VGA_O 			:= 	$(BUILD_DIR)/vga.o
ISR_STUBS_O     :=  $(BUILD_DIR)/isr_stubs.o
ISR_O 			:=  $(BUILD_DIR)/isr.o
IRQ_STUBS_O     :=  $(BUILD_DIR)/irq_stubs.o
IRQ_O 			:=  $(BUILD_DIR)/irq.o
KEYBOARD_O 		:=  $(BUILD_DIR)/keyboard.o
IDT_LOAD_O 		:=  $(BUILD_DIR)/idt_load.o
IDT_O 			:=  $(BUILD_DIR)/idt.o
KERNEL_ELF 		:=  $(BUILD_DIR)/kernel.elf
KERNEL_BIN 		:=  $(BUILD_DIR)/kernel.bin
KERNEL_SECTORS_FILE := $(BUILD_DIR)/kernel_sectors.txt
OS_IMAGE 		:=  $(BUILD_DIR)/ShemaOS.img


#-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*Build Rules*-*-*-*-*-*-*-*-*-*-*-*-*-*-
all: $(OS_IMAGE)

$(BOOT_BIN): $(BOOT_DIR)/boot.asm
	@mkdir -p $(BUILD_DIR)
	$(NASM) -f bin $< -o $@

$(LOADER_BIN): $(BOOT_DIR)/loader.asm $(KERNEL_BIN) $(KERNEL_SECTORS_FILE)
	@mkdir -p $(BUILD_DIR)
	@KERNEL_SECTORS=$$(cat $(KERNEL_SECTORS_FILE)); \
	$(NASM) -f bin -dKERNEL_SECTORS=$$KERNEL_SECTORS $< -o $@

$(KERNEL_ENTRY): $(KERNEL_DIR)/kernel_entry.asm
	@mkdir -p $(BUILD_DIR)
	$(NASM) -f elf32 $< -o $@

$(KERNEL_C): $(KERNEL_DIR)/kernel.c
	@mkdir -p $(BUILD_DIR)
	$(GCC) $(CFLAGS) -c $< -o $@

$(VGA_O): $(KERNEL_DIR)/vga.c
	@mkdir -p $(BUILD_DIR)
	$(GCC) $(CFLAGS) -c $< -o $@

$(ISR_STUBS_O): $(KERNEL_DIR)/isr_stubs.asm
	@mkdir -p $(BUILD_DIR)
	$(NASM) -f elf32 $< -o $@

$(ISR_O): $(KERNEL_DIR)/isr.c
	@mkdir -p $(BUILD_DIR)
	$(GCC) $(CFLAGS) -c $< -o $@

$(IRQ_STUBS_O): $(KERNEL_DIR)/irq_stubs.asm
	@mkdir -p $(BUILD_DIR)
	$(NASM) -f elf32 $< -o $@

$(IRQ_O): $(KERNEL_DIR)/irq.c
	@mkdir -p $(BUILD_DIR)
	$(GCC) $(CFLAGS) -c $< -o $@

$(KEYBOARD_O): $(KERNEL_DIR)/keyboard.c
	@mkdir -p $(BUILD_DIR)
	$(GCC) $(CFLAGS) -c $< -o $@

$(IDT_LOAD_O): $(KERNEL_DIR)/idt_load.asm
	@mkdir -p $(BUILD_DIR)
	$(NASM) -f elf32 $< -o $@

$(IDT_O): $(KERNEL_DIR)/idt.c
	@mkdir -p $(BUILD_DIR)
	$(GCC) $(CFLAGS) -c $< -o $@

$(KERNEL_ELF): $(KERNEL_ENTRY) $(KERNEL_C) $(VGA_O) $(ISR_STUBS_O) $(ISR_O) $(IRQ_STUBS_O) $(IRQ_O) $(KEYBOARD_O) $(IDT_LOAD_O) $(IDT_O)
	$(GCC) $(LDFLAGS) $^ -o $@

$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $< $@

$(KERNEL_SECTORS_FILE): $(KERNEL_BIN)
	@mkdir -p $(BUILD_DIR)
	@python3 $(MAKEFILE_DIR)/calc_sectors.py $(KERNEL_BIN) > $@ 2>/dev/null || \
	 python $(MAKEFILE_DIR)/calc_sectors.py $(KERNEL_BIN) > $@ 2>/dev/null || \
	 (echo "1" > $@ && echo "Warning: Could not calculate kernel sectors, using default: 1")
	@PYTHON_CMD=$$(which python3 2>/dev/null || which python 2>/dev/null || echo python); \
	KERNEL_SIZE=$$($$PYTHON_CMD -c "import os; print(os.path.getsize('$(KERNEL_BIN)'))" 2>/dev/null || echo 0); \
	KERNEL_SECTORS=$$(cat $@ 2>/dev/null || echo 1); \
	echo "---Kernel size: $$KERNEL_SIZE bytes ($$KERNEL_SECTORS sectors)---"

$(OS_IMAGE): $(BOOT_BIN) $(LOADER_BIN) $(KERNEL_BIN)
	@echo "---Creating 1.44MB floppy image...---"
	dd if=/dev/zero of=$(OS_IMAGE) bs=512 count=2880 status=none

	@echo "---Writing bootloader to sector 0...---"
	dd if=$(BOOT_BIN) of=$(OS_IMAGE) conv=notrunc status=none

	@echo "---Writing loader to sector 1...---"
	dd if=$(LOADER_BIN) of=$(OS_IMAGE) seek=1 conv=notrunc status=none

	@echo "---Writing kernel to sector 2...---"
	@KERNEL_SECTORS=$$(cat $(KERNEL_SECTORS_FILE)); \
	dd if=$(KERNEL_BIN) of=$(OS_IMAGE) bs=512 seek=2 conv=notrunc status=none; \
	echo "---Kernel written: $$KERNEL_SECTORS sectors---"

	@echo "-*-*-✅ OS image created at $(OS_IMAGE)-*-*-"

run: $(OS_IMAGE)
	$(QEMU) -drive format=raw,file=$(OS_IMAGE)

clean:
	@echo files removed
	rm -rf $(BUILD_DIR)
