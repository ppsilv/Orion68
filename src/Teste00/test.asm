            SECTION .vectors
            ORG     $00000000

            ; Vetores de Reset (Iniciais enquanto BOOT=1)
            DC.L $00020000    ; Stack Pointer inicial
            DC.L START        ; Program Counter inicial

            SECTION .text
            ORG     $00001000

START:
    ; 1. Mata o BOOT (Ativa KILLBT na GAL)
    TST.B $F00000

    ; 2. Salto para o endereço real da ROM em 0xE00000
    ; Usamos JMP para garantir que o PC saia da faixa de $0000
    JMP   MAIN_ROM

    ORG   $E01000            ; Endereço dentro da ROM mapeada em High
MAIN_ROM:
    ; --- TESTE DE MEMÓRIA RAM ---
    ; Endereço de teste: 0x100000
    ; Valor de teste: 0x55AA (Padrão de bits alternados)

    MOVE.L #$100000,A0      ; Carrega endereço da RAM em A0
    MOVE.W #$55AA,D0        ; Carrega padrão de teste em D0

    MOVE.W D0,(A0)          ; ESCREVE na RAM

    ; Limpamos D1 para garantir que a comparação seja real
    CLR.W  D1
    MOVE.W (A0),D1          ; LÊ da RAM

    CMP.W  D0,D1            ; Compara Escrito vs Lido
    BNE    RAM_ERROR         ; Se for diferente, vai para erro

RAM_OK:
    ; Se deu OK, pulsa o pino 12 (F10000) e fica em loop eterno
    TST.B $F10000
    BRA   RAM_OK

RAM_ERROR:
    ; Se deu erro, executa uma instrução ILLEGAL ou gera um DOUBLE BUS FAULT
    ; para forçar a CPU ao estado de HALT (ou travar)
    ; Uma forma simples é resetar a pilha para um endereço ímpar e tentar acessar
    MOVE.L #$00000001,A7
    MOVE.W (A7),D7          ; Isso gera um Address Error -> Halt se não houver handler

    ; Caso a CPU não pare, instrução ilegal:
    DC.W   $4AFC             ; ILLEGAL instruction
    STOP   #$2700            ; Para a CPU
