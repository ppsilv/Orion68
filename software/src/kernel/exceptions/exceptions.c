/* excecoes.c */
#include "exceptions.h"
#include <stdio.h>   /* ou seu kprintf */

extern void kernel_puts(const char *s);
extern void kernel_puthex(uint32_t v);   /* você precisa ter isso */
extern void kernel_putdec(uint32_t v);

static void dump_frame(ExcFrame *f) {
    kernel_puts("\n--- EXCECAO ---\n");
    kernel_puts("PC = "); kernel_puthex(f->pc); kernel_puts("\n");
    kernel_puts("SR = "); kernel_puthex(f->sr); kernel_puts("\n");
    kernel_puts("D0 = "); kernel_puthex(f->d0); kernel_puts("\n");
    kernel_puts("D1 = "); kernel_puthex(f->d1); kernel_puts("\n");
    kernel_puts("D2 = "); kernel_puthex(f->d2); kernel_puts("\n");
    kernel_puts("D3 = "); kernel_puthex(f->d3); kernel_puts("\n");
    kernel_puts("D4 = "); kernel_puthex(f->d4); kernel_puts("\n");
    kernel_puts("D5 = "); kernel_puthex(f->d5); kernel_puts("\n");
    kernel_puts("D6 = "); kernel_puthex(f->d6); kernel_puts("\n");
    kernel_puts("D7 = "); kernel_puthex(f->d7); kernel_puts("\n");
    kernel_puts("A0 = "); kernel_puthex(f->a0); kernel_puts("\n");
    kernel_puts("A1 = "); kernel_puthex(f->a1); kernel_puts("\n");
    kernel_puts("A2 = "); kernel_puthex(f->a2); kernel_puts("\n");
    kernel_puts("A3 = "); kernel_puthex(f->a3); kernel_puts("\n");
    kernel_puts("A4 = "); kernel_puthex(f->a4); kernel_puts("\n");
    kernel_puts("A5 = "); kernel_puthex(f->a5); kernel_puts("\n");
    kernel_puts("A6 = "); kernel_puthex(f->a6); kernel_puts("\n");
}

void exc_generic(ExcFrame *f, const char *nome) {
    kernel_puts("\n!!! EXCECAO: ");
    kernel_puts(nome);
    dump_frame(f);
    kernel_puts("\nSistema travado.\n");
    for (;;) {
        /* fica aqui pra sempre -- ou reinicia, ou volta pro scheduler */
    }
}

void exc_bus_error(ExcFrame *f)      { exc_generic(f, "BUS ERROR"); }
void exc_address_error(ExcFrame *f)  { exc_generic(f, "ADDRESS ERROR"); }
void exc_illegal(ExcFrame *f)        { exc_generic(f, "ILLEGAL INSTRUCTION"); }
void exc_div0(ExcFrame *f)           { exc_generic(f, "DIVISAO POR ZERO"); }
void exc_priv(ExcFrame *f)           { exc_generic(f, "PRIVILEGE VIOLATION"); }
void exc_trapv(ExcFrame *f)          { exc_generic(f, "TRAPV"); }
void exc_linea(ExcFrame *f)          { exc_generic(f, "LINE A (nao implementado)"); }
void exc_linef(ExcFrame *f)          { exc_generic(f, "LINE F (nao implementado)"); }