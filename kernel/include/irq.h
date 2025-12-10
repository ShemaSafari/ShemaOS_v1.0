#pragma once
#include <stdint.h>
#include "isr.h"

// IRQ numbers (mapped to interrupt numbers 32-47)
#define IRQ0  32  // Timer
#define IRQ1  33  // Keyboard
#define IRQ2  34  // Cascade
#define IRQ3  35  // COM2
#define IRQ4  36  // COM1
#define IRQ5  37  // LPT2
#define IRQ6  38  // Floppy
#define IRQ7  39  // LPT1
#define IRQ8  40  // CMOS RTC
#define IRQ9  41  // Free
#define IRQ10 42  // Free
#define IRQ11 43  // Free
#define IRQ12 44  // PS/2 Mouse
#define IRQ13 45  // FPU
#define IRQ14 46  // Primary ATA
#define IRQ15 47  // Secondary ATA

extern void *irq_stubs[16];  // Array of IRQ stubs defined in irq_stubs.asm

void irq_handler(registers_t *regs);
void irq_init(void);
void irq_install_handler(int irq, void (*handler)(registers_t *regs));
void irq_uninstall_handler(int irq);

