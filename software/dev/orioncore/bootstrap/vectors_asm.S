/*
 * vectors.S - Tratadores de Exceção e Interrupção para o Orion68K
 */

.section .text
.align 2

// Exporta os pontos de entrada para o arquivo de vetores principal
.global Int2Handler
.global Int3Handler
.global SvcBusError    
.global Svcaddress_err 
.global SvcIllegalIns  
.global SvcDiv0_handler
.global fdummy


.extern tick_count

SvcBusError:
Svcaddress_err:
        /* Frame de bus/address error no 68000 tem 14 bytes (SR+PC+4 words extras).
           NUNCA fazer rte direto aqui -- desempilhar o frame inteiro e travar,
           já que não há retry de barramento implementado. */
        ADDA.L  #10,%A7        /* remove os 5 words extras (Format/Vector, Access Addr, IR, SW)
                                   deixando só SR+PC no topo, formato que RTE entende */
        /* opcional: acender um LED de diagnóstico específico de bus/address error aqui,
           antes de travar, pra diferenciar esse caso de um HALT genérico */
.trava_bus_error:
        bra.s   .trava_bus_error   /* trava aqui de propósito -- não faz rte */

SvcIllegalIns:
SvcDiv0_handler:
        rte

fdummy:
    rte

/* =========================================================================
   INTERRUPÇÃO DE HARDWARE NÍVEL 2 - TIMER (SYSTEM TICK)
   ========================================================================= */
Int2Handler:
        /* Como mexe em registrador para somar na RAM, salva o D0 e A0 */
        movem.l %d0-%d7/%a0-%a6,-(%sp)

        /* Incrementa a variável de 32 bits do C */
        move.l  #systemTick,%a0
        addq.l  #1,(%a0)

    addq.w  #1,tick_count       
    cmpi.w  #400,tick_count     
    beq.s   .action_100
    cmpi.w  #800,tick_count     
    beq.s   .action_200
    movem.l (%sp)+,%d0-%d7/%a0-%a6
    rte

.action_100:
    move.b  #0x08,0xFF901D        
    movem.l (%sp)+,%d0-%d7/%a0-%a6
    rte

.action_200:
    move.b  #0x08,0xFF901F        
    clr.w   tick_count   
    movem.l (%sp)+,%d0-%d7/%a0-%a6
    rte


Int3Handler:
        movem.l %d0-%d7/%a0-%a6,-(%sp)

        MOVE.B  0x00FF9113,%D0

        ANDI.L  #0x000000FF,%D0  /*; Limpa a parte alta (garante o unsigned)*/
        MOVE.L  %D0, -(%A7)       /*; Empilha 4 bytes (mantém A7 alinhado em par)*/
        
        ORI.W   #0x0700,%SR   /*Disable all interrupts supervisor mode*/       
        jsr     ring_buf_put
        andi.w  #0xF8FF,%SR    /*Enable all interrupts supervisor mode*/    
        
        ADDQ.L  #4,%A7          /*; Limpa os 4 bytes da pilha!*/

        movem.l (%sp)+,%d0-%d7/%a0-%a6
        rte

