#include "filesystem.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "atapi.h"
#include "k/iso9660.h"

static struct fd_entry fd_tab[64];

int split_path(const char* path, char parts[64][64])
{
    int count = 0;
    int start = 0;
    int i = 0;

    while (1)
    {
        if (path[i] == '/' || path[i] == '\0')
        {
            int len = i - start;

            if (len > 0 && count < 64)
            {
                memcpy(parts[count], &path[start], len);
                parts[count][len] = '\0';
                count++;
            }

            start = i + 1;
        }

        if (path[i] == '\0')
            break;

        i++;
    }

    return count;
}

int find(u32 root_lba, u32 root_size, const char* filename, struct iso_dir* out)
{
    while (root_size > 0)
    {
        const u8* block = read_block(root_lba);
        u32 off = 0;

        while (off < ISO_BLOCK_SIZE && off < root_size)
        {
            struct iso_dir* d = (struct iso_dir*)(block + off);
            if (d->dir_size == 0)
                break;

            if (d->idf_len == 1 && ((u8)d->idf[0] <= 1))
            {
                off += d->dir_size;
                continue;
            }

            u8 len = d->idf_len;
            if (len >= 2 && d->idf[len - 2] == ';')
                len -= 2;
            if (len > 0 && d->idf[len - 1] == '.')
                len -= 1;
            if (strncasecmp(d->idf, filename, len) == 0
                && strlen(filename) == len)
            {
                printf("found: %.*s size=%d lba=%d\n", d->idf_len, d->idf,
                       d->file_size.le, d->data_blk.le);
                memcpy(out, d, sizeof(struct iso_dir));
                return 1;
            }

            off += d->dir_size;
        }

        root_size -= (root_size > ISO_BLOCK_SIZE ? ISO_BLOCK_SIZE : root_size);
        root_lba++;
    }

    return 0;
}

int open(const char* path, int flags)
{
    (void)flags;

    if (!path || path[0] == '\0')
        return -1;

    struct iso_prim_voldesc pvd_copy;
    memcpy(&pvd_copy, read_block(ISO_PRIM_VOLDESC_BLOCK), sizeof(pvd_copy));

    u32 root_lba = pvd_copy.root_dir.data_blk.le;
    u32 root_size = pvd_copy.root_dir.file_size.le;

    char file_path[64][64];
    int n = split_path(path, file_path);

    if (n == 0)
        return -1;

    for (int i = 0; i < n; i++)
    {
        struct iso_dir entry_copy;
        if (!find(root_lba, root_size, file_path[i], &entry_copy))
        {
            return -1;
        }
        root_lba = entry_copy.data_blk.le;
        root_size = entry_copy.file_size.le;
    }

    for (int fd = 0; fd < 64; fd++)
    {
        if (!fd_tab[fd].active)
        {
            fd_tab[fd].active = 1;
            fd_tab[fd].lba = root_lba;
            fd_tab[fd].size = root_size;
            fd_tab[fd].cursor = 0;
            return fd;
        }
    }
    return -1;
}

ssize_t read(int fd, void* buf, size_t count)
{
    if (fd_tab[fd].cursor >= fd_tab[fd].size)
    {
        return 0;
    }

    size_t rest = fd_tab[fd].size - fd_tab[fd].cursor;
    if (count > rest)
        count = rest;

    u8* dst = (u8*)buf;

    size_t tmp = count;
    while (tmp > 0)
    {
        u32 lba = fd_tab[fd].lba + (fd_tab[fd].cursor / ISO_BLOCK_SIZE);
        u32 offset = fd_tab[fd].cursor % ISO_BLOCK_SIZE;
        size_t chunk = ISO_BLOCK_SIZE - offset;
        if (chunk > tmp)
            chunk = tmp;
        const u8* block = read_block(lba);
        memcpy(dst, block + offset, chunk);
        dst += chunk;
        fd_tab[fd].cursor += chunk;
        tmp -= chunk;
    }
    return (ssize_t)count;
}

off_t seek(int fd, off_t offset, int whence)
{
    if (fd < 0 || fd > 64)
        return -1;

    int tmp;

    switch (whence)
    {
    case SEEK_SET:
        tmp = offset;
        break;
    case SEEK_CUR:
        tmp = (off_t)fd_tab[fd].cursor + offset;
        break;
    case SEEK_END:
        tmp = (off_t)fd_tab[fd].size + offset;
        break;
    default:
        return (off_t)-1;
    }

    if (tmp < 0)
    {
        return (off_t)-1;
    }

    fd_tab[fd].cursor = tmp;

    return fd_tab[fd].cursor;
}

int close(int fd)
{
    if (fd < 0 || fd > 64)
        return -1;

    fd_tab[fd].active = 0;

    return 0;
}
