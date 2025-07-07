#ifndef FS_H
#define FS_H

void fs_init(void);
int fs_read_file(const char* path, void* buf, int max);

#endif