
;Como interpretar o Byte 0 (Modificadores)
;Como o Shift ou Ctrl não geram uma tecla normal, 
;eles ligam bits específicos no primeiro byte:
;
;Bit 0: Control Esquerdo (Left Ctrl)
;
;Bit 1: Shift Esquerdo (Left Shift)
;
;Bit 2: Alt Esquerdo (Left Alt)
;
;Bit 3: GUI Esquerda (Tecla Windows/Command)
;
;Bit 4: Control Direito (Right Ctrl)
;
;Bit 5: Shift Direito (Right Shift)
;
;Se o usuário pressionar Shift + A, o Byte 0 será 
;0x02 (bit 1 ligado) e o Byte 2 será 0x04 (código 
;da letra A).
;Exemplos de USB Scan Codes (Byte 2)
;Diferente do PS/2 onde a tecla 'A' costuma ser 0x1C, 
;no padrão USB HID os códigos começam do zero de 
;forma mais sequencial:
;0x00: Nenhuma tecla pressionada
;0x04 a 0x1D: Letras A a Z (em ordem: A=0x04, B=0x05, C=0x06...)
;0x1E a 0x27: Números 1 a 0 (1=0x1E, 2=0x1F... 9=0x26, 0=0x27)
;0x28: Enter
;0x29: ESC
;0x2A: Backspace
;0x2B: Tab
;0x2C: Espaço
;0x4F: Seta Direita
;0x50: Seta Esquerda

; =============================================================================
; DRIVER DE LEITURA CH9350 PARA MOTOROLA 68000
; Assembler: vasm68kmot (Sintaxe Motorola)
; =============================================================================

; --- Constantes do Protocolo CH9350 ---
CH_HEAD1    EQU     $57             ; Primeiro byte de sincronismo
CH_HEAD2    EQU     $AB             ; Segundo byte de sincronismo

; -----------------------------------------------------------------------------
; Rotina Principal de Captura de Pacote do CH9350
; Retorno: 
;   D0.B = Número de bytes úteis de dados lidos (0 se erro/timeout)
;   A0   = Ponteiro para o buffer contendo os dados puros do relatório HID
; -----------------------------------------------------------------------------
ReadPacketKbd:
            movem.l d1-d3/a1,-(sp)    ; Preserva registradores

.WaitHead1:
            bsr     UartbReadChar     ; Aguarda e lê um byte
            cmp.b   #CH_HEAD1,d0      ; É o primeiro byte do cabeçalho?
            bne     .WaitHead1        ; Se não, continua procurando

.CheckHead2:
            bsr     UartbReadChar     ; Lê o próximo byte
            cmp.b   #CH_HEAD2,d0      ; É o segundo byte de sincronismo?
            bne     .WaitHead1        ; Se falhar, reinicia a busca do cabeçalho

            ; Cabeçalho alinhado! Passando para os bytes de controle
            bsr     UartbReadChar     ; Lê Byte 2 (Address/Type)
            
            bsr     UartbReadChar     ; Lê Byte 3 (Command)
            move.b  d0,d2             ; Guarda o comando em D2 para validação posterior

            bsr     UartbReadChar     ; Lê Byte 4 (Length - Tamanho dos dados)
            move.b  d0,d1             ; D1 = Contador de bytes do payload HID
            move.b  d0,d3             ; Salva tamanho original para retorno em D0
            beq     .PacketDone       ; Se tamanho for 0, encerra

            ; Prepara ponteiro para receber os dados úteis
            lea     chPayloadBuffer,a1

.ReadPayload:
            bsr     UartbReadChar     ; Lê byte de dados do teclado/mouse
            move.b  d0,(a1)+          ; Salva no buffer
            dbra    d1,.ReadPayload   ; Decrementa e repete até ler todo o pacote

.PacketDone:
            move.b  d3,d0             ; Retorna quantidade de bytes lidos em D0
            lea     chPayloadBuffer,a0 ; Retorna o endereço do buffer em A0
            
            movem.l (sp)+,d1-d3/a1    ; Restaura registradores
            rts

