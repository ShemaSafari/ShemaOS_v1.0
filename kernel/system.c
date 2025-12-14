#include "system.h"
#include "io.h"
#include "vga.h"
#include <stdint.h>

void reboot(){

    __asm__ volatile("cli"); // make sure nothing interrupts the reboot process

    while (inb(0x64) & 0x02) {}// Wait until the keyboard controller is ready

    outb(0x64, 0xFE); // Send the reboot command to the keyboard controller

    // If the reboot doesn't happen, halt the CPU
    __asm__ volatile("hlt"); 
}

void shut_down(){
    print("Shutting down...\n");

    // use qemu shutdown port
    outw(0x604, 0x2000); // qemu port 0x604 method

    while(1) {
        __asm__ volatile("hlt"); // halt CPU if shutdown fails
    }
}