#include <stdint.h>
#include "interrupts.h"

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

static struct idt_entry idt[256];
static struct idt_ptr idtr;

extern void isr0();
extern void irq0();
extern void irq1();
extern void keyboard_isr(void);
extern void timer_tick(void);
extern void load_idt(struct idt_ptr*);
extern void load_gdt(struct idt_ptr*);

static void idt_set_gate(int n, void* handler, uint8_t type) {
    uint64_t addr = (uint64_t)handler;
    idt[n].offset_low  = addr & 0xFFFF;
    idt[n].selector    = 0x08;
    idt[n].ist         = 0;
    idt[n].type_attr   = type;
    idt[n].offset_mid  = (addr >> 16) & 0xFFFF;
    idt[n].offset_high = (addr >> 32) & 0xFFFFFFFF;
    idt[n].zero        = 0;
}

__attribute__((interrupt))
void isr0_handler(void* frame) {
    (void)frame;
    while (1) __asm__("hlt");
}

__attribute__((interrupt))
void irq0_handler(void* frame) {
    (void)frame;
    timer_tick();
    __asm__("outb %%al, $0x20" :: "a"(0x20));
}

__attribute__((interrupt))
void irq1_handler(void* frame) {
    (void)frame;
    keyboard_isr();
    __asm__("outb %%al, $0x20" :: "a"(0x20));
}

void interrupts_init(void) {
    for (int i = 0; i < 256; ++i)
        idt_set_gate(i, isr0, 0x8E);
    idt_set_gate(0, isr0, 0x8E);
    idt_set_gate(32, irq0, 0x8E);
    idt_set_gate(33, irq1, 0x8E);
    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint64_t)&idt;
    load_idt(&idtr);
}
