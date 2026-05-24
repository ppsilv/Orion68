#ifndef ORION_API_H
#define ORION_API_H

// Usaremos a TRAP #0 para chamadas de sistema (System Calls)
// D0 vai conter o ID da função que queremos chamar

#define SYS_WRITE_KBD   0
#define SYS_READ_KBD    1
#define SYS_INIT_UART   2

// Exemplo: Enviar um caractere
inline void orion_write_kbd(char c) {
    __asm__ volatile (
        "move.l %0, %%d1\n\t"  // Passa o caractere em D1
        "move.l %1, %%d0\n\t"  // Passa o ID da função em D0
        "trap #0\n\t"          // Dispara a TRAP 0
        :
        : "g" ((unsigned long)c), "g" ((unsigned long)SYS_WRITE_KBD)
        : "d0", "d1"
    );
}

// Exemplo: Ler um caractere (retorna o valor em D0)
inline char orion_read_kbd(void) {
    register char resultado __asm__("d0");
    __asm__ volatile (
        "move.l %1, %%d0\n\t"
        "trap #0\n\t"
        : "=r" (resultado)
        : "g" ((unsigned long)SYS_READ_KBD)
        : "d0"
    );
    return resultado;
}

#endif

