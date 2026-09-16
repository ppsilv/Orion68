#include "stdio_internal.h"
#include "fcntl.h"
#include "unistd.h"
#include "sys/heap.h"



int fflush(FILE *f)
{
    if (!f) return -1;

    if (f->buf_mode != _IONBF && f->buf_pos > 0) {
        int written = write(f->fd, f->buf, f->buf_pos);
        if (written < 0) {
            f->error = 1;
            return -1;
        }
        f->buf_pos = 0;
    }
    return 0;
}
