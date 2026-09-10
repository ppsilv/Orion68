|
| ctx_switch_68000.s -- troca de contexto para CPU 68000 PURO
|
| Frame de excecao do 68000 (SEM format/vector word):
|   SP+0: SR   (2 bytes)
|   SP+2: PC   (4 bytes)
|   -- total 6 bytes --
|
| Use este arquivo se seu alvo de build for o 68000. Para 68010,
| use ctx_switch_68010.s em vez deste (nao linke os dois juntos).
|

.global OS_StartHighRdy
.global OS_TickISR

.extern current_task
.extern OS_Schedule

.extern systemTick
.extern tick_count

OS_StartHighRdy:
    move.l  current_task,%A0       | %A0 = ponteiro pro TCB atual
    move.l  (%A0), %A7               | %A7 (SP) = tcb->sp (offset 0 da struct)
    movem.l (%A7)+, %D0-%D7/%A0-%A6     | restaura %D0-%D7, %A0-%A6
    rte                            | restaura PC e SR (6 bytes), salta pra tarefa

OS_TickISROLD:
    movem.l %D0-%D7/%A0-%A6, -(%A7)     | salva contexto da tarefa interrompida
    move.l  current_task, %A0
    move.l  %A7, (%A0)               | tcb->sp = %A7

    jsr     OS_Schedule            | escolhe a proxima tarefa, atualiza current_task

    move.l  current_task, %A0
    move.l  (%A0), %A7               | %A7 = tcb->sp da tarefa escolhida
    movem.l (%A7)+, %D0-%D7/%A0-%A6     | restaura contexto dela
    rte                            | volta a executar de onde ela parou

OS_TickISR:
        movem.l %d0-%d7/%a0-%a6,-(%sp)

        move.l  #systemTick,%a0
        addq.l  #1,(%a0)

        addq.w  #1,tick_count
        cmpi.w  #400,tick_count
        beq.s   .action_100
        cmpi.w  #800,tick_count
        beq.s   .action_200
        bra.s   .do_schedule

.action_100:
        move.b  #0x08,0xFF901D
        bra.s   .do_schedule

.action_200:
        move.b  #0x08,0xFF901F
        clr.w   tick_count
        | cai direto pra .do_schedule

.do_schedule:
        movem.l (%sp)+,%d0-%d7/%a0-%a6
        rte
        move.l  current_task,%a0
        move.l  %sp,(%a0)              | tcb->sp = pilha da tarefa que estava rodando

        jsr     OS_Schedule            | decide a proxima tarefa, atualiza current_task

        move.l  current_task,%a0
        move.l  (%a0),%sp              | pilha vira a da tarefa escolhida

        movem.l (%sp)+,%d0-%d7/%a0-%a6
        rte


