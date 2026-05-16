



;Uart register offsets
UART_BASE   equ         $FF9100
RHR         equ         $1   ; receive holding register (read)
THR         equ         $1   ; transmit holding register (write)
IER         equ         $3   ; interrupt enable register
ISR         equ         $5   ; interrupt status register (read)
FCR         equ         $5   ; FIFO control register (write)
LCR         equ         $7   ; line control register
MCR         equ         $9   ; modem control register
LSR         equ         $B   ; line status register
MSR         equ         $D   ; modem status register
SPR         equ         $F   ; scratchpad register (reserved for system use)
DLL         equ         $1   ; divisor latch LSB
DLM         equ         $3   ; divisor latch MSB
; aliases for register names (used by different manufacturers)cd ..
RBR         equ         RHR ; receive buffer register
IIR         equ         ISR ; interrupt identification register
SCR         equ         SPR ; scratch register

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
; Parametros: D0 = baudrate number to write in DLM e DLL.
; ----------------------------------------------------------------------
UartInit:
        move.l  currentUart,A1
        move.b  #%00001101,FCR(A1)      ; enable FIFO
        move.b  #%10000011,LCR(A1)      ; 8 data bits, no parity, 1 stop bit, DLAB=1
        move.b  D0,DLL(A1)              ; Byte 0 (LSB) -> DLL
        lsr.l   #8,D0                   ; Shift right 8 bits
        move.b  D0,DLM(A1)              ; Byte 1 (agora no LSB) -> DLM
        bclr.b  #7,LCR(A1)              ; disable divisor latch
        clr.b   SCR(A1)                 ; clear the scratch register
        RTS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Write char
; Change A1
; Receive char in D0
UartWriteChar:
        lea     currentUart,A1
.WaitTx:
        btst    #5,LSR(A1)      ; wait until transmit holding register is empty
        beq     .WaitTx
        move.b  D0,THR(A1)      ; transmit byte
        RTS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Write char
; Change A1
;
; Return char in D0)
UartReadChar:
        lea     currentUart,A1
.WaitRx:
        btst    #0,LSR(A1)        ; RX ready?
        beq     .WaitRx
        move.b  RHR(A1),D0
        RTS
