// xmodem_asm.S
// Sintaxe 100% compatível com o GNU Assembler (m68k-elf-gas)

.section .text
.global xmodem_receive

.extern writec_char
.extern uart0_read_timeout


UART_WriteChar:
        move.l  #1,%D1
        trap    #14
        rts
UART_ReadChar:
        move.l  #2,%D1
        trap    #14
        rts
UART_ReadCharTimeout:
        move.l  #3,%D1
        trap    #14
        jsr     debug_char
        cmp.b   #0x00,%d0            // Timeout termina recepção 
        beq     Transfer_INComplete   
        rts

debug_char:
        move.l  %d0,-(%sp) 
        jsr writec_char
        addq.l  #4,%sp 
        rts

xmodem_receive:
        // 1. MOVEM correto para empilhar (Sem parênteses na lista)
        movem.l %d2-%d7/%a0-%a6,-(%sp)
        
        // 2. LEA com destino explícito usando %pc para labels locais
        lea     expected_block,%a0
        move.b  #1,%d0
        move.b  %d0,(%a0)             
        lea     xmodem_buffer,%a0

        lea     flag_pgm_loaded,%a0   
        move.b  #0,%d0
        move.b  %d0,(%a0)              

        // ---- 1. INICIALIZAÇÃO ----
        move.b  #0x15,%d0            
        jsr     UART_WriteChar      

        // LEA de endereço absoluto longo direto
        lea     0x00082000,%a2       

// ---- 2. LOOP PRINCIPAL ----
Receive_Loop:
        jsr     UART_ReadCharTimeout

        cmp.b   #0x04,%d0            // EOT = 0x04
        beq     Transfer_Complete   

        cmp.b   #0x01,%d0            // SOH = 0x01
        bne     Receive_Loop        

        // ---- 3. RECEBE HEADER ----
        jsr     UART_ReadCharTimeout       
        lea     block_number,%a0
        move.b  %d0,(%a0)

        jsr     UART_ReadChar       

        // ---- 4. RECEBE DADOS ----
        move.l  #32,%d1              // 128/4 = 32 longs
        lea     xmodem_buffer,%a1
Read_Loop:
        jsr     UART_ReadChar    
        lsl.l   #8,%d0
        jsr     UART_ReadChar    
        lsl.l   #8,%d0
        jsr     UART_ReadChar    
        lsl.l   #8,%d0
        jsr     UART_ReadChar    
        move.l  %d0,(%a1)+                
        subq.l  #1,%d1
        bne     Read_Loop

        // ---- 5. VERIFICA CHECKSUM ----
        jsr     UART_ReadChar       
        move.b  %d0,%d2

        lea     xmodem_buffer,%a1
        move.w  #127,%d1
        clr.l   %d3

Calc_Checksum:
        add.b   (%a1)+,%d3
        dbf     %d1,Calc_Checksum

        cmp.b   %d2,%d3
        bne     Send_NAK            

        // ---- 7. COPIA DADOS VÁLIDOS ----
        lea     xmodem_buffer,%a1     
        move.l  #32,%d1               
Copy_Data:
        move.l  (%a1)+,(%a2)+          
        subq.l  #1,%d1                
        bne     Copy_Data            

        // ---- 8. CONFIRMA RECEPÇÃO ----
        lea     expected_block,%a0
        addq.b  #1,(%a0)   
        move.b  #0x06,%d0            // ACK = 0x06
        jsr     UART_WriteChar
        bra     Receive_Loop

Send_ACK:
        move.b  #0x06,%d0            
        jsr     UART_WriteChar
        bra     Receive_Loop

Send_NAK:
        move.b  #0x15,%d0            
        jsr     UART_WriteChar
        bra     Receive_Loop

Transfer_Complete:
        move.b  #0x06,%d0            
        jsr     UART_WriteChar

        lea     flag_pgm_loaded,%a0   
        move.b  #1,%d0
        move.b  %d0,(%a0)              

Transfer_INComplete:
        // MOVEM correto para desempilhar
        movem.l (%sp)+,%d2-%d7/%a0-%a6
        rts

// ---- Seção BSS ----
.section .bss
.align 2
expected_block:   .skip 1
block_number:     .skip 1
flag_pgm_loaded:  .skip 1
.align 4
xmodem_buffer:    .skip 128