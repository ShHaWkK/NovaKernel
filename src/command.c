#include "command.h"
#include "string.h"

extern struct command_entry command_table[];

const struct command_entry* command_find(const char* name) {
    for (int i = 0; command_table[i].name; i++) {
        if (!strcmp(command_table[i].name, name))
            return &command_table[i];
    }
    return 0;
}
