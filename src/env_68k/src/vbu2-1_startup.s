.section .text
.global _start

.global UartWrCh
.global uart0_initialize

.equ RHR, 0x01
.equ THR, 0x01
.equ FCR, 0x05
.equ LCR, 0x07
.equ LSR, 0x0B
.equ DLL, 0x01
.equ DLM, 0x03

_vectors:
          .long    0x000FFFF0       /* Pilha no topo real da RAM (ajuste se seu limite for outro) */
          .long    _start
          .long    SvcBusError
          .long    Svcaddress_err
          .long    SvcIllegalIns
          .long    SvcDiv0_handler
          .long    SvcChkIns
          .long    SvcTrapvIns
          .long    SvcPrivViolation
          .long    SvcTrace
          .long    SvcLineA
          .long    SvcLineF
          .long    bad_exception
          .long    bad_exception
          .long    bad_exception
          .long    bad_exception
          .long    bad_exception
          .long    bad_exception
          .long    bad_exception
          .long    bad_exception
          .long    bad_exception
          .long    bad_exception
          .long    bad_exception
          .long    bad_exception
          .long    SpuriousHandler
          .long    Int1Handler
          .long    Int2Handler
          .long    Int3Handler
          .long    Int4Handler
          .long    Int5Handler
          .long    Int6Handler
          .long    Int7Handler
          .long    Trap0Handler
          .long    Trap1Handler
          .long    Trap2Handler
          .long    Trap3Handler
          .long    Trap4Handler
          .long    Trap5Handler
          .long    Trap6Handler
          .long    Trap7Handler
          .long    Trap8Handler
          .long    Trap9Handler
          .long    TrapAHandler
          .long    TrapBHandler
          .long    TrapCHandler
          .long    TrapDHandler
          .long    TrapEHandler
          .long    TrapFHandler


_start:
        ORI.W   #0x0700,%SR         /* Desabilita interrupções no boot */
        LEA     0x000FFFF0,%A7      /* Garante o ponteiro da pilha limpo */

        /* 1. Limpa a RAM (bss) */
        LEA     0x080000,%A0
        LEA     0x100000,%A1
        MOVE.L  #0x0,%D0
.ClearLoop:
        MOVE.L  %D0,(%A0)+
        CMPA.L  %A0,%A1
        BHI     .ClearLoop

        /* 2. Carrega seção .data da ROM para a RAM */
        lea     __data_load_start, %a0
        lea     __data_start, %a1
        lea     __data_end, %a2
copy_data:
        cmpa.l  %a2, %a1
        bge.s   go_ahead
        move.b  (%a0)+, (%a1)+
        bra.s   copy_data
go_ahead:

        /* Uart 0 init */
        LEA     0xFF4000,%A1
        move.b  #0x07,FCR(%a1)
        move.b  #0x83,LCR(%a1)
        move.b  #0x08,DLL(%A1)
        move.b  #0x00,DLM(%A1)
        bclr.b  #0x07,LCR(%a1)

        lea     bootstrap_string,%a0
        jsr     print_string

        jsr     main

.dead:
        bra.s   .dead

UartWrCh:
        move.l  #0xFF4000,%A1
.WaitTx:
        btst    #5,LSR(%A1)
        beq     .WaitTx
        move.b  %D0,THR(%A1)
        RTS

PicoWrCh:
        LEA     0x00FF8001,%A1
        MOVE.B  %D0,(%A1)
        NOP
        NOP
        NOP
        NOP
        RTS

uart0_initialize:
        LEA     0xFF4000,%A1
        move.b  #0x07,FCR(%a1)
        move.b  #0x83,LCR(%a1)
        move.b  #0x08,DLL(%A1)
        move.b  #0x00,DLM(%A1)
        bclr.b  #0x07,LCR(%a1)
        RTS

print_string:
        move.b  (%A0)+,%d0
        beq     .end_print_string
        jsr     UartWrCh
        bra     print_string
.end_print_string:
        RTS

startup_delay:
        move.l  #2000,%d1
.outer_loop:
        move.l  #400,%d2
.inner_loop:
        subq.l  #1,%d2
        bne     .inner_loop
        subq.l  #1,%d1
        bne     .outer_loop
        rts

.section .text
.align 2
bootstrap_string:
        .ascii  "PDS317 - Bootstrap 2026 V1.0"
        .byte   13, 10, 0
.align 2        /* <--- CRÍTICO: Garante que o PRÓXIMO arquivo linkado comece em endereço PAR */
