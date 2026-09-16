#include "stdio_internal.h"
#include "fcntl.h"
#include "unistd.h"
#include "sys/heap.h"

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *f)
{
    size_t total = size * nmemb;

    int n = read(f->fd, ptr, (int)total);
    if (n < 0) {
        f->error = 1;
        return 0;
    }
    if ((size_t)n < total)
        f->eof = 1;

    return (size_t)n / size;
}
