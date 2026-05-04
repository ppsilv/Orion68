            SECTION .vectors
            ORG     $00000000
            DC.L    $FFFF0    ; Stack Pointer
            DC.L    START    ; PC aponta para o início do código

            ORG     $00000400
; Exemplo rápido de teste de RAM (Base em $00004000)
RAM_START   EQU  $88000
;;picoVGA Register address
WRITE_SCREEN   equ $FB8001        ;;Endereço real  3 0x03  o pico enxerga 0x00
REG_02         equ $FB8003        ;;Endereço real  3 0x03  o pico enxerga 0x01
REG_03         equ $FB8005        ;;Endereço real  5 0x05  o pico enxerga 0x02
REG_04         equ $FB8007        ;;Endereço real  7 0x07  o pico enxerga 0x03
CONFIG_REG     equ $FB8009        ;;Endereço real  9 0x09  o pico enxerga 0x04
REG_06         equ $FB800b        ;;Endereço real 11 0x0b  o pico enxerga 0x05
REG_07         equ $FB800d        ;;Endereço real 13 0x0d  o pico enxerga 0x06
REG_08         equ $FB800f        ;;Endereço real 15 0x0f  o pico enxerga 0x07
REG_09         equ $FB8011        ;;Endereço real 17 0x11  o pico enxerga 0x08
REG_0A         equ $FB8013        ;;Endereço real 19 0x13  o pico enxerga 0x09
REG_0B         equ $FB8015        ;;Endereço real 21 0x15  o pico enxerga 0x0A
REG_0C         equ $FB8017        ;;Endereço real 23 0x17  o pico enxerga 0x0B
REG_0D         equ $FB8019        ;;Endereço real 25 0x19  o pico enxerga 0x0C
REG_0E         equ $FB801b        ;;Endereço real 27 0x1b  o pico enxerga 0x0D
REG_0F         equ $FB801d        ;;Endereço real 29 0x1d  o pico enxerga 0x0E
REG_10         equ $FB801f        ;;Endereço real 31 0x1f  o pico enxerga 0x0F
REG_11         equ $FB8021        ;;Endereço real 33 0x21  o pico enxerga 0x10
REG_12         equ $FB8023        ;;Endereço real 35 0x23  o pico enxerga 0x11
REG_13         equ $FB8025        ;;Endereço real 37 0x25  o pico enxerga 0x12
SET_MODE       equ $FB8027        ;;Endereço real 39 0x27  o pico enxerga 0x13 (0=Texto+Scroll, 1=Texto Fixo, 2=320x200, 3=640x200)
SET_TXT_COLOR  equ $FB8029        ;;Endereço real 41 0x29  o pico enxerga 0x14
CHANGE_CUR_POS equ $FB802b        ;;Endereço real 43 0x2b  o pico enxerga 0x15
REG_X_HIGH     equ $FB802d        ;;Endereço real 45 0x2d  o pico enxerga 0x16
REG_X_LOW      equ $FB802f        ;;Endereço real 47 0x2f  o pico enxerga 0x17
REG_Y_HIGH     equ $FB8031        ;;Endereço real 49 0x31  o pico enxerga 0x18
REG_Y_LOW      equ $FB8033        ;;Endereço real 51 0x33  o pico enxerga 0x19
CHANGE_BUFFER  equ $FB8035        ;;Endereço real 53 0x35  o pico enxerga 0x1A
SELECT_SCREEN  equ $FB8037        ;;Endereço real 55 0x37  o pico enxerga 0x1B
SET_HORIZONTAL equ $FB8039        ;;Endereço real 57 0x39  o pico enxerga 0x1C
SET_VERTICAL   equ $FB803b        ;;Endereço real 59 0x3b  o pico enxerga 0x1D
RUN_CMD        equ $FB803d        ;;Endereço real 61 0x3d  o pico enxerga 0x1E
CORINGA        equ $FB803f        ;;Endereço real 63 0x3f  o pico enxerga 0x1F    


