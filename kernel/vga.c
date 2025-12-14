#include "io.h"
#include <stdint.h>

#define VGA_MEMORY (uint8_t*)0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t* vga_buffer = (uint16_t*)0xB8000;
static uint16_t cursor_x = 0;
static uint16_t cursor_y = 0;
static uint8_t color = 0x07; // white text on black background

static uint16_t make_vga_entry(char c, uint8_t color){
    return (uint16_t) c | ((uint16_t) color << 8);
}

void vga_enable_cursor(uint8_t start, uint8_t end){
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | start);

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | end);
}

static void vga_update_cursor(){

    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

static void vga_scroll(){
    // if cursor is still in screen, do nothing
    if (cursor_y < VGA_HEIGHT){
        return;
    }
    // if cursor is out of screen, scroll up
    for (int y = 1; y < VGA_HEIGHT; y++){
        for (int x = 0; x < VGA_WIDTH; x++){
            uint16_t from = y * VGA_WIDTH + x;
            uint16_t to = (y - 1) * VGA_WIDTH + x;
            vga_buffer[to] = vga_buffer[from];
        }
    }

    // clear last line
    int last_line = VGA_HEIGHT - 1;
    for (int x = 0; x < VGA_WIDTH; x++){
        vga_buffer[last_line * VGA_WIDTH + x] = make_vga_entry(' ', color);
    }

    // move cursor to last line
    cursor_y = VGA_HEIGHT - 1;

}

void vga_put_char(char c){
    // make newline
    if (c == '\n'){
        cursor_x = 0;
        cursor_y++;
        vga_scroll();
        vga_update_cursor();
        return;
    }

    // make backspace
    if (c == '\b'){
        if (cursor_x > 0) cursor_x--;
        uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
        vga_buffer[pos] = make_vga_entry(' ', color);
        if(cursor_x == 0 && cursor_y > 0){
            cursor_y--;
            cursor_x = VGA_WIDTH - 1;
        }
        vga_update_cursor();
        return;
    }

    // make tab
    if (c == '\t'){
        cursor_x = (cursor_x + 4) & ~(4 - 1);

        if (cursor_x >= VGA_WIDTH){
            cursor_x = 0;
            cursor_y++;
            vga_scroll();
            vga_update_cursor();
        }
        return;
    }

    // position in the vga buffer
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    // write character + collor to vga memory in one go
    vga_buffer[pos] = make_vga_entry(c, color);

    // move cursor right
    cursor_x++;
    if (cursor_x >= VGA_WIDTH){
        cursor_x = 0;
        cursor_y++;
    }

    vga_scroll();
    vga_update_cursor();
}

void print(const char* str){
    for (int i = 0; str[i] !=0; i++){
        vga_put_char(str[i]);
    }
}
void print_hex(uint32_t num) {
    char hex_chars[] = "0123456789ABCDEF";
    print("0x");

    for (int i = 28; i >= 0; i -= 4) {
        uint8_t nibble = (num >> i) & 0xF;
        char c = hex_chars[nibble];
        vga_put_char(c);
    }
}

void clear_screen(){
    for (int i = 0; i < 80 * 25; i++) {
        vga_buffer[i] = make_vga_entry(' ', color);
    }
    cursor_x = 0;
    cursor_y = 0;
    vga_update_cursor();
}