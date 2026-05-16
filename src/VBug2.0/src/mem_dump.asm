



; ----------------------------------------------------------------------
; MemDump - Imprime dump de memória formatado
; Entrada:
;   A0 = Endereço inicial (ex: $80000)
;   D0 = Quantidade de bytes (ex: 256)
; ----------------------------------------------------------------------
MEMDUMP:
        LEA     MsgDumpHeader,A0
        JSR     UART_WriteString
        MOVE.L  (addressInHex),A0
        MOVE.L  A0,D0
        JSR     PicoPrintHexAddress
        JSR     NewLine

        LEA     MsgDumpHeader1,A0
        JSR     UART_WriteString

        MOVE.L  (addressInHex),A0
        ; Calcula endereço final

DUMPLOOPMASTER:
        CLR.L   D1
        MOVE.L  A0,D1
        ADDI.L  #$000000FF,D1             ; D1 = endereço final
        ;MOVE.L  D1,D0
        ;JSR     PicoPrintHexAddress
        ;JSR     new_line

        ;aguarda um caractere ser digitado mas nao usa é somente para parar a execução aqui
        ;JSR     UART_ReadChar


DumpLoop:
        ; Nova linha a cada 16 bytes
        MOVE.L  A0,D0
        ANDI.L  #$0000000F,D0     ; Verifica se é início de linha
        BNE     NoNewLine

        ; Imprime endereço
        MOVE.L  A0,D0
        JSR     PicoPrintHexAddress    ; Imprime 8 dígitos hex

        MOVE.B  #':',D0
        JSR     UART_WriteChar
        MOVE.B  #' ',D0
        JSR     UART_WriteChar

NoNewLine:
        ; Imprime byte em hex
        MOVE.B  (A0)+,D0
        JSR     PicoPrintByteHex

        MOVE.B  #' ',D0
        JSR     UART_WriteChar

        ; Verifica fim da linha (16 bytes)
        MOVE.L  A0,D0
        ANDI.L  #$0000000F,D0
        BNE     NoEndLine

        ; Imprime caracteres ASCII
        MOVE.B  #' ',D0
        JSR     UART_WriteChar
        MOVE.B  #'|',D0
        JSR     UART_WriteChar

        LEA     -16(A0),A1        ; Volta ao início da linha
        MOVEQ   #15,D2            ; 16 caracteres

AsciiLoop:
        MOVE.B  (A1)+,D0
        CMP.B   #32,D0            ; Verifica se é imprimível
        BLT     NonPrintable
        CMP.B   #126,D0
        BGT     NonPrintable

        JSR     UART_WriteChar
        BRA     NextAscii

NonPrintable:
        MOVE.B  #'.',D0           ; Substitui não imprimíveis
        JSR     UART_WriteChar

NextAscii:
        DBRA    D2,AsciiLoop

        MOVE.B  #'|',D0
        JSR     UART_WriteChar
        MOVE.B  #13,D0            ; CR
        JSR     UART_WriteChar
        MOVE.B  #10,D0            ; LF
        JSR     UART_WriteChar

NoEndLine:
        ; Verifica fim do dump
        CMP.L   D1,A0
        BLS     DumpLoop

        MOVE.L  A0,-(SP)          ; Salva endereço atual
        LEA     MsgHitAnyKey,A0
        JSR     UART_WriteString
        JSR     UART_ReadChar
        CMP.B   #$1B,D0
        BEQ     .fim

        MOVE.L  (SP)+,A0          ; Recupera endereço atual
        BRA     DUMPLOOPMASTER
.fim
        RTS