
;=== Uart receiver ================= CONSTANTES =================
SOH         EQU     $01        ; Start Of Header
EOT         EQU     $04        ; End Of Transmission
ACK         EQU     $06        ; Acknowledge
NACK         EQU     $15        ; Negative Acknowledge
CAN         EQU     $18        ; Cancel

XmodemRec:
        clr.l     D5
        JSR     UartFlush
        MOVE.B  #NACK,D0
        JSR     UartWriteChar
        MOVE.B  #13,D0
        JSR     WriteConout      
        MOVE.B  #10,D0
        JSR     WriteConout      
.receivePacote:
        JSR     ReadConin
        CMP.B   #EOT,D0
        BEQ     .Transfer_Complete   ; Fim da transmissão          
        JSR     PrintByteHexConout      
        JSR     ReadConin
        JSR     PrintByteHexConout      
        JSR     ReadConin
        JSR     PrintByteHexConout         
        MOVE.W  #$7F,D1
.centoEvinteOito:
        JSR     ReadConin
        add     D0,D5
        JSR     PrintByteHexConout      
        dbra    d1,.centoEvinteOito
        ;Aguardando check sum
        JSR     ReadConin
        JSR     PrintByteHexConout      
        MOVE.B  D5,D0
        JSR     PrintByteHexConout      
        MOVE.B  #ACK,D0
        JSR     UartWriteChar 
        JSR     NewLine                
        jmp     .receivePacote
.Transfer_Complete:

        JSR     ReadConin

        RTS ;