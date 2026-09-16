#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>

/* Inicializa o heap dentro de uma região de memória já reservada
 * (ex: um pedaço do slot de 256K da task, depois do .bss). */
void  __heap_init(void* base, size_t size);

void* malloc(size_t size);
void  free(void* ptr);

#endif