; -----------------------------------------------------------------------------
; Rotina: DecodeToAscii
; Entrada:
;   D0.B = Scan Code puro do USB HID (obtido do Byte 2 do pacote)
;   D1.B = Byte de Modificadores (obtido do Byte 0 do pacote)
; Saída:
;   D0.B = Caractere ASCII correspondente, ou $00 se não for mapeável/solto
; -----------------------------------------------------------------------------
DecodeToAscii:
            move.l  a0,-(sp)          ; Preserva A0

            ; 1. Verifica se há tecla pressionada
            tst.b   d0                ; Código $00 significa nenhuma tecla
            beq     .NoChar

            ; 2. Verifica se o código está dentro do limite da tabela principal
            cmp.b   #SCAN_START,d0
            blo     .CheckSpecial     ; Menor que $04? Pode ser tecla especial fora da tabela
            cmp.b   #SCAN_END,d0
            bhi     .CheckSpecial     ; Maior que $38? Trata separadamente

            ; 3. Calcula o deslocamento (Índice = ScanCode - SCAN_START)
            sub.b   #SCAN_START,d0
            and.l   #$000000FF,d0     ; Garante extensão limpa para Word/Long

            ; 4. Determina qual tabela usar baseado no Shift (Bits 1 e 5 do Modificador)
            btst    #1,d1             ; Verifica Left Shift
            bne     .UseShiftTable
            btst    #5,d1             ; Verifica Right Shift
            bne     .UseShiftTable

.UseNormalTable:
            lea     ASCII_Table_Normal,a0
            move.b  (a0,d0.l),d0      ; Carrega o caractere correspondente
            bra     .Done

.UseShiftTable:
            lea     ASCII_Table_Shift,a0
            move.b  (a0,d0.l),d0      ; Carrega o caractere com Shift aplicado
            bra     .Done
; -----------------------------------------------------------------------------
; Tratamento de Teclas fora da faixa contínua alfabética/numérica
; -----------------------------------------------------------------------------
.CheckSpecial:
            ; O Keypad numérico e teclas como CapsLock e F1-F12 ficam aqui.
            ; Exemplo rápido: Mapear teclas do teclado numérico comum ou setas
            cmp.b   #$4F,d0           ; Scan code $4F = Seta Direita
            beq     .IsArrowRight
            cmp.b   #$50,d0           ; Scan code $50 = Seta Esquerda
            beq     .IsArrowLeft
            
            ; Se não mapeado, retorna nulo
            bra     .NoChar

.IsArrowRight:
            move.b  #$06,d0           ; Retorna código ASCII de controle ACK ou customizado
            bra     .Done
.IsArrowLeft:
            move.b  #$15,d0           ; Retorna código customizado
            bra     .Done

.NoChar:
            moveq   #0,d0             ; Retorna $00 se não for uma tecla válida de texto
.Done:
            move.l  (sp)+,a0          ; Restaura A0
            rts

; -----------------------------------------------------------------------------
; Rotina: DumpPayloadHex
; Objetivo: Lê sequencialmente os 16 bytes de CH_PayloadBuffer e os imprime
;           em formato hexadecimal usando a rotina externa PicoPrintByteHex.
; Passagem: PicoPrintByteHex espera o dado bruto no byte menos significativo de D0.
; -----------------------------------------------------------------------------
DumpPayloadHex:
            movem.l d0-d1/a0,-(sp)    ; Preserva registradores que serão usados

            lea     chPayloadBuffer,a0 ; Aponta A0 para o início do buffer de 16 bytes
            moveq   #16-1,d1          ; D1 = 15 (Contador para a instrução dbra, que para em -1)

.LoopDump:
            moveq   #0,d0             ; Limpa D0 por segurança
            move.b  (a0)+,d0          ; Copia o byte atual para D0 e avança o ponteiro A0

            ; -----------------------------------------------------------------
            ; Aqui chamamos a sua rotina externa. 
            ; Ela deve ler o byte de D0 e jogá-lo para a tela/serial.
            ; -----------------------------------------------------------------
            bsr     PicoPrintByteHex  

            dbra    d1,.LoopDump      ; Decrementa D1. Se não for -1, pula para .LoopDump

            movem.l (sp)+,d0-d1/a0    ; Restaura os registradores originais
            JSR     ReadConin
            rts            