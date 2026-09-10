#ifndef CRITICAL_H
#define CRITICAL_H

/*
 * LOCK/UNLOCK: salva o SR, desliga todas as interrupcoes, executa a
 * secao critica, e restaura o SR (deixando interrupcoes do jeito que
 * estavam antes -- se ja estavam desligadas por outro motivo, nao
 * liga elas de volta por engano).
 *
 * Precisa ser macro (nao funcao): o 'move.w %sr' tem que executar no
 * contexto de quem chamou, senao voce so estaria salvando/restaurando
 * o SR de dentro de uma sub-rotina, sem efeito nenhum em quem chamou.
 *
 * IMPORTANTE: por ser macro, isso tem que estar incluido ANTES do
 * primeiro uso de LOCK/UNLOCK no arquivo -- diferente de funcao, nao
 * da pra "declarar em cima, definir embaixo".
 */

#define LOCK(saved) {                                        \
    asm("move.w %%sr, %0\n" : "=dm" ((saved)));               \
    m68k_disable_all_interrupts();                            \
}

#define UNLOCK(saved) {                                       \
    asm("move.w %0, %%sr\n" : : "dm" ((saved)) : "cc");        \
}

#endif
