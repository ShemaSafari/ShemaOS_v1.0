#include "vga.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "shell.h"
#include "keyboard.h"
#include <stdint.h>

void kernel_main(){
    vga_enable_cursor(14, 15); // enable cursor
    print("STARTING KERNEL...\n");
    print("Interrupts initialized...\n");

    idt_init();   // set up IDT memory
    isr_init();   // install CPU exceptions 0-31
    irq_init();   // install hardware IRQs 32-47
    keyboard_init();

    shell_run();
    
    __asm__ volatile("sti");  // enable interrupts
    while(1) {
        __asm__ volatile("hlt");  // halt until next interrupt
    }
}
