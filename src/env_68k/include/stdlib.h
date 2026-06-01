#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include <errno.h>

#define NULL 0

void *malloc(unsigned int size);
void free(void *ptr);
void *calloc(unsigned int num, unsigned int size);
void *realloc(void *ptr, unsigned int size);

int atoi(const char *str);
void exit(int status);

long atol(const char *str);
long long atoll (const char *str);

char * __itoa (int value, char *str, int base);
char * itoa (int value, char *str, int base);


char *  utoa (unsigned value, char *str,  int base);
char * __utoa (unsigned value, char *str,  int base);

long strtol(const char *nptr, char **endptr, int base);
long long strtoll(const char *str, char **endptr, int base);
uintmax_t strntoumax(const unsigned char *nptr, char **endptr, int base, unsigned int  n);

char isspace(char c);
char isdigit(char c);
char isalpha(char c);
char isupper(char c);
char islower(char c);

#endif
