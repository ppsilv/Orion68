        SECTION .vectors
        ORG     $00000000
        ; --- Vetores de Exceção do 68000 ---
        DC.L    $000FFFF0         ; SP inicial
        DC.L    vbug2_start       ; PC inicial
        DC.L    SERVICE_BUS_ERR   ; Bus Error
        DC.L    SERVICE_ADDR_ERR  ; Address Error
        DC.L    SERVICE_ILLEGAL   ; Illegal Instruction
        DC.L    SERVICE_DIV0      ; Division by Zero
        DC.L    SERVICE_CHECK     ; CHK Instruction
        DC.L    SERVICE_TRAPV     ; TRAPV Instruction
        DC.L    SERVICE_PRIV      ; Privilege Violation
        DC.L    SERVICE_TRACE     ; Trace
        DC.L    SERVICE_LINE_A    ; Line A Emulator
        DC.L    SERVICE_LINE_F    ; Line F Emulator
        DC.L    DefaultHandler     ; $60: Spurious Interrupt
        DC.L    DefaultHandler     ; $64: Level 1 Interrupt
        DC.L    DefaultHandler     ; $68: Level 2 Interrupt
        DC.L    DefaultHandler     ; $6C: Level 3 Interrupt
        DC.L    DefaultHandler     ; $70: Level 4 Interrupt
        DC.L    DefaultHandler     ; $74: Level 5 Interrupt
        DC.L    DefaultHandler     ; $78: Level 6 Interrupt
        DC.L    DefaultHandler     ; $7C: Level 7 Interrupt
        DC.L    DefaultHandler
        DC.L    DefaultHandler
        DC.L    DefaultHandler
        DC.L    DefaultHandler
        DC.L    SpuriousHandler
        DC.L    Int1Handler
        DC.L    Int2Handler
        DC.L    Int3Handler
        DC.L    Int4Handler
        DC.L    Int5Handler
        DC.L    Int6Handler
        DC.L    Int7Handler
        DC.L    Trap0Handler
        DC.L    Trap1Handler
        DC.L    Trap2Handler
        DC.L    Trap3Handler
        DC.L    Trap4Handler
        DC.L    Trap5Handler
        DC.L    Trap6Handler
        DC.L    Trap7Handler
        DC.L    Trap8Handler
        DC.L    Trap9Handler
        DC.L    TrapAHandler
        DC.L    TrapBHandler
        DC.L    TrapCHandler
        DC.L    TrapDHandler
        DC.L    TrapEHandler
        DC.L    TrapFHandler
        DCB.L   128,Universal_Trampoline

        SECTION .jumptable
        ORG     $0400
ROM_JUMPTABLE:
        ; BRA     UART_Init          ;        0x400
        ; BRA     UART_WriteChar     ;        0x404
        ; BRA     UART_ReadChar      ;        0x408
        ; BRA     UART_Select        ;        0x40C
        ; BRA     UART_Setbaudrate   ;        0x410
        ; BRA     DELAY_MS           ;        0x414
        ; BRA     MEMDUMP            ;        0x418
        ; BRA     UART_ReadCharNonEcho;       0x41C
        ; BRA     vbug2_start         ;       0x420
        ; BRA     warm_start          ;       0x424
        ; BRA     MenuLoop            ;       0x428
        ; BRA     new_line            ;       0x42C
        ; BRA     UART_WriteString    ;       0x430
        ; BRA     UART_INIT1          ;       0x434
; =============================================================================
; FIM DA TABELA DE VETORES (Endereço $000400 alcançado de forma contínua!)
; =============================================================================
        SECTION .text
        ORG $00001000

ROMBASE         equ $00000
ROMSIZE         equ $3FFFF    ;256K WORDS
RAMBASE         equ $80000
RAMSIZE         equ $7FFFF    ;512K WORDS
USER_SP         equ $6F000
RAM_VECTOR_BASE equ RAMBASE

SERVICE_BUS_ERR:
        RTE
SERVICE_ADDR_ERR:
        RTE
SERVICE_ILLEGAL:
        RTE
SERVICE_DIV0:
        RTE
SERVICE_CHECK:
        RTE
SERVICE_TRAPV:
        RTE
SERVICE_PRIV:
        RTE
SERVICE_TRACE:
        RTE
SERVICE_LINE_A:
        RTE
SERVICE_LINE_F:
        RTE
