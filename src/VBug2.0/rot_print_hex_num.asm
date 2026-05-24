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
        JMP     UartWriteChar    ; Usa JMP para tail call optimization
