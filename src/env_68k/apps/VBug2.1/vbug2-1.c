#include <stdio.h>
#include <stdlib.h>
#include "drv_picoVga.h"
#include "drv_uart.h"
#include "drv_kbd.h"
#include "io.h"
#include "mc68000.h"
#include "color.h"
#include "timers.h"

//__attribute__((section(".mram"))) char vbug_buffer[256];
//__attribute__((section(".minha_ram"))) int vbug_status_flag;
__attribute__((section(".mram"))) long systemTick;
__attribute__((section(".mram"))) unsigned int flg_system;

#include "./tools/build_counter.h"

const char MsgOrionInit[] = 
    "PDS317 - copyright (C) pdsilva(pgordao).VBug2.1\n"
    "MC68000 System Monitor MERDA\n"
    "Build Date: " __DATE__ " - " __TIME__ "\n"
    "Build Counter: " BUILD_COUNTER "\n"
    "-----------------------------------------------\n\n";
extern volatile unsigned char debug_pkt;
 
extern void dump_memory(long addr);
 

void main() {
    systemTick = 0;
    // Inicializa os hardwares normalmente

    // 1. Direciona o console para a PicoVGA
    // Basta alterar o ponteiro!
    set_console_output(picovga_putchar);
    
    set_console_input(get_char);

    picovga_set_color(RED,BLACK);

    printf("%s",MsgOrionInit);
    picovga_set_color(GREEN,BLACK);
    int col=0,row=15;
    unsigned int ch;
    print_capslock();

    //uart0_init();
    delay10ms(1000);  //100ms    
    // uart1_init();
    // delay10ms(100);  //100ms    
    // uart2_init();
    // delay10ms(100);  //100ms    
    // uart3_init();
    // delay10ms(100);  //100ms    
    init_kbd();
    ch9350_shut_up();
    delay10ms(1000);  //100ms    


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
        printf(" 7 - Cale a boca\n");
        printf(" 8 - Enable interrupts\n");
        printf(" a - escreve na uart\n");

        printf("Choose an option: ");
        ch = get_char();
        printf("%c",ch);
        ch -= '0';
        printf("\n");
        switch(ch){
            case 0x61:
                    break;    
            case 0: clrscr();
                    col=0,row=0;
                    break;
            case 1:
                    printf("systemTick: [%08ld]\n",systemTick);
                    break;
            case 2:
                    //char arr[] = "82000";
                    //char *ptr_fim;
                    //long resultado;
                    //resultado = strtol(arr, &ptr_fim, 10);

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
            case 7:
                    ch9350_shut_up();
                    break;    
            case 8:
                    ch9350_shut_up();
                    enable_kbd_interrupts();                                        
                    break;    
            default:
                    printf("Wrong option\n");        
        }
    }
}

