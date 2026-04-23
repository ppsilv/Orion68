# Mapa de memória inicial (recomendado)

# O Motorola 68000 possui 24 bits de endereço → 16 MB de espaço.

Orion68 Memory Map (Initial)

000000 – 0FFFFF   SRAM 1 MB & ROM (Boot / Monitor) ROM de 128K selecionada somente no boot.
100000 – 1FFFFF   SRAM 1 MB
200000 – DFFFFF   Expansion space (memory boards)

A23 A22 A21 A20   = 0xE = ROM_CS

E00000 - EFFFFF   Espelhamento da ROM 128K 1MB de espaço

A23 A22 A21 A20   = 0xF = IO_CS 1MB de espaço

F00000 – FFFFFF   I/O space
                  Serial ports
                  Video interface
                  Future peripherals



# I/O mapeado                  

F00000 – Bottin address rom acess this address to change its address from 0x00000 to 0xE00000
F00001 – F000FF   Free
F00100 – F000FF   UART0
F00200 – F001FF   UART1
F00300 – F002FF   UART2
F00400 – F003FF   UART3

F10000 – F1FFFF   Video interface (RPi Pico)

F20000 – FFFFFF   Future devices

