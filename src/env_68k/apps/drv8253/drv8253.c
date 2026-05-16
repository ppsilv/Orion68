#include <stdio.h>
#include <stdlib.h>

#include <mc68000.h>

#include "drv8253.h"

#define LEDS_ADDRESS 0x4400
#define LEDS (*(volatile unsigned char *)LEDS_ADDRESS)
uint16_t timer_last_value=0;

void delay(unsigned int time) {
    for (volatile unsigned int i = 0; i < time; i++);
}

// Função para verificar stack
void check_stack(void) {
    unsigned long stack_val;
    asm volatile (
        "move.l %%sp, %0\n\t"
        : "=r" (stack_val)
    );
    printf("Stack pointer: 0x%08X\n", stack_val);
}

void led_effects() {
    // Efeito de "carrinho"
    unsigned char patterns[] = {
        0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
        0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
    };

    for (int i = 0; i < 15; i++) {
        LEDS = patterns[i];
        delay(30000);
    }

    // Efeito de intensidade crescente
    for (int i = 0; i < 8; i++) {
        LEDS |= (1 << i);
        delay(20000);
    }

    for (int i = 0; i < 8; i++) {
        LEDS &= ~(1 << i);
        delay(20000);
    }
}

void timer_init(void) {
    // Configurar canal 0 como modo 3 (square wave), leitura/escrita de 16-bit, binary
    TIMER_CTRL = TIMER_SEL0 | TIMER_RW_BOTH | TIMER_MODE3 | TIMER_BINARY;

    // Inicializar contador com valor máximo temporariamente
    TIMER0_COUNT = 0xFF;  // LSB
    TIMER0_COUNT = 0xFF;  // MSB
}


void timer_set_10ms_tick(void) {
    // Calcular valor do contador para 10ms com clock de 1MHz
    // Count = (clock_freq * period) = 1000000 * 0.01 = 10000
    uint16_t count_value = 2000;

    // Configurar canal 0: modo 3, 16-bit, binary
    TIMER_CTRL = TIMER_SEL0 | TIMER_RW_BOTH | TIMER_MODE3 | TIMER_BINARY;

    // Escrever valor do contador (LSB primeiro, depois MSB)
    TIMER0_COUNT = count_value & 0xFF;        // LSB
    TIMER0_COUNT = (count_value >> 8) & 0xFF; // MSB
}
uint16_t timer_read_count(void) {
    uint16_t count;
    uint8_t lsb, msb;

    // Comando latch para congelar o valor atual do contador
    TIMER_CTRL = TIMER_SEL0 | TIMER_RW_BOTH;

    // Ler LSB primeiro, depois MSB
    lsb = TIMER0_COUNT;
    msb = TIMER0_COUNT;

    count = (msb << 8) | lsb;

    return count;
}
// Função para verificar se o timer está rodando
uint8_t timer_is_running(void) {
    uint16_t count1, count2;

    // Ler duas vezes rapidamente
    count1 = timer_read_count();
    count2 = timer_read_count();

    // Se os valores são diferentes, o timer está rodando
    return (count1 != count2);
}

// Função para medir frequência real
uint32_t timer_measure_frequency(void) {
    uint16_t start_count, end_count;
    uint32_t elapsed_ticks;

    start_count = timer_read_count();

    // Esperar um tempo conhecido (aproximadamente 100ms)
    // Você precisa implementar um delay preciso baseado em loops
    for (volatile int i = 0; i < 10000; i++);

    end_count = timer_read_count();

    // Calcular ticks por segundo (assumindo delay de ~100ms)
    if (end_count < start_count) {
        // Overflow ocorreu
        elapsed_ticks = (0xFFFF - start_count) + end_count;
    } else {
        elapsed_ticks = start_count - end_count;
    }

    return elapsed_ticks * 10;  // Convert to Hz (100ms → *10 para 1s)
}

// Função para parar o timer (8253 style)
void timer_stop(void) {
    // Para o 8253, "parar" significa configurar modo 0 com contador = 0
    TIMER_CTRL = TIMER_SEL0 | TIMER_RW_BOTH | TIMER_MODE0 | TIMER_BINARY;
    TIMER0_COUNT = 0x00;  // LSB
    TIMER0_COUNT = 0x00;  // MSB
}

