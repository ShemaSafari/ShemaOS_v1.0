#pragma once
#include <stdint.h>

// Port I/O functions
static inline uint8_t inb(uint16_t port) { // Read a byte from a port
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) { // Write a byte to a port
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline void io_wait(void) { //  function to make a small delay
    __asm__ volatile("outb %%al, $0x80" : : "a"(0));
}

static inline void outw(uint16_t port, uint16_t val) { // Write a word to a port
    __asm__ volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}
