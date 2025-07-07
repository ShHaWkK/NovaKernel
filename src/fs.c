#include "memory.h"

void fs_init(void) {}

/* Dummy file reader */
int fs_read_file(const char* path, void* buf, int max) {
    (void)path; (void)buf; (void)max;
    return -1;
}