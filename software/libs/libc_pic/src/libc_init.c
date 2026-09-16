#include "stdio_internal.h"
#include "sys/vfs.h"     /* vfs_open() - ajuste pro nome real da sua API */
#include "sys/heap.h"    /* __heap_init(), malloc() */

/* As três structs FILE ficam estáticas na libc - não tem malloc ainda
   rodando quando isso começa, então não dá pra alocá-las dinamicamente. */
static FILE stdin_file;
static FILE stdout_file;
static FILE stderr_file;

FILE *stdin  = &stdin_file;
FILE *stdout = &stdout_file;
FILE *stderr = &stderr_file;

#define STACK_RESERVED 8192  /* ajuste pro valor que voce ja usa */
#define SLOT_SIZE       256000

extern long data_end_offset;
extern long task_base;

static void init_file(FILE *f, int fd, int mode, size_t buf_size)
{
    f->fd        = fd;
    f->buf_mode  = mode;
    f->buf_pos   = 0;
    f->buf_size  = buf_size;
    f->buf       = buf_size ? (unsigned char *)malloc(buf_size) : NULL;
    f->error     = 0;
    f->eof       = 0;

    /* entra na mesma lista que fopen() usa - assim o exit() flusha
       stdin/stdout/stderr e todo arquivo aberto pelo programa, com
       o mesmo laço, sem caso especial pros 3 fixos */
    __stdio_register(f);
}

/* Chamado pelo crt0.s, antes de main. */
void __libc_init(int argc, char **argv)
{
    (void)argc;
    (void)argv;


   uint32_t heap_base = task_base + ((data_end_offset + 3) & ~3); // alinha em 4
   uint32_t heap_size = SLOT_SIZE - ((data_end_offset + 3) & ~3) - STACK_RESERVED;

   __heap_init((void*)heap_base, heap_size);

    int console_fd = vfs_open("/dev/console", O_RDWR);

    /* stdin: sem termios/raw mode ainda, unbuffered simplifica a vida */
    init_file(stdin, console_fd, _IONBF, 0);

    /* stdout: line buffered - junta os bytes e só manda pro DUART no '\n',
       em vez de um write por caractere */
    init_file(stdout, console_fd, _IOLBF, 256);

    /* stderr: sempre unbuffered - erro tem que aparecer na hora, mesmo
       que o programa trave logo depois */
    init_file(stderr, console_fd, _IONBF, 0);
}
