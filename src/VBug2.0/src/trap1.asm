ORG     $00002000

MAIN_START:
    ; -------------------------------------------------------------------------
    ; COMO INSTALAR NA RAM:
    ; Copiamos o endereço do nosso handler real para a posição $404 da RAM
    ; -------------------------------------------------------------------------
    LEA     MEU_OS_TRAP1,A0             ; Pega o endereço da nossa rotina na RAM
    MOVE.L  A0,($00000404)              ; Grava na posição da TRAP 1 na RAM!

    ; A partir deste momento, qualquer instrução "TRAP #1" vai cair aqui!
    
; -----------------------------------------------------------------------------
; O SEU CÓDIGO DO TCPBOX68K (Agora rodando na RAM)
; -----------------------------------------------------------------------------
MEU_OS_TRAP1:
    CMP.W   #1,D0          ; Compara com CCONIN (ler caractere)
    BEQ     trap_cconin    ; Se for 1, vai para leitura
    CMP.W   #2,D0          ; Compara com CCONOUT (escrever caractere)
    BEQ     trap_cconout   ; Se for 2, vai para escrita
    CMP.W   #3,D0          ; Compara com CCONOUT (escrever string)
    BEQ     trap_strout    ; Se for 3, vai para escrita
    CMP.W   #0,D0          ; Compara com PTERM0 (terminar)
    BEQ     trap_pterm0    ; Se for 0, vai para terminar
    MOVE.L  #-1,D0         ; Retorna erro se função não reconhecida
    RTE

; --------------------------------
; TRAP_CCONIN - Ler caractere do console
; Saída: D0.L - Caractere lido
; --------------------------------
trap_cconin:
    JSR         UART_ReadCharNonEcho
    ANDI.L      #$FF,D0       ; Mantém apenas o byte inferior
    RTE                   ; Retorna da exceção

; --------------------------------
; TRAP_CCONOUT - Escrever caractere no console
; Entrada: D1.L - Caractere a escrever
; --------------------------------
trap_cconout:
    ANDI.L      #$FF,D1         ; Mantém apenas o byte inferior
    MOVE.B      D1,D0
    JSR         UART_WriteChar
    RTE                   ; Retorna da exceção

trap_strout:
    MOVE.B      (A0)+,D0
    CMP.B       #0,D0
    BEQ         .fim
    JSR         UART_WriteChar
    BRA         trap_strout
.fim:
    RTE

    