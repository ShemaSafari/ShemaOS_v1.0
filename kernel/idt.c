#include "idt.h"
#include "isr.h"
#include "irq.h"
#include <stdint.h>

#define IDT_SIZE 256

idt_entry_t idt[IDT_SIZE];
idt_ptr_t idt_ptr;

extern void idt_load(uint32_t); //ASM function

void idt_set_entry(int num, uint32_t base, uint16_t selector, uint8_t flags){
    idt[num].offset_low  = base & 0xFFFF;
    idt[num].selector    = selector;   // kernel code segment
    idt[num].zero        = 0;
    idt[num].flags       = flags;   // present, ring 0, interrupt gate
    idt[num].offset_high = (base >> 16) & 0xFFFF;
};

void idt_init(){
    idt_ptr.limit = sizeof(idt_entry_t) * IDT_SIZE - 1;
    idt_ptr.base  = (uint32_t)&idt;

    // Load the IDT
    idt_load((uint32_t)&idt_ptr);
};