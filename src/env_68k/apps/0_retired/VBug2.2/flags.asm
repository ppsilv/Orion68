        section .code
        even


SPECIAL_KEY_UP      equ     1
SPECIAL_KEY_DOWN    equ     1
KEY_DOWN            equ     1



; =============================================================================
; MACROS DE MANIPULAÇÃO DE FLAGS DIRETAMENTE NA MEMÓRIA (VASM)
; =============================================================================

; -----------------------------------------------------------------------------
; MACRO: SET_FLAG
; Parâmetros: \1 = Máscara do Bit (ex: SPECIAL_KEY_MASK)
;             \2 = Variável na RAM (ex: flg_system)
; -----------------------------------------------------------------------------
SET_FLAG    macro
            or.w    #\1,\2
            endm

; -----------------------------------------------------------------------------
; MACRO: RESET_FLAG
; Parâmetros: \1 = Máscara do Bit (ex: SPECIAL_KEY_MASK)
;             \2 = Variável na RAM (ex: flg_system)
; -----------------------------------------------------------------------------
RESET_FLAG  macro
            and.w   #\1,\2
            endm

; -----------------------------------------------------------------------------
; MACRO: TEST_FLAG
; Parâmetros: \1 = Máscara do Bit (ex: SPECIAL_KEY_MASK)
;             \2 = Variável na RAM (ex: flg_system)
; Nota: Altera a flag Z (Zero) da CPU para uso com BEQ/BNE logo em seguida.
; -----------------------------------------------------------------------------
TEST_FLAG   macro
            and.w   #\1,\2
            endm
