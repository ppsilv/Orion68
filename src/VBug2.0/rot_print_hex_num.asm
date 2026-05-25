
; ----------------------------------------------------------------------
; PrintHexAddress - Imprime endereço de 32 bits
; Entrada: D0 = endereço
; ----------------------------------------------------------------------
PrintHexAddress:
        MOVE.L  D1,-(SP)
        SWAP    D0                ; Imprime parte alta primeiro
        JSR     PrintWordHex
        MOVE.L  D1,D0
        SWAP    D0                ; Parte baixa
        JSR     PrintWordHex
        MOVE.L (SP)+,D1
        RTS
        
PrintWordHex:
        MOVE.L  D0,D1
        ROR     #8,D0             ; Byte mais significativo primeiro
        JSR     PrintByteHex
        MOVE.L  D1,D0             ; Byte menos significativo
        JSR     PrintByteHex
        RTS

PrintByteHex:
        MOVE.B  D0,-(SP)          ; Salva byte original
        LSR.B   #4,D0             ; Nibble alto
        BSR     .PrintNibble
        MOVE.B  (SP)+,D0          ; Recupera byte
        ANDI.B  #$0F,D0           ; Nibble baixo
.PrintNibble:
        CMP.B   #9,D0
        BLS     .Decimal
        ADD.B   #7,D0             ; Ajuste para A-F
.Decimal:
        ADD.B   #'0',D0
        JSR     WriteConout    ; Usa JMP para tail call optimization
        RTS
