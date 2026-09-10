.section .text

.global _start

.extern kmain

_start:
    /* Configurar stack pointer para o topo da RAM em 0x82000 */
    move.l #_estack, %sp  /* Ajuste conforme sua RAM */

/* Limpar BSS - ajustar para seus endereços reais */
/* Em verdade vos digo, não precisa limpara .bss já limpei quando o sistema entrou no ar */
/*
    move.l #__bss_start, %a0
    move.l #__bss_end, %a1
clear_bss:
    cmp.l %a0, %a1
    beq.s skip_clear
    clr.l (%a0)+
    bra.s clear_bss
*/
skip_clear:
    jsr kmain

    move.l #0,%D0
    trap #0
    /* Parar a CPU */
halt:
    stop #0x2700
    bra halt
.align 2    