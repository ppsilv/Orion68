; ----------------------------------------------------------------------
; SECTION bss
;-----------------------------------------------------------------------
    SECTION .bss
    ORG     RAM_VARIABLES               ; Área para variáveis
;=== System variables
romBase             DS.L 1
romSize             DS.L 1
ramBase             DS.L 1
ramSize             DS.L 1
currentUart         DS.L 1
currentBaudRate     DS.W 1
cconout             DS.L 1
cconin              DS.L 1
currentTimer        DS.L 1
;flg_orion           DS.L   1         ;minhas_flags   

        ALIGN 2
buf_xmodem          DS.B   512        ; Buffer de dados
;flg_pgm_loaded      DS.B   1
;blockNumber         DS.B   1          ; Número do bloco atual
;expectedBlock       DS.B   1          ; Próximo bloco esperado

        ALIGN 4
;checksumCalc       DS.L   1
;monitorStack       DS.L   1
systemTick          DS.L   1
addressInHex        DS.L   1

        ORG $00082000
        ALIGN 2
buf_pgm             DS.B   8192
        END