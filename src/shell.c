#include "shell.h"
#include "scheduler.h"
#include "syscalls.h"
#include "fs.h"
#include "command.h"
#include "../drivers/keyboard.h"
#include "../drivers/timer.h"
#include <stdint.h>
#include "string.h"

static void print_char(char c) {
    char buf[2] = { c, 0 };
    syscall_handler(SYSCALL_WRITE, (long)buf, 0, 0);
}

static void print_char(char c) {
    char buf[2] = { c, 0 };
    syscall_handler(SYSCALL_WRITE, (long)buf, 0, 0);
}

static void print(const char* s) {
    syscall_handler(SYSCALL_WRITE, (long)s, 0, 0);
}

static void cmd_help(const char* args) {
    (void)args;
    print("Commands: help, echo, ls, cat, uptime\n");
}

static void cmd_echo(const char* args) {
    print(args);
    print("\n");
}

static void cmd_ls(const char* args) {
    (void)args;
    const char* names[8];
    int n = fs_list(names, 8);
    for (int i = 0; i < n; i++) {
        print(names[i]);
        print("\n");
    }
}

static void cmd_cat(const char* args) {
    char buf[128];
    int r = fs_read_file(args, buf, sizeof(buf) - 1);
    if (r >= 0) {
        buf[r] = 0;
        print(buf);
        print("\n");
    } else {
        print("File not found\n");
    }
}

static void cmd_uptime(const char* args) {
    (void)args;
    unsigned long t = timer_ticks();
    char num[32];
    int i = 30; num[31] = 0; num[30] = '\n';
    do { num[i--] = '0' + (t % 10); t /= 10; } while (t && i > 0);
    print(&num[i+1]);
}

COMMAND_LIST_START
COMMAND("help",   cmd_help)
COMMAND("echo",   cmd_echo)
COMMAND("ls",     cmd_ls)
COMMAND("cat",    cmd_cat)
COMMAND("uptime", cmd_uptime)
COMMAND_LIST_END

static void dispatch(const char* line) {
    char name[16];
    int i = 0;
    while (*line && *line != ' ' && i < (int)sizeof(name) - 1)
        name[i++] = *line++;
    name[i] = 0;
    while (*line == ' ')
        line++;
    const struct command_entry* cmd = command_find(name);
    if (cmd)
        cmd->func(line);
    else
        print("Unknown command\n");
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
            dispatch(buf);
            pos = 0;
            print("NovaShell> ");
        } else if (pos < (int)sizeof(buf) - 1) {
            buf[pos++] = c;
        }
    }
}
