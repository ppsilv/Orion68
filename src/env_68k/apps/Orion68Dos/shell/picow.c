#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "timers.h"

// Definição dos registradores mapeados na memória do m68k
// Usamos 'volatile uint8_t' para obrigar o m68k a ler o hardware toda vez
#define PICO_DATA_REG    (*(volatile uint8_t *)0xFF9101)
#define PICO_STATUS_REG  (*(volatile uint8_t *)0xFF9103)
#define PICO_SIZE_HIGH   (*(volatile uint8_t *)0xFF9105)
#define PICO_SIZE_LOW    (*(volatile uint8_t *)0xFF9107)

// Estados do STATUS REGISTER que definimos na PIO do Pico
#define PICO_STATE_IDLE       0x00
#define PICO_STATE_HAS_FILE   0x01
#define PICO_STATE_EOF        0x02

/**
 * Faz o polling do Pico 2 W e, se houver um arquivo pronto,
 * captura o tamanho e descarrega os bytes para um buffer na RAM do m68k.
 * 
 * @param destino_ram Ponteiro para onde o arquivo deve ser copiado na RAM do m68k
 * @return uint16_t O tamanho do arquivo recebido (0 se não houver arquivo)
 */
 extern void _delay_ms();
uint16_t receber_arquivo_do_pico(uint8_t *destino_ram, uint8_t preg) {
    volatile uint8_t *reg = (volatile uint8_t *)(0xFF9100 + preg);
    printf("reg=[%08X]\n",reg);
    uint8_t status = *reg; //PICO_STATUS_REG;
    // 1. Polling: Aguarda até que o Pico mude o status para "HAS_FILE" (0x01)
    // Se o status for IDLE (0x00), o m68k fica preso aqui esperando o Wi-Fi
   // while ( status == PICO_STATE_IDLE) {
   //     status = PICO_STATUS_REG;
   // }
    // Se o Pico respondeu com algo diferente ou deu EOF direto, aborta
   // if (status < 1 ) {
   //     return 0;
   // }
    printf("Pico respondeu[%02x]\n",status);
return 1;
    delay10ms(1);
    // 2. Captura o tamanho do arquivo enviado pelo Pico (16 bits fatiados em 2 bytes)
    uint16_t tamanho_arquivo=0;
    uint8_t tamanho_high = PICO_SIZE_HIGH;
    delay10ms(1);
    tamanho_high = PICO_SIZE_HIGH;
    printf("Tamanho HIGH do arquivo=[%04x]\n",tamanho_high);
    delay10ms(1);
    uint8_t tamanho_low = PICO_SIZE_LOW;
    delay10ms(1);
    tamanho_low = PICO_SIZE_LOW;
    printf("Tamanho low do arquivo=[%04x]\n",tamanho_low);
    delay10ms(1);
    
    tamanho_arquivo = (tamanho_high <<8) | tamanho_low;

    printf("Tamanho do arquivo=[%04x][%04d]\n",tamanho_arquivo,tamanho_arquivo);
    if (tamanho_arquivo == 0) {
        return 0;
    }
    PICO_DATA_REG;
    delay10ms(1);
    PICO_DATA_REG;
    delay10ms(1);

    printf("Reading file...\n");
    // 3. Loop de leitura dos dados do arquivo
    for (uint16_t i = 0; i < tamanho_arquivo+1; i++) {       
        destino_ram[i] = PICO_DATA_REG; 
        _delay_ms();
    }

    return tamanho_arquivo;
}
