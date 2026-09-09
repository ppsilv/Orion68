.section .vectors

_vectors:
          .long    0x000FFFF0                   /*0x00000000*//* Pilha no topo real da RAM (ajuste se seu limite for outro) */
          .long    _start                       /*0x00000004*/
          .long    SvcBusError                  /*0x00000008*/
          .long    Svcaddress_err               /*0x0000000C*/
          .long    SvcIllegalIns                /*0x00000010*/
          .long    SvcDiv0_handler              /*0x00000014*/
          .long    0x00080018      /*SvcChkIns             0x0018*/
          .long    0x0008001e      /*SvcTrapvIns           0x001c*/
          .long    0x00080024      /*SvcPrivViolation      0x0020*/
          .long    0x0008002a      /*SvcTrace              0x0024*/
          .long    0x00080030      /*SvcLineA              0x0028*/
          .long    0x00080036      /*SvcLineF              0x002c*/
          .long    0x0008003c      /*bad_exception         0x0030*/
          .long    0x00080042      /*bad_exception         0x0034*/
          .long    0x00080048      /*bad_exception         0x0038*/
          .long    0x0008004e      /*bad_exception         0x003c*/
          .long    0x00080054      /*bad_exception         0x0040*/
          .long    0x0008005a      /*bad_exception         0x0044*/
          .long    0x00080060      /*bad_exception         0x0048*/
          .long    0x00080066      /*bad_exception         0x004c*/
          .long    0x0008006c      /*bad_exception         0x0050*/
          .long    0x00080072      /*bad_exception         0x0054*/
          .long    0x00080078      /*bad_exception         0x0058*/
          .long    0x0008007e      /*bad_exception         0x005c*/
          .long    0x00080084      /*SpuriousHandler       0x0060*/
          .long    0x0008008a      /*Int1Handler           0x0064*/
          .long    Int2Handler          /*0x00080090*/      /*Int2Handler           0x0068*/
          .long    Int3Handler          /*0x00080096*/      /*Int3Handler           0x006c*/
          .long    0x0008009c      /*Int4Handler           0x0070*/
          .long    0x000800a2      /*Int5Handler           0x0074*/
          .long    0x000800a8      /*Int6Handler           0x0078*/
          .long    0x000800ae      /*Int7Handler           0x007c*/
          .long    0x000800b4      /*Trap0Handler          0x0080*/
          .long    0x000800ba      /*Trap1Handler          0x0084*/
          .long    0x000800c0      /*Trap2Handler          0x0088*/
          .long    0x000800c6      /*Trap3Handler          0x008c*/
          .long    0x000800cc      /*Trap4Handler          0x0090*/
          .long    0x000800d2      /*Trap5Handler          0x0094*/
          .long    0x000800d8      /*Trap6Handler          0x0098*/
          .long    0x000800de      /*Trap7Handler          0x009c*/
          .long    0x000800e4      /*Trap8Handler          0x00a0*/
          .long    0x000800ea      /*Trap9Handler          0x00a4*/
          .long    0x000800f0      /*TrapAHandler          0x00a8*/
          .long    0x000800f6      /*TrapBHandler          0x00ac*/
          .long    0x000800fc      /*TrapCHandler          0x00b0*/
          .long    0x00080102      /*TrapDHandler          0x00b4*/
          .long    0x00080108      /*TrapEHandler          0x00b8*/
          .long    0x0008010e      /*TrapFHandler          0x00bc*/

          .rept    207
          .long    DefaultHandler
          .endr

.section .text

.global _start
.global _delay_ms
.global printString
.global UartWriteCh
.global TrapFHandler

.extern duart_send_char
.extern build_jump_table
.extern SvcBusError    
.extern Svcaddress_err 
.extern SvcIllegalIns  
.extern SvcDiv0_handler
.extern Int2Handler
.extern Int3Handler
.extern fdummy

.equ WR_CHAR, 0xFF912B
.equ DUART_BASE, 0x00FF9000

_start:
        ORI.W   #0x0700,%SR         /* Desabilita interrupções no boot */
        LEA     0x000FFFF0,%A7      /* Garante o ponteiro da pilha limpo */

        MOVE.L  #5,%D1        /* espera 5 segundos */
        JSR     delay_seconds

        MOVE.B  #'1',WR_CHAR

        andi.w #0xF8FF,%SR