// Função para continuar (reiniciar com último valor)
void timer_continue(void) {
    // Reconfigurar para modo 3 (square wave) com último valor
    TIMER_CTRL = TIMER_SEL0 | TIMER_RW_BOTH | TIMER_MODE3 | TIMER_BINARY;
    TIMER0_COUNT = timer_last_value & 0xFF;        // LSB
    TIMER0_COUNT = (timer_last_value >> 8) & 0xFF; // MSB
}

// Função para iniciar com valor específico
void timer_start(uint16_t value) {
    TIMER_CTRL = TIMER_SEL0 | TIMER_RW_BOTH | TIMER_MODE3 | TIMER_BINARY;
    TIMER0_COUNT = value & 0xFF;        // LSB
    TIMER0_COUNT = (value >> 8) & 0xFF; // MSB
    timer_last_value = value;
}
// Função para pausar e continuar (toggle)
void timer_pause_resume(void) {
    static uint8_t paused = 0;
    static uint16_t saved_value = 0;

    if (!paused) {
        // Pausar: salvar valor e parar
        saved_value = timer_read_count();
        timer_stop();
        paused = 1;
    } else {
        // Continuar: restaurar valor
        TIMER_CTRL = TIMER_SEL0 | TIMER_RW_BOTH | TIMER_MODE3 | TIMER_BINARY;
        TIMER0_COUNT = saved_value & 0xFF;
        TIMER0_COUNT = (saved_value >> 8) & 0xFF;
        paused = 0;
        timer_set_10ms_tick();

    }
}
// Rotina que configura o canal 0 para gerar um pulso a cada 15 segundos
// Configuração REAL para 15 segundos usando cascateamento
void timer_15s_config(void) {
    // Canal 0: Divide por 10000 = 100Hz (10ms)
    TIMER_CTRL = TIMER_SEL0 | TIMER_MODE2 | TIMER_RW_BOTH | TIMER_BINARY;
    TIMER0_COUNT = 10000;

    // Canal 1: Conta 1500 pulsos do canal 0 (100Hz * 15s = 1500)
    TIMER_CTRL = TIMER_SEL1 | TIMER_MODE0 | TIMER_RW_BOTH | TIMER_BINARY;
    TIMER1_COUNT = 1500;
}


// Função para ler o canal 0 - 8253 NÃO TEM LATCH COMMAND!
uint16_t timer_read_channel0(void) {
    // No 8253, precisamos parar a contagem para ler corretamente
    // Salvamos a configuração atual
    uint8_t original_config = TIMER_CTRL;

    // Congela a contagem escrevendo um latch não oficial
    TIMER_CTRL = TIMER_SEL0 | TIMER_RW_LSB;

    // Lê LSB e MSB
    uint8_t lsb = TIMER0_COUNT;
    uint8_t msb = TIMER0_COUNT;

    // Restaura a configuração original
    TIMER_CTRL = original_config;

    return (msb << 8) | lsb;
}
int main() {
    char c='0';
    uint16_t count=0;

    check_stack();  // ✅ Verificar stack no início

    LEDS = 0x01;

    printf("MC68000  DRV 8253\n");

    puts("Chamando init do 8253\n");
    timer_init();
    puts("Configurando o 8253 para um tick 10ms\n");
    timer_set_10ms_tick();

    while(1){
        printf("\n1 - Read counter \n");
        printf("2 - Stop timer \n");
        printf("3 - Restart timer \n");
        printf("4 - Timer pause/resume.... \n");
        printf("5 - Config 4 x minuto.... \n");
        printf("9 - Termina app.... \n");
        c = getchar();
        printf("Opção escolhida %c\n",c);
        if ( c == '9' ){
            break;
        }
        switch(c){
            case '1':
                count = timer_read_count();
                printf("Count [%08X]\n",count);
                break;
            case '2':
                timer_stop();
                break;
            case '3':
                timer_init();
                timer_set_10ms_tick();
                break;
            case '4':
                timer_pause_resume();
                break;
            case '5':
                timer_init();
                timer_15s_config();
                break;
        }
    }

    led_effects();

    LEDS = 0xFF;    // Todos acesos
    delay(50000);
    LEDS = 0x00;    // Todos apagados
    delay(50000);

    return 0;
}


