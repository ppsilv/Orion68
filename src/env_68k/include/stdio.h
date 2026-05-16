#ifndef STDIO_H
#define STDIO_H

#include <stddef.h>

void putchar(char c);
void puts(const char *str);

int printf(const char *format, ...);
int sprintf(char *str, const char *format, ...);
int getchar(void);
char* gets_s( char* str, int n );

#endif
