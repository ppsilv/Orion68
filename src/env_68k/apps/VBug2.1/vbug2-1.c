#include "stdio.h"
#include "drv_picoVga.h"
#include "drv_uart.h"
#include "io.h"
#include "mc68000.h"

__attribute__((section(".mram"))) char vbug_buffer[256];
__attribute__((section(".minha_ram"))) int vbug_status_flag;


#include "build_counter.h"

const char MsgOrionInit[] = 
    "PDS317 - copyright (C) pdsilva(pgordao).VBug2.1\n"
    "MC68000 System Monitor\n"
    "Build Date: " __DATE__ " - " __TIME__ "\n"
    "Build Counter: " BUILD_COUNTER "\n"
    "-----------------------------------------------\n\n";


void main() {
    volatile int delay;
    // Inicializa os hardwares normalmente
    uart_init();
   
    // 1. Direciona o console para a PicoVGA
   // set_console_output(uart_putchar);
   // print_string("E este texto vai direto para o terminal Serial a 115200!\r\n");

    // 2. O usuário digitou um comando para mudar de canal? 
    // Basta alterar o ponteiro!
    set_console_output(picovga_putchar);

    printf("Iniciando o sistema PDS317\n");

    printf("%s",MsgOrionInit);

    while(1) {
        picovga_putchar('U');
        
        for(delay = 0; delay < 50000; delay++); // Delay tosco em software
    }
}

