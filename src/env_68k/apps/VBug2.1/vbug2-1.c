#include <stdio.h>
#include <stdlib.h>
#include "drv_picoVga.h"
#include "drv_uart.h"
#include "drv_kbd.h"
#include "io.h"
#include "mc68000.h"
#include "color.h"

__attribute__((section(".mram"))) char vbug_buffer[256];
__attribute__((section(".minha_ram"))) int vbug_status_flag;
__attribute__((section(".mram"))) long systemTick;
__attribute__((section(".mram"))) unsigned int flg_system;

#include "build_counter.h"

const char MsgOrionInit[] = 
    "PDS317 - copyright (C) pdsilva(pgordao).VBug2.1\n"
    "MC68000 System Monitor\n"
    "Build Date: " __DATE__ " - " __TIME__ "\n"
    "Build Counter: " BUILD_COUNTER "\n"
    "-----------------------------------------------\n\n";
extern volatile unsigned char debug_pkt;

void dump_memory(long addr);
void print_capslock();
void main() {
    systemTick = 0;
    // Inicializa os hardwares normalmente
    init_kbd();
    uart0_init();

    // 1. Direciona o console para a PicoVGA
   // set_console_output(uart_putchar);
   // print_string("E este texto vai direto para o terminal Serial a 115200!\r\n");

    // 2. O usuário digitou um comando para mudar de canal? 
    // Basta alterar o ponteiro!
    set_console_output(picovga_putchar);
    set_console_input(get_char);

    picovga_set_color(RED,BLACK);

    printf("%s",MsgOrionInit);
    picovga_set_color(GREEN,BLACK);
    int col=0,row=15;
    unsigned int ch;
    print_capslock();
    while(1) {
        //clrscr();
        picovga_gotoxy(col,row);
        printf("VBug2.1 - Menu\n");
        printf(" 0 - Clear screen\n");
        printf(" 1 - Verificar o systemtick\n");
        printf(" 2 - Memory dump\n");
        printf(" 3 - Xmodem download\n");
        printf(" 4 - Posiciona cursor\n");
        printf(" 5 - Ler kbd\n");
        printf(" 6 - Ler kbd\n");

        printf("Choose an option: ");
        ch = get_char();
        printf("%c",ch);
        ch -= '0';
        printf("\n");
        switch(ch){
            case 0: clrscr();
                    col=0,row=0;
                    break;
            case 1:
                    printf("systemTick: [%08ld]\n",systemTick);
                    break;
            case 2:
                    char arr[] = "82000";
                    char *ptr_fim;
                    long resultado;

                    resultado = strtol(arr, &ptr_fim, 10);
                    dump_memory(0x000);
                    break;
            case 3:
                    break;
            case 4:
                    unsigned char y,x;
                    printf("Digite o valor de y: ");y = get_char();
                    printf("Digite o valor de x: ");x = get_char();
                    y -= '0';
                    x -= '0';
                    col=x;row=y;
                    picovga_gotoxy(y,x);
                    break;
            case 5:
                    debug_pkt = 1;
                    while(1){
                        printf("Hit any <ENTER> to continue <ESC> to terminate: ");
                        ch = get_char();
                        printf("ch[%02x]",ch);
                        if( ch == 0x1B ){
                            break;
                        }
                    }
                    debug_pkt = 0;
                    break;
            case 6:
                    while(1){
                        ch = get_char();
                        printf("ch[%02x]",ch);
                        if( ch == 0x1B ){
                            break;
                        }
                    }
                    break;
            default:
                    printf("Wrong option\n");        
        }
    }
}

