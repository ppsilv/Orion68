#include "drv_uart.h"
#include "timers.h"


void uart0_init(){
    volatile unsigned char *uart_reg = (volatile unsigned char *)DRV_UART0_BASE;

    // 1. Entra no modo DLAB para configurar Baud Rate
    *(uart_reg + FCR) = 0x0D;
    *(uart_reg + LCR) = 0x83;
    *(uart_reg + DLL) = 8; // Divisor para 115200 com 14.7456MHz
    *(uart_reg + DLM) = 0;
    // 2. Sai do modo DLAB e define 8N1 (MUITO IMPORTANTE: usar 0x03)
    *(uart_reg + LCR) = 0x03;
    *(uart_reg + SCR) = 0x00;

    // 3. Configura FIFO (Habilita, limpa buffers e seta trigger de 14 bytes)
    //*(uart_reg + FCR0) = 0xC7;
    // 3. Configura FIFO (Habilita, limpa buffers e seta trigger de 1 byte)

    // 4. Limpa registradores de controle
    //*(uart_reg + MCR0) = 0x00;
    //*(uart_reg + IER0) = 0x00; // Garante que interrupções estão desligadas
}


unsigned int uart0_read(){
    volatile unsigned char *uart_reg = (volatile unsigned char *)DRV_UART0_BASE;
    unsigned char ch;
    while (!(*(uart_reg + LSR) & 0x01)) ;
    ch = (unsigned char)*(uart_reg + RHR);

    return ch;
}
void uart0_write(unsigned char ch){
    volatile unsigned char *uart_reg = (volatile unsigned char *)DRV_UART0_BASE;
    
    while (!(*(uart_reg + LSR) & 0x05)) ;
    *(uart_reg + THR) = ch;
}
int _inbyte(int delay){
    volatile unsigned char *uart_reg = (volatile unsigned char *)DRV_UART0_BASE;
    unsigned char ch;
    long timeout = delay + systemTick;
    while (!(*(uart_reg + LSR) & 0x01)){
        if( systemTick > timeout ){
            return -1;
        }
    }
    ch = (unsigned char)*(uart_reg + RHR);
    return (int)ch;
}
void _outbyte(unsigned char ch){
    uart0_write(ch);
}

void uart0_write_string(char * str){

    while(*str){
        uart0_write(*str++);
    }
}
