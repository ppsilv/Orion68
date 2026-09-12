|===========================================================================
| print_hex_d0.S - imprime o conteudo de D0 em hexadecimal (8 digitos) no
| video (PICO_VGA_BASE), sem alterar D0 nem nenhum outro registrador do
| chamador. Pensado pra debug: da pra chamar de qualquer lugar do .S com
| um simples "bsr print_hex_d0" logo depois de por o valor suspeito em D0.
|
| Uso tipico dentro do seu orion68_startup.S:
|
|     MOVE.L  %A0,%D0        | quero ver o que tem em A0 nesse ponto
|     bsr     print_hex_d0
|
| Compilar junto com o resto (arquivo .S entra na lista ASMS do Makefile,
| mesma regra "%.o: %.S" que voce ja usa pros outros).
|===========================================================================

    .equ    PICO_VGA_BASE, 0x00FF8000
    .equ    WRITE_SCREEN,  PICO_VGA_BASE + 0x01

    .section .text
    .global print_hex_d0

|---------------------------------------------------------------------------
| print_hex_d0 - imprime D0 em hex (8 digitos) + CRLF
| Entrada:  D0 = valor a imprimir (32 bits)
| Destroi:  nada visivel ao chamador (D1-D4 salvos/restaurados na pilha;
|           D0 nunca e escrito, so lido)
| Chamada:  bsr print_hex_d0   (ou jsr, se preferir)
|---------------------------------------------------------------------------
print_hex_d0:
    movem.l %d1-%d4,-(%sp)     | salva d1..d4 (usamos so esses como scratch)

    move.b  #'D',WRITE_SCREEN
    move.b  #'0',WRITE_SCREEN
    move.b  #'=',WRITE_SCREEN
    move.b  #'0',WRITE_SCREEN
    move.b  #'x',WRITE_SCREEN

    move.l  %d0,%d3             | d3 = copia de trabalho (nao mexe no d0 real)
    moveq   #7,%d4              | d4 = indice do nibble atual: 7 (MSB) ate 0 (LSB)

.print_hex_d0_loop:
    move.l  %d3,%d1
    move.l  %d4,%d2
    lsl.b   #2,%d2              | d2 = indice * 4 = quantos bits deslocar
    lsr.l   %d2,%d1             | traz o nibble desejado pros 4 bits baixos
    andi.l  #0x0F,%d1

    cmpi.b  #9,%d1
    bhi     .print_hex_d0_alpha
    addi.b  #'0',%d1
    bra     .print_hex_d0_out
.print_hex_d0_alpha:
    addi.b  #('A'-10),%d1
.print_hex_d0_out:
    move.b  %d1,WRITE_SCREEN

    subq.l  #1,%d4
    bpl     .print_hex_d0_loop  | continua enquanto d4 >= 0

    move.b  #13,WRITE_SCREEN
    move.b  #10,WRITE_SCREEN

    movem.l (%sp)+,%d1-%d4
    rts