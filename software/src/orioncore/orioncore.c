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

FATFS FatFs;      // Objeto de controle do sistema de arquivos (Work area)

extern void _delay_ms();
extern int ata_read_identity(void);
extern void duart_init_canal_a(void);
extern void picovga_putchar(char ch);
extern int dhcp_client(void);
extern void duart_a_init_38400(void);
extern void pico_write_ch(uint8_t ch);

//__attribute__((section(".mram"))) char vbug_buffer[256];
//__attribute__((section(".minha_ram"))) int vbug_status_flag;
//volatile __attribute__((section(".mram"))) long systemTick;
//volatile __attribute__((section(".mram"))) unsigned int tick_count;
//volatile __attribute__((section(".mram"))) unsigned int flg_system;

uint32_t *last_mem_address = (volatile uint32_t *)0x80000UL;

#include "./tools/build_counter.h"

extern void duart_putc(char c);
extern char duart_getc(void);

typedef void (*ProgramaXModem)(void);

extern char getkbd();

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
    "Orioncore V1.0.0 M68k Firmware Copyright 2026(C) pdsilva(pgordao)\n"
    "Build Date: " __DATE__ " - " __TIME__ "\n"
    "Build Counter: " BUILD_COUNTER "\n"
    "-----------------------------------------------\n\n";
extern volatile unsigned char debug_pkt;
extern int shell(int argc, char *argv[]);

void main(int argc, char *argv[]) {
    pico_write_ch('A');
    vputs("Memory: Rom start addr.............: 0\n");
    vputs("        Rom installed  low and high: 65536 2 of 32768\n");
    vputs("        Rom space end..............: 524287\n");
    vputs("        First sram address.........: 524288\n");
    //vputs("        Last  sram address.........: %ld\n",*last_mem_address);
    //vputs("        CPU sram memory............: %ld words\n",*last_mem_address-0x80000);
    //vputs("        Total sram memory..........: %ld bytes\n",(*last_mem_address-0x80000)*2);
    pico_write_ch('c');
//    m68k_enable_all_interrupts();
//    vputs("* - All Interrupts enabled.\n");

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
    //ring_buf_init();
    int delay=0;
    char ch = getkbd();
    if(ch == 0x7f){
        vputs("User wants do bypass Offline State\n");
        //LOAD SHELL.
        vputs("Loading oshell\n");
        shell(argc,argv);
    }else{
        vputs("Loading oKernel\n");
    }

    while (1){
        char ch=duart_getc();
        duart_putc(ch);
        picovga_putchar(ch);
    }

}
