# ShemaOS Boot Process

This document describes the complete flow from BIOS power-on to the point where the kernel starts executing.

---

## Stage 0 — BIOS (Before our code runs)

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
- Eventually switch to protected mode.

### Actions
1. Disables interrupts (`cli`).
2. Initializes data segments:
   - `ds = 0`
   - `es = 0`
   - `ss = 0x9000`, `sp = 0xFFFF` → stack at 0x9FFFF.
3. Enables **A20 line** via port `0x92`:
   - Ensures access to memory beyond 1MB.
4. Loads the kernel from disk:
   - Loads **4 sectors** starting at **sector 3**.
   - Stores them at **physical address 0x2000 (0x1000:0x1000)**.
5. Displays `"kernel_loaded_successfully!"` on screen.
6. Prepare to jump to the kernel:
   - Build the **GDT**.
   - Load **GDTR**.
   - Set **CR0.PE = 1** to enter protected mode.
   - Jump to **kernel entry** (kernel.asm).

---

## Stage 3 — Kernel (`kernel.asm` + `kernel.c`)

**Loaded at:** `0x1000:0x1000` (physical 0x2000)

### Purpose
- Take over control after protected mode switch.
- Initialize the hardware and start the OS.

### Actions
1. Set up data segments (`ds`, `ss`, `es`, etc.).
2. Initialize the stack (`esp`).
3. Call `kmain()` in `kernel.c`.
4. `kmain()` runs the OS logic (text output, drivers, etc.).

---



