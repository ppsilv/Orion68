



; ----------------------------------------------------------------------
; MemDump - Imprime dump de memória formatado
; Entrada:
;   A0 = Endereço inicial (ex: $80000)
;   D0 = Quantidade de bytes (ex: 256)
; ----------------------------------------------------------------------
MemDump:
        LEA     MsgDumpHeader,A0
        JSR     WriteString
        MOVE.L  (addressInHex),A0
        CMPA.L  #$FFFFF,A0        ; Compara A0 com o limite superior
        BHI     OutOfRange       ; BHI (Branch if Higher): Desvia se A0 > $FFFFF

        MOVE.L  A0,D0
        JSR     PicoPrintHexAddress
        JSR     NewLine

        LEA     MsgDumpHeader1,A0
        JSR     WriteString

        MOVE.L  (addressInHex),A0
        ; Calcula endereço final
DumpLoopMaster:
        MOVE.W  #$0000,D0
        MOVE.W  #$0008,D1
        JSR     PicoSetCursor
        CLR.L   D3
        MOVE.L  A0,D3
        ADDI.L  #$000000FF,D3             ; D3 = endereço final    
DumpLoop:
        ; Nova linha a cada 16 bytes
        MOVE.L  A0,D0
        ANDI.L  #$0000000F,D0     ; Verifica se é início de linha
        BNE     NoNewLine

        ; Imprime endereço
        MOVE.L  A0,D0
        JSR     PicoPrintHexAddress    ; Imprime 8 dígitos hex

        MOVE.B  #':',D0
        JSR     WriteChar
        MOVE.B  #' ',D0
        JSR     WriteChar

NoNewLine:
        ; Imprime byte em hex
        MOVE.B  (A0)+,D0
        JSR     PicoPrintByteHex

        MOVE.B  #' ',D0
        JSR     WriteChar

        ; Verifica fim da linha (16 bytes)
        MOVE.L  A0,D0
        ANDI.L  #$0000000F,D0
        BNE     NoNewLine

        ; Imprime caracteres ASCII
        MOVE.B  #' ',D0
        JSR     WriteChar
        MOVE.B  #'|',D0
        JSR     WriteChar

        LEA     -16(A0),A1        ; Volta ao início da linha
        MOVEQ   #15,D2            ; 16 caracteres

AsciiLoop:
        MOVE.B  (A1)+,D0
        CMP.B   #$20,D0            ; Verifica se é imprimível
        BLO     NonPrintable
        CMP.B   #$7E,D0
        BHI     NonPrintable

        JSR     WriteChar
        BRA     NextAscii

NonPrintable:
        MOVE.B  #'.',D0           ; Substitui não imprimíveis
        JSR     WriteChar

NextAscii:
        DBRA    D2,AsciiLoop

        MOVE.B  #'|',D0
        JSR     WriteChar
        MOVE.B  #13,D0            ; CR
        JSR     WriteChar
        MOVE.B  #10,D0            ; LF
        JSR     WriteChar

NoEndLine:
        ; Verifica fim do dump
        CMP.L   D3,A0
        BLS     DumpLoop

        MOVE.L  A0,-(SP)          ; Salva endereço atual
        LEA     MsgHitAnyKey,A0
        JSR     WriteString
        JSR     UartReadChar
        CMP.B   #$1B,D0
        BEQ     .fim

        MOVE.B  #13,D0            ; CR
        JSR     WriteChar
        MOVE.B  #10,D0            ; LF
        JSR     WriteChar

        MOVE.L  (SP)+,A0          ; Recupera endereço atual
        BRA     DumpLoopMaster
.fim
        MOVE.L  (SP)+,A0          ; Recupera endereço atual
        JMP     subLoop
OutOfRange:
        LEA     MsgOutOfRange,A0
        JSR     WriteString        
        JMP     subLoop
     