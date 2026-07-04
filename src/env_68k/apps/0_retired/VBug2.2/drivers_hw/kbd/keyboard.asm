

NO_KEY     equ $00
VALID_KEY  equ $FE
KEY_CTRL   equ $01
KEY_SHIFT  equ $02
KEY_ALT    equ $04
KEY_ALTGR  equ $08
KEY_CAPS   equ $10


KbdGetCharAscii:
    ; Supondo que D0 tem o Scancode do teclado (0x00 a 0x7F)
    lea     _OE_BASE_KEYMAP,A0   ; Carrega o endereço base da tabela
    move.b  (A0,D0.w),D1         ; D1 agora tem o caractere ASCII correspondente!
    rts

InitKbd:
        rts     clear_buffer
        jmp     InitUartb        

ReadKbd:
        jmp     UartbReadChar

_get_0x81:
        jsr     ReadKbd
        jsr     ReadKbd
        move.b  d0,d2
        sub.b   #1,d2
.flush  jsr     ReadKbd      
        dbra    d2,.flush
        rts

_get_0x88:        
        jsr     ReadKbd
        move.b  d0,d2
        sub.b   #1,d2
        lea     key_buffer,a0      ; A0 aponta para o início do array
.read_all:
        jsr     ReadKbd           ; Chama sua rotina. O caractere volta em D0.b        
        move.b  d0,(a0,d2.w)        ; key_buffer[d2] = d0        
        dbra    d2,.read_all         ; Decrementa D2 (o 'i') e repete até ser -1        

        tst.b   key_buffer+2       ; If position 2 equal zero    if (key_buffer[2] == 0x0 && key_buffer[4] == 0x0) /*normal*/   
        bne     .has_char
        tst.b   key_buffer+4       ; If position 4 equal zero
        bne     .has_char
        ;Pos 2 and 4 are equal zero
        ;special_key_status is more than zero
        TEST_FLAG       SPECIAL_KEY_DOWN,flg_system
        beq             .keydown
        SET_FLAG        SPECIAL_KEY_UP,flg_system
        RESET_FLAG      SPECIAL_KEY_DOWN,flg_system
        clr.w           special_key_status
.keydown:
        TEST_FLAG       KEY_DOWN,flg_system
        beq     .fim_keydown
        RESET_FLAG      KEY_DOWN,flg_system
.fim_keydown        
        clr             D0
        rts   
.has_char:
        ; Has arrived a char, so lets verify if it is one of the special char
        SET_FLAG        SPECIAL_KEY_DOWN,flg_system
        RESET_FLAG      SPECIAL_KEY_UP,flg_system
        ;KEY_CTRL
        cmp.b   #$01,key_buffer+2     ; key_buffer[2] == 0x01?
        beq     .inside_kctrl           ; Se FOR igual, pula DIRETO para o corpo do IF!
        cmp.b   #$10,key_buffer+2     ; key_buffer[2] == 0x10?
        bne     .end_kctrl                 ; Se NÃO for igual, pula o IF (ambas falharam)
.inside_kctrl:
        move.b  #KEY_CTRL,special_key_status
        move.b  #KEY_CTRL,D0
        rts      
        ;KEY_SHIFT  
.end_kctrl:
        cmp.b   #$02,key_buffer+2     ; key_buffer[2] == 0x01?
        beq     .inside_kshift           ; Se FOR igual, pula DIRETO para o corpo do IF!
        cmp.b   #$20,key_buffer+2     ; key_buffer[2] == 0x10?
        bne     .end_kshift             ; Se NÃO for igual, pula o IF (ambas falharam)
.inside_kshift:
        move.b  #KEY_SHIFT,special_key_status
        move.b  #KEY_SHIFT,D0
        rts
.end_kshift:
        ;KEY_ALT
        cmp.b   #$04,key_buffer+2     ; key_buffer[2] == 0x01?
        bne     .end_kalt           ; Se FOR igual, pula DIRETO para o corpo do IF!
        move.b  #KEY_ALT,special_key_status
        move.b  #KEY_ALT,D0
        rts
