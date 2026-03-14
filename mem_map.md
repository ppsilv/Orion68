# Mapa de memória inicial (recomendado)

# O Motorola 68000 possui 24 bits de endereço → 16 MB de espaço.

Orion68 Memory Map (Initial)

000000 – 00FFFF   ROM (Boot / Monitor)           64 KB

010000 – 0FFFFF   Reserved / Future expansion

100000 – 1FFFFF   SRAM                           1 MB

200000 – EFFFFF   Expansion space
                  (future peripherals / memory boards)

F00000 – FFFFFF   I/O space
                  Serial ports
                  Video interface
                  Future peripherals

# I/O mapeado                  

F00000 – F000FF   UART0
F00100 – F001FF   UART1
F00200 – F002FF   UART2
F00300 – F003FF   UART3

F10000 – F1FFFF   Video interface (RPi Pico)

F20000 – FFFFFF   Future devices

