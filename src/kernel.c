#include <stdint.h>
#include <stddef.h>
#include "memory.h"
#include "interrupts.h"
#include "scheduler.h"
#include "syscalls.h"
#include "fs.h"
#include "shell.h"
#include "console.h"
#include "../drivers/keyboard.h"
#include "../drivers/timer.h"
#include "rust.h"

extern void gdt_descriptor();
extern void load_gdt(void*);


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
    console_set_color(0x0A);
    console_clear();
    console_print("NovaKernel loaded\n");
    scheduler_add(shell);
    while (1) scheduler_schedule();
}
