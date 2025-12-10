global irq_stubs
extern irq_handler

section .text

irq_stubs:
%assign i 0
%rep 16
    dd irq%+i     ; Store address of each IRQ handler
%assign i i+1
%endrep

%macro IRQ 1
global irq%1
irq%1:
    cli
    push dword 0        ; error code (IRQs don't have error codes) so we push a zero
    push dword %1 + 32   ; interrupt number (IRQ0 = 32, IRQ1 = 33, etc.) what hardware caused the interrupt
    jmp irq_common_stub
%endmacro

;--------Hardware interrupts (IRQ 0-15)--------
IRQ 0   ; Timer
IRQ 1   ; Keyboard
IRQ 2   ; Cascade
IRQ 3   ; COM2
IRQ 4   ; COM1
IRQ 5   ; LPT2
IRQ 6   ; Floppy
IRQ 7   ; LPT1
IRQ 8   ; CMOS RTC
IRQ 9   ; Free
IRQ 10  ; Free
IRQ 11  ; Free
IRQ 12  ; PS/2 Mouse
IRQ 13  ; FPU
IRQ 14  ; Primary ATA
IRQ 15  ; Secondary ATA

;---------Common stub for all IRQs-------
irq_common_stub:
    pusha
    push ds

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp            ; pointer to registers_t
    call irq_handler
    add esp, 4

    pop ds
    popa
    add esp, 8          ; pop int_num + err_code
    sti
    iret

