#ifndef STDIO_H
#define STDIO_H

#include <stddef.h>


extern void  kputchar(char c);
extern char  kgetchar(void);
extern int   kprintf(const char *format, ...);
extern int   ksprintf(char *str, const char *format, ...);
extern void  kputs(char *str);
extern char* kgets_s( char* str, int n );
extern char kbdhit(void);


#endif
