#include "io.h"
#include "keyboard.h"
#include "interrupt.h"

extern volatile unsigned char rx_head;
extern volatile unsigned char rx_tail;
extern volatile char rx_buffer[RX_BUFFER_SIZE];


static __attribute__((section(".mram"))) long systemTick=0;

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
    systemTick++;    
}
void __attribute__((interrupt)) Int3Handler(){
    
}
void __attribute__((interrupt)) Int4Handler(){

}
void __attribute__((interrupt)) Int5Handler(){
    
}
void __attribute__((interrupt)) Int6Handler(){
   
}
void __attribute__((interrupt)) Int7Handler(){
    //systemTick++;
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

// A SUA FUNÇÃO DE LEITURA FICA ASSIM:
unsigned long get_system_tick(void) {
    unsigned long tick;
    unsigned int status_antigo;
    
    status_antigo = m68k_disable_level2_perfect(); // Salva o estado atual do SR e barra o nível 2
    tick = systemTick;                     // Copia os 32 bits em segurança
    m68k_restore_interrupts(status_antigo); // Devolve o SR exatamente como estava
    
    return tick;
}