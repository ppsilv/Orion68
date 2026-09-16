
extern void picovga_putchar( char ch);
extern int ring_buf_get_char();


// handler C — o GCC m68k gera prologue/epilogue que salva o frame e usa rte
__attribute__((interrupt))
void trap15_handler(void) {
    long op;
    asm volatile("move.l %%d0, %0" : "=r"(op));

    switch (op) {
        case 0: {   // SYS_GETCHAR
            int c;
            do {
                c = ring_buf_get_char();     // tenta tirar do ring
                if (c < 0) {
                    // nada no ring: dorme até próxima IRQ do Pico
                    // cuidado: dormir DENTRO do handler é complicado
                }
            } while (c < 0);
            asm volatile("move.l %0, %%d0" :: "r"((long)c));
            break;
        }
        case 1: {   // SYS_PUTCHAR
            long ch;
            asm volatile("move.l %%d1, %0" : "=r"(ch));
            picovga_putchar((int)ch);
            asm volatile("move.l %0, %%d0" :: "r"(0L));
            break;
        }
    }
}