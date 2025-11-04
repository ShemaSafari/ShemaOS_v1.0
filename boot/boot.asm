[org 0x7C00]            ; loads this boot sector to 0x7C00
bits 16                 ; real mode


mov si, message
print_loop:
    lodsb
    or al, al
    jz done
    mov ah, 0x0E
    int 0x10
    jmp print_loop

done:
    jmp loading_stage2


boot_drive db 0x80
loading_stage2:
    mov al, 1           ; number of sectors to read
    mov ch, 0           ; number of cylinders
    mov cl, 2           ; read the second sector 
    mov dh, 0           ; head number
    ; mov [boot_drive], dl
    mov dl, [boot_drive]
    ;mov ax, 0x0000
    ;mov es, ax
    mov bx, 0x7E00
    mov ah, 0x02        ; BIOS interrupt, function read sector
    int 0x13            ; call BIOS disk service
    jc disk_error
    

    jmp 0x0000:0x7e00       ; jump to next sector

disk_error:
    mov si, error_msg
error_print:
    lodsb
    or al, al
    jz $
    mov ah, 0x0E
    int 0x10
    jmp error_print


message db "Loading...", 0x0D, 0x0A, 0
error_msg db "Failed_to_locate_boot..", 0x0D, 0x0A, 0

times 510 - ($ - $$) db 0
dw 0xAA55