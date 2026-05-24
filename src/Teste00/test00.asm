            SECTION .vectors
            ORG     $00000000

            ; Vetores de Reset
            DC.L $00020000    ; Initial Stack Pointer (Topo da sua RAM)
            DC.L START        ; Initial Program Counter (Onde o código começa)

            SECTION .jumptable
            ORG     $0400

            SECTION .text
            ORG $00001000

START:
    ; 1. Mata o BOOT (Move a ROM para E00000)
    ; Após este comando, a ROM some do endereço 0x00 e aparece em 0xE0
    TST.B $F00000

    ; 2. Salto para o endereço "real" da ROM
    ; Precisamos de um jump absoluto para cair no novo endereço da ROM
    JMP $E00020       ; Endereço aproximado após o salto (ajuste conforme seu código)

    ORG $E00020
AT_HIGH_ROM:
    ; 3. Se chegamos aqui, a ROM está mapeada em E00000 corretamente!
    ; Pulsamos o pino 12 para comemorar no osciloscópio
    TST.B $F10000

    BRA AT_HIGH_ROM   ; Fica pulsando o pino 12 em loop




START2:
    ; O 68k começa aqui logo após o reset
LOOP:
    ; Acessa o endereço que ativa o KILLBT na GAL (F00000)
    ; Usamos TST.B para apenas ler o endereço sem alterar registradores
    TST.B $F00000

    ; Delay para o LED não piscar rápido demais
    MOVE.L #$40000,D1
DELAY:
    SUBQ.L #1,D1
    BNE DELAY

    BRA LOOP