DefaultHandler:
        MOVEM.L D0-D7/A0-A6,-(SP)
        ; --- IDENTIFICA O TIPO DE EXCEÇÃO, AGORA lê corretamente da pilha ---
        MOVE.L  (14,SP),D0      ; ⭐⭐ PC (14 bytes abaixo por causa dos registradores salvos)
        MOVE.W  (18,SP),D1      ; ⭐⭐ SR (18 bytes abaixo)
        MOVE.W  (20,SP),D2      ; ⭐⭐ Vector Offset (20 bytes abaixo)
        ; Mostra informações detalhadas
        LEA     MsgDebug,A0
        JSR     WriteString
        MOVE.L  D0,D0           ; PC
        JSR     PrintAddrHex
        JSR     NewLine
        LEA     MsgSr,A0
        JSR     WriteString
        MOVE.W  D1,D0           ; SR
        JSR     PrintAddrHex
        JSR     NewLine
        LEA     MsgVector,A0
        JSR     WriteString
        MOVE.W  D2,D0           ; Vector offset
        JSR     PrintAddrHex
        JSR     NewLine
        ; --- ANALISA O VETOR ---
        LSR.W   #2,D2           ; Divide por 4 para get vector number
        ANDI.W  #$FF,D2         ; Vector number em D2
        LEA     MsgVectorNum,A0
        JSR     WriteString
        MOVE.W  D2,D0
        JSR     PrintAddrHex
        JSR     NewLine
        ; --- MOSTRA QUAL EXCEÇÃO É ---
        CMP.W   #8,D2
        BNE     .not_bus_error
        LEA     MsgBusError,A0
        BRA     .show_msg
.not_bus_error:
        CMP.W   #9,D2
        BNE     .not_address_error
        LEA     MsgAddrError,A0
        BRA     .show_msg
.not_address_error:
        CMP.W   #10,D2
        BNE     .not_illegal
        LEA     MsgIllegal,A0
        BRA     .show_msg
.not_illegal:
        CMP.W   #32,D2
        BLO     .other_exception
        LEA     MsgTrap,A0
        BRA     .show_msg
.other_exception:
        LEA     MsgUnknown,A0
.show_msg:
        JSR     WriteString
        JSR     NewLine
        ; Pequeno delay para visualização
        MOVE.L  #$50000,D3
.DELAY:
        SUBQ.L  #1,D3
        BNE     .DELAY
        MOVEM.L (SP)+,D0-D7/A0-A6
        ;MOVE.W  #$CC00,LED_ADDRESS  ; Indica spurious
        RTE
SpuriousHandler:
        RTE
Trap0Handler:
        MOVE.L  A0,-(SP)
        MOVE.L  (RAM_VECTOR_BASE),A0     ; Trap 0 mapeada em $400
        BRA     Gateway
Trap1Handler:
        MOVE.L  A0,-(SP)
        MOVE.L  (RAM_VECTOR_BASE+4),A0   ; Trap 1 mapeada em $404
        BRA     Gateway
Trap2Handler:
        MOVE.L  A0,-(SP)
        MOVE.L  (RAM_VECTOR_BASE+8),A0   ; Trap 2 mapeada em $408
        BRA     Gateway
Trap3Handler:
        MOVE.L  A0,-(SP)
        MOVE.L  (RAM_VECTOR_BASE+12),A0
        BRA     Gateway
Trap4Handler:
        MOVE.L  A0,-(SP)
        MOVE.L  (RAM_VECTOR_BASE+16),A0
        BRA     Gateway
Trap5Handler:
        MOVE.L  A0,-(SP)
        MOVE.L  (RAM_VECTOR_BASE+20),A0
        BRA     Gateway
Trap6Handler:
        MOVE.L  A0,-(SP)
        MOVE.L  (RAM_VECTOR_BASE+24),A0
        BRA     Gateway
Trap7Handler:
        MOVE.L  A0,-(SP)
        MOVE.L  (RAM_VECTOR_BASE+28),A0
        BRA     Gateway
Trap8Handler:
        MOVE.L  A0,-(SP)
        MOVE.L  (RAM_VECTOR_BASE+32),A0
        BRA     Gateway
Trap9Handler:
        MOVE.L  A0,-(SP)
        MOVE.L  (RAM_VECTOR_BASE+36),A0
        BRA     Gateway
TrapAHandler:
        MOVE.L  A0,-(SP)
        MOVE.L  (RAM_VECTOR_BASE+40),A0
        BRA     Gateway
TrapBHandler:
        MOVE.L  A0,-(SP)
        MOVE.L  (RAM_VECTOR_BASE+44),A0
        BRA     Gateway
TrapCHandler:
        MOVE.L  A0,-(SP)
        MOVE.L  (RAM_VECTOR_BASE+48),A0
        BRA     Gateway
TrapDHandler:
        MOVE.L  A0,-(SP)
        MOVE.L  (RAM_VECTOR_BASE+52),A0
        BRA     Gateway
