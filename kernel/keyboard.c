#include "keyboard.h"
#include "isr.h"
#include "irq.h"
#include "vga.h"
#include "io.h"
#include "shell.h"
#include <stdint.h>


// Simple keyboard state
static uint8_t shift_pressed = 0;
static uint8_t caps_lock = 0;

// US QWERTY keymap - normal
static const char keymap_normal[128] = {
    0,   27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    0,   0,   ' '
};

// US QWERTY keymap - shift
static const char keymap_shift[128] = {
    0,   27,  '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0,   'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0,   '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    0,   0,   ' '
};

// Keyboard interrupt handler
void keyboard_handler(registers_t *regs) {
    (void)regs;
    
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    
    // Handle key release
    if (scancode & KEY_RELEASED) {
        uint8_t key = scancode & ~KEY_RELEASED;
        if (key == KEY_LSHIFT || key == KEY_RSHIFT) {
            shift_pressed = 0;
        }
        return;
    }
    
    // Handle modifier keys
    switch (scancode) {
        case KEY_LSHIFT:
        case KEY_RSHIFT:
            shift_pressed = 1;
            return;
        case KEY_CAPSLOCK:
            caps_lock = !caps_lock;
            return;

    }
    
    // Convert scancode to ASCII
    if (scancode >= 128) return;
    
    uint8_t use_shift = shift_pressed;
    if (caps_lock && scancode >= KEY_A && scancode <= KEY_Z) {
        use_shift = !use_shift;
    }
    
    char c = use_shift ? keymap_shift[scancode] : keymap_normal[scancode];
    if (c != 0) {
        shell_handle_char(c);
    }
}

// Initialize keyboard
void keyboard_init(void) {
    shift_pressed = 0;
    caps_lock = 0;
    irq_install_handler(1, keyboard_handler);
    print("Keyboard ready\n");
}
