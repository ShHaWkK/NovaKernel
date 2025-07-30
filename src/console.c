#include "console.h"
#include <stdint.h>

static volatile uint16_t* const vga = (uint16_t*)0xB8000;
static size_t vga_index = 0;
static uint8_t text_color = 0x0F;

void console_set_color(uint8_t color) {
    text_color = color;
}

void console_clear(void) {
    for (size_t i = 0; i < 80 * 25; i++)
        vga[i] = (text_color << 8) | ' ';
    vga_index = 0;
}

void console_putc(char c) {
    if (c == '\n')
        vga_index += 80 - (vga_index % 80);
    else
        vga[vga_index++] = (text_color << 8) | c;
}

void console_print(const char* s) {
    while (*s)
        console_putc(*s++);
}
