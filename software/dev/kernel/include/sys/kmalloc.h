#ifndef KMALLOC_H
#define KMALLOC_H

#include <stdint.h>

/*
 * Heap global do kernel -- separado dos slots de 256K de cada task.
 *
 * kmalloc_init() PRECISA ser chamado uma vez, antes de qualquer
 * kmalloc()/kfree(), passando uma regiao de RAM que nao seja usada
 * por mais nada (nem pelos slots de task, nem pela stack do kernel).
 *
 * 'base' deve estar alinhado em 4 bytes (a maioria dos enderecos de
 * inicio de RAM do seu mapa ja e', mas confira antes de chamar).
 */
void  kmalloc_init(void *base, uint32_t size);

void *kmalloc(uint32_t size);
void  kfree(void *ptr);
void *kcalloc(uint32_t nmemb, uint32_t size);
void *krealloc(void *ptr, uint32_t size);

#endif
