#include <stdio.h>
#include <stdlib.h>
#include "timers.h"
#include "interrupt.h"



void delay10ms(unsigned int tempo){
    unsigned long time_start = get_system_tick();
    while((get_system_tick() - time_start) >= tempo);            
}