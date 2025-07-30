#include "string.h"

int strcmp(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return *(const unsigned char*)a - *(const unsigned char*)b;
}

size_t strlen(const char* s) {
    size_t n = 0;
    while (s[n]) n++;
    return n;
}

void* memcpy(void* dst, const void* src, size_t n) {
    unsigned char* d = dst;
    const unsigned char* s = src;
    for (size_t i = 0; i < n; i++)
        d[i] = s[i];
    return dst;
}