;Commands
CMD_SYSTEM_ENABLE equ  $A5
CMD_CLEAR_SCREEN  equ  $A4
CMD_SET_CUR_POS   equ  $A3
CMD_SET_TXT_COLOR equ  $A2
CMD_GO_HOME       equ  $A1

START:
            MOVE.W  #$2700,SR
            LEA     $FFFF0,SP ; Garante o Stack Pointer (se o hardware não carregou)
    
            ; Limpa todos os registradores de dados e endereços
            MOVE.L  #0,D0
            MOVE.L  D0,D1
            MOVE.L  D0,D2
            MOVE.L  D0,D3
            MOVE.L  D0,D4
            MOVE.L  D0,D5
            MOVE.L  D0,D6
            MOVE.L  D0,D7            


            JMP     start
            SECTION .text  
            
start:
        JSR     ClearScreen
        LEA     MSGINIT,A0
        JSR     PrintString
        JSR     SetCursor
        MOVE.L  #$00FB8001,D0
        JSR     PrintHexAddress
fim:
            BRA.S   fim 

mem_test:
        move.l #$88000,a0           ; Inicio da RAM
        move.l #$100000,d2      ; Limite de 1MB
        move.w #$aa55,d0        ; Padrao de teste 1
test_loop:
        move.w d0,(a0)          ; Grava padrao 1
        move.w (a0),d1          ; Le de volta
        cmp.w d0,d1             ; Verifica
        bne.s show_error        ; Se erro, desvia para tratar

        lea     $00FB8001,A1
        move.b  #$41,D0     ; Carrega 'A' em D0
        move.b  D0,(A1); Escreve no endereço do PicoVGA (LDS ativo)        

            MOVE.L  #$0000FFF,D1 ; Contador para o delay (ajuste se precisar de mais)
.DELAY00:
            SUBQ.L  #1,D1         ; Subtrai 1 de D1 (4 ciclos)
            BNE.S   .DELAY00     ; Pula se não for zero (10 ciclos se pular, 8 se não)
            ;BRA.S   MAIN_ROM    


next_addr:
        addq.l #2,a0            ; Proxima word (16 bits)
        cmp.l d2,a0             ; Fim da memoria?
        bne.s test_loop         ; Se nao, continua
        bra end_test          ; Fim do teste total

end_test:
            LEA     $00FB8001,A1
            MOVE.B  #$50,D0     ; Carrega 'A' em D0
            MOVE.B  D0,(A1); Escreve no endereço do PicoVGA (LDS ativo)

            MOVE.L  #$0000FFF,D1 ; Contador para o delay (ajuste se precisar de mais)
.DELAY00:
            SUBQ.L  #1,D1         ; Subtrai 1 de D1 (4 ciclos)
            BNE.S   .DELAY00     ; Pula se não for zero (10 ciclos se pular, 8 se não)
            bra.s   end_test    


show_error:
            LEA     $00FB8001,A1
            MOVE.B  #$45,D0     ; Carrega 'A' em D0
            MOVE.B  D0,(A1); Escreve no endereço do PicoVGA (LDS ativo)

            MOVE.L  #$0000FFF,D1 ; Contador para o delay (ajuste se precisar de mais)
.DELAY00:
            SUBQ.L  #1,D1         ; Subtrai 1 de D1 (4 ciclos)
            BNE.S   .DELAY00     ; Pula se não for zero (10 ciclos se pular, 8 se não)
            jmp     next_addr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Posiociona o cursor
; Detroy: A1
; Recebe X em D0
; Recebe Y em D1
SetCursor:
        MOVE.W  #$0190,D0
        MOVE.W  #$00A0,D0
        MOVE.B  D0,REG_X_LOW
        LSR.W   #8,D0             ; Desloca o byte alto para a posição baixa em D0
        MOVE.B  D0,REG_X_HIGH
        MOVE.B  D1,REG_Y_LOW
        LSR.W   #8,D1             ; Desloca o byte alto para a posição baixa em D0
        MOVE.B  D1,REG_Y_HIGH
        LEA     RUN_CMD,A1    
        MOVE.B  #CMD_SET_CUR_POS,D0
        MOVE.B  D0,(A1);
        RTS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Limpa a tela 
