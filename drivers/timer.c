#include "timer.h"
#include "../src/io.h"

static volatile unsigned long ticks = 0;

void timer_init(void) {
    /* Set PIT to 100Hz */
    outb(0x43, 0x36);
    uint16_t divisor = 1193182 / 100;
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
    /* unmask timer IRQ0 */
    outb(0x21, inb(0x21) & ~0x01);
}

void timer_tick(void) {
    ticks++;
}

unsigned long timer_ticks(void) {
    return ticks;
}

