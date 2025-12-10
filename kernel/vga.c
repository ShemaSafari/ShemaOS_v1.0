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
        return;
    }

    // make backspace
    if (c == '\b'){
        if (cursor_x > 0) cursor_x--;
        uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
        vga_buffer[pos] = make_vga_entry(' ', color);
        if(cursor_x == 0 && cursor_y > 0){
            cursor_y--;
            cursor_x = VGA_HEIGHT - 1;
        }
        return;
    }

    // make tab
    if (c == '\t'){
        cursor_x = (cursor_x + 4) & ~(4 - 1);

        if (cursor_x >= VGA_WIDTH){
            cursor_x = 0;
            cursor_y++;
            vga_scroll();
        }
        return;
    }

    // position in the vga buffer
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    // write character + collor to vga memory in one go
    vga_buffer[pos] = make_vga_entry(c, color);

    // move cursor right
    cursor_x++;
    if (cursor_x >= VGA_HEIGHT){
        cursor_x = 0;
        cursor_y++;
    }

    vga_scroll();
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