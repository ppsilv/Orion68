#include <stdio.h>
#include "drv_picoVga.h"
#include "drv_kbd.h"

void dump_memory(long addr){
    unsigned char * pcharhex = (unsigned char *)addr;
    unsigned char * pcharasc = (unsigned char *)addr;
    unsigned char ch;
    clrscr();
    while(1){
        //clrscr();
        picovga_gotoxy(0,0);
        printf("--------------------------------------------------------------------------------\n");
        printf("Dumping 255 bytes from addr %08X\n",addr);
        printf("--------------------------------------------------------------------------------\n");
        //printf("Address  Data hexacimal                                    Data ASCII\n");
        printf("Address  00 12 13 14 15 16 17 18 19 10 11 12 13 14 15 16 - ASCII\n");
        for(int j=0;j<16;j++){
            printf("%07X  ",pcharhex);
            for(int i = 0; i<16; i++){
                printf("%02x ",*pcharhex++);            
            }
            printf("- ");
            pcharasc = pcharhex - 16;
            for(int i = 0; i<16; i++){
                ch = *pcharasc++;
                if( ch > 0x20 && ch < 0x80   )
                    printf("%c",ch);            
                else    
                    printf(".");            
            }
            printf("\n");
        }
        //
        ch = get_char();
        if( ch == 0x1B ){
            return;
        }
    }
}