.end_kalt:
        ;KEY_ALTGR
        cmp.b   #$04,key_buffer+2     ; key_buffer[2] == 0x01?
        bne     .end_kaltgr           ; Se FOR igual, pula DIRETO para o corpo do IF!
        move.b  #KEY_ALTGR,special_key_status
        move.b  #KEY_ALTGR,D0
        rts
.end_kaltgr:
        ;KEY_CAPSLOCK
        cmp.b   #$39,key_buffer+4     ; key_buffer[2] == 0x01?
        bne     .end_kcapslock        ; Se FOR igual, pula DIRETO para o corpo do IF!
        cmp.b   #$0,mod_caps          ; key_buffer[2] == 0x01?
        bne     .end_modcaps
        move.b  #$02,mod_caps
        move.b  mod_caps,D0  
        jsr     send_cmd_keyboard    
        bra.s   .ret_capslock    
.end_modcaps:
        move.b  #$0,mod_caps
        move.b  mod_caps,D0  
        jsr     send_cmd_keyboard        
.ret_capslock:
        SET_FLAG        SPECIAL_KEY_DOWN,flg_system
        move.b          #KEY_CAPS,D0
        rts
.end_kcapslock:
        SET_FLAG        KEY_DOWN,flg_system
        move.b          #VALID_KEY,D0        
        rts

get_packet:
        jsr     ReadKbd
        cmp     #$57,D0
        bne     get_packet
        jsr     ReadKbd
        cmp     #$AB,D0
        bne     get_packet
        jsr     ReadKbd
        cmp     #$81,D0
        bne     .cmp_0x82
        jsr     _get_0x81
        bra.s   get_packet
.cmp_0x82:
        cmp     #$82,D0
        bne     .cmp_0x85
        jsr     ReadKbd
        bra.s   get_packet
.cmp_0x85:
        cmp     #$85,D0
        bne     .cmp_0x87
        jsr     ReadKbd
        bra.s   get_packet
.cmp_0x87:
        cmp     #$87,D0
        bne     .cmp_0x88
        bra.s   get_packet
.cmp_0x88:
        cmp     #$88,D0
        bne     .fim_get_packet
        jsr     _get_0x88
        rts
.fim_get_packet:        
        move.b  #NO_KEY,D0        
        rts

semd_cmd_shutup:
        ; Carrega buf[0], buf[1], buf[2] e buf[3] de uma vez só (32 bits)
        move.l  #$57AB1200,buf        
        ; Carrega buf[4], buf[5], buf[6] e buf[7] de uma vez só (32 bits)
        move.l  #$000000FF,buf+4
        ; Carrega buf[8] e buf[9] de uma vez só (16 bits)
        move.w  #$8000,buf+8        

        clr.l   D0                  ; D0 será o nosso acumulador 'ck' (limpa sujeira)
        moveq   #7,D1              ; D1 será o contador. O loop roda de 2 a 9 (8 iterações). ; Como o DBRA para em -1, contamos de 7 até 0.                                    
;;        lea     buf+2,A0          ; A0 aponta direto para o início da festa: buf[2]
;;.loop_checksum:
;;        add.b   (A0)+,D0           ; ck += *buf++. Soma o byte na RAM e já avança o ponteiro para o próximo!
;;        dbra    D1,.loop_checksum  ; Decrementa D1. Se D1 não for -1, pula para o topo.
;;        ; --- FIM DO LOOP ---
;;        move.b  D0,chksum             ; Despeja o resultado final na sua variável 'ck' na RAM
        move.b  #$20,chksum
        move.b  chksum,buf+10
        moveq   #7,D1              ; D1 será o contador.
        lea     buf,A0
.loop_envia:
        move.b  (A0)+,D0
        jsr     UartbWriteChar
        dbra    D1,.loop_envia
        rts        

