#include "stdio_internal.h"
#include "sys/vfs.h"
#include "sys/heap.h"

FILE *fopen(const char *path, const char *mode)
{
    int flags = (mode[0] == 'r') ? O_RDONLY : O_WRONLY | O_CREAT;

    int fd = vfs_open(path, flags);
    if (fd < 0)
        return NULL;

    FILE *f = (FILE *)malloc(sizeof(FILE));
    if (!f) {
        vfs_close(fd);
        return NULL;
    }

    f->fd        = fd;
    f->buf_mode  = _IOFBF;      /* arquivo em disco: full buffered por padrão */
    f->buf_size  = 512;
    f->buf_pos   = 0;
    f->buf       = (unsigned char *)malloc(f->buf_size);
    f->error     = 0;
    f->eof       = 0;

    __stdio_register(f);        /* é isso que garante o flush no exit() */

    return f;
}

int fflush(FILE *f)
{
    if (!f) return -1;

    if (f->buf_mode != _IONBF && f->buf_pos > 0) {
        int written = vfs_write(f->fd, f->buf, f->buf_pos);
        if (written < 0) {
            f->error = 1;
            return -1;
        }
        f->buf_pos = 0; /* buffer esvaziado */
    }
    return 0;
}

int fclose(FILE *f)
{
    int ret = fflush(f);

    __stdio_unregister(f);      /* sai da lista - exit() não mexe mais nele */
    vfs_close(f->fd);
    free(f->buf);
    free(f);

    return ret;
}
