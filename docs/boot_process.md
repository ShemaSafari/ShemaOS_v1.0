# ShemaOS Boot Process

This document describes the complete flow from BIOS power-on to the point where the kernel starts executing.

---

## Stage 0 — BIOS (Before the code runs)

1. The computer powers on.
2. The BIOS performs the POST (Power-On Self Test).
3. BIOS detects bootable devices (hard drive, USB, etc.).
4. BIOS reads the **first sector (512 bytes)** of the selected boot device into **physical address 0x7C00**.
5. BIOS checks the last two bytes for the boot signature `0x55AA`.
6. If valid, BIOS jumps to **0x7C00** and begins executing our **bootloader**.

---


## Stage 1 — Bootloader (`boot.asm`)

**Loaded at:** `0x7C00`

### Purpose
- Display “Loading...” message.
- Load the second-stage loader (the real workhorse).
- Jump to it.

### Actions
1. Prints `"Loading..."` using BIOS interrupt `0x10` (teletype mode).
2. Calls BIOS disk interrupt `int 0x13`:
   - Loads **1 sector** starting from **sector 2** (since sector 1 = bootloader).
   - Loads it into **memory address 0x7E00**.
3. Jumps to **0x0000:0x7E00** (where the loader now resides).
4. Bootloader’s job is now done.

---

## Stage 2 — Loader (`loader.asm`)

**Loaded at:** `0x7E00`

### Purpose
- Set up the CPU environment (stack, segments).
- Enable the A20 line.
- Load the kernel into memory.
- Set up the GDt and build it.
- Eventually switch to protected mode then jump to the kernel.

### Actions
1. Disables interrupts (`cli`).
2. Initializes data segments:
   - `ds = 0`
   - `es = 0`
   - `ss = 0x9000`, `sp = 0xFFFF` → stack at 0x9FFFF.
3. Enables **A20 line** via port `0x92`:
   - Ensures access to memory beyond 1MB.
4. Loads the kernel from disk:
   - Loads the kernel at **sector 3**.
   - Stores them at **physical address 0x10000 (0x1000:0x0000)**.
5. Displays `"kernel_loaded_successfully!"` on screen.
6. Prepare to jump to the kernel:
   - Build the **GDT**.
   - Load **GDTR**.
   - Set **CR0.PE = 1** to enter protected mode.
   - load again the kernel to **0x00100000 (1mb)**
   - Jump to **kernel entry** (kernel_entry.asm).

---

## Stage 3 — Kernel (`kernel_entry.asm` + `kernel.c`)

**Loaded at:** `0x00100000:0x0000` (physical 0x00100000)

### Purpose
- Take over control after protected mode switch.
- Initialize the hardware and start the OS.

### Actions
1. Set up data segments (`ds`, `ss`, `es`, etc.).
2. Initialize the stack (`esp`).
3. Clears the screen.
4. Call `kernel_main()` in `kernel.c`.
5. `kernel_main()` runs the OS logic (text output, drivers, etc.).
6. Made the VGA driver, we can now print text on the screen.
7. Setting up interrupt handlers:
   - Build the **IDT, ISR, IRQ**.
   - Make the keyboard driver
   - Make a graphics mode driver
   - 
   - 
   

---



