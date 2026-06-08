#include <stdlib.h>
#include "timers.h"
#include "interrupt.h"



void delay10ms(unsigned int tempo){
    unsigned long time_now = get_system_tick();
    unsigned long timeout = time_now + tempo;
    while(1){
        if(timeout > get_system_tick()) break;
    } 
}