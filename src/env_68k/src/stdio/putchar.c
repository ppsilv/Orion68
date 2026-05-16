

void putchar(char c) {
    asm volatile (
        "movem.l %%d0/%%d1/%%a0,-(%%sp)\n\t"  // Save modified registers
        "move.b %0, %%d1\n\t"                 // Put character in D1
        "move.w #2, %%d0\n\t"                 // OUTCH trap function code
        "trap #1\n\t"                         // Call TUTOR function
        "movem.l (%%sp)+,%%d0/%%d1/%%a0"      // Restore registers
        :
        : "r" (c)
        : "d0", "d1", "a0", "cc", "memory"
    );
}

