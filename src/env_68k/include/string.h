#ifndef __STRING_H__
#define __STRING_H__

int strlen(const char *str);
char *strcpy(char *dest, const char *src);
void *memcpy(void *dest, const void *src, int n);
void memset(void *s, int c, int n);
char *strncpy(char *dest, const char *src, int n);
int strncmp(const char *s1, const char *s2, int n);
char *strtok(char *str, const char *delim);

#endif

