.section .text
.global _start             /* Torna o símbolo visível para o Linker Script */

/*
.section .vectors
.org 0x00000000*/
.equ RHR, 0x01
.equ THR, 0x01
.equ FCR, 0x05
.equ LCR, 0x07
.equ LSR, 0x0B
.equ DLL, 0x01
.equ DLM, 0x03


_vectors:
          .long    0x000FFFF0
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
        /* Salta direto para a função principal do seu VBug escrita em C! */
        JSR     main



        LEA     0x00FF8001,%A0
        MOVE.B  #0x41,(%A0)
        NOP
        MOVE.B  #0x42,(%A0)
        NOP
        MOVE.B  #0x0A,(%A0)
        NOP
        MOVE.B  #0x0D,(%A0)
        NOP

        LEA     0xFF4000,%A1
        move.b  #0x07,FCR(%a1)      /*; enable FIFO*/
        move.b  #0x83,LCR(%a1)      /*; 8 data bits, no parity, 1 stop bit, DLAB=1*/
        move.b  #0x08,DLL(%A1)      /*; Byte 0 (LSB) -> DLL*/
        move.b  #0x00,DLM(%A1)      /*; Byte 1 (agora no LSB) -> DLM*/
        bclr.b  #0x07,LCR(%a1)      /*; 8 data bits, no parity, 1 stop bit, DLAB=0*/

        move.b  #0x41,%D0
        JSR     UartWrCh
        move.b  #0x42,%D0
        JSR     UartWrCh

        jsr     main

        /* Se a main retornar por algum milagre, trava a CPU num loop infinito */
.dead:
        bra.s   .dead

UartWrCh:
        move.l  #0xFF4000,%A1
.WaitTx:
        btst    #5, LSR(%A1)
        beq     .WaitTx
        move.b  %D0,THR(%A1)
        RTS

