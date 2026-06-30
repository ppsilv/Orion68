#include "stdio.h"
#include "io.h"
#include "keyboard.h"
#include "interrupt.h"

extern volatile unsigned char rx_head;
extern volatile unsigned char rx_tail;
extern volatile char rx_buffer[RX_BUFFER_SIZE];

// A variável do tick agora é perfeitamente acessível pelo Assembly
extern volatile long systemTick; 

unsigned long TRAP14_Handler(unsigned long d0_val, unsigned long d1_val) {
    unsigned long retorno_final = 0; // Cria a variável com 32-bits vazia
    unsigned char ch = 0;
    
    switch(d1_val & 0xFF) {
        case 0: 
            uart0_init(); 
            break;
        case 1: 
            uart0_write(d0_val); 
            break;
        case 2: 
            retorno_final = (unsigned long)uart0_read(); 
            break;
        case 3: 
            ch = uart0_read_timeout(); 
            printf("TRAP14_Handler: Retornando com ch... [%02x]\n", ch);
            retorno_final = (unsigned long)ch; // Garante os 32 bits limpos aqui
            break;
        default:
            printf("\n\nQUE MERDA É ESSA: D1 era %ld\n\n\n\n", d1_val);
            break;
    }
    
    return retorno_final; 
}

// Sua função de leitura atômica do tick continua linda aqui
unsigned long get_system_tick(void) {
    unsigned long tick;
    unsigned int status_antigo;
    
    status_antigo = m68k_disable_level2_perfect(); 
    tick = systemTick;                     
    m68k_restore_interrupts(status_antigo); 
    
    return tick;
}

