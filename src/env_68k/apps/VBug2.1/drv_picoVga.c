#include "drv_picoVga.h"
#include "color.h"

void init_picoVga(){

}

void picovga_putchar(unsigned char ch){
    WRITE_SCREEN = ch;
}
void picovga_gotoxy(int col,int row){
    REG_Y_LOW  = (unsigned char)(row & 0x00FF);
    REG_Y_HIGH = (unsigned char)(row > 8);
    REG_X_LOW  = (unsigned char)(col & 0x00FF);
    REG_X_HIGH = (unsigned char)(col > 8);

    RUN_CMD = CMD_SET_CUR_POS;
}
void picovga_set_color(color_t color){
    SET_TXT_COLOR = color;
}