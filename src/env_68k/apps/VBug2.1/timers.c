#include <stdlib.h>
#include "timers.h"

void delay10ms(unsigned int tempo){
    unsigned long time_now = systemTick;
    unsigned long timeout = time_now + tempo;
    while(1){
        if(timeout > systemTick ) break;
    } 
}