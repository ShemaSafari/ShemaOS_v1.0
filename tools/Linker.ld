ENTRY(kernel_entry)        /*starting point of the kernel*/

SECTIONS
{
    . = 0x00100000;    /* Kernel load address (1 MB) */

    .text : {
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(COMMON)
        *(.bss*)
        . = ALIGN(4);
    }
}