send_cmd_keyboard:
        ; Carrega buf[0], buf[1], buf[2] e buf[3] de uma vez só (32 bits)
        move.l  #$57AB1200,buf        
        ; Carrega buf[4], buf[5], buf[6] e buf[7] de uma vez só (32 bits)
        move.l  #$000000FF,buf+4
        ; Carrega buf[8] e buf[9] de uma vez só (16 bits)
        move.w  #$8000,buf+8        

        move.b  D0,buf+7

        clr.l   D2                  ; D0 será o nosso acumulador 'ck' (limpa sujeira)
        moveq   #7,D1              ; D1 será o contador. O loop roda de 2 a 9 (8 iterações). ; Como o DBRA para em -1, contamos de 7 até 0.                                    
        lea     buf+2,A0          ; A0 aponta direto para o início da festa: buf[2]
.loop_checksum:
        add.b   (A0)+,D2           ; ck += *buf++. Soma o byte na RAM e já avança o ponteiro para o próximo!
        dbra    D1,.loop_checksum  ; Decrementa D1. Se D1 não for -1, pula para o topo.
        ; --- FIM DO LOOP ---
        move.b  D2,chksum             ; Despeja o resultado final na sua variável 'ck' na RAM
        move.b  chksum,buf+10
        moveq   #7,D1              ; D1 será o contador.
        lea     buf,A0
.loop_envia:
        move.b  (A0)+,D0
        jsr     UartbWriteChar
        dbra    D1,.loop_envia
        rts        

;key in D0        
get_kbd_key:
        ; --- IF PRINCIPAL: if (special_key_status == 0x00 && mod_caps == 0x00) ---
        tst.b   special_key_status     ; É zero?
        bne     .fim_get_kbd_key                ; Curto-circuito: se não for, pula fora
        tst.b   mod_caps               ; É zero?
        bne     .else_if_key_shift     ; Curto-circuito: se não for, pula fora
        lea     _OE_BASE_KEYMAP,A0     ; A0 aponta para o início da tabela (Endereço Base)     
        bra     .busca_caractere   

.else_if_key_shift:
        ; --- IF PRINCIPAL: if (special_key_status & KEY_SHIFT) ---
        move.b  special_key_status,D2
        and.b   #KEY_SHIFT,D2          ; Isola o bit do SHIFT
        beq     .else_if_key_ctrl          ; Se der zero, o Shift NÃO está apertado. Pula pro 'else' externo.
        ; --- DENTRO DO IF: O Shift está ativo! ---
        ; --- if (mod_caps == 2) ---
        cmp.b   #2,mod_caps           ; O Caps Lock está no estado 2?
        bne     .carrega_shift_map      ; Se NÃO for igual a 2, pula pro 'else' (usa tabela Shift)
        ; --- Caso mod_caps == 2: Usa a tabela BASE (reseta o efeito do Shift) ---
        lea     _OE_BASE_KEYMAP,A0
        bra     .busca_caractere        ; Pula para o bloco comum de busca
.carrega_shift_map:
        ; --- Caso contrário (else): Usa a tabela SHIFT ---
        lea     _OE_SHIFT_KEYMAP,A0
        bra     .busca_caractere   

.else_if_key_ctrl:
        move.b  special_key_status,D2
        and.b   #KEY_CTRL,D2          ; Isola o bit do SHIFT
        beq     .else_if_key_alt          ; Se der zero, o Shift NÃO está apertado. Pula pro 'else' externo.
        lea     _OE_CTRL_KEYMAP,A0
        bra     .busca_caractere   

.else_if_key_alt:
        move.b  special_key_status,D2
        and.b   #KEY_ALT,D2          ; Isola o bit do SHIFT
        beq     .else_if_key_altgr          ; Se der zero, o Shift NÃO está apertado. Pula pro 'else' externo.
        lea     _OE_ALT_KEYMAP,A0
        bra     .busca_caractere   

