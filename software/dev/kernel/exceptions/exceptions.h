/* excecoes.h */
#ifndef EXCECOES_H
#define EXCECOES_H

#include <stdint.h>

/* Frame salvo pelo handler em assembly.
   A ordem DEVE casar com a ordem dos pushes no .S */
typedef struct {
    uint32_t d0, d1, d2, d3, d4, d5, d6, d7;   /* D0-D7 */
    uint32_t a0, a1, a2, a3, a4, a5, a6;       /* A0-A6 */
    uint32_t pc;                                /* PC no momento da exceção */
    uint16_t sr;                                /* SR no momento da exceção */
    uint16_t _pad;                              /* padding pra alinhar */
} ExcFrame;

void exc_bus_error(ExcFrame *f);
void exc_address_error(ExcFrame *f);
void exc_illegal(ExcFrame *f);
void exc_div0(ExcFrame *f);
void exc_priv(ExcFrame *f);
void exc_trapv(ExcFrame *f);
void exc_linea(ExcFrame *f);
void exc_linef(ExcFrame *f);
void exc_generic(ExcFrame *f, const char *nome);

#endif