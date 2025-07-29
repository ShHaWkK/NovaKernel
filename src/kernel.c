#include <stdint.h>
#include <stddef.h>
#include "memory.h"
#include "interrupts.h"
#include "scheduler.h"
#include "syscalls.h"
#include "fs.h"
#include "shell.h"
#include "../drivers/keyboard.h"
#include "../drivers/timer.h"
#include "rust.h"

extern void gdt_descriptor();
extern void load_gdt(void*);

static volatile uint16_t* const vga = (uint16_t*)0xB8000;
static size_t vga_index = 0;
static uint8_t text_color = 0x0F;

static void set_text_color(uint8_t color) {
    text_color = color;
}

static void vga_clear(void) {
    for (size_t i = 0; i < 80 * 25; i++)
        vga[i] = (text_color << 8) | ' ';
    vga_index = 0;
}

static void putc(char c) {
    if (c == '\n') vga_index += 80 - (vga_index % 80);
    else vga[vga_index++] = (text_color << 8) | c;
}

static void puts(const char* s) { while (*s) putc(*s++); }

void kernel_main(void) {
    load_gdt(&gdt_descriptor);
    interrupts_init();
    memory_init();
    keyboard_init();
    timer_init();
    fs_init();
    scheduler_init();
    uint8_t demo[4] = { 't','e','s','t' };
    uint8_t key[32] = {0};
    aes_encrypt(demo, 4, key);
    aes_decrypt(demo, 4, key);
    sandbox_process(0);
    set_text_color(0x0A);
    vga_clear();
    puts("NovaKernel loaded\n");
    scheduler_add(shell);
    while (1) scheduler_schedule();
}