.else_if_key_altgr:
        move.b  special_key_status,D2
        and.b   #KEY_ALTGR,D2          ; Isola o bit do SHIFT
        beq     .else_if_mod_caps         ; Se der zero, o Shift NÃO está apertado. Pula pro 'else' externo.
        lea     _OE_ALTGR_KEYMAP,A0
        bra     .busca_caractere   
.else_if_mod_caps:
        tst.b   mod_caps              ; Testa o byte na RAM (mod_caps == 0?)
        beq     .fim_get_kbd_key       ; Se for IGUAL a zero (falso), pula o bloco IF        
        lea     _OE_SHIFT_KEYMAP,A0
.busca_caractere:
        ; --- Executa a indexação: RetKey = TABELA[code] ---
        move.b  (A0,D0.L),D1           ; Busca o caractere na tabela selecionada (A0 + D0)
        move.b  D1,RetKey             ; Salva em RetKey
        ; --- return RetKey; ---
        move.b  D1,D0                  ; Padrão do C: coloca o valor de retorno em D0
        rts                             ; Dá o fora da função (retorna)
.fim_get_kbd_key:
        clr.l   D0
        rts

get_char:
        jsr     get_packet
        tst.b   D0
        beq     get_char
        cmp.b   #KEY_CAPS,D0
        bne     .getkey
        move.b  mod_caps,D0
        jsr     send_cmd_keyboard
        bra.s   get_char
.getkey:        
        ; --- IF PARTE 1: if (key_flag != KEY_SHIFT) ---
        cmp.b   #KEY_SHIFT,D0      ; Compara com KEY_SHIFT
        beq     .fim_if             ; Se for IGUAL, quebra o "AND" e pula fora

        ; --- IF PARTE 2: && (key_flag < VALID_KEY) ---
        cmp.b   #VALID_KEY,D0      ; Compara key_flag com VALID_KEY
        bcc     .fim_if             ; BCC = Branch if Carry Clear (Maior ou Igual)
                                    ; Se for maior ou igual, condição falhou, pula fora        
        ; Mas o jeito mais limpo e rápido de jogar um byte na parte alta de uma Word é:
        lsl.w   #8,D0              ; Desloca 8 bits para a esquerda. D0 vira = $XX00
        move.w  d0,RetKey             ; Salva o resultado final na Word 'ch' na RAM
.fim_if:
; --- IF: if( key_buffer[4] > 0x0 ) ---
        tst.b   key_buffer+4       ; Testa o byte na posição 4 do buffer (compara com 0)
        ble     .get_char             ; Se for IGUAL a zero ou menor, a condição falhou. Pula fora!
        move.b key_buffer+4,D0
        jsr     get_kbd_key
        move.b  D0,D1
        clr.w   D0
        move.b  RetKey,D0
        or.w    D1,D0        
        rts
.get_char:
        move.b  RetKey,D0
        ori.w   #$FF00,D0
        rts




clear_buffer:
        lea     key_buffer,a0     ; A0 aponta para o início do seu buffer de 48 bytes        
        ; 48 bytes / 4 bytes por instrução = 12 linhas de clr.l
        clr.l   (a0)+               ; Bytes 0-3
        clr.l   (a0)+               ; Bytes 4-7
        clr.l   (a0)+               ; Bytes 8-11
        clr.l   (a0)+               ; Bytes 12-15
        clr.l   (a0)+               ; Bytes 16-19
        clr.l   (a0)+               ; Bytes 20-23
        clr.l   (a0)+               ; Bytes 24-27
        clr.l   (a0)+               ; Bytes 28-31
        clr.l   (a0)+               ; Bytes 32-35
        clr.l   (a0)+               ; Bytes 36-39
        clr.l   (a0)+               ; Bytes 40-43
        clr.l   (a0)+               ; Bytes 44-47                
        rts
        INCLUDE "drivers_hw/kbd/keyboard_map.asm"