; Destroy: A1 = Endereço da string
; Comando em D0
ClearScreen:
        LEA     RUN_CMD,A1    
        MOVE.B  #CMD_CLEAR_SCREEN,D0
        MOVE.B  D0,(A1);
        RTS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Imprime string terminada em ZERO
; Destroy: A0 = Endereço da string
PrintString:
        MOVE.B  (A0)+,D0      ; Pega caractere e avança ponteiro
        BEQ.S   .Done         ; Se for zero, termina
        BSR.S   WriteChar     ; Chama sua função gloriosa!
        BRA.S   PrintString   ; Loop para o próximo
.Done:
        RTS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Escreve um caractere no picoVGA
; Destroy: A1,D1
WriteChar:
            LEA     WRITE_SCREEN,A1
            MOVE.B  D0,(A1); Escreve no endereço do PicoVGA (LDS ativo)

            MOVE.L  #$FF,D1 ; Contador para o delay (ajuste se precisar de mais)
.DELAY00:
            SUBQ.L  #1,D1         ; Subtrai 1 de D1 (4 ciclos)
            BNE.S   .DELAY00     ; Pula se não for zero (10 ciclos se pular, 8 se não)
            RTS

; ----------------------------------------------------------------------
; PrintHexAddress - Imprime endereço de 32 bits
; Entrada: D0 = endereço
; ----------------------------------------------------------------------
PrintHexAddress:
        SWAP    D0                ; Imprime parte alta primeiro
        JSR     PrintWordHex
        SWAP    D0                ; Parte baixa
        ; Continua para PrintWordHex

; ----------------------------------------------------------------------
; PrintWordHex - Imprime word em hex (16 bits)
; Entrada: D0.w = valor
; ----------------------------------------------------------------------
PrintWordHex:
        ROL.W   #8,D0             ; Byte mais significativo primeiro
        JSR     PrintByteHex
        ROR.W   #8,D0             ; Byte menos significativo
        ; Continua para PrintByteHex

; ----------------------------------------------------------------------
; PrintByteHex - Imprime byte em hex (8 bits)
; Entrada: D0.b = valor
; ----------------------------------------------------------------------
PrintByteHex:
        MOVE.B  D0,-(SP)          ; Salva byte original
        LSR.B   #4,D0             ; Nibble alto
        BSR     PrintNibble
        MOVE.B  (SP)+,D0          ; Recupera byte
        ANDI.B  #$0F,D0           ; Nibble baixo
        ; Continua para PrintNibble
; ----------------------------------------------------------------------
; PrintNibble - Imprime um nibble em hex
; Entrada: D0.b bits 3-0 = nibble (0-15)
; ----------------------------------------------------------------------
PrintNibble:
        CMP.B   #9,D0
        BLS     .Decimal
        ADD.B   #7,D0             ; Ajuste para A-F
.Decimal:
        ADD.B   #'0',D0
        JMP     WriteChar    ; Usa JMP para tail call optimization





;;newtest_loop:
;;        move.w d0,(a0)          ; Grava padrao 1
;;        move.w (a0),d1          ; Le de volta
;;        cmp.w d0,d1             ; Verifica
;;        bne.s show_error        
;;
;;        ; --- NOVO: Feedback Visual Econômico ---
;;        move.l a0,d3           ; Copia endereço atual
;;        and.l  #$3FFF,d3       ; Mascara para 16KB (0x3FFF)
;;        bne.s  newnext_addr        ; Se não for múltiplo de 16KB, pula a impressão
;;        
;;        lea    $00FB8001,A1
;;        move.b #$2E,D0          ; Imprime um ponto '.' em vez de 'A'
;;        move.b D0,(A1)
;;        ; ---------------------------------------
;;
;;newnext_addr:
;;        addq.l #2,a0




MSGINIT:
    DC.B    13,10,"Orion68k - copyright (C) pdsilva(pgordao).V1.0",13,10,0            


