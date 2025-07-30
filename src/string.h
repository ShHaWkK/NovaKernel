#ifndef STRING_H
#define STRING_H
#include <stddef.h>

int strcmp(const char* a, const char* b);
size_t strlen(const char* s);
void* memcpy(void* dst, const void* src, size_t n);

#endif

