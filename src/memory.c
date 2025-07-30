#include "memory.h"

extern unsigned char kernel_end; // defined by linker
static unsigned char* heap = &kernel_end;

struct block { size_t size; struct block* next; };
static struct block* free_list = 0;

void memory_init(void) {
    free_list = 0;
}

void* kmalloc(size_t size) {
    size = (size + 15) & ~15; /* align */
    struct block **prev = &free_list, *b = free_list;
    while (b) {
        if (b->size >= size) {
            *prev = b->next;
            return (void*)(b + 1);
        }
        prev = &b->next;
        b = b->next;
    }
    struct block* blk = (struct block*)heap;
    heap += sizeof(struct block) + size;
    blk->size = size;
    return (void*)(blk + 1);
}

void kfree(void* ptr) {
    if (!ptr) return;
    struct block* blk = ((struct block*)ptr) - 1;
    blk->next = free_list;
    free_list = blk;
}

