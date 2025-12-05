#pragma once
#include <stdint.h>

typedef struct registers{
    uint32_t ds;                                        // data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;    // pusha order
    uint32_t int_num;                                   // interrupt number
    uint32_t err_code;                                  // error code (if pushed) or 0  
    uint32_t eip, cs, eflags, useresp, ss;              // pushed by cpu on interrupt
} registers_t;

extern void *isr_stubs[32]; //array of ISR stubs defined in isr_stubs.asm

void isr_handler(registers_t *regs);
void isr_init();