#include "stdio.h"
#include "drv_picoVga.h"
#include "drv_uart.h"
#include "drv_kbd.h"
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
    init_kbd();

    // 1. Direciona o console para a PicoVGA
   // set_console_output(uart_putchar);
   // print_string("E este texto vai direto para o terminal Serial a 115200!\r\n");

    // 2. O usuário digitou um comando para mudar de canal? 
    // Basta alterar o ponteiro!
    set_console_output(picovga_putchar);
    set_console_input(get_char);

    printf("Iniciando o sistema PDS317\n");

    printf("%s",MsgOrionInit);

    while(1) {
        //picovga_putchar('U');
        unsigned int ch = get_char();
        if( ch >= 0x20){
            printf("%c",ch);
        }else{
            if(ch == 0x0A || ch == 0x0D){
                printf("\n");
            }
        }
       // for(delay = 0; delay < 50000; delay++); // Delay tosco em software
    }
}

