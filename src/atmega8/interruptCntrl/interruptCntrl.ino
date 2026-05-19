#include <avr/io.h>
#include <util/atomic.h>
#include <util/delay.h>

//When cpu say FC0,FC1 and FC2  = 1,1,1 but no interrupt active.
#define CPU_INT_ACK_WRONG     0x80
//When arrive a irq resquest but we have one active irq in curse
#define IRQ_REQUEST_LOST      0x81
//ACK sem interrupção
#define ACK_WITHOUT_INT       0x82
//Irq <level> must be released
#define IRQ_MUST_BE_RELEASED  0x83

// Definições dos pinos IPL no PORTB
#define IPL0  PC0
#define IPL1  PC1  
#define IPL2  PC2
#define IPL_MASK ((1 << IPL0) | (1 << IPL1) | (1 << IPL2))

// Definições dos pinos de IRQ no PORTD
#define IRQ1_PIN  PD2
#define IRQ2_PIN  PD3
#define IRQ3_PIN  PD4
#define IRQ4_PIN  PD5
#define IRQ5_PIN  PD6
#define IRQ7_PIN  PD7
#define ALL_IRQ_MASK ((1<<IRQ1_PIN)|(1<<IRQ2_PIN)|(1<<IRQ3_PIN)|(1<<IRQ4_PIN)|(1<<IRQ5_PIN)|(1<<IRQ7_PIN))

#define TIMEOUT  1000
long timer1=millis()+100;
long timer2=millis()+TIMEOUT;

#define NOT_ACTIVE 7
unsigned char irq_s[8]={NOT_ACTIVE,NOT_ACTIVE,NOT_ACTIVE,NOT_ACTIVE,NOT_ACTIVE,NOT_ACTIVE,NOT_ACTIVE,NOT_ACTIVE};
// Variáveis globais
volatile uint8_t active_irq = NOT_ACTIVE;

#define LED_PIN PB5  // Exemplo: LED no PB5 (ajuste conforme seu hardware)
#define LED_PORT PORTB

// Configuração dos ports
void setup_ports() {
    // Configurar PC0-PC2 como saídas (IPL0-IPL2)
    DDRC |= (1 << IPL0) | (1 << IPL1) | (1 << IPL2) ;
    // VPA inicialmente inativo    
    PORTC |=  (1 << IPL0) | (1 << IPL1) | (1 << IPL2) ;  
    
    // Configurar PB1 como saída (OC1A - Debug) LED
    DDRB |= (1 << PB1)|(1 << PB5)  ;

    // Configura PD0-PD5 como entradas com pull-up
    DDRD &= ~ALL_IRQ_MASK;
    PORTD |= ALL_IRQ_MASK;    
}

/*
  Gerando o sinal de 100HZ no pino PB1
  Usado para gerar interrupção 7 no 68000
*/
void setup_timer1() {
    TCCR1A = (1 << COM1A0);    // Toggle OC1A on compare match
    TCCR1B = (1 << WGM12) | (1 << CS11)  | (1 << CS10);  // CTC mode, prescaler 8
    OCR1A = 1249;              // 1ms period
    TIMSK |= (1 << OCIE1A);    // Habilitar interrupção
}
//Dummy counter, not used
volatile uint32_t ms_counter = 0;
ISR(TIMER1_COMPA_vect) {
  ms_counter++;
  //start_irq(1);  // Gera IRQ nível 6
}

//Timer 2 not used
void setup_timer2() {  // ATMega8 usa Timer2, não Timer0
    // Configurar Timer2 para CTC mode
    TCCR2 = (1 << WGM21);              // CTC mode
    OCR2 = 78;                         // 50us @ 16MHz/8
    TCCR2 |= (1 << CS21);              // Prescaler 8
    TIMSK |= (1 << OCIE2);             // Enable interrupt
}
ISR(TIMER2_COMP_vect) {

}

//int0 not used
void setup_fc_interrupt() {
    // Configurar INT0 para FALLING EDGE (transição 1→0)
    MCUCR |= (1 << ISC01);      // Falling edge generates interrupt  
    MCUCR &= ~(1 << ISC00);     // Clear ISC00 bit
    GICR |= (1 << INT0);        // Enable INT0
}
ISR(INT0_vect) {

}

uint8_t check_irq_pins_status() {
    return (~PIND) & ALL_IRQ_MASK;
}
/*
  To start:
          int7 send 0
          int6 send 1
          int5 send 2
          int4 send 3
          int3 send 4
          int2 send 5
          int1 send 6
       no int  send 7 
*/
uint8_t get_irq_started() {
    uint8_t status = check_irq_pins_status();
    if (status & (1<<IRQ7_PIN)) return 0;
    if (status & (1<<IRQ5_PIN)) return 2;
    if (status & (1<<IRQ4_PIN)) return 3;
    if (status & (1<<IRQ3_PIN)) return 4;
    if (status & (1<<IRQ2_PIN)) return 5;
    if (status & (1<<IRQ1_PIN)) return 6;
    return NOT_ACTIVE;
}

// ✅ Função set and reset IPLx
void set_ipl(uint8_t level) {
  level &= 0x07;
  PORTC = (PORTC & 0xF8) | level;  // Seta nível
  // ⏰ Timing calibrado manualmente
    __asm__ volatile("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop");

  PORTC = (PORTC & 0xF8) | NOT_ACTIVE;  // Limpa ANTES do retrigger
  _delay_us(500);
  irq_s[active_irq]=NOT_ACTIVE;
}

void start_irq(uint8_t new_irq){
  if(new_irq == NOT_ACTIVE) return; //excesso de zelo quando estiver funcionando corretamente tirar isso daqui
  if (new_irq < 7 && irq_s[new_irq] == NOT_ACTIVE) {
    active_irq = new_irq;
    irq_s[active_irq]=active_irq;
    set_ipl(active_irq);
  }
}

void setup(){
  Serial.begin(9600);
  Serial.println("Teste do gerenciador de interrupção 1.0"); 
  setup_ports();

  //Setup interrups
  setup_timer1();
  //setup_timer2();

  sei();
  pinMode(LED_BUILTIN, OUTPUT);    
  timer1=millis()+TIMEOUT;
  delay(0.100); // 100 mili segundos
}

void loop() {

  while(1) {
    // 1️⃣ Verificação SÍNCRONA das IRQs
    start_irq(get_irq_started());

    //Activity led
    if ( (millis() > timer1) ){
      timer1=millis()+500;
      LED_PORT ^= (1 << LED_PIN);  // toggle LED
    }
  }
}
