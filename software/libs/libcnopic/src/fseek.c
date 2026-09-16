#include "stdio_internal.h"
#include "fcntl.h"
#include "unistd.h"
#include "sys/heap.h"



int fseek(FILE *f, long offset, int whence)
{
    /* descarrega o buffer de escrita ANTES de mudar posicao, senao
       esses bytes pendentes vao parar escritos no lugar errado */
    if (fflush(f) != 0)
        return -1;

    int32_t pos = lseek(f->fd, (uint32_t)offset, whence);
    if (pos < 0) {
        f->error = 1;
        return -1;
    }

    f->eof = 0;
    return 0;
}
