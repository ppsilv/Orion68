#include "stdio.h"
#include "drv_picoVga.h"
#include "color.h"
#include "timers.h"

// delay.c

#define PICO_VGA_BASE 0x00FF8000
#define WRITE_SCREEN   (*((volatile unsigned char *)(PICO_VGA_BASE + 0x01)))


static uint32_t tamanho_total = 0;

void vputc(char ch){
    WRITE_SCREEN = ch;
}

void vputs( char *s){
    while (*s) {
        vputc(*s++);
    }
}

void delay_us(uint32_t us) {
    // Cada iteração do loop "dbra" leva ~8 ciclos.
    // 8MHz => 8 ciclos = 1us. 
    // Portanto, 1us = 1 iteração do loop.
    // O loop em C com while pode ser impreciso, esta versão é melhor.
    __asm__ volatile (
        "1:                             \n"  // Label do loop
        "subq.l  #1, %0                 \n"  // Decrementa o contador (4 ciclos)
        "cmpi.l  #0, %0                 \n"  // Compara com zero (6 ciclos)
        "bne     1b                     \n"  // Se não for zero, volta (6 ciclos se tomado)
        : "+r" (us)                          // Entrada e saída: registrador para 'us'
        :
        : "cc"                               // Altera flags
    );
}

void delay_1ms(void) {
    asm volatile (
        "move.w  #1000, %%d0\n"
        "1:\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "dbra    %%d0, 1b\n"
        : : : "d0"
    );
}

void delay_ms(int ms) {
    for(int i = 0; i < ms; i++) {
        delay_1ms();
    }
}

void picovga_putchar( char ch){
    WRITE_SCREEN = ch;
}


void init_picoVga(){

}
inline void run_cmd(unsigned char cmd){
    RUN_CMD = cmd;
}

void picovga_gotoxy(int col,int row){

    REG_Y_LOW  = row; //(unsigned char)(row & 0x00FF);
    REG_Y_HIGH = 0;   //(unsigned char)(row > 8);
    REG_X_LOW  = col; //(unsigned char)(col & 0x00FF);
    REG_X_HIGH = 0;   //(unsigned char)(col > 8);
    run_cmd(CMD_SET_CUR_POS);
}
void picovga_gohome(){
    run_cmd(CMD_GO_HOME);
}
void picovga_set_color(unsigned char txtcolor,unsigned char bgcolor){
    unsigned int color;
    color = (txtcolor << 4) &0xF0;
    color |= bgcolor;
    SET_TXT_COLOR = color;
}
void clrscr(){
    run_cmd(CMD_CLEAR_SCREEN);
}

