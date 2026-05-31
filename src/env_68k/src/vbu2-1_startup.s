.section .text
.global _start             /* Torna o símbolo visível para o Linker Script */

/*
.section .vectors
.org 0x00000000*/

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
        /*LEA     0x000FFFF0,%A7*/
        /*LEA     0x00FF8001,%A0*/
        /*MOVE.B  #0x41,(%A0)*/

        /*2. Rotina que CARREGA a seção .data da ROM para a RAM*/
        lea     __data_load_start, %a0  | A0 aponta para a origem (na ROM)
        lea     __data_start, %a1       | A1 aponta para o destino (na RAM)
        lea     __data_end, %a2         | A2 determina o fim do bloco na RAM

copy_data:
        cmpa.l  %a2, %a1                | Chegou no fim do bloco de RAM?
        bge.s   go_ahead                | Se sim, vai para a próxima etapa
        move.b  (%a0)+, (%a1)+          | Copia o byte da ROM para a RAM e avança ambos
        bra.s   copy_data               | Loop
go_ahead:
        /* Salta direto para a função principal do seu VBug escrita em C! */
        jsr     main

        /* Se a main retornar por algum milagre, trava a CPU num loop infinito */
.dead:
        bra.s   .dead



