#include <mc68000.h>

void _putchar(int c) {
    asm volatile (
        "movem.l %%d0/%%d1/%%a0,-(%%sp)\n\t"  // Save modified registers
        "move.l %0, %%d1\n\t"                 // Put character in D1
        "move.w #2, %%d0\n\t"                 // OUTCH trap function code
        "trap #1\n\t"                         // Call TUTOR function
        "movem.l (%%sp)+,%%d0/%%d1/%%a0"      // Restore registers
        :
        : "r" (c)
        : "d0", "d1", "a0", "cc", "memory"
    );
}

int _getchar(void) {
    int result;
    asm volatile (
        "MOVE.W #1, %%D0\n\t"   // CCONIN
        "TRAP #1\n\t"
        "MOVE.L %%D0, %0"
        : "=g" (result)
        :
        : "d0", "d1", "cc", "memory"
    );
    return result & 0xFF; // Retorna apenas o byte inferior
}

