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
#include "show_registers.h"
#include "sysflags.h"

// Aqui a memória é alocada de verdade!
SystemFlags sys_flags;

//__attribute__((section(".mram"))) char vbug_buffer[256];
//__attribute__((section(".minha_ram"))) int vbug_status_flag;
volatile __attribute__((section(".mram"))) long systemTick;
volatile __attribute__((section(".mram"))) unsigned int flg_system;

#include "./tools/build_counter.h"

void dump_memory(void * addr,int size);
extern void xmodem_loader();
extern uint32_t get_system_tick_nmi_safe(void);
extern void listar_diretorio_raiz(void);


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
 

extern void liga_debug(); 
extern int ata_detect(void);
extern int ata_init(void);
extern void abrir_arquivo();

extern void UartWrCh(unsigned char ch);

void write_string(char * str){
    while(*str){
        UartWrCh(*str++);
    }
}
extern void ler_e_exibir_joblog(char * filename);
extern void ler_comando(char *buffer) ;
extern void processar_comando(char *cmd_line);
extern int ata_read_identity(void);

char buffer[64];
void main() {
    // Inicializa os hardwares normalmente
    // 1. Direciona o console para a PicoVGA
    // Basta alterar o ponteiro!
    m68k_enable_all_interrupts(); 
    set_console_output(picovga_putchar);
    init_kbd();
    ch9350_shut_up();
    delay10ms(10);  //100ms    
    set_console_input(get_char);

    picovga_set_color(RED,BLACK);
    printf("%s",MsgOrionInit);

    picovga_set_color(GREEN,BLACK);
    unsigned int ch;
    print_capslock();

    delay10ms(100);  //10000ms    
    // uart1_init();
    // delay10ms(100);  //100ms    
    // uart2_init();
    // delay10ms(100);  //100ms    
    // uart3_init();
    // delay10ms(100);  //100ms    

    ata_read_identity();    

    while(1) {
        clrscr();
menu:        
        picovga_gotoxy(0,0);
        printf(" VBug2.1 - Menu\n");
        printf(" 0 - Clear screen\n");
        printf(" 1 - Verificar o systemtick\n");
        printf(" 2 - Memory dump\n");
        printf(" 3 - Xmodem download\n");
        printf(" 4 - command\n");
        printf(" 5 - Executa programa\n");
        printf(" 6 - Desabilita interrupcao\n");
        printf(" 7 - habilitita interrupcao\n");
        printf(" 8 - ata_init\n");
        printf(" 9 - read dir\n");
        printf(" A|a A.TXT\n");
        printf(" B|b - dump registers\n");
        printf(" C|c - /A.TXT\n");
        printf("Choose an option: ");

        ch = get_char();
        if( ch >= 0x30 && ch <= 0x39)
                ch -= '0';
        
        printf("\n");
        switch(ch){
            case 0x41:
            case 0x61:
                    ler_e_exibir_joblog("A.TXT");
                    goto menu;     
            case 0x42:
            case 0x62:
                    dump_registradores();
                    goto menu;     

            case 0x43:
            case 0x63:
                    ler_e_exibir_joblog("/A.TXT");
                    goto menu;     

            case 0: clrscr();
                    goto menu;
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

                    dump_memory((void*)0x82000,512);
                    break;
            case 3:
                    xmodem_loader();    
                    goto menu;
                    break;
            case 4:
                    ler_comando(buffer) ;
                    processar_comando(buffer);
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
                    goto menu;
                    break;  
                             
            case 9:
                    listar_diretorio_raiz();
                    goto menu;
                    break;  
            default:
                    printf("Wrong option\n");        
        }
    }
}

