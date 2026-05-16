    ORG     $00008000

InitTrap1:
    ; -------------------------------------------------------------------------
    ; COMO INSTALAR NA RAM:
    ; Copiamos o endereço do nosso handler real para a posição $404 da RAM
    ; -------------------------------------------------------------------------
    LEA     vbug2_trap1,A0             ; Pega o endereço da nossa rotina na RAM
    MOVE.L  A0,($00080004)              ; Grava na posição da TRAP 1 na RAM!
    RTS
    ; A partir deste momento, qualquer instrução "TRAP #1" vai cair aqui!
    
; -----------------------------------------------------------------------------
; O SEU CÓDIGO DO TCPBOX68K (Agora rodando na RAM)
; -----------------------------------------------------------------------------
vbug2_trap1:
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
    MOVE.L  (cconin),A0
    JSR     (A0)
    ANDI.L      #$FF,D0       ; Mantém apenas o byte inferior
    RTE                   ; Retorna da exceção

; --------------------------------
; TRAP_CCONOUT - Escrever caractere no console
; Entrada: D1.L - Caractere a escrever
; --------------------------------
trap_cconout:
    MOVE.L  (cconout),A0
    JSR     (A0)
    RTE                   ; Retorna da exceção

trap_strout:
    MOVE.L  (cconout),A1
.strout:
    MOVE.B  (A0)+,D0
    CMP.B   #0,D0
    BEQ     .fim
    JSR     (A1)
    BRA     .strout
.fim:
    RTE

; --------------------------------
; TRAP_PTERM0 - Terminar programa
; --------------------------------
trap_pterm0:
    move.w  #0,d0         ; Código de saída 0
    bsr     hardware_exit ; Chama rotina de término
    RTE     

; --------------------------------
; hardware_exit - Terminar execução
; Entrada: D0.W - Código de saída
; --------------------------------
hardware_exit:
    ; >>> ADAPTE PARA SUA PLACA <<<
    ; Exemplo: parar o processador
    ; stop    #$2700

    ; Exemplo simples: loop infinito
    BRA     hardware_exit
    RTS