#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>

void timer_init(void);
void timer_tick(void);
unsigned long timer_ticks(void);

#endif

