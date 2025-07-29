#include "memory.h"

extern unsigned char kernel_end; // defined by linker
static unsigned char* heap = &kernel_end;

void memory_init(void) {}

void* kmalloc(size_t size) {
    void* ptr = heap;
    heap += size;
    return ptr;
}

void kfree(void* ptr) {
    (void)ptr;
}
