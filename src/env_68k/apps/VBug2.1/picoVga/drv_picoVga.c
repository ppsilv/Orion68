#include "stdio.h"
#include "drv_picoVga.h"
#include "color.h"
#include "timers.h"

void init_picoVga(){

}
void run_cmd(unsigned char cmd){
    RUN_CMD = cmd;
}
void picovga_putchar(unsigned char ch){
    int x=0x20;
    WRITE_SCREEN = ch;
    while(x--);
}
void picovga_gotoxy(int col,int row){
    REG_Y_LOW  = row; //(unsigned char)(row & 0x00FF);
    REG_Y_HIGH = 0;   //(unsigned char)(row > 8);
    REG_X_LOW  = col; //(unsigned char)(col & 0x00FF);
    REG_X_HIGH = 0;   //(unsigned char)(col > 8);
    delay10ms(1);
    //printf("Setando a position Y [%02X] X[%02X]\n",row,col);
    RUN_CMD = CMD_SET_CUR_POS;
}
void picovga_gohome(){
    delay10ms(1);
    RUN_CMD = CMD_GO_HOME;
}
void picovga_set_color(unsigned char txtcolor,unsigned char bgcolor){
    unsigned int color;
    color = (txtcolor << 4) &0xF0;
    color |= bgcolor;
    //printf("Setando a cor [%02X]\n",color);
    SET_TXT_COLOR = color;
}
void clrscr(){
    delay10ms(2);
    RUN_CMD = CMD_CLEAR_SCREEN;
}