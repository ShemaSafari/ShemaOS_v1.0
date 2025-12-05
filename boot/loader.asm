[org 0x7E00]
bits 16

%define KERNEL_LOAD_SEG 0x1000
%define KERNEL_LOAD_OFF 0x0000
%define KERNEL_PM_DEST 0x00100000
%ifndef KERNEL_SECTORS
%define KERNEL_SECTORS 100
%endif

start:
    cli                 ; disable interrupts while setting up
    mov ax, 0          ; ax = 0
    mov ds, ax          ; data segment starts at 0x0000
    mov es, ax          ; extra segment starts at 0x0000
    mov [boot_drive], dl

    mov ax, 0x9000      ; use segment 0x9000
    mov ss, ax
    mov sp, 0xFFFF      ; stack top at 0x9000 + 0xFFFF
    sti                 ; re-enable interrupts

enable_A20:
    in al, 0x92         ; read current value from port 0x92 into Al
    or al, 00000010b    ; set bit 1 (A20 enable) into register AL while preserving other bits
    out 0x92, al        ; write updated value in AL back into port 0x92 enabling A20

load_kernel:
    mov ax, KERNEL_LOAD_SEG
    mov es, ax
    mov bx, KERNEL_LOAD_OFF        ; where BIOS will place the kernel temporarily

    mov al, KERNEL_SECTORS
    mov ch, 0
    mov cl, 3
    mov dh, 0
    mov dl, [boot_drive]
    mov ah, 0x02
    int 0x13
    jc kern_error

    mov si, success_msg
    call print_string

    call load_gdt
    jmp prote_mode_entry            ; jump to protected mode

kern_error:
    mov si, kern_msg
kern_print:
    lodsb
    or al, al
    jz $
    mov ah, 0x0E
    int 0x10
    jmp kern_print

print_string:
    mov ah, 0x0E
.next_char:
    lodsb
    or al, al
    jz .done
    int 0x10
    jmp .next_char

.done:
    ret

gdt_start:
gdt_null:               ;------Entry 0: Null descriptor
    dq 0x0000000000000000

gdt_code:               ;------Entry 1: code segment descriptor
    dw 0xFFFF           ; Limit (low)
    dw 0x0000           ; Base (low)
    db 0x00             ; Base (middle)
    db 10011010b        ; Access byte: present, ring 0, code, readable
    db 11001111b        ; Granularity: 4KB, 32-bit
    db 0x00             ;Base (high)

gdt_data:               ;------Entry: Data segment descriptor
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b        ; Access byte: present, ring 0, data, writable
    db 11001111b        ; Granularity
    db 0x00

gdt_end:

;------------GDT Descriptor (pointer)
gdt_descriptor:         ;GDTR storage
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODESEG equ gdt_code - gdt_start
DATASEG equ gdt_data - gdt_start



load_gdt:
    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax            ; cr0 = 1
        

jmp CODESEG:prote_mode_entry       ;0x08 = selector for GDT code segment


[bits 32]
prote_mode_entry:
    mov ax, DATASEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x9FC00

    ; copy the kernel from its real-mode load address (0x00010000) to 1 MiB
    mov esi, 0x00010000
    mov edi, KERNEL_PM_DEST
    mov ecx, (KERNEL_SECTORS * 512) / 4   ; number of dwords to copy
    rep movsd

    mov eax, KERNEL_PM_DEST
    jmp eax



success_msg db "kernel_loaded_successfully!",0x0D, 0x0A, 0
kern_msg db "kernel_not_found", 0

boot_drive db 0

times 512 - ($ - $$) db 0


    