TrapEHandler:
        MOVE.L  A0,-(SP)
        MOVE.L  (RAM_VECTOR_BASE+56),A0
        BRA     Gateway
TrapFHandler:
        MOVE.L  A0,-(SP)
        MOVE.L  (RAM_VECTOR_BASE+60),A0
        BRA     Gateway

Gateway:
        MOVEM.L (SP)+,A0
        JMP     (A0)
Int1Handler:        
        MOVEM.L D0-D7/A0-A6,-(A7)
        MOVE.L  (RAM_VECTOR_BASE+64),A0
        MOVEM.L (A7)+,D0-D7/A0-A6
        RTE
Int2Handler:        
        MOVEM.L D0-D7/A0-A6,-(A7)
        MOVE.L  (RAM_VECTOR_BASE+68),A0
        MOVEM.L (A7)+,D0-D7/A0-A6
        RTE
Int3Handler:        
        MOVEM.L D0-D7/A0-A6,-(A7)
        MOVE.L  (RAM_VECTOR_BASE+72),A0
        MOVEM.L (A7)+,D0-D7/A0-A6
        RTE
Int4Handler:        
        MOVEM.L D0-D7/A0-A6,-(A7)
        MOVE.L  (RAM_VECTOR_BASE+76),A0
        MOVEM.L (A7)+,D0-D7/A0-A6
        RTE
Int5Handler:        
        MOVEM.L D0-D7/A0-A6,-(A7)
        MOVE.L  (RAM_VECTOR_BASE+80),A0
        MOVEM.L (A7)+,D0-D7/A0-A6
        RTE
Int6Handler:        
        MOVEM.L D0-D7/A0-A6,-(A7)
        MOVE.L  (RAM_VECTOR_BASE+84),A0
        MOVEM.L (A7)+,D0-D7/A0-A6
        RTE
Int7Handler:        
        MOVEM.L D0-D7/A0-A6,-(A7)
        ADDQ.L  #1,system_tick
        MOVEM.L (A7)+,D0-D7/A0-A6
        RTE

; Tratador para todos os outros vetores que você não mapeou individualmente ainda
Universal_Trampoline:
        RTE
WriteChar:
        RTS
WriteString:
        RTS
PrintAddrHex:
        RTS
NewLine:
        MOVE.L  D0,-(SP)          ; Salva D0
        MOVE.B  #10,D0
        JSR     WriteChar
        MOVE.B  #13,D0
        JSR     WriteChar
        MOVE.L  (SP)+,D0          ; Restaura D0
        RTS
;Essa rotina não pode limpar toda a RAM senão ela sobreescreve o StackPointer
;Acho que um limite seguro 512 bytes abaixo de $FFFFF.        
ClearRam:
        LEA     $080000,A0
        LEA     $0FFE00,A1
        MOVEQ   #0,D0
.ClearLoop:
        MOVE.L  D0,(A0)+
        CMPA.L  A0,A1
        BHI     .ClearLoop
        RTS
; =============================================================================
; 3. CÓDIGO DE INICIALIZAÇÃO (START)
; =============================================================================
vbug2_start:
        ORI     #$2700,SR           ; Desabilita interrupções (M68K)
        ;MOVE.W #$2700,SR           ; Habilita interrupções
        LEA     $FFFF0,SP           ; Garante o Stack Pointer (se o hardware não carregou)
        ;AINDA NÃO DÁ PARA VALIDAR A ROM TROQUEI POR UMA MUITO MAIOR 
        ;É PRECISO REAVALIAR O SISTEMA DE VALIDAÇÃO
        ;JSR     VALIDATE_ROM        ; Verifica a ROM

        ; Configura USP (pilha de usuário)
        LEA     USER_SP,A0
        MOVE.L  A0,USP        ; 🔥 Define User Stack Pointe
        ;Inicializa o tamanho das memorias
        MOVE.L  #$00000000,romBase
        MOVE.L  #$00004000,romSize
        MOVE.L  #RAMBASE,ramBase    ;Total ram 1572864 de 0x8000 até 0x180000
        MOVE.L  #RAMSIZE,ramSize

        LEA     RAM_VECTOR_BASE,A0
        MOVE.W  #15,D0                   ; Limita inicialização às 16 Traps primárias
        LEA     DefaultHandler,A1
Init_Ram_Loop:
        MOVE.L  A1,(A0)+
        DBRA    D0,Init_Ram_Loop

        ;roda um programa aqui inicialização terminada

        INCLUDE "uart.asm"
        INCLUDE "pico_vga.asm"
        INCLUDE "mem_dump.asm"


        INCLUDE "section_data.asm"
        INCLUDE "section_bss.asm"

        END

   



