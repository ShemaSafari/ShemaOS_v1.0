#ifndef VGA_H
#define VGA_H
#include <stdint.h>

void print(const char* str);
void print_hex(uint32_t num);
void clear_screen();
void vga_put_char(char c);
void vga_scroll();
void vga_enable_cursor(uint8_t start, uint8_t end);

#endif