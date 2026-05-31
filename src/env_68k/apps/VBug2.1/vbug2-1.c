#include "drv_picoVga.h"
#include "drv_uart.h"

//__attribute__((section(".mram"))) char vbug_buffer[256];
//__attribute__((section(".minha_ram"))) int vbug_status_flag;


void main(){
while(1) {
        __asm__ volatile (
            "movea.l #0x00FF8001, %%a0\n\t"
            "move.b  #0x55, (%%a0)\n\t"
            : // Sem saídas
            : // Sem entradas
            : "a0" // Avisa o GCC que usamos o A0
        );

        // Delay puramente em registradores (Assembly) para não usar a RAM
        __asm__ volatile (
            "move.l #50000, %%d0\n\t"
            "1:\n\t"
            "subq.l #1, %%d0\n\t"
            "bne.s  1b\n\t"
            :
            :
            : "d0"
        );
    }    
}

#define wr_screen 0xFF8001
void main1() {
    volatile int delay;

    unsigned char *screen_reg = (unsigned char *)wr_screen;
 
    //uart_init();
    // Se tiver picovga_init(), chame aqui

    while(1) {
        *screen_reg = 'U';      // Cospe 'U' na UART
        
        
        for(delay = 0; delay < 50000; delay++); // Delay tosco em software
    }
}

/*    
    // 1. Inicializa o hardware usando seus drivers em C
    uart_init();
    init_kbd();
    init_picoVga();

    printf("\r\n--- Orion68K Monitor VBug 2.1 ---\r\n");

    // 2. Loop principal do Prompt
    while(1) {
        printf("VBug> ");
        
        // Aqui você vai usar a sua função get_key() para capturar o comando
        char cmd = get_key(); 
        
        switch(cmd) {
            case 'D': case 'd':
                // Chamar sua rotina de Dump de Memória em C
                break;
            case 'M': case 'm':
                // Chamar rotina de Modificar Memória
                break;
            case 'R': case 'r':
                // Mostrar registradores
                break;
            default:
                printf("\r\nComando Invalido!\r\n");
                break;
        }
    }
    */
