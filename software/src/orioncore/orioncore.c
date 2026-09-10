#include <stdio.h>
#include <stdlib.h>
#include <vga_video.h>
#include <fatfs/ff.h>
#include "drv_kbd.h"
#include "mc68000.h"
#include "ata.h"
#include "orion68.h"
#include "drv_ps2.h"
#include "drivers_raw/kbd/ringbuffer.h"
#include "interrupt.h"
#include "jumptable.h"


FATFS FatFs;      // Objeto de controle do sistema de arquivos (Work area)

extern void _delay_ms();
extern int ata_read_identity(void);
extern void duart_init_canal_a(void);
extern void picovga_putchar(char ch);
extern int dhcp_client(void);
extern void duart_a_init_38400(void);
extern void pico_write_ch(uint8_t ch);
extern void Int2Handler(void);
extern void Int3Handler(void);
extern void setaVetorFuncao(uint8_t vetor, uint32_t funcao);

volatile uint32_t tick_count = 0;
volatile uint32_t systemTick = 0;

//__attribute__((section(".mram"))) char vbug_buffer[256];
//__attribute__((section(".minha_ram"))) int vbug_status_flag;
//volatile __attribute__((section(".mram"))) long systemTick;
//volatile __attribute__((section(".mram"))) unsigned int tick_count;
//volatile __attribute__((section(".mram"))) unsigned int flg_system;

volatile uint32_t *last_mem_address = ( uint32_t *)0x80000UL;

#include "./tools/build_counter.h"

extern void duart_putc(char c);
extern char duart_getc(void);

typedef void (*ProgramaXModem)(void);


// Sua função de leitura atômica do tick continua linda aqui
uint32_t get_system_tick(void) {
    unsigned long tick;
    //unsigned int status_antigo;
    
    //status_antigo = m68k_disable_level2_perfect(); 
    tick = systemTick;                     
    //m68k_restore_interrupts(status_antigo); 
    
    return tick;
}
 
void vputs(char * str){
    while(*str){
        picovga_putchar(*str);
       // duart_putc(*str);
        str++;
    }
}
extern char *  utoa (unsigned value, char *str,  int base);

void vputi(unsigned long i){
    char buf[32];
    vputs(utoa(i,buf,10));
}

void do_ideinit(int argc, char *argv[])
{
    FRESULT fr;
    fr = f_mount(&FatFs, "", 0);
    if (fr != FR_OK) {
        vputs("PANIC: Erro ao montar FAT\n");
    }else{
        vputs(": FAT success mounted!\n");
    }
}

const char MsgOrionInit[] = 
  "\nPDS317-Hardware Copyright 2026(C) pdsilva(pgordao).\n"
    "Orioncore V1.0.1 M68k Firmware Copyright 2026(C) pdsilva(pgordao)\n"
    "Build Date: " __DATE__ " - " __TIME__ "\n"
    "Build Counter: " BUILD_COUNTER "\n"
    "-----------------------------------------------\n\n";
extern volatile unsigned char debug_pkt;
extern int shell(int argc, char *argv[]);
void jump_to_kernel(void);
int load_kernel_flat(const char *path);
extern uint8_t ring_buf_is_empty(void);
void main(int argc, char *argv[]) {
    pico_write_ch('A');
    vputs("Memory: Rom start addr.............: 0\n");
    vputs("        Rom installed  low and high: 65536 2 of 32768\n");
    vputs("        Rom space end..............: 524287\n");
    vputs("        First sram address.........: 524288\n");
    printf("        Last  sram address.........: %ld\n",*last_mem_address);
    printf("        CPU sram memory............: %ld words\n",*last_mem_address-0x80000);
    printf("        Total sram memory..........: %ld bytes\n",(*last_mem_address-0x80000)*2);
    
    pico_write_ch('c');
    setaVetorFuncao(vect_Int2Handler,(uint32_t) Int2Handler );
    setaVetorFuncao(vect_Int3Handler,(uint32_t) Int3Handler );
    //m68k_enable_all_interrupts();
    //vputs("* - All Interrupts enabled.\n");

    pico_write_ch('G');
    vputs("* - Initializing:\n");
    vputs("    * duart GPIO\n");
    duart_opr_init();
    pico_write_ch('H');
    vputs("    * duart A\n");
    duart_a_init_38400();
    pico_write_ch('I');
   // vputs("    * duart B\n");
   // duart_init_canal_a();
    
#ifdef DEBUG_ON
    ata_read_identity();    
#endif
    vputs("    * IDE ");
    do_ideinit(0,NULL);
    pico_write_ch('J');

    pico_write_ch('K');
    vputs("    * Ethernet board: ");
    dhcp_client();

    pico_write_ch('L');
    vputs("\n");
    ring_buf_init();

    pico_write_ch('M');

    uint32_t timeout = systemTick;
    timeout += 5000;
    char ch = 0;
    vputs("Pressione DELETE to go offline system!\n");
    while( systemTick < timeout ){
        if( ! ring_buf_is_empty() ){
            ch = ring_buf_get_char();
            break;
        } 
    }
    if(ch == 0x7f){
        vputs("User wants do bypass Offline State\n");
        //LOAD SHELL.
        vputs("Loading oshell\n");
        shell(argc,argv);
    }else{
        vputs("Loading oKernel\n");
        if (load_kernel_flat("kernel.sys")) {
            jump_to_kernel();
        } else {
            vputs("No kernel, loading oshell\n");
            shell(argc,argv);
        }        
    }

    while (1){
        char ch=duart_getc();
        duart_putc(ch);
        picovga_putchar(ch);
    }

}

void jump_to_kernel(void)
{
    m68k_disable_all_interrupts();
    asm volatile(
        "jmp 0x82000\n"
    );
}

#define KERNEL_LOAD_ADDR ((void *) 0x82000)

int load_kernel_flat(const char *path)
{
    FIL fil;
    FRESULT fr;
    UINT br;

    fr = f_open(&fil, path, FA_READ);
    if (fr != FR_OK) {
        vputs("kernel: falha ao abrir kernel.bin\n");
        return 0;
    }

    fr = f_read(&fil, KERNEL_LOAD_ADDR, f_size(&fil), &br);
    f_close(&fil);

    if (fr != FR_OK || br == 0) {
        vputs("kernel: falha ao ler kernel.bin\n");
        return 0;
    }

    return 1;
}

