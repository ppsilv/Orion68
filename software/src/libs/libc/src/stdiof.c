#include "stdio_internal.h"
#include "sys/vfs.h"   /* vfs_write() */

static int flush_buffer(FILE *f)
{
    if (f->buf_pos == 0)
        return 0;

    int written = vfs_write(f->fd, f->buf, f->buf_pos);
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
        if (vfs_write(f->fd, &ch, 1) != 1) {
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

/* fopen() chama isso ao criar um FILE* novo; __libc_init também chama
   pra stdin/stdout/stderr, assim eles entram na mesma lista */
void __stdio_register(FILE *f)
{
    f->next = __open_files;
    __open_files = f;
}

/* fclose() chama isso antes de liberar o FILE* - se você esquecer de
   chamar fclose, o arquivo continua na lista, e é exatamente por isso
   que o exit() ainda consegue flushar ele mesmo sem fclose explícito */
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

/* chamado pelo exit(), antes da syscall final de terminar a task -
   percorre TODOS os arquivos abertos, não só os 3 fixos */
void __stdio_flush_all(void)
{
    for (FILE *f = __open_files; f != NULL; f = f->next)
        fflush(f);
}
