#include "drv_uart.h"

// Altere o divisor aqui dependendo do cristal real da sua UART:
// Para Cristal de 1.8432 MHz: 115200 baud -> Divisor = 1  (DLL=0x01, DLM=0x00)
// Para Cristal de 7.3728 MHz: 115200 baud -> Divisor = 4  (DLL=0x04, DLM=0x00)
#define UART_DIVISOR_LSB  0x01  
#define UART_DIVISOR_MSB  0x00

void uart_init(void) {
    // 1. Desabilita todas as interrupções da UART temporariamente
    IER0 = 0x00;

    // 2. Ativa o DLAB (Bit 7 = 1) no LCR para liberar acesso a DLL e DLM
    //    Aproveitamos e definimos temporariamente o formato para não quebrar
    LCR0 = 0x80; 

    // 3. Configura o Baud Rate (Carrega o divisor de clock)
    DLL0 = UART_DIVISOR_LSB; // LSB do divisor
    DLM0 = UART_DIVISOR_MSB; // MSB do divisor

    // 4. Desativa o DLAB (Bit 7 = 0) para voltar os registradores ao modo normal
    //    E define o formato do frame: 8 bits de dados, Sem Paridade, 1 Stop Bit (8N1)
    //    Valor: 0x03 (00000011b) -> Bits 0 e 1 definem Word Length = 8 bits
    LCR0 = 0x03;

    // 5. Configura e ativa as FIFOs de transmissão e recepção
    //    Valor: 0x07 -> Ativa FIFO (Bit 0), limpa RX FIFO (Bit 1), limpa TX FIFO (Bit 2)
    FCR0 = 0x07;

    // 6. Configura o Modem Control Register
    //    Valor: 0x03 -> Ativa as linhas RTS (Bit 1) e DTR (Bit 0) para controle de fluxo se houver
    MCR0 = 0x03;
    
    // Pronto! A UART está configurada e pronta para transmitir/receber a 115200 8N1
}

void uart_putchar(unsigned char ch) {
    // 1. Aguarda o Transmit Holding Register ficar vazio
    //    Bit 5 (0x20) do LSR0 é o THRE (Transmit Holding Register Empty)
    while ((LSR0 & 0x20) == 0);
    
    // 2. Escreve o caractere no registrador de transmissão
    THR0 = ch;
}