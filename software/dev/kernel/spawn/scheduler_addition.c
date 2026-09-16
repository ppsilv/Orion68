/* ============================================================
 * COLE ISTO no mesmo arquivo .c onde ja existe OS_TaskCreate
 * (precisa estar no mesmo arquivo porque usa task_pool, task_count,
 * push32, push16, ready_insert -- que devem ser static ali)
 * ============================================================
 *
 * Diferenca pro OS_TaskCreate original: recebe um 'a5_value' extra
 * e planta ele no slot certo do bloco de registradores (i==13),
 * que e' onde vai cair o A5 quando o movem.l restaurar depois.
 * Todo o resto (empilhar entry/SR, alinhar, ready_insert) e'
 * identico ao original.
 */
tcb_t *OS_TaskCreatePIC(void (*entry)(void), void *arg, uint8_t *stack,
                         uint32_t stack_size, int id, uint32_t a5_value)
{
    if (task_count >= OS_MAX_TASKS || entry == NULL || stack == NULL)
        return NULL;

    tcb_t *tcb = &task_pool[task_count++];
    tcb->stack_base = stack;
    tcb->stack_size = stack_size;
    tcb->arg        = arg;
    tcb->id         = id;
    tcb->next       = NULL;

    uint8_t *sp = stack + stack_size;
    sp = (uint8_t *)((uint32_t)sp & ~1u);

#ifdef OS_CPU_68010
    sp = push16(sp, 0x0000);
#endif
    sp = push32(sp, (uint32_t)entry);
    sp = push16(sp, 0x2000);

    /* i==13 é o slot que corresponde a A5 no bloco D0-D7/A0-A6 --
     * ver tabela de correspondencia explicada na conversa. Todos
     * os outros ficam zerados, igual no OS_TaskCreate original. */
    for (int i = 14; i >= 0; i--) {
        uint32_t val = (i == 13) ? a5_value : 0;
        sp = push32(sp, val);
    }

    tcb->sp    = (uint32_t *)sp;
    tcb->state = TASK_READY;
    ready_insert(tcb);

    return tcb;
}
