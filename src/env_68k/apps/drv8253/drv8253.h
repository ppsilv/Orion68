#include <stdint.h>

#define TIMER_BASE 0x4600

// Registradores do 8253 (Canal 0)
#define TIMER0_COUNT   (*(volatile uint8_t *)(TIMER_BASE + 0x00))  // A0=0
#define TIMER1_COUNT   (*(volatile uint8_t *)(TIMER_BASE + 0x02))  // A0=0
#define TIMER2_COUNT   (*(volatile uint8_t *)(TIMER_BASE + 0x04))  // A0=0
#define TIMER_CTRL     (*(volatile uint8_t *)(TIMER_BASE + 0x06))  // A0=0

// Bits de controle do 8253
#define TIMER_SEL0     (0 << 6)  // Select channel 0
#define TIMER_SEL1     (1 << 6)  // Select channel 1
#define TIMER_SEL2     (2 << 6)  // Select channel 2
#define TIMER_RW_LSB   (1 << 4)  // Read/Write LSB only
#define TIMER_RW_MSB   (2 << 4)  // Read/Write MSB only
#define TIMER_RW_BOTH  (3 << 4)  // Read/Write LSB then MSB
#define TIMER_MODE0    (0 << 1)  // Mode 0: Interrupt on terminal count
#define TIMER_MODE2    (2 << 1)  // Mode 2: Rate generator
#define TIMER_MODE3    (3 << 1)  // Mode 3: Square wave generator
#define TIMER_BCD      (1 << 0)  // BCD counting
#define TIMER_BINARY   (0 << 0)  // Binary counting

// Novo macro para configuração de pulso de 15 segundos
#define TIMER0_15S_PULSE_CONFIG() \
    timer_init_channel0(0xFFFF, TIMER_MODE0 | TIMER_RW_BOTH | TIMER_BINARY)
uint16_t timer_read_channel0(void);

// Protótipo da função de pulso longo
void timer_15s_pulse(void);

// Frequência do clock do 8253 (ajuste conforme seu hardware)
#define TIMER_CLK_FREQ 1000000   // 1 MHz (comum para 8253)
