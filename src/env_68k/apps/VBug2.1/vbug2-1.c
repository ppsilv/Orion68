#include <stdio.h>
#include <stdlib.h>
#include "drv_picoVga.h"
#include "drv_uart.h"
#include "drv_kbd.h"
#include "io.h"
#include "mc68000.h"
#include "color.h"
#include "timers.h"
#include "interrupt.h"
#include "ata.h"

//__attribute__((section(".mram"))) char vbug_buffer[256];
//__attribute__((section(".minha_ram"))) int vbug_status_flag;
volatile __attribute__((section(".mram"))) long systemTick;
volatile __attribute__((section(".mram"))) unsigned int flg_system;

#include "./tools/build_counter.h"


extern void xmodem_receive();
extern uint32_t get_system_tick_nmi_safe(void);

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
void print_msg(char *str){
        unsigned char x,y;
        y = 0x1D; //29
        x = 0;
        picovga_gotoxy(x,y); //padrao col,row
        printf("%s",str);
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
extern int ata_detect(void);
extern int ata_init(void);

void main() {
    // Inicializa os hardwares normalmente
    // 1. Direciona o console para a PicoVGA
    // Basta alterar o ponteiro!
    set_console_output(picovga_putchar);
    
    set_console_input(get_char);

    picovga_set_color(RED,BLACK);

    printf("%s",MsgOrionInit);
    picovga_set_color(GREEN,BLACK);
    //int col=0,row=15;
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

    m68k_enable_all_interrupts(); 

    while(1) {
        clrscr();
menu:        
        picovga_gotoxy(0,0);
        printf(" VBug2.1 - Menu\n");
        printf(" 0 - Clear screen\n");
        printf(" 1 - Verificar o systemtick\n");
        printf(" 2 - Memory dump\n");
        printf(" 3 - Xmodem download\n");
        printf(" 4 - Posiciona cursor\n");
        printf(" 5 - Executa programa\n");
        printf(" 6 - Desabilita interrupcao\n");
        printf(" 7 - habilitita interrupcao\n");
        printf(" 8 - le o setor 0 do disco\n");


        printf("Choose an option: ");
        ch = get_char();
        printf("%c",ch);
        ch -= '0';
        printf("\n");
        switch(ch){
            case 0: clrscr();
                    break;
            case 1:
                    print_msg("systemTick: ");    
                    printf("[%08ld]",get_system_tick());
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
                    unsigned char x,y;
                    printf("Digite o valor de y: ");y = get_char();
                    printf("Digite o valor de x: ");x = get_char();
                    y -= '0';
                    x -= '0';
                    picovga_gotoxy(x,y);
                    putchar('A');
                    goto menu;
                    break;
            case 5:
                    if ( get_flg_program_loaded() ){
                        executar_xmodem();
                    }    
                    print_msg("Nao tem programa na memoria");                    
                    goto menu;
                    break;               
            case 6:
                    m68k_disable_all_interrupts();
                    break;      
            case 7:
                    m68k_enable_all_interrupts();
                    break;
            case 8:
                    ata_init();
                    //ata_read((unsigned char*)0x82000, 0, 1, ATA_MASTER);    
                    goto menu;
                    break;  
                             
            case 9:
                    break;  
            default:
                    printf("Wrong option\n");        
        }
    }
}

