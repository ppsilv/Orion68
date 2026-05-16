; ----------------------------------------------------------------------
; SECTION bss
;-----------------------------------------------------------------------
    SECTION .bss
    ORG     RAMBASE+$400               ; Área para variáveis
romBase             DS.L 1
romSize             DS.L 1
ramBase             DS.L 1
ramSize             DS.L 1
currentUart         DS.L 1
currentBaudRate     DS.W 1
cconout             DS.L 1
cconin              DS.L 1
currentTimer        DS.L 1
; === Buffer circular for uart (256 bytes) ===
addressInHex        DS.L 1            ; ENDEREÇO LIDO
buffer_head         DS.L 1            ; Ponteiro de escrita (próxima posição livre)
buffer_tail         DS.L 1            ; Ponteiro de leitura (próximo dado a ler)
buffer_count        DS.W 1            ; Contador de bytes no buffer
buffer              DS.B 256          ; Buffer de recepção
;=== System variables
xmodem_buffer       DS.B   512        ; Buffer de dados
block_number        DS.B   1          ; Número do bloco atual
expected_block      DS.B   1          ; Próximo bloco esperado
usr_buffer_addr     DS.B   512
flag_pgm_loaded     DS.B   1
        ALIGN 4
checksum_calc       DS.L   1
minhas_flags        DS.L   1
monitor_stack       DS.L   1
system_tick         DS.L   1

        ORG $00082000
        ALIGN 2
pgm_buffer          DS.B   8192
        END