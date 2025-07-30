#ifndef COMMAND_H
#define COMMAND_H

typedef void (*command_fn)(const char* args);

struct command_entry {
    const char* name;
    command_fn func;
};

#define COMMAND_LIST_START struct command_entry command_table[] = {
#define COMMAND(name, fn) { name, fn },
#define COMMAND_LIST_END {0, 0} };

const struct command_entry* command_find(const char* name);

#endif
