#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <k/iso9660.h>
#include <k/kstd.h>
#include <k/types.h>

#include "atapi.h"

struct fd_entry
{
    u8 active;
    u32 lba;
    u32 cursor;
    u32 size;
};

int open(const char* pathname, int flags);
ssize_t read(int fd, void* buf, size_t count);
off_t seek(int fd, off_t offset, int whence);
int close(int fd);

#endif /* !FILESYSTEM_H */
