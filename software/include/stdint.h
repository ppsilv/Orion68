/* stdint.h - OrionOS
 * Tipos inteiros de largura fixa.
 *
 * Escrito para funcionar em ambiente freestanding (sem libc).
 * Usa __INT8_TYPE__, __INT16_TYPE__, etc. do compilador (GCC/Clang)
 * que são os únicos garantidos em -ffreestanding.
 */

#ifndef _ORION_STDINT_H
#define _ORION_STDINT_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================
 * Detecção de ambiente
 * ============================================================ */
#if defined(__STDC_HOSTED__) && (__STDC_HOSTED__ == 1)
    /* Ambiente hosted: podemos usar os limites do compilador */
    #define __ORION_HOSTED 1
#else
    #define __ORION_HOSTED 0
#endif

/* ============================================================
 * Tipos com largura exata
 * ============================================================ */
typedef __INT8_TYPE__    int8_t;
typedef __INT16_TYPE__   int16_t;
typedef __INT32_TYPE__   int32_t;
typedef __INT64_TYPE__   int64_t;

typedef __UINT8_TYPE__   uint8_t;
typedef __UINT16_TYPE__  uint16_t;
typedef __UINT32_TYPE__  uint32_t;
typedef __UINT64_TYPE__  uint64_t;

/* ============================================================
 * Tipos de menor largura (pelo menos N bits)
 * ============================================================ */
typedef __INT_LEAST8_TYPE__    int_least8_t;
typedef __INT_LEAST16_TYPE__   int_least16_t;
typedef __INT_LEAST32_TYPE__   int_least32_t;
typedef __INT_LEAST64_TYPE__   int_least64_t;

typedef __UINT_LEAST8_TYPE__   uint_least8_t;
typedef __UINT_LEAST16_TYPE__  uint_least16_t;
typedef __UINT_LEAST32_TYPE__  uint_least32_t;
typedef __UINT_LEAST64_TYPE__  uint_least64_t;

/* ============================================================
 * Tipos mais rápidos (pelo menos N bits)
 * ============================================================ */
typedef __INT_FAST8_TYPE__    int_fast8_t;
typedef __INT_FAST16_TYPE__   int_fast16_t;
typedef __INT_FAST32_TYPE__   int_fast32_t;
typedef __INT_FAST64_TYPE__   int_fast64_t;

typedef __UINT_FAST8_TYPE__   uint_fast8_t;
typedef __UINT_FAST16_TYPE__  uint_fast16_t;
typedef __UINT_FAST32_TYPE__  uint_fast32_t;
typedef __UINT_FAST64_TYPE__  uint_fast64_t;

/* ============================================================
 * Tipos para ponteiros e maior inteiro
 * ============================================================ */
typedef __INTPTR_TYPE__   intptr_t;
typedef __UINTPTR_TYPE__  uintptr_t;

typedef __INTMAX_TYPE__   intmax_t;
typedef __UINTMAX_TYPE__  uintmax_t;

/* ============================================================
 * Limites de tipos com largura exata
 * ============================================================ */
#define INT8_MIN    (-__INT8_MAX__ - 1)
#define INT16_MIN   (-__INT16_MAX__ - 1)
#define INT32_MIN   (-__INT32_MAX__ - 1)
#define INT64_MIN   (-__INT64_MAX__ - 1)

#define INT8_MAX    __INT8_MAX__
#define INT16_MAX   __INT16_MAX__
#define INT32_MAX   __INT32_MAX__
#define INT64_MAX   __INT64_MAX__

#define UINT8_MAX   __UINT8_MAX__
#define UINT16_MAX  __UINT16_MAX__
#define UINT32_MAX  __UINT32_MAX__
#define UINT64_MAX  __UINT64_MAX__

/* ============================================================
 * Limites de tipos least
 * ============================================================ */
#define INT_LEAST8_MIN   (-__INT_LEAST8_MAX__ - 1)
#define INT_LEAST16_MIN  (-__INT_LEAST16_MAX__ - 1)
#define INT_LEAST32_MIN  (-__INT_LEAST32_MAX__ - 1)
#define INT_LEAST64_MIN  (-__INT_LEAST64_MAX__ - 1)

#define INT_LEAST8_MAX   __INT_LEAST8_MAX__
#define INT_LEAST16_MAX  __INT_LEAST16_MAX__
#define INT_LEAST32_MAX  __INT_LEAST32_MAX__
#define INT_LEAST64_MAX  __INT_LEAST64_MAX__

#define UINT_LEAST8_MAX  __UINT_LEAST8_MAX__
#define UINT_LEAST16_MAX __UINT_LEAST16_MAX__
#define UINT_LEAST32_MAX __UINT_LEAST32_MAX__
#define UINT_LEAST64_MAX __UINT_LEAST64_MAX__

/* ============================================================
 * Limites de tipos fast
 * ============================================================ */
#define INT_FAST8_MIN    (-__INT_FAST8_MAX__ - 1)
#define INT_FAST16_MIN   (-__INT_FAST16_MAX__ - 1)
#define INT_FAST32_MIN   (-__INT_FAST32_MAX__ - 1)
#define INT_FAST64_MIN   (-__INT_FAST64_MAX__ - 1)

#define INT_FAST8_MAX    __INT_FAST8_MAX__
#define INT_FAST16_MAX   __INT_FAST16_MAX__
#define INT_FAST32_MAX   __INT_FAST32_MAX__
#define INT_FAST64_MAX   __INT_FAST64_MAX__

#define UINT_FAST8_MAX   __UINT_FAST8_MAX__
#define UINT_FAST16_MAX  __UINT_FAST16_MAX__
#define UINT_FAST32_MAX  __UINT_FAST32_MAX__
#define UINT_FAST64_MAX  __UINT_FAST64_MAX__

/* ============================================================
 * Limites de ponteiros e maior inteiro
 * ============================================================ */
#define INTPTR_MIN   (-__INTPTR_MAX__ - 1)
#define INTPTR_MAX   __INTPTR_MAX__
#define UINTPTR_MAX  __UINTPTR_MAX__

#define INTMAX_MIN   (-__INTMAX_MAX__ - 1)
#define INTMAX_MAX   __INTMAX_MAX__
#define UINTMAX_MAX  __UINTMAX_MAX__

/* ============================================================
 * Limites de outros tipos padrão (também exigidos pelo C99)
 * ============================================================ */
#define PTRDIFF_MIN  (-__PTRDIFF_MAX__ - 1)
#define PTRDIFF_MAX  __PTRDIFF_MAX__

#define SIG_ATOMIC_MIN  (-__SIG_ATOMIC_MAX__ - 1)
#define SIG_ATOMIC_MAX  __SIG_ATOMIC_MAX__

#define SIZE_MAX     __SIZE_MAX__

#define WCHAR_MIN    __WCHAR_MIN__
#define WCHAR_MAX    __WCHAR_MAX__

#define WINT_MIN     __WINT_MIN__
#define WINT_MAX     __WINT_MAX__

/* ============================================================
 * Macros para constantes inteiras (C99 7.18.4)
 * ============================================================ */
#define INT8_C(c)    c
#define INT16_C(c)   c
#define INT32_C(c)   c
#define INT64_C(c)   c ## L

#define UINT8_C(c)   c
#define UINT16_C(c)  c
#define UINT32_C(c)  c ## U
#define UINT64_C(c)  c ## UL

#define INTMAX_C(c)  c ## L
#define UINTMAX_C(c) c ## UL

#ifdef __cplusplus
}
#endif

#endif /* _ORION_STDINT_H */
