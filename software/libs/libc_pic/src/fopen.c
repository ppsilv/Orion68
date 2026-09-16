#include "stdio_internal.h"
#include "fcntl.h"
#include "unistd.h"
#include "sys/heap.h"

FILE *fopen(const char *path, const char *mode)
{
    int flags = (mode[0] == 'r') ? O_RDONLY : O_WRONLY | O_CREAT;

    int fd = open(path, flags);
    if (fd < 0)
        return NULL;

    FILE *f = (FILE *)malloc(sizeof(FILE));
    if (!f) {
        close(fd);
        return NULL;
    }

    f->fd        = fd;
    f->buf_mode  = _IOFBF;
    f->buf_size  = 512;
    f->buf_pos   = 0;
    f->buf       = (unsigned char *)malloc(f->buf_size);
    f->error     = 0;
    f->eof       = 0;

    __stdio_register(f);

    return f;
}





/*
int fopen(int fd, char* path, BYTE mode) {
    return do_trap3(SYS_FOPEN, fd, (long)path,(long)mode);
}
int fclose(int fd) {
    return do_trap3(SYS_FCLOSE, fd, 0, 0);
}
int fread(int fd, void* buff, long btr, long* br) {
    return do_trap4(SYS_FREAD, fd, (long)buff, btr, (long)br);
}
*
/* lá no receptor
 case SYS_FREAD: {
    int   fd  = (int)a1;
    void* buf = (void*)a2;
    UINT  btr = (UINT)a3;
    UINT* br  = (UINT*)a4;
    ...
}
*/
//int fwrite(int fd, void* buff, long btw, long* br) {
//    return do_trap4(SYS_FWRITE, fd, (long)buff, btw, (long)br);
//}

