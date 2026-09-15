#ifndef _UNISTD_H
#define _UNISTD_H

#include <stdint.h>

/* valores de 'whence' pra lseek() */
#define SEEK_SET   0
#define SEEK_CUR   1
#define SEEK_END   2

int      read(int fd, void *buf, int count);
int      write(int fd, const void *buf, int count);
int      close(int fd);
int32_t  lseek(int fd, uint32_t offset, int whence);

#endif /* _UNISTD_H */
