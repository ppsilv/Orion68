; ----------------------------------------------------------------------
; SECTION data
;-----------------------------------------------------------------------
    SECTION .rodata
    SECTION .data

; ----------------------------------------------------------------------
; Strings do Sistema
; ----------------------------------------------------------------------
MsgDumpHeader:
    DC.B    13,10," - Memory Dump from :",0
MsgDumpHeader1:
    DC.B    13,10,"Address   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F  ASCII",13,10
    DC.B    "--------  -----------------------------------------------  ------------------",13,10,0
MsgOrionInit:
    DC.B    13,10,"PDS317 - copyright (C) pdsilva(pgordao).VBug2.2",13,10
    DC.B    "MC68000 System Monitor with ch9350",13,10
    INCLUDE "tools/build_date.inc"
    INCLUDE "tools/build_counter.inc"
    DC.B          "-----------------------------------------------",13,10,13,10,0
MsgMenuText:
    DC.B    "Menu choose an option: ",13,10,13,10
    DC.B    "0. Clear screen",13,10
    DC.B    "1. Verificar o systemtick\n",13,10
    DC.B    "3. Run xmodem",13,10
    DC.B    "5. Run Program",13,10
    DC.B    "7. Memory dump from address buffer",13,10
    DC.B    "8. Disable int 2",13,10
    DC.B    "9. Enable int 2",13,10
    DC.B    "> ",0
MsgOutOfRange:
    DC.B    13,10,"Invalid address range!",13,10,0
MsgLoadDoneMsg:
    DC.B    13,10,"Program loaded successfully!",13,10,0
MsgWritePrompt:
    DC.B    13,10,"Write Address: ",0
MsgWriteDoneMsg:
    DC.B    13,10,"Data written to memory!",13,10,0
MsgRunPrompt:
    DC.B    13,10,"Running program...",13,10,0
MsgNoProgramToRUN:
    DC.B    "No program loaded to run",13,10,0
MsgPromptNotImplemented:
    DC.B    "Not Implemented!",13,10,0
MsgBufferEmpty:
    DC.B    "Ooops Buffer Empty!",13,10,0
MsgTestHexInput:
    DC.B    13,10,"Digite um endereco de no maximo 4bytes 8 caracteres!",13,10
    DC.B    "mais que isso serao descatados os excedentes...",13,10,0
MsgHitAnyKey:
    DC.B    13,10,"Hit any <ENTER> to continue <ESC> to terminate: ",0
MsgXmodemInit:
    DC.B    "XMODEM Receiver Initialized",13,10,0
MsgXmodemWaitingSoh:
    DC.B    "Waiting for SOH (Start of Header)...",13,10,0
MsgCs:
    DC.B    "Checksum ROM.: ",0
MsgCscalc:
    DC.B    " - Checksum CAL.: ",0
MsgDefaultHandler:
    DC.B    " Default handler wrote UART ",13,10,0
MsgSystemtick:
    DC.B    13,10,"Systemtick: ",0
MsgNewLine:
    DC.B    13,10,0
MsgA0:
    DC.B    13,10,"A0: ",0
MsgA1:
    DC.B    " - A1: ",0
MsgA2:
    DC.B    " - A2: ",0
MsgA3:
    DC.B    " - A3: ",0
MsgA4:
    DC.B    13,10,"A4: ",0
MsgA5:
    DC.B    " - A5: ",0
MsgA6:
    DC.B    " - A6: ",0
MsgA7:
    DC.B    " - A7: ",0
MsgD0:
    DC.B    13,10,"D0: ",0
MsgD1:
    DC.B    " - D1: ",0
MsgD2:
    DC.B    " - D2: ",0
MsgD3:
    DC.B    " - D3: ",0
MsgD4:
    DC.B    13,10,"D4: ",0
MsgD5:
    DC.B    " - D5: ",0
MsgD6:
    DC.B    " - D6: ",0
MsgD7:
    DC.B    " - D7: ",0
MsgAntes:
    DC.B    "ANTES:",13,10,0
MsgDepois:
    DC.B    "DEPOIS:",13,10,0


MsgDebug:     DC.B "Exception - PC: ",0
MsgSr:        DC.B "Status Reg: ",0
;MsgSp:        DC.B "Stack Reg: ",0
;MsgSpu:       DC.B "StackU Reg: ",0
MsgVector:    DC.B "Vector offset: ",0
MsgVectorNum: DC.B "Vector number: ",0
MsgBusError:  DC.B "Bus Error!",0
MsgAddrError: DC.B "Address Error!",0
MsgIllegal:   DC.B "Illegal Instruction!",0
MsgTrap:      DC.B "TRAP Instruction!",0
MsgUnknown:   DC.B "Unknown Exception!",0

PROGRAM_LOADED  equ     0
ECHO_ON         equ     1 
;    ALIGN   2
;    ;Isso preenche 762 com 00
;    ;DS.B    $00000762 - *, $00
;    DC.B    "ROMv4.1",0   ; String de identificação
;    ;DS.B    $00002968 - *, $00
;    ORG     $3FFC
;checksum_rom:
;    DC.L    0     ; Valor calculado
