#include "fs.h"
#include "memory.h"
#include "string.h"

struct file {
    const char* name;
    const char* data;
};

static struct file ram_files[] = {
    {"hello.txt", "Hello from NovaKernel!\n"},
    {"readme.txt", "This is an in-memory file system.\n"},
};
static const int ram_file_count = sizeof(ram_files)/sizeof(ram_files[0]);

void fs_init(void) {
    /* Nothing to do for RAM files */
}

int fs_list(const char** names, int max) {
    int n = (ram_file_count < max) ? ram_file_count : max;
    for (int i = 0; i < n; i++)
        names[i] = ram_files[i].name;
    return n;
}

int fs_read_file(const char* path, void* buf, int max) {
    for (int i = 0; i < ram_file_count; i++) {
        if (!strcmp(path, ram_files[i].name)) {
            int len = strlen(ram_files[i].data);
            if (len > max) len = max;
            memcpy(buf, ram_files[i].data, len);
            return len;
        }
    }
    return -1;
}

