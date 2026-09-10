/*

Perfeito — isso simplifica bastante, é um heap único, sem precisar isolar por task. 
Vou te dar a família completa: kmalloc_init, kmalloc, kfree, kcalloc e krealloc.

Duas decisões de design que valem explicação antes do código:

Proteção contra interrupção — agora que o systick preempta tasks de verdade, se o 
OS_TickISR/Int2Handler disparar bem no meio de um kmalloc mexendo nos headers da 
lista, e a task escolhida em seguida também chamar kmalloc, a lista fica corrompida. 

Por isso usei o mesmo padrão LOCK/UNLOCK (desliga interrupção) que você já usa no 
ata.c — aqui é ainda mais crítico porque essa heap vai ser mexida por qualquer parte 
do kernel a qualquer momento.

Header de 4 bytes só — no 68000 puro, acesso a word/long em endereço ímpar dá address 
error (trap). Fiz o header caber em exatamente 4 bytes (uint32_t, usando o bit mais 
alto como flag "livre" e o resto como tamanho) pra garantir que tudo fique sempre 
alinhado, sem depender de padding do compilador.

*/
#include "kmalloc.h"
#include "interrupt.h"   /* pra m68k_disable_all_interrupts, usado dentro de LOCK() */
#include <critical.h>    /* macros LOCK/UNLOCK -- precisa vir depois de interrupt.h */
#include <string.h>

/*
 * LOCK/UNLOCK (definidas em critical.h): salvam o SR, desligam todas
 * as interrupcoes, e restauram depois. Precisa disso porque o systick
 * pode preemptar bem no meio de um kmalloc/kfree mexendo nos headers
 * da lista -- sem isso a heap corrompe silenciosamente.
 */

/*
 * Header de 4 bytes. Bit mais alto (KM_FREE_BIT) = 1 quando o bloco
 * esta livre. Os outros 31 bits sao o tamanho da area de dados (sem
 * contar o proprio header). Cabe tranquilo: nao tem RAM homebrew
 * nesse projeto que chegue perto de 2GB.
 */
typedef struct {
    uint32_t size_and_flag;
} km_header_t;

#define KM_FREE_BIT     0x80000000UL
#define KM_SIZE(h)      ((h)->size_and_flag & ~KM_FREE_BIT)
#define KM_IS_FREE(h)   (((h)->size_and_flag & KM_FREE_BIT) != 0)
#define KM_SET(h, sz, f) ((h)->size_and_flag = ((sz) & ~KM_FREE_BIT) | ((f) ? KM_FREE_BIT : 0))

#define HDR_SIZE        ((uint32_t) sizeof(km_header_t))
#define KM_ALIGN(x)     (((x) + 3u) & ~3u)
#define MIN_SPLIT       16u   /* nao compensa dividir se sobrar menos que isso de bloco novo */

static uint8_t  *heap_base = NULL;
static uint32_t  heap_size = 0;

void kmalloc_init(void *base, uint32_t size)
{
    heap_base = (uint8_t *) base;
    heap_size = size;

    km_header_t *h = (km_header_t *) heap_base;
    KM_SET(h, size - HDR_SIZE, 1);
}

void *kmalloc(uint32_t size)
{
    short saved_sr;
    uint8_t *p, *end;
    void *result = NULL;

    if (heap_base == NULL || size == 0)
        return NULL;

    size = KM_ALIGN(size);
    end = heap_base + heap_size;

    LOCK(saved_sr);

    p = heap_base;
    while (p < end) {
        km_header_t *h = (km_header_t *) p;
        uint32_t block_size = KM_SIZE(h);

        if (KM_IS_FREE(h) && block_size >= size) {
            /* sobra espaco suficiente pra virar outro bloco util? divide. */
            if (block_size >= size + HDR_SIZE + MIN_SPLIT) {
                km_header_t *next = (km_header_t *) (p + HDR_SIZE + size);
                KM_SET(next, block_size - size - HDR_SIZE, 1);
                KM_SET(h, size, 0);
            } else {
                KM_SET(h, block_size, 0);   /* usa o bloco inteiro, sem dividir */
            }
            result = p + HDR_SIZE;
            break;
        }

        p += HDR_SIZE + block_size;
    }

    UNLOCK(saved_sr);
    return result;
}

void kfree(void *ptr)
{
    short saved_sr;
    km_header_t *h, *next;
    uint8_t *next_addr;

    if (ptr == NULL || heap_base == NULL)
        return;

    LOCK(saved_sr);

    h = (km_header_t *) ((uint8_t *) ptr - HDR_SIZE);
    KM_SET(h, KM_SIZE(h), 1);

    /* funde com o bloco seguinte se ele tambem estiver livre.
     * (so funde pra frente -- fundir pra tras exigiria varrer a heap
     * inteira do inicio; se isso virar problema de fragmentacao na
     * pratica, da pra trocar por uma lista com ponteiro 'prev'.) */
    next_addr = (uint8_t *) h + HDR_SIZE + KM_SIZE(h);
    if (next_addr < heap_base + heap_size) {
        next = (km_header_t *) next_addr;
        if (KM_IS_FREE(next)) {
            KM_SET(h, KM_SIZE(h) + HDR_SIZE + KM_SIZE(next), 1);
        }
    }

    UNLOCK(saved_sr);
}

void *kcalloc(uint32_t nmemb, uint32_t size)
{
    uint32_t total = nmemb * size;   /* projeto homebrew, sem overflow check aqui de proposito -- valores pequenos */
    void *p = kmalloc(total);
    if (p != NULL)
        memset(p, 0, total);
    return p;
}

void *krealloc(void *ptr, uint32_t size)
{
    km_header_t *h;
    uint32_t old_size;
    void *new_ptr;

    if (ptr == NULL)
        return kmalloc(size);

    if (size == 0) {
        kfree(ptr);
        return NULL;
    }

    h = (km_header_t *) ((uint8_t *) ptr - HDR_SIZE);
    old_size = KM_SIZE(h);

    if (KM_ALIGN(size) <= old_size)
        return ptr;   /* ja cabe no bloco atual, nao mexe em nada */

    new_ptr = kmalloc(size);
    if (new_ptr == NULL)
        return NULL;   /* ptr original continua valido, igual realloc() de verdade */

    memcpy(new_ptr, ptr, old_size);
    kfree(ptr);
    return new_ptr;
}