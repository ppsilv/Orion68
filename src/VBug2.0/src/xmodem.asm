
; Masks
RX_RDY     EQU $01           ; Receive Data Ready mask

; XMODEM Control Characters
SOH        EQU $01           ; Start of Header
EOT        EQU $04           ; End of Transmission
ACK        EQU $06           ; Acknowledge
NAK        EQU $15           ; Negative Acknowledge
CAN        EQU $18           ; Cancel


InitXMODEM:
    MOVE.B  #NAK, d0         ; Start by requesting first packet
    BSR     SendByte

ReceiveLoop:
    BSR     WaitByte         ; Wait for SOH or EOT
    CMP.B   #EOT, d0         ; Check for End of Transmission
    BEQ     FileComplete

    CMP.B   #SOH, d0         ; Valid data block?
    BNE     SendNAK          ; If garbage, ask again

    ; --- Read Block Number & Inverse ---
    BSR     WaitByte
    MOVE.B  d0, d1           ; Save block number
    BSR     WaitByte
    NOT.B   d0               ; Verify inverse block number
    CMP.B   d0, d1
    BNE     SendNAK

    ; --- Read 128 Data Bytes ---
    LEA     Buffer, a0       ; Point to RAM buffer
    MOVE.W  #127, d2         ; Loop 128 times (0 to 127)
ReadData:
    BSR     WaitByte
    MOVE.B  d0, (a0)+
    DBRA    d2, ReadData

    ; --- Read 2-Byte Checksum/CRC ---
    BSR     WaitByte         ; High byte
    MOVE.B  d0, d3
    BSR     WaitByte         ; Low byte
    MOVE.B  d0, d4

    ; --- Verify CRC and Send ACK/NAK ---
    BSR     VerifyCRC
    TST.B   d0               ; Zero means CRC match
    BNE     SendNAK

    ; (Implement memory writing logic here)
    MOVE.B  #ACK, d0         ; Everything is good, ACK
    BSR     SendByte
    BRA     ReceiveLoop

SendNAK:
    MOVE.B  #NAK, d0
    BSR     SendByte
    BRA     ReceiveLoop

FileComplete:
    MOVE.B  #ACK, d0         ; Acknowledge EOT
    BSR     SendByte
    RTS
