            SECTION .vectors
            ORG     $00000000
            DC.L    $000FFFF0    ; Stack Pointer
            DC.L    START     ; PC aponta para o início do código

            ORG     $00000400
; Exemplo rápido de teste de RAM (Base em $00004000)
RAM_START      equ $080000
RAM_END        equ $0FFFFF 

            SECTION .text  

START:
        ori.w   #$0700,SR    ; Desabilita as interrupções (Nível 7)
        LEA     $FFFF0,SP ; Garante o Stack Pointer (se o hardware não carregou)

        MOVE.L  #$01FFFFF,D1 ; Contador para o delay (ajuste se precisar de mais)
.DELAY01:
        SUBQ.L  #1,D1         ; Subtrai 1 de D1 (4 ciclos)
        BNE.S   .DELAY01     ; Pula se não for zero (10 ciclos se pular, 8 se não)

        JSR     InitUart
        MOVE.L  #$1FF,D1    ; Contador para o delay (ajuste se precisar de mais)
.DELAY00:
        SUBQ.L  #1,D1       ; Subtrai 1 de D1 (4 ciclos)
        BNE.S   .DELAY00    ; Pula se não for zero (10 ciclos se pular, 8 se não)

mainLoop:
        JSR     PicoClearScreen
        LEA     MsgMSGINIT,A0
        JSR     PicoPrintString

subLoop:        
        move.b  #$41,D0
        JSR     UartWriteChar
        move.b  #$42,D0
        JSR     PicoWriteChar

        MOVE.L  #$001FFFF,D1 ; Contador para o delay (ajuste se precisar de mais)
.DELAY01:
        SUBQ.L  #1,D1         ; Subtrai 1 de D1 (4 ciclos)
        BNE.S   .DELAY01     ; Pula se não for zero (10 ciclos se pular, 8 se não)

        JMP     subLoop



WriteChar:
        RTS
ReadChar:
        RTS
WriteString:
        RTS

        INCLUDE "drv_uart.asm"
        INCLUDE "drv_pico_vga.asm"
        INCLUDE "cmd_mem_dump.asm"

        ;ORG $00008000
;        INCLUDE "trap1.asm"

        INCLUDE "section_data.asm"
        INCLUDE "section_bss.asm"
