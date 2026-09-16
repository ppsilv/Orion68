#include "stdio_internal.h"
#include "fcntl.h"
#include "unistd.h"
#include "sys/heap.h"

int fclose(FILE *f)
{
    int ret = fflush(f);

    __stdio_unregister(f);
    close(f->fd);
    free(f->buf);
    free(f);

    return ret;
}
