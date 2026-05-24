UartXReadChar:
        

Get0x81:
        JSR     UartXReadChar   ; GET TYPE
Get0x87:
        JSR     UartXReadChar   ; GET LENGHT
        MOVE.B  D0,D2
LoopGet0x87:            
        JSR     UartXReadChar        
        DBRA    D2,LoopGet0x87
        RTS
        
Get0x88:
        JSR     UartXReadChar   ; GET SIZE IN D0
        LEA     buf_key_A,A1    ; BUFFER A
        LEA     buf_key_B,A2    ; BUFFER B
        MOVE.B  D0,D2           ; D2 HAS SIZE
        MOVE.B  newPacket,D1    ; D1 HAS PACKET NUMBER A OR B
        CLR.L   D3              ; BUFFER INDEX
        CMP.B   D1,#$0          ; IF D1 == 0 READ BUFFER A
        JNE     ReadBufferB
LoopGet0x88:
        ;READ BUFFER A
        JSR     UartXReadChar        
        MOVE.B  D0,(A1, D3.B)      
        ADDQ.W  #1,D3
        DBRA    D2,LoopGet0x88
        
ReadBufferB:        
        ;READ BUFFER B
        JSR     UartXReadChar        
        MOVE.B  D0,(A2, D3.B)      
        ADDQ.W  #1,D3
        DBRA    D2,LoopGet0x88