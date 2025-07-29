#ifndef SYSCALLS_H
#define SYSCALLS_H
#include <stddef.h>

long syscall_handler(long num, long arg1, long arg2, long arg3);

#define SYSCALL_WRITE 1
#define SYSCALL_VULN  42

#endif
