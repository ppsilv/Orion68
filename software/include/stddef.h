/* stddef.h - OrionOS
 * Tipos e macros fundamentais de uso geral (C99 7.17).
 *
 * Ambiente freestanding. Sem dependência de libc.
 */

#ifndef _ORION_STDDEF_H
#define _ORION_STDDEF_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================
 * Tipos fundamentais
 * ============================================================ */

/* Diferença entre dois ponteiros.
 * __PTRDIFF_TYPE__ é fornecido pelo GCC/Clang e é sempre o
 * tipo signed correto para a arquitetura alvo. */
typedef __PTRDIFF_TYPE__ ptrdiff_t;

/* Resultado do operador sizeof.
 * __SIZE_TYPE__ é unsigned e garante cobrir todo o espaço
 * de endereçamento. Em x86_64 é unsigned long (64 bits). */
typedef __SIZE_TYPE__    size_t;

/* Tipo usado para alinhamento máximo.
 * Em C11 é obrigatório. Em C99 é extensão comum (GCC/Clang
 * fornecem __max_align_t de qualquer jeito). */
/* Tipo usado para alinhamento máximo.
 * __BIGGEST_ALIGNMENT__ é fornecido por GCC e Clang e
 * representa o maior alinhamento fundamental da arquitetura. */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    typedef struct {
        _Alignas(__BIGGEST_ALIGNMENT__) char __dummy;
    } max_align_t;
#else
    typedef struct {
        long long   __ll;
        long double __ld;
    } max_align_t;
#endif

/* ============================================================
 * Macros
 * ============================================================ */

/* offsetof: deslocamento de um membro dentro de um struct.
 *
 * Implementação clássica usando aritmética de ponteiro nulo.
 * O cast para (size_t) no final evita warnings de conversão. */
#define offsetof(type, member)  ((size_t)&(((type *)0)->member))

/* NULL: ponteiro nulo.
 *
 * Em C, define como ((void *)0) para dar type-safety.
 * Em C++, NULL é tradicionalmente 0 (ou nullptr em C++11+). */
#ifndef NULL
    #ifdef __cplusplus
        #define NULL 0
    #else
        #define NULL ((void *)0)
    #endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* _ORION_STDDEF_H */
