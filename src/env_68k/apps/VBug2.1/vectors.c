#include "io.h"
#include "keyboard.h"

extern volatile unsigned char rx_head;
extern volatile unsigned char rx_tail;
extern volatile char rx_buffer[RX_BUFFER_SIZE];


extern __attribute__((section(".mram"))) long systemTick;

void __attribute__((interrupt)) SvcBusError     (){
    
}
void __attribute__((interrupt)) Svcaddress_err  (){
    
}
void __attribute__((interrupt)) SvcIllegalIns   (){
    
}
void __attribute__((interrupt)) SvcDiv0_handler (){
    
}
void __attribute__((interrupt)) SvcChkIns       (){
    
}
void __attribute__((interrupt)) SvcTrapvIns(){
    
}
void __attribute__((interrupt)) SvcPrivViolation(){
    
}
void __attribute__((interrupt)) SvcTrace    (){
    
}
void __attribute__((interrupt)) SvcLineA        (){
    
}
void __attribute__((interrupt)) SvcLineF        (){
    
}
void __attribute__((interrupt)) bad_exception(){
    
}
void __attribute__((interrupt)) SpuriousHandler(){
    
}
void __attribute__((interrupt)) Int1Handler(){
    
}
void __attribute__((interrupt)) Int2Handler(){
    
}
void __attribute__((interrupt)) Int3Handler(){
    
}
void __attribute__((interrupt)) Int4Handler(){

}
void __attribute__((interrupt)) Int5Handler(){
    
}
void __attribute__((interrupt)) Int6Handler(){
//    while (*((volatile unsigned char *)LSR) & 0x01) {
//        rx_buffer[rx_head] = *((volatile unsigned char *)RBR);
//        rx_head++; // Overflow natural em 256 por ser uint8_t
//    }    
}
void __attribute__((interrupt)) Int7Handler(){
    systemTick += 1;
}
void __attribute__((interrupt)) Trap0Handler(){
    
}
void __attribute__((interrupt)) Trap1Handler(){
// Amarre variáveis locais diretamente aos registradores físicos
    register unsigned short function_code asm("d0");
    register unsigned char  character     asm("d1");

    // Agora você pode usar variáveis C normais!
    switch(function_code) {
        case 1:
            if (cconin) {
                function_code=cconin();
            }
            break;
        case 2:
            if (cconout) {
                cconout(character);
            }
            break;
    }    
}
void __attribute__((interrupt)) Trap2Handler(){
    
}
void __attribute__((interrupt)) Trap3Handler(){
    
}
void __attribute__((interrupt)) Trap4Handler(){
    
}
void __attribute__((interrupt)) Trap5Handler(){
    
}
void __attribute__((interrupt)) Trap6Handler(){
    
}
void __attribute__((interrupt)) Trap7Handler(){
    
}
void __attribute__((interrupt)) Trap8Handler(){
    
}
void __attribute__((interrupt)) Trap9Handler(){
    
}
void __attribute__((interrupt)) TrapAHandler(){
    
}
void __attribute__((interrupt)) TrapBHandler(){
    
}
void __attribute__((interrupt)) TrapCHandler(){
    
}
void __attribute__((interrupt)) TrapDHandler(){
    
}
void __attribute__((interrupt)) TrapEHandler(){
    
}
void __attribute__((interrupt)) TrapFHandler(){
    
}
