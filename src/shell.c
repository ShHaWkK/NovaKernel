#include "shell.h"
#include "scheduler.h"
#include "syscalls.h"
#include <stdint.h>

static void print(const char* s) {
    syscall_handler(SYSCALL_WRITE, (long)s, 0, 0);
}

void shell(void) {
    print("NovaShell> \0");
}