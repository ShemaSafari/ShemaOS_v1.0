#include "system.h"
#include "io.h"
#include <stdint.h>

void reboot(){

    __asm__ volatile("cli"); // make sure nothing interrupts the reboot process

    while (inb(0x64) & 0x02) {}// Wait until the keyboard controller is ready

    outb(0x64, 0xFE); // Send the reboot command to the keyboard controller

    // If the reboot doesn't happen, halt the CPU
    __asm__ volatile("hlt"); 
}