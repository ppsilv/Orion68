#ifndef __TIMERS_H__
#define __TIMERS_H__

extern __attribute__((section(".mram"))) long systemTick;

void delay10ms(unsigned int tempo);

#endif