.text
    .global _start
    .extern __libc_init
    .extern main
    .extern exit

_start:
    move.l  #_stack_top, %sp

    lea     __bss_start(%pc), %a0
    lea     __bss_end(%pc), %a1
1:
    cmp.l   %a1, %a0
    bge.s   2f
    clr.b   (%a0)+
    bra.s   1b
2:

    clr.l   -(%sp)           /* argv = NULL */
    clr.l   -(%sp)           /* argc = 0   */

    move.l  __libc_init@GOT(%a5), %a0
    jsr     (%a0)
    addq.l  #8, %sp

    move.l  main@GOT(%a5), %a0
    jsr     (%a0)

    move.l  %d0, -(%sp)
    move.l  exit@GOT(%a5), %a0
    jsr     (%a0)

    .bss
    .align  4
    .space  4096
_stack_top:
