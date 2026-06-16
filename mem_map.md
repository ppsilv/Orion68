# Mapa de memória inicial (recomendado)

# O Motorola 68000 possui 24 bits de endereço → 16 MB de espaço.

Orion68 Memory Map (Initial)

ROM
000000  -   07FFFF   512Kb de espaço para rom

RAM
080000  -   0FFFFF   512KW de espaço para ram - populado
100000  -   17FFFF   512KW de espaço para ram - NÃO populado
180000  -   17FFFF   512KW de espaço para ram - NÃO populado
180000  -   1FFFFF   512KW de espaço para ram - NÃO populado

200000  -   27FFFF   512KW de espaço para ram - NÃO populado
280000  -   2FFFFF   512KW de espaço para ram - NÃO populado
300000  -   37FFFF   512KW de espaço para ram - NÃO populado
380000  -   3FFFFF   512KW de espaço para ram - NÃO populado

400000  -   47FFFF   512KW de espaço para ram - NÃO populado
480000  -   4FFFFF   512KW de espaço para ram - NÃO populado
500000  -   57FFFF   512KW de espaço para ram - NÃO populado
580000  -   5FFFFF   512KW de espaço para ram - NÃO populado





# I/O mapeado          FF0xxx -> FFFxxx

F00000 – Bottin address rom acess this address to change its address from 0x00000 to 0xE00000
F00001 – F000FF   Free
F00100 – F000FF   UART0
F00200 – F001FF   UART1
F00300 – F002FF   UART2
F00400 – F003FF   UART3

F10000 – F1FFFF   Video interface (RPi Pico)

F20000 – FFFFFF   Future devices

23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1



