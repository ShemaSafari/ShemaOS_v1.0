#include "isr.h"
#include "vga.h"
#include "idt.h"

static const char* exception_messages[32] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceed",
    "Invalid Opcode",
    "Device not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "x86 Floating Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Security Exception",
    "Reserved",
};

void isr_handler(registers_t *regs){
    uint32_t int_num = regs->int_num;
    //Defensive: Bound check
    if (int_num < 32) {
        print("EXCEPTION: ");
        print(exception_messages[int_num]);
        print("\n");
    } 

    // for page faults inspect CR2 (faulting address)
    if (int_num == 14) { // page fault
        uint32_t faulting_address;
        __asm__ volatile("mov %%cr2, %0" : "=r"(faulting_address));
        print("Page fault at address: ");
        print_hex(faulting_address);     // print hex of faulting address
        print("\n");
    }

    // if its a cpu excpection (0-31), halt the system
    if (int_num < 32){
        print("Exception occurred, OS halted!\n");
        for (;;){ __asm__ volatile("hlt"); }
    }
}
void isr_init(){
    for (int i = 0; i < 32; i++){
        idt_set_entry(i, (uint32_t)isr_stubs[i], 0x08, 0x8E);
    }
}