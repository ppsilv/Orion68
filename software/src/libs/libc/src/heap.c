#include "sys/heap.h"

typedef struct block_header {
    size_t                size;   /* tamanho da area util (sem contar o header) */
    int                   free;   /* 1 = livre, 0 = em uso */
    struct block_header*  next;
} block_header_t;

#define ALIGN 4
#define HDR_SIZE (sizeof(block_header_t))

static block_header_t* heap_start = NULL;

static size_t align_up(size_t n) {
    return (n + (ALIGN - 1)) & ~(ALIGN - 1);
}

void __heap_init(void* base, size_t size) {
    heap_start = (block_header_t*)base;
    heap_start->size = size - HDR_SIZE;
    heap_start->free = 1;
    heap_start->next = NULL;
}

void* malloc(size_t size) {
    size = align_up(size);
    block_header_t* curr = heap_start;

    while (curr) {
        if (curr->free && curr->size >= size) {
            /* sobrou espaço suficiente pra splitar em dois blocos? */
            if (curr->size >= size + HDR_SIZE + ALIGN) {
                block_header_t* rest = (block_header_t*)((char*)curr + HDR_SIZE + size);
                rest->size = curr->size - size - HDR_SIZE;
                rest->free = 1;
                rest->next = curr->next;

                curr->size = size;
                curr->next = rest;
            }
            curr->free = 0;
            return (void*)((char*)curr + HDR_SIZE);
        }
        curr = curr->next;
    }
    return NULL; /* sem memoria */
}

void free(void* ptr) {
    if (!ptr) return;

    block_header_t* blk = (block_header_t*)((char*)ptr - HDR_SIZE);
    blk->free = 1;

    /* funde com o proximo bloco se ele tambem estiver livre */
    if (blk->next && blk->next->free) {
        blk->size += HDR_SIZE + blk->next->size;
        blk->next = blk->next->next;
    }
}
