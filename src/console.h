#ifndef CONSOLE_H
#define CONSOLE_H
#include <stddef.h>
#include <stdint.h>

void console_set_color(uint8_t color);
void console_clear(void);
void console_putc(char c);
void console_print(const char* s);

#endif
