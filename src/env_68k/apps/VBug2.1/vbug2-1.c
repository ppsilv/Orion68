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


extern void xmodem_receive();
typedef void (*ProgramaXModem)(void);

void executar_xmodem(void) {
    // 2. Cria o ponteiro apontando diretamente para o endereço 0x82000
    ProgramaXModem rodar_xmodem = (ProgramaXModem)0x82000;

    printf("Passando o controle para o XModem em 0x82000...\n");

    // 3. Salta para o programa! (O GCC vai traduzir isso em um JSR ou JMP)
    rodar_xmodem(); 
}

void clr_flg_program_loaded(){
       flg_system &= 0xFE;
}
void set_flg_program_loaded(){
       flg_system |= 0x01;
}
unsigned int get_flg_program_loaded(){
       return flg_system & 0x01 ;
}

const char MsgOrionInit[] = 
    "PDS317 - copyright (C) pdsilva(pgordao).VBug2.1\n"
    "MC68000 System Monitor with ch9350\n"
    "Build Date: " __DATE__ " - " __TIME__ "\n"
    "Build Counter: " BUILD_COUNTER "\n"
    "-----------------------------------------------\n\n";
extern volatile unsigned char debug_pkt;
 
extern void dump_memory(long addr);
extern void liga_debug(); 

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

    uart0_init();
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
        clrscr();
menu:        
        picovga_gotoxy(1,0);
        printf(" VBug2.1 - Menu\n");
        printf(" 0 - Clear screen\n");
        printf(" 1 - Verificar o systemtick\n");
        printf(" 2 - Memory dump\n");
        printf(" 3 - Xmodem download\n");
        printf(" 4 - Posiciona cursor\n");
        printf(" 5 - Escreve na uart\n");
        printf(" 6 - Le e escreve na uart\n");
        printf(" 7 - Le e escreve na uart\n");
        printf(" 8 - Le teclado escreve na uart\n");
        printf(" 9 - Usa _inbute escreve na uart\n");

        printf("Choose an option: ");
        ch = get_char();
        printf("%c",ch);
        ch -= '0';
        printf("\n");
        switch(ch){
            case 0: clrscr();
                    col=0,row=1;
                    break;
            case 1:
                    printf("systemTick: [%08ld]\n",systemTick);
                    goto menu;
                    break;
            case 2:
                    //char arr[] = "82000";
                    //char *ptr_fim;
                    //long resultado;
                    //resultado = strtol(arr, &ptr_fim, 10);

                    dump_memory(0x82000);
                    break;
            case 3:
                    xmodem_receive();    
                    goto menu;
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
                    if ( get_flg_program_loaded() ){
                        executar_xmodem();
                    }    
                    break;               
            case 6:
                    while(1){    
                        int ch = _inbyte(100);
                        if( ch < 0 ){
                            continue;
                        }
                        if( ch > 0 ){
                            uart0_write((char)ch);
                            printf("%c",ch);
                        }
                        if( ch == 'Z') break;
                    }
                    break;      
            case 7:
                    while(1){    
                        ch = uart0_read();
                        uart0_write((char)ch);
                        if( ch == 'Z') break;
                    }
                    break;
            case 8:
                    while(1){
                        ch = get_char();
                        printf("%c ",ch);
                        uart0_write((char)ch);
                        if( ch == 'Z')
                           break;     
                    }
                    break;  
                             
            case 9:
                    while(1){
                        ch = _inbyte(100)  ;
                        if( ch < 0) continue;
                        printf("%c ",ch);
                        uart0_write((char)ch);
                        if( ch == 'Z')
                           break;     
                    }
                    break;  
            default:
                    printf("Wrong option\n");        
        }
    }
}

