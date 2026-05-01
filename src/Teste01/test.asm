            SECTION .vectors
            ORG     $00000000
            DC.L    $00200000    ; Stack Pointer
            DC.L    START    ; PC aponta para o início do código

            ORG     $00000400
; Exemplo rápido de teste de RAM (Base em $00004000)
RAM_START   EQU  $88000

START:
            MOVE.W  #$2700,SR
            LEA     $1FFFF0,SP ; Garante o Stack Pointer (se o hardware não carregou)
    
            ; Limpa todos os registradores de dados e endereços
            MOVE.L  #0,D0
            MOVE.L  D0,D1
            MOVE.L  D0,D2
            MOVE.L  D0,D3
            MOVE.L  D0,D4
            MOVE.L  D0,D5
            MOVE.L  D0,D6
            MOVE.L  D0,D7            

            LEA     $00FB8001,A1
            MOVE.B  #$41,D0     ; Carrega 'A' em D0
            MOVE.B  D0,(A1); Escreve no endereço do PicoVGA (LDS ativo)

            ;TST.B   $F00000

            JMP     $01000

            SECTION .text  
            
UART_WriteString:
        LEA     MSGINIT,A0

.WriteLoop:
        MOVE.B  (A0)+,D0      ; Pega caractere
        BEQ     MAIN_ROM
        LEA     $00FB8001,A1
        MOVE.B  D0,(A1); Escreve no endereço do PicoVGA (LDS ativo)

        MOVE.L  #$00000FF,D1 ; Contador para o delay (ajuste se precisar de mais)
.DELAY00:
        SUBQ.L  #1,D1         ; Subtrai 1 de D1 (4 ciclos)
        BNE.S   .DELAY00     ; Pula se não for zero (10 ciclos se pular, 8 se não)

        BRA     .WriteLoop

MAIN_ROM:
            LEA     $00FB8001,A1
            MOVE.B  #$41,D0     ; Carrega 'A' em D0
            MOVE.B  D0,(A1); Escreve no endereço do PicoVGA (LDS ativo)

            MOVE.L  #$0000FFF,D1 ; Contador para o delay (ajuste se precisar de mais)
.DELAY00:
            SUBQ.L  #1,D1         ; Subtrai 1 de D1 (4 ciclos)
            BNE.S   .DELAY00     ; Pula se não for zero (10 ciclos se pular, 8 se não)
            ;BRA.S   MAIN_ROM    

mem_test:
        move.l #$0,a0           ; Inicio da RAM
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

next_addr:
        addq.l #2,a0            ; Proxima word (16 bits)
        cmp.l d2,a0             ; Fim da memoria?
        bne.s test_loop         ; Se nao, continua
        bra.s end_test          ; Fim do teste total

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

MSGINIT:
    DC.B    13,10,"Orion68k - copyright (C) pdsilva(pgordao).V1.0",13,10,0            