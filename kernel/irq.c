#include "irq.h"
#include "idt.h"
#include "isr.h"
#include "io.h"
#include <stdint.h>

// PIC ports
#define PIC1_COMMAND_mast 0x20
#define PIC1_DATA_mast    0x21
#define PIC2_COMMAND_slav 0xA0
#define PIC2_DATA_slav    0xA1

// PIC initialization commands
#define ICW1_INIT    0x10
#define ICW1_ICW4    0x01
#define ICW4_8086    0x01

// End of interrupt
#define PIC_EOI      0x20

// Array of IRQ handler function pointers
static void (*irq_handlers[16])(registers_t *regs) = {0};

// Remap PIC to avoid conflicts with CPU exceptions (0-31)
void pic_remap(void) {
    // Save masks
    uint8_t a1 = inb(PIC1_DATA_mast);
    uint8_t a2 = inb(PIC2_DATA_slav);

    // Initialize PIC1
    outb(PIC1_COMMAND_mast, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA_mast, 32);  // IRQ 0-7 mapped to interrupts 32-39
    io_wait();
    outb(PIC1_DATA_mast, 0x04); // PIC1 has a slave at IRQ2
    io_wait();
    outb(PIC1_DATA_mast, ICW4_8086);
    io_wait();

    // Initialize PIC2
    outb(PIC2_COMMAND_slav, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_DATA_slav, 40);  // IRQ 8-15 mapped to interrupts 40-47
    io_wait();
    outb(PIC2_DATA_slav, 0x02); // PIC2 is a slave
    io_wait();
    outb(PIC2_DATA_slav, ICW4_8086);
    io_wait();

    // Restore masks
    outb(PIC1_DATA_mast, a1);
    outb(PIC2_DATA_slav, a2);
}

// I/O functions are now in io.h

// IRQ handler - called by assembly stub
void irq_handler(registers_t *regs) {
    uint32_t irq_num = regs->int_num - 32;  // Convert interrupt number to IRQ number

    // Bounds check: IRQs are 0-15 (interrupts 32-47)
    if (irq_num >= 16) {
        // Invalid IRQ number, just send EOI and return
        outb(PIC1_COMMAND_mast, PIC_EOI);
        return;
    }

    // Call the registered handler if one exists
    if (irq_handlers[irq_num] != 0) {
        irq_handlers[irq_num](regs);
    }

    // Send EOI to PIC
    if (irq_num >= 8) {
        outb(PIC2_COMMAND_slav, PIC_EOI);  // Acknowledge slave PIC
    }
    outb(PIC1_COMMAND_mast, PIC_EOI);      // Acknowledge master PIC
}

// Initialize IRQ system
void irq_init(void) {
    // Remap PIC
    pic_remap();

    // Register IRQ stubs in IDT (interrupts 32-47)
    for (int i = 0; i < 16; i++) {
        idt_set_entry(32 + i, (uint32_t)irq_stubs[i], 0x08, 0x8E);
    }
}

// Install an IRQ handler
void irq_install_handler(int irq, void (*handler)(registers_t *regs)) {
    if (irq >= 0 && irq < 16) {
        irq_handlers[irq] = handler;
    }
}

// Uninstall an IRQ handler
void irq_uninstall_handler(int irq) {
    if (irq >= 0 && irq < 16) {
        irq_handlers[irq] = 0;
    }
}

