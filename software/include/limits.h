/* limits.h - OrionOS
 * Limites de tipos inteiros e de caracteres (C99 5.2.4.2.1).
 *
 * Ambiente freestanding. Usa os macros internos do GCC/Clang
 * para garantir valores exatos na arquitetura alvo.
 */

#ifndef _ORION_LIMITS_H
#define _ORION_LIMITS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================
 * Caracteres
 * ============================================================ */

/* Número de bits em um char (geralmente 8). */
#define CHAR_BIT    __CHAR_BIT__

/* Limites de char signed/unsigned.
 * O tipo char puro é implementation-defined; deixamos os
 * dois conjuntos de macros definidos, como a norma permite. */
#define SCHAR_MIN   (-__SCHAR_MAX__ - 1)
#define SCHAR_MAX   __SCHAR_MAX__
#define UCHAR_MAX   __SCHAR_MAX__ * 2 + 1

/* O tipo char puro — usa __CHAR_UNSIGNED__ se o compilador
 * declarar que char é unsigned nesta arquitetura. */
#ifdef __CHAR_UNSIGNED__
    #define CHAR_MIN    0
    #define CHAR_MAX    UCHAR_MAX
#else
    #define CHAR_MIN    SCHAR_MIN
    #define CHAR_MAX    SCHAR_MAX
#endif

/* ============================================================
 * Inteiros curtos
 * ============================================================ */

#define SHRT_MIN    (-__SHRT_MAX__ - 1)
#define SHRT_MAX    __SHRT_MAX__
#define USHRT_MAX   (__SHRT_MAX__ * 2 + 1)

/* ============================================================
 * Inteiros
 * ============================================================ */

#define INT_MIN     (-__INT_MAX__ - 1)
#define INT_MAX     __INT_MAX__
#define UINT_MAX    (__INT_MAX__ * 2u + 1u)

/* ============================================================
 * Inteiros longos
 * ============================================================ */

#define LONG_MIN    (-__LONG_MAX__ - 1L)
#define LONG_MAX    __LONG_MAX__
#define ULONG_MAX   (__LONG_MAX__ * 2ul + 1ul)

/* ============================================================
 * Inteiros long long (C99)
 * ============================================================ */

#define LLONG_MIN   (-__LONG_LONG_MAX__ - 1LL)
#define LLONG_MAX   __LONG_LONG_MAX__
#define ULLONG_MAX  (__LONG_LONG_MAX__ * 2ull + 1ull)

/* ============================================================
 * Multibyte / wide char (podem não ser relevantes no kernel,
 * mas a norma exige que estejam definidos)
 * ============================================================ */

/* MB_LEN_MAX: número máximo de bytes em um caractere multibyte. */
#define MB_LEN_MAX  16

#ifdef __cplusplus
}
#endif

#endif /* _ORION_LIMITS_H */
