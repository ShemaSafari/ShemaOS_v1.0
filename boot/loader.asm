[org 0x7E00]
bits 16

start:
    mov si, msg2

print_loop:
    lodsb
    mov ah, 0x0E
    or al, al
    jz done
    int 0x10
    jmp print_loop

done:
    jmp $

msg2 db " Booting_ShemaOS.v01....",0

times 510 - ($ - $$) db 0  
dw 0xAA55