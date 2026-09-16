extern char getkbd();
char getchar(void){
    return getkbd();
}

 int sys_getchar(void) {
    register long op  asm("d0") = 0;   // SYS_GETCHAR
    register long ret asm("d0");
    asm volatile("trap #15" : "+r"(op) : : "memory");
    ret = op;
    return (int)ret;
}
