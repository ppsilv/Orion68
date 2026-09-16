#include "stdio_internal.h"
#include "unistd.h"   /* write() */

static int flush_buffer(FILE *f)
{
    if (f->buf_pos == 0)
        return 0;

    int written = write(f->fd, f->buf, f->buf_pos);
    f->buf_pos = 0;

    if (written < 0) {
        f->error = 1;
        return -1;
    }
    return 0;
}

int fputc(int c, FILE *f)
{
    if (f->buf_mode == _IONBF) {
        unsigned char ch = (unsigned char)c;
        if (write(f->fd, &ch, 1) != 1) {
            f->error = 1;
            return -1;
        }
        return c;
    }

    f->buf[f->buf_pos++] = (unsigned char)c;

    if (f->buf_pos >= f->buf_size)
        flush_buffer(f);
    else if (f->buf_mode == _IOLBF && c == '\n')
        flush_buffer(f);

    return c;
}

int fflush(FILE *f)
{
    return flush_buffer(f);
}

FILE *__open_files = NULL;

void __stdio_register(FILE *f)
{
    f->next = __open_files;
    __open_files = f;
}

void __stdio_unregister(FILE *f)
{
    FILE **p = &__open_files;
    while (*p) {
        if (*p == f) {
            *p = f->next;
            return;
        }
        p = &(*p)->next;
    }
}

void __stdio_flush_all(void)
{
    for (FILE *f = __open_files; f != NULL; f = f->next)
        fflush(f);
}


