; ----------------------------------------------------------------------
; SECTION bss
;-----------------------------------------------------------------------
    SECTION .bss
    ORG     RAM_VARIABLES ;RAM VARIABLES               ; Área para variáveis
        ALIGN 4
;=== System variables
romBase             DS.L     1
romSize             DS.L     1
ramBase             DS.L     1
ramSize             DS.L     1
currentUart         DS.L     1
cconout             DS.L     1
cconin              DS.L     1
monitorStack        DS.L     1
systemTick          DS.L     1
addressInHex        DS.L     1
debug_regs          DS.L     8
        ALIGN 2
buf_xmodem          DS.B   512        ; Buffer de dados
buf_put             DS.B   512    
buf_get             DS.B   512    
chPayloadBuffer     DS.B    16        ; Buffer temporário para acomodar os dados HID
        ALIGN 2
currentBaudRate     DS.W     1
flg_system          DS.W     1        ; Flags do sistema PROGRAM_LOADED  





;***********************************************************************************
        ALIGN 2
;Keyboard variables
special_key_status  DS.B     1
cmd                 DS.B     1
mod_caps            DS.B     1
chksum              DS.B     1
    even
key_buffer          DS.B    48
    even
buf                 DS.B    11
    even

RetKey              DS.B     1
key_flag            DS.B     1
length              DS.B     1
type                DS.B     1
_CapsFlag           DS.B     1
debug_pkt           DS.B     1


SECTION DATA

        ORG $00082000
        ALIGN 2
buf_pgm             DS.B   8192
        END
