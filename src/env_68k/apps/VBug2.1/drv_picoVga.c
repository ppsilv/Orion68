#include "drv_picoVga.h"

void init_picoVga(){

}

void picovga_putchar(unsigned char ch){
    WRITE_SCREEN = ch;
}