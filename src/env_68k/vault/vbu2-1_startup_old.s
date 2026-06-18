.section .text
.global _start             /* Torna o símbolo visível para o Linker Script */

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
          .long    0x00100000
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
        /* Se o seu hardware precisa de alguma inicialização crítica, faça aqui. */
        ORI.W   #0x0700,%SR
        /*MOVE.W  #0x2000, %SR*/    /*Habilita todas as interruções*/
        LEA     0x000FFFF0,%A7
        LEA     0x00FF8001,%A0
        MOVE.B  #0x41,(%A0)
        /*1. Limpa a RAM e por conseguinte a área bss*/
        LEA     0x080000,%A0
        LEA     0x100000,%A1
        MOVE.L  #0x0,%D0
.ClearLoop:
        MOVE.L  %D0,(%A0)+
        CMPA.L  %A0,%A1
        BHI     .ClearLoop

        /*2. Rotina que CARREGA a seção .data da ROM para a RAM*/
        lea     __data_load_start, %a0  | A0 aponta para a origem (na ROM)
        lea     __data_start, %a1       | A1 aponta para o destino (na RAM)
        lea     __data_end, %a2         | A2 determina o fim do bloco na RAM

        lea     0x84000,%A5
        MOVE.L  __data_load_start,(%A5)+
        MOVE.L  __data_start,(%A5)+
        MOVE.L  __data_end,(%A5)+
copy_data:
        cmpa.l  %a2, %a1                | Chegou no fim do bloco de RAM?
        bge.s   go_ahead                | Se sim, vai para a próxima etapa
        move.b  (%a0)+, (%a1)+          | Copia o byte da ROM para a RAM e avança ambos
        bra.s   copy_data               | Loop
go_ahead:

        /*Uart 0 init*/
        LEA     0xFF4000,%A1
        move.b  #0x07,FCR(%a1)      /*; enable FIFO*/
        move.b  #0x83,LCR(%a1)      /*; 8 data bits, no parity, 1 stop bit, DLAB=1*/
        move.b  #0x08,DLL(%A1)      /*; Byte 0 (LSB) -> DLL*/
        move.b  #0x00,DLM(%A1)      /*; Byte 1 (agora no LSB) -> DLM*/
        bclr.b  #0x07,LCR(%a1)      /*; 8 data bits, no parity, 1 stop bit, DLAB=0*/

        lea     bootstrap_string,%a0
        jsr     print_string
       /* jsr     startup_delay */


        jsr     main

        /* Se a main retornar por algum milagre, trava a CPU num loop infinito */
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
        move.b  #0x07,FCR(%a1)      /*; enable FIFO*/
        move.b  #0x83,LCR(%a1)      /*; 8 data bits, no parity, 1 stop bit, DLAB=1*/
        move.b  #0x08,DLL(%A1)      /*; Byte 0 (LSB) -> DLL*/
        move.b  #0x00,DLM(%A1)      /*; Byte 1 (agora no LSB) -> DLM*/
        bclr.b  #0x07,LCR(%a1)      /*; 8 data bits, no parity, 1 stop bit, DLAB=0*/
        RTS

print_string:
        move.b  (%A0)+,%d0              /* Lê o caractere atual e avança o ponteiro %a0 */
        beq     .end_print_string
  /*      jsr     PicoWrCh */
        jsr     UartWrCh
        bra     print_string    /* Volta para pegar o próximo caractere */

.end_print_string:
        RTS

startup_delay:
        move.l  #2000,%d1       /* Contador do laço externo */
.outer_loop:
        move.l  #400,%d2       /* Contador do laço interno */
.inner_loop:
        subq.l  #1,%d2          /* Decrementa interno (2 ciclos) */
        bne     .inner_loop     /* Pula se não for zero (10 ciclos se pular) */
        subq.l  #1,%d1          /* Decrementa externo */
        bne     .outer_loop     /* Pula se não for zero */
        rts

.section .text    /* <-- Força a string a ficar na ROM junto com o código */
.align 2
bootstrap_string:
        .ascii  "PDS317 - Bootstrap 2026 V1.0"
        .byte   13, 10, 0       /* Carriage Return (13), Line Feed (10) e o NULL (0) */
