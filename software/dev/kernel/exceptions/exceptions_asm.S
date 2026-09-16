/* excecoes_asm.S */

    .text

/* Macro que gera um handler.
   \nome  = nome da função C a chamar
   \label = label do handler */
.macro EXC_HANDLER label, cfunc
    .globl  \label
\label:
    /* Salva D0-D7 */
    moveml  %d0-%d7, %sp@-
    /* Salva A0-A6 */
    moveml  %a0-%a6, %sp@-
    /* SP agora aponta pro frame salvo (ExcFrame) */
    moveal  %sp, %a0          /* A0 = &frame */
    movel   %a0, %sp@-        /* empilha o argumento */
    jsr     \cfunc
    addql   #4, %sp           /* limpa o argumento */
    /* Restaura A0-A6 */
    moveml  %sp@+, %a0-%a6
    /* Restaura D0-D7 */
    moveml  %sp@+, %d0-%d7
    rte
.endm

EXC_HANDLER SvcBusError,   exc_bus_error
EXC_HANDLER Svcaddress_err, exc_address_error
EXC_HANDLER SvcIllegalIns, exc_illegal
EXC_HANDLER SvcDiv0_handler, exc_div0
EXC_HANDLER SvcPrivViolation, exc_priv
EXC_HANDLER SvcTrapvIns,   exc_trapv
EXC_HANDLER SvcLineA,      exc_linea
EXC_HANDLER SvcLineF,      exc_linef