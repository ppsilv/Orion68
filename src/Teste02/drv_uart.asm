



;Uart register offsets
UART_BASE   equ     $FF4000
RHR         equ     $01   ; receive holding register (read)
THR         equ     $01   ; transmit holding register (write)
IER         equ     $03   ; interrupt enable register
ISR         equ     $05   ; interrupt status register (read)
FCR         equ     $05   ; FIFO control register (write)
LCR         equ     $07   ; line control register
MCR         equ     $09   ; modem control register
LSR         equ     $0B   ; line status register
MSR         equ     $0D   ; modem status register
SPR         equ     $0F   ; scratchpad register (reserved for system use)
DLL         equ     $01   ; divisor latch LSB
DLM         equ     $03   ; divisor latch MSB
; aliases for register names (used by different manufacturers)cd ..
RBR         equ     RHR ; receive buffer register
IIR         equ     ISR ; interrupt identification register
SCR         equ     SPR ; scratch register

B009600     equ         $0060
B019200     equ         $0030
B038400     equ         $0018
B057600     equ         $0010
B115200     equ         $0008
B230400     equ         $0004
B460800     equ         $0002
B921600     equ         $0001


B009600L    equ         $60
B019200L    equ         $30
B038400L    equ         $18
B057600L    equ         $10
B115200L    equ         $08
B230400L    equ         $04
B460800L    equ         $02
B921600L    equ         $01
B009600H    equ         $00
B019200H    equ         $00     
B038400H    equ         $00     
B057600H    equ         $00     
B115200H    equ         $00     
B230400H    equ         $00
B460800H    equ         $00
B921600H    equ         $00


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Initialize uart
; Change A1
; Parametros: none
; ----------------------------------------------------------------------
InitUart:
        move.l  currentUart,A1
        move.l  #currentBaudRate,D0
        move.b  #%10000011,LCR(A1) ;DLAB=1
        NOP
        NOP      
        move.b  #$08,DLL(A1)       ; set divisor latch low byte
        move.b  #$00,DLM(A1)       ; set divisor latch high byte
        move.b  #%00000011,LCR(A1) ; 8 data bits, no parity, 1 stop bit, DLAB=0
        move.b  #%00001101,FCR(A1) ; enable FIFO
        clr.b   SCR(A1)            ; clear the scratch register
        RTS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Write char
; Change A1
; Receive char in D0
UartWriteChar:
        move.l  currentUart,A1
.WaitTx:
        btst    #5,LSR(A1)      ; wait until transmit holding register is empty
        beq     .WaitTx
        move.b  D0,THR(A1)      ; transmit byte
        RTS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Read char block
; Change A1
;
; Return char in D0)
UartReadChar:
        move.l  currentUart,A1
.WaitRx:
        btst    #0,LSR(A1)        ; RX ready?
        beq     .WaitRx
        move.b  RHR,D0
        RTS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Read char block with echo
; Change A1
;
; Return char in D0)
UartReadCharEcho:
        move.l  currentUart,A1
.WaitRx:
        btst    #0,LSR(A1)        ; RX ready?
        beq     .WaitRx
        move.b  RHR,D0
        JSR     UartWriteChar
        RTS
