#include "stdio_internal.h"
#include "unistd.h"
#include <string.h>

static int flush_buffer(FILE *f); /* já existe em stdio_internal.c */

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *f)
{
    size_t total = size * nmemb;
    const unsigned char *p = (const unsigned char *)ptr;
    size_t remaining = total;

    if (f->buf_mode == _IONBF) {
        int n = write(f->fd, p, (int)total);
        if (n < 0) {
            f->error = 1;
            return 0;
        }
        return (size_t)n / size;
    }

    while (remaining > 0) {
        size_t space = f->buf_size - f->buf_pos;
        size_t chunk = (remaining < space) ? remaining : space;

        memcpy(f->buf + f->buf_pos, p, chunk);
        f->buf_pos += chunk;
        p          += chunk;
        remaining  -= chunk;

        /* buffer encheu -- descarrega antes de copiar o resto */
        if (f->buf_pos >= f->buf_size) {
            if (flush_buffer(f) != 0)
                return (total - remaining) / size;
        }
    }

    /* line-buffered: se tinha '\n' no bloco, descarrega agora
       (checagem simples -- so' olha o ultimo byte escrito) */
    if (f->buf_mode == _IOLBF && total > 0 && p[-1] == '\n')
        flush_buffer(f);

    return nmemb;
}