/*
; Busca de RAM para m68k (Orion68)
; Retorna em A0 o endereço onde a RAM termina.
*/
/************************************************************************************* */
/* Find the total of SRAM */
FIND_RAM:
    MOVE.L  #0x00080000,%A0    /*; A0 = Endereço inicial da busca (Ajuste para o seu mapa)*/
    MOVE.L  #0x55AA55AA,%D1    /*; D1 = Padrão de teste (alternando 0s e 1s)*/
    MOVE.L  #0x00000400,%D2    /*; D2 = Incremento de salto (ex: 1KB)*/

    MOVE.B  #'2',WR_CHAR

.busca_loop:
    ADD.L   %D2,%A0            /*; Avança o ponteiro de memória*/
    MOVE.L  (%A0),%D0          /*; Salva o dado original da memória no D0*/
    MOVE.L  %D1,(%A0)          /*; Escreve o padrão de teste na RAM*/

    CMP.L   (%A0),%D1          /*; Lê da RAM e compara com o padrão*/
    BNE.S   .fim_ram          /*; Se não for igual (não há RAM), sai do loop*/

    MOVE.L  %D0,(%A0)          /*; Restaura o dado original (é RAM válida)*/
    BRA.S   .busca_loop       /*; Repete para o próximo bloco*/

    MOVE.B  #'3',WR_CHAR

.fim_ram:
        /*; A0 contém o endereço do fim da RAM.*/
        MOVE.L  %A0,%A4
        /*; A4 contém o endereço do fim da RAM.*/

    MOVE.B  #'4',WR_CHAR

/********************************************/
/* Clear the entire SRAM */
        /* 1. Limpa a RAM (bss) */
        LEA     0x080000,%A0
        MOVE.L  %A4,%A1
        MOVE.L  #0x0,%D0
.ClearLoop:
        MOVE.L  %D0,(%A0)+
        CMPA.L  %A0,%A1
        BHI     .ClearLoop

        LEA     0x080000,%A0
        MOVE.L  %A4,(%A0)

        MOVE.B  #'5',WR_CHAR

        MOVE.B  #'6',WR_CHAR

/********************************************/
/* 2. Carrega seção .data da ROM para a RAM */
        lea     __data_load_start, %A0
        lea     __data_start, %A1
        lea     __data_end, %A2
copy_data:
        cmpa.l  %A2, %A1
        bge.s   go_ahead
        move.b  (%A0)+, (%A1)+
        bra.s   copy_data
go_ahead:

        MOVE.B  #'7',WR_CHAR

        /* Duart A init */
        // DUART_ACR = 0x00; (seleciona baud Set 1)
        move.b #0x00, (DUART_BASE + 0x09).l
        // DUART_SRA_CSRA = 0xCC; (Rx/Tx = 38400 baud, cristal 3.6864MHz)
        move.b #0xCC, (DUART_BASE + 0x03).l
        // DUART_CRA = 0x10; (reseta ponteiro MR -> aponta p/ MR1A)
        move.b #0x10, (DUART_BASE + 0x05).l
        // DUART_MR1A_MR2A = 0x13; (MR1A: 8 bits, sem paridade, char mode)
        move.b #0x13, (DUART_BASE + 0x01).l
        // DUART_MR1A_MR2A = 0x07; (MR2A: modo normal, 1 stop bit)
        move.b #0x07, (DUART_BASE + 0x01).l
        // DUART_CRA = 0x20; (reset receiver)
        move.b #0x20, (DUART_BASE + 0x05).l
        // DUART_CRA = 0x30; (reset transmitter)
        move.b #0x30, (DUART_BASE + 0x05).l
        // DUART_CRA = 0x05; (habilita Rx e Tx)
        move.b #0x05, (DUART_BASE + 0x05).l

        MOVE.B  #'8',WR_CHAR

        lea     bootstrap_string,%a0
        jsr     print_string

        lea     exception_string,%a0
        jsr     print_string
        /*Para empilhar os parametros de uma função
          começamos do ultimo para o primeiro.
          e no retorno da função temos que somar (4 * total de parâmetros) no SP.      
        */        
        | 1. Empilha o último argumento (count)
        move.l  #50, -(%SP)            /*#254, -(%SP)*/
        | 2. Empilha o segundo argumento (target_addr)
        move.l  fdummy, -(%SP)
        | 3. Empilha o primeiro argumento (base_addr)
        move.l  #0x00080000, -(%SP)
        | Chama a função C
        BSR     build_jump_table
        | Limpa a pilha após o retorno (3 argumentos * 4 bytes = 12 bytes)
        ADDA.L  #12, %SP


        MOVE.B  #'9',WR_CHAR

        jsr     main

