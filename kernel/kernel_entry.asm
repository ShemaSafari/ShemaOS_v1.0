;[org 0x00100000]         ; its at (0x00010000) 1 MiB physical memory
[bits 32]      
global kernel_entry
extern kernel_main

kernel_entry:
    mov ax, 0x10      ; Data segment selector (from GDT)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x90000
    mov edi, 0xB8000        ; Start of VGA text memory
    mov ecx, 80*25          ; Total number of characters on screen (2000)
    mov ax, 0x0720          ; Space (0x20) + light grey on black (0x07)

clear_loop:
    stosw                   ; store AX at [EDI], EDI += 2
    loop clear_loop

    call kernel_main

.hang:
    jmp .hang          ; never return

times 512 - ($ - $$) db 0
