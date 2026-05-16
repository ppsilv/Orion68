#ifndef STDLIB_H
#define STDLIB_H

void *malloc(unsigned int size);
void free(void *ptr);
void *calloc(unsigned int num, unsigned int size);
void *realloc(void *ptr, unsigned int size);

int atoi(const char *str);
char *itoa(int value, char *str, int base);
long atol(const char *str);

void exit(int status);

#endif