.dead:
        bra.s   .dead

PicoWrCh:
        LEA     0x00FF8001,%A1
        MOVE.B  %D0,(%A1)
        NOP
        NOP
        NOP
        NOP
        RTS

printString:
        MOVE.L  4(%A7),%A0     /* pega o 1º argumento da pilha (ponteiro da string) e põe em A0 */
        MOVE.L  %A0,-(%A7)     /* preserva A0 original do chamador (é caller-saved, mas por segurança) */
        ADDQ.L  #4,%A7
print_string:
        move.b  (%A0)+,%d0
        beq     .end_print_string
        jsr     duart_send_char
        /*jsr     PicoWrCh*/
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
/*
; ---------------------------------------------------------
; DELAY_1MS: Gera um atraso de aproximadamente 1 ms
; Frequência da CPU: 16 MHz (Período do clock = 62.5 ns)
; ---------------------------------------------------------
*/
_delay_ms:
    MOVE.L  %D0, -(%A7)
    MOVE.L  #1333,%D0
.loop1:
    DBRA    %D0, .loop1

    MOVE.L  (%A7)+,%D0
    RTS
/*
; ---------------------------------------------------------
; DELAY_1MS_8MHZ: Gera um atraso de aproximadamente 1 ms
; Frequência da CPU: 8 MHz (Período do clock = 125 ns)
; ---------------------------------------------------------
*/
DELAY_1MS_8MHZ:
    MOVE.L  %D0,-(%A7)
    MOVE.L  #666,%D0
.loop2:
    DBRA    %D0, .loop2
    MOVE.L  (%A7)+,%D0
    RTS
/*
; ---------------------------------------------------------
; DELAY_1MS_4MHZ: Gera um atraso de aproximadamente 1 ms
; Frequência da CPU: 4 MHz (Período do clock = 250 ns)
; ---------------------------------------------------------
*/
DELAY_1MS_4MHZ:
    MOVE.L  %D0,-(%A7)
    MOVE.L  #333,%D0
.loop3:
    DBRA    %D0, .loop3
    MOVE.L  (%A7)+,%D0
    RTS

/*
; ---------------------------------------------------------
; DELAY_SECONDS: Gera um atraso de N segundos
; Entrada: D1 = número de segundos (0 = retorna imediatamente)
; Usa _delay_ms internamente (assume 16MHz, ajuste se mudar clock)
; ---------------------------------------------------------
*/
delay_seconds:
        MOVE.L  %D0,-(%SP)
        MOVE.L  %D1,-(%SP)

        TST.L   %D1
        BEQ     .delay_seconds_done   /* se pediram 0 segundos, sai direto */

.loop_outer:
        MOVE.L  #550,%D0             /* 1000 x 1ms = 1 segundo */
.loop_inner:
        JSR     DELAY_1MS_8MHZ
        SUBQ.L  #1,%D0
        BNE     .loop_inner

        SUBQ.L  #1,%D1
        BNE     .loop_outer

.delay_seconds_done:
        MOVE.L  (%SP)+,%D1
        MOVE.L  (%SP)+,%D0
        RTS


.section .text
.align 2
bootstrap_string:
        .byte   13, 10
        .ascii  "PDS317 - Bootstrap 2026 V1.0"
        .byte   13, 10, 0
exception_string:        
        .ascii  "Mounting exceptions table..."
        .byte   13, 10, 0

Svcaddress_err:
SvcIllegalIns:
SvcDiv0_handler:
SvcChkIns:
SvcTrapvIns:
SvcPrivViolation:
SvcTrace:
SvcLineA:
SvcLineF:
bad_exception:
bad_exception:
bad_exception:
bad_exception:
bad_exception:
bad_exception:
bad_exception:
bad_exception:
bad_exception:
bad_exception:
bad_exception:
bad_exception:
SpuriousHandler:
Int1Handler:
Int4Handler:
Int5Handler:
Int6Handler:
Int7Handler:
Trap0Handler:
Trap1Handler:
Trap2Handler:
Trap3Handler:
Trap4Handler:
Trap5Handler:
Trap6Handler:
Trap7Handler:
Trap8Handler:
Trap9Handler:
TrapAHandler:
TrapBHandler:
TrapCHandler:
TrapDHandler:
TrapEHandler:
TrapFHandler:
        rte
DefaultHandler:
        rte
.align 2        /* <--- CRÍTICO: Garante que o PRÓXIMO arquivo linkado comece em endereço PAR */
