#include "vga.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "keyboard.h"
#include <stdint.h>

void kernel_main(){
    print("STARTING KERNEL...\n");
    print("Interrupts initialized...\n");
    //print("Type something on your keyboard...\n");

    idt_init();   // set up IDT memory
    isr_init();   // install CPU exceptions 0-31
    irq_init();   // install hardware IRQs 32-47
    keyboard_init();
    
    __asm__ volatile("sti");  // enable interrupts

     // keyboard can now safely generate IRQs

    while(1) {
        __asm__ volatile("hlt");  // halt until next interrupt
    }
}
