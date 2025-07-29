#include "shell.h"
#include "scheduler.h"
#include "syscalls.h"
#include "../drivers/keyboard.h"
#include <stdint.h>

static void print_char(char c) {
    char buf[2] = { c, 0 };
    syscall_handler(SYSCALL_WRITE, (long)buf, 0, 0);
}

static void print(const char* s) {
    syscall_handler(SYSCALL_WRITE, (long)s, 0, 0);
}

static int str_eq(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++; b++;
    }
    return *a == *b;
}

static int str_starts(const char* s, const char* p) {
    while (*p) {
        if (*s++ != *p++) return 0;
    }
    return 1;
}

static void handle_cmd(const char* cmd) {
    if (str_eq(cmd, "help")) {
        print("Commands: help, echo\n");
    } else if (str_starts(cmd, "echo ")) {
        print(cmd + 5);
        print("\n");
    } else {
        print("Unknown command\n");
    }
}

void shell(void) {
    char buf[64];
    int pos = 0;
    print("NovaShell> ");
    while (1) {
        int ch = keyboard_getc();
        if (ch < 0)
            continue;
        char c = (char)ch;
        print_char(c);
        if (c == '\n') {
            buf[pos] = 0;
            handle_cmd(buf);
            pos = 0;
            print("NovaShell> ");
        } else if (pos < (int)sizeof(buf) - 1) {
            buf[pos++] = c;
        }
    }
}
