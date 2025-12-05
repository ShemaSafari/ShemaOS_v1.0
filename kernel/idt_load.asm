global idt_load

idt_load:
    mov eax, [esp + 4]    ; Get the idt_ptr address from stack
    lidt [eax]            ; Load IDT
    ret