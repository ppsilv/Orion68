* Vetor de exceção TRAP 0 aponta para cá
TRAP0_Handler:
    movem.l d2-d7/a0-a6,-(sp)   * Salva o contexto do programa do usuário
                                * (D0 e D1 não salvamos pois contêm parâmetros/retorno)

    cmp.l   #MAX_SYSCALLS,d0    * Valida se a TRAP solicitada existe
    bhs     .trap_invalida

    lsl.l   #2,d0               * Multiplica o ID por 4 (tamanho de um endereço de 32 bits)
    move.l  .syscall_table(pc,d0.l),a0
    jsr     (a0)                * Salta para a função real da ROM (pode ser em C!)

.retornar:
    movem.l (sp)+,d2-d7/a0-a6   * Restaura o contexto
    rte                         * Retorno da Exceção (RTE limpa o Status Register e desempilha o PC)

.trap_invalida:
    * Trate o erro aqui (ex: imprime "Bad Trap" ou trava)
    bra     .retornar

* Tabela de ponteiros para as suas funções reais na ROM
    align 2
.syscall_table:
    dc.l    _write_kbd          * Suas rotinas em C/Assembly reais da ROM
    dc.l    _read_kbd
    dc.l    _init_uart
MAX_SYSCALLS equ (*-.syscall_table)/4
