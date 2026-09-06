#ifndef MC68000_H
#define MC68000_H

// Definições de hardware
#define UART_BASE 0x10000000
#define TIMER_BASE 0x10000010
#define RAM_BASE 0x00080000
#define ROM_BASE 0x00000000

// Registradores de hardware
typedef volatile struct {
    unsigned char data;
    unsigned char status;
} uart_regs_t;

#define UART ((uart_regs_t *)UART_BASE)

// Protótipos de funções de sistema
void system_init(void);
void delay(unsigned int ms);
void enable_interrupts(void);
void disable_interrupts(void);

#endif

