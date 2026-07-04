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
#include "ff.h"
#include "show_registers.h"
#include "sysflags.h"

// Aqui a memória é alocada de verdade!
SystemFlags sys_flags;

FATFS FatFs;      // Objeto de controle do sistema de arquivos (Work area)

#define LINELEN 128
char g_cmd_buffer[LINELEN];
extern int getline(char *line, int linesize);
extern void execute_cmd(char *linebuffer);

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
    "\nPDS317-Hardware copyright (C) pdsilva(pgordao).\n"
    "Orion68OS for m68k System.\n"
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

char buffer[128];
char syspath[128];

void display_prompt(void)
{
    char drv;
    if (f_getcwd(syspath, sizeof(syspath)) == FR_OK) {
        // CurrVol armazena o número do drive atual (0, 1, 2...)
        // path armazena o caminho (que no seu caso está vindo apenas "/")
        drv = syspath[0];
        //printf("DRV: [%c]\n",drv);
        drv = drv+0x11;
        syspath[0]=drv;
        if( drv >= 'A' && drv <= 'I'){
                printf("%s>", syspath); 
        }else{
                printf("0:%s>", syspath); 
        }
    } else {
        printf("0:/> ");
    }
}
void main() {
    FRESULT fr;

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

#ifdef DEBUG_ON
    ata_read_identity();    
#endif


    fr = f_mount(&FatFs, "", 1);
    if (fr != FR_OK) {
        printf("PANIC: Erro ao montar FAT: %d\n", fr);
    }else{
        printf("FAT montado com sucesso!\n");
    }


    display_prompt();



    while (1){
        if (getline(g_cmd_buffer, LINELEN) != -1) {
            execute_cmd(g_cmd_buffer);
            display_prompt();
        }
    }

}

