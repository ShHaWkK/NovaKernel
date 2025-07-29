#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>

void keyboard_init(void);
void keyboard_isr(void);
int keyboard_getc(void);

#endif

