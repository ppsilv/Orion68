; ----------------------------------------------------------------------
; MemDump - Imprime dump de memória formatado
; Entrada:
;   A0 = Endereço inicial (ex: $80000)
;   D0 = Quantidade de bytes (ex: 256)
; ----------------------------------------------------------------------
MemDump:
        JSR     PicoClearScreen

        LEA     MsgDumpHeader,A0
        JSR     WriteStringConout
        MOVE.L  (addressInHex),A0
        MOVE.L  A0,D0
        JSR     PrintHexAddress
        JSR     NewLine

        LEA     MsgDumpHeader1,A0
        JSR     WriteStringConout

        MOVE.L  (addressInHex),A0
        ; Calcula endereço final

DUMPLOOPMASTER:
        MOVE.W  #$0,D0
        SWAP    D0
        MOVE.W  #$5,D0
        JSR     PicoSetCursor
        
        CLR.L   D1
        MOVE.L  A0,D1
        ADDI.L  #$000000FF,D1             ; D1 = endereço final

DumpLoop:
        ; Nova linha a cada 16 bytes
        MOVE.L  A0,D0
        ANDI.L  #$0000000F,D0     ; Verifica se é início de linha
        BNE     NoNewLine

        ; Imprime endereço
        MOVE.L  A0,D0
        JSR     PrintHexAddress    ; Imprime 8 dígitos hex

        MOVE.B  #':',D0
        JSR     WriteConout
        MOVE.B  #' ',D0
        JSR     WriteConout

NoNewLine:
        ; Imprime byte em hex
        MOVE.B  (A0)+,D0
        JSR     PrintByteHex

        MOVE.B  #' ',D0
        JSR     WriteConout

        ; Verifica fim da linha (16 bytes)
        MOVE.L  A0,D0
        ANDI.L  #$0000000F,D0
        BNE     NoEndLine

        ; Imprime caracteres ASCII
        MOVE.B  #' ',D0
        JSR     WriteConout
        MOVE.B  #'|',D0
        JSR     WriteConout

        LEA     -16(A0),A1        ; Volta ao início da linha
        MOVEQ   #15,D2            ; 16 caracteres

AsciiLoop:
        MOVE.B  (A1)+,D0
        CMP.B   #32,D0            ; Verifica se é imprimível
        BLT     NonPrintable
        CMP.B   #$7E,D0
        BGT     NonPrintable

        JSR     WriteConout
        BRA     NextAscii

NonPrintable:
        MOVE.B  #'.',D0           ; Substitui não imprimíveis
        JSR     WriteConout

NextAscii:
        DBRA    D2,AsciiLoop

        MOVE.B  #'|',D0
        JSR     WriteConout
        MOVE.B  #13,D0            ; CR
        JSR     WriteConout
        MOVE.B  #10,D0            ; LF
        JSR     WriteConout

NoEndLine:
        ; Verifica fim do dump
        CMP.L   D1,A0
        BLS     DumpLoop

        JSR     PrintAx
        JSR     PrintDx

        MOVE.L  A0,-(SP)          ; Salva endereço atual
        LEA     MsgHitAnyKey,A0
        JSR     WriteStringConout
        JSR     ReadConin
        CMP.B   #$1B,D0
        BEQ     .fim

        MOVE.L  (SP)+,A0          ; Recupera endereço atual
        BRA     DUMPLOOPMASTER
.fim
        MOVE.L  (SP)+,A0          ; Recupera endereço atual
        RTS


PrintAx:
        MOVE.L  A0,-(SP)          ; Salva endereço atual

        LEA     MsgA0,A0
        JSR     WriteStringConout
        MOVE.L  (SP)+,A0          ; Recupera endereço atual
        MOVE.L  A0,-(SP)          ; Salva endereço atual

        MOVE.L  A0,D0
        JSR     PrintHexAddress    ; Imprime 8 dígitos hex


        LEA     MsgA1,A0
        JSR     WriteStringConout
        MOVE.L  A1,D0
        JSR     PrintHexAddress    ; Imprime 8 dígitos hex

        LEA     MsgA2,A0
        JSR     WriteStringConout
        MOVE.L  A2,D0
        JSR     PrintHexAddress    ; Imprime 8 dígitos hex

        LEA     MsgA3,A0
        JSR     WriteStringConout
        MOVE.L  A3,D0
        JSR     PrintHexAddress    ; Imprime 8 dígitos hex

        LEA     MsgA4,A0
        JSR     WriteStringConout
        MOVE.L  A4,D0
        JSR     PrintHexAddress    ; Imprime 8 dígitos hex


        LEA     MsgNewLine,A0
        JSR     WriteStringConout

        MOVE.L  (SP)+,A0          ; Recupera endereço atual

        RTS

PrintDx:
        MOVE.L  A0,-(SP)          ; Salva endereço atual

        LEA     MsgD0,A0
        JSR     WriteStringConout
        JSR     PrintHexAddress    ; Imprime 8 dígitos hex

        LEA     MsgD1,A0
        JSR     WriteStringConout
        MOVE.L  D1,D0
        JSR     PrintHexAddress    ; Imprime 8 dígitos hex

        LEA     MsgD2,A0
        JSR     WriteStringConout
        MOVE.L  D2,D0
        JSR     PrintHexAddress    ; Imprime 8 dígitos hex

        LEA     MsgD3,A0
        JSR     WriteStringConout
        MOVE.L  D3,D0
        JSR     PrintHexAddress    ; Imprime 8 dígitos hex

        LEA     MsgD4,A0
        JSR     WriteStringConout
        MOVE.L  D4,D0
        JSR     PrintHexAddress    ; Imprime 8 dígitos hex

        LEA     MsgNewLine,A0
        JSR     WriteStringConout
        
        MOVE.L  (SP)+,A0          ; Recupera endereço atual

        RTS                