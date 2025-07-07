#include "syscalls.h"
#include "shell.h"
#include <stdint.h>

extern void syscall_return();

long syscall_write(const char* s) {
    while (*s) {
        __asm__("outb %0, $0xe9" :: "a"(*s));
        s++;
    }
    return 0;
}

/* Intentionally vulnerable syscall: writes to kernel memory without checks */
long syscall_vuln(uint64_t ptr, uint64_t len) {
    char* dst = (char*)ptr;
    for (uint64_t i = 0; i < len; i++)
        dst[i] = 'A';
    return 0;
}

long syscall_handler(long num, long arg1, long arg2, long arg3) {
    switch (num) {
    case SYSCALL_WRITE:
        return syscall_write((const char*)arg1);
    case SYSCALL_VULN:
        return syscall_vuln((uint64_t)arg1, (uint64_t)arg2);
    default:
        return -1;
    }
}