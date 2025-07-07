#include <stdint.h>
#include <stddef.h>

/*
 * kernel.c - coeur du noyau minimal
 * Affiche un message et initialise les structures principales
 */

/* Buffer VGA en mode texte à l'adresse 0xB8000 */
static volatile uint16_t* const vga = (uint16_t*)0xB8000;
static size_t vga_index = 0;

/* Affiche un caractère sur l'écran */
static void putc(char c)
{
    if (c == '\n') {
        vga_index += 80 - (vga_index % 80);
    } else {
        vga[vga_index++] = (0x0F << 8) | c; /* blanc sur noir */
    }
}

/* Affiche une chaîne terminée par \0 */
static void puts(const char* s)
{
    while (*s)
        putc(*s++);
}

/* Petit allocateur de type bump */
extern uint8_t kernel_end; /* symbole fourni par le linker */
static uint8_t* heap = &kernel_end;

/* Renvoie un bloc de mémoire de 'size' octets */
void* malloc(size_t size)
{
    void* ptr = heap;
    heap += size;
    return ptr;
}

/* Structures pour l'IDT */
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

/* Forward declarations for interrupt service routines */
void isr0();
void irq1();
extern void load_idt(struct idt_ptr*);
extern void load_gdt(struct idt_ptr*);

/* Basic outb/inb for PIC */
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/* Remplit une entrée de l'IDT */
static void idt_set_gate(int n, void* handler, uint8_t type)
{
    uint64_t addr = (uint64_t)handler;
    idt[n].offset_low  = addr & 0xFFFF;
    idt[n].selector    = 0x08; /* segment code */
    idt[n].ist         = 0;
    idt[n].type_attr   = type;
    idt[n].offset_mid  = (addr >> 16) & 0xFFFF;
    idt[n].offset_high = (addr >> 32) & 0xFFFFFFFF;
    idt[n].zero        = 0;
}

/* Handler d'exception pour la division par zéro */
__attribute__((interrupt))
void isr0_handler(void* frame)
{
    puts("Divide by zero!\n");
    while (1) __asm__("hlt");
}

/* Handler pour l'interruption clavier */
__attribute__((interrupt))
void irq1_handler(void* frame)
{
    uint8_t sc = inb(0x60);
    (void)sc;
    outb(0x20, 0x20); /* Send EOI */
}

/* Installe l'IDT avec quelques handlers */
static void idt_init()
{
    for (int i = 0; i < 256; ++i)
        idt_set_gate(i, isr0, 0x8E); /* default */

    idt_set_gate(0, isr0, 0x8E);
    idt_set_gate(33, irq1, 0x8E);

    idtr.limit = sizeof(idt) - 1;
    idtr.base  = (uint64_t)&idt;
    load_idt(&idtr);
}

/* La GDT est définie dans le fichier d'amorce */
extern struct idt_ptr gdt_descriptor;
static void gdt_init()
{
    load_gdt(&gdt_descriptor);
}

/* Paging déjà configuré en assembleur */
static void paging_init() { }

/* Point d'entrée du noyau en C */
void kernel_main(void)
{
    gdt_init();
    idt_init();
    paging_init();
    puts("Hello from kernel\n");
    while (1)
        __asm__("hlt");
}
