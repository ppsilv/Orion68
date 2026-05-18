; ----------------------------------------------------------------------
; SECTION bss
;-----------------------------------------------------------------------
    SECTION .bss
    ORG     RAM_VARIABLES ;RAM VARIABLES               ; Área para variáveis
        ALIGN 4
;=== System variables
romBase             DS.L 1
romSize             DS.L 1
ramBase             DS.L 1
ramSize             DS.L 1
currentUart         DS.L 1
cconout             DS.L 1
cconin              DS.L 1
;currentTimer        DS.L 1
;flg_orion           DS.L   1         ;minhas_flags   
;checksumCalc       DS.L   1
monitorStack       DS.L   1
systemTick          DS.L   1
addressInHex        DS.L   1

        ALIGN 2
buf_xmodem          DS.B   512        ; Buffer de dados
buf_put             DS.B   512    
buf_get             DS.B   512    
currentBaudRate     DS.W 1

flg_pgm_loaded      DS.B   1
;blockNumber         DS.B   1          ; Número do bloco atual
;expectedBlock       DS.B   1          ; Próximo bloco esperado

        ORG $00082000
        ALIGN 2
buf_pgm             DS.B   8192
        END