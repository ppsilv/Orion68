/* stdbool.h - OrionOS
 * Macros para o tipo booleano (C99 7.16).
 *
 * Em C99+, _Bool é palavra-chave do compilador. Este header
 * só dá nomes mais amigáveis (bool, true, false).
 */

#ifndef _ORION_STDBOOL_H
#define _ORION_STDBOOL_H

/* Em C++, bool/true/false já são palavras-chave nativas.
 * Não precisamos definir nada. */
#ifndef __cplusplus

/* Em C99+, __STDC_VERSION__ >= 199901L. */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)

    #define bool  _Bool
    #define true  1
    #define false 0

#else
    /* Fallback para C89/C90 (não deveria acontecer no OrionOS,
     * mas deixa o header robusto para compiladores antigos). */
    #define bool  int
    #define true  1
    #define false 0

#endif

/* __bool_true_false_are_defined: exigido pela norma C99.
 * Programas podem testar #ifdef __bool_true_false_are_defined
 * para saber se o header foi incluído. */
#define __bool_true_false_are_defined 1

#endif /* !__cplusplus */

#endif /* _ORION_STDBOOL_H */
