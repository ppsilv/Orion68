/* stdarg.h - OrionOS
 * Acesso a argumentos variádicos (C99 7.15).
 *
 * A implementação real fica no compilador. Este header apenas
 * expõe os macros com os nomes da norma.
 */

#ifndef _ORION_STDARG_H
#define _ORION_STDARG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================
 * va_list
 * ============================================================ */

typedef __builtin_va_list va_list;

/* ============================================================
 * Macros
 * ============================================================ */

#define va_start(ap, last)  __builtin_va_start(ap, last)
#define va_end(ap)          __builtin_va_end(ap)
#define va_arg(ap, type)    __builtin_va_arg(ap, type)

/* va_copy é obrigatório em C99. */
#define va_copy(dest, src)  __builtin_va_copy(dest, src)

#ifdef __cplusplus
}
#endif

#endif /* _ORION_STDARG_H */
