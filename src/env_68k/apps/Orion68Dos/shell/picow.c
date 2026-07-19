#include <stdint.h>
#include <stdbool.h>

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
uint16_t receber_arquivo_do_pico(uint8_t *destino_ram) {
    // 1. Polling: Aguarda até que o Pico mude o status para "HAS_FILE" (0x01)
    // Se o status for IDLE (0x00), o m68k fica preso aqui esperando o Wi-Fi
    while (PICO_STATUS_REG == PICO_STATE_IDLE) {
        // Loop de espera ativa (Polling)
        // O compilador manterá a leitura física por causa do 'volatile'
    }

    // Se o Pico respondeu com algo diferente ou deu EOF direto, aborta
    if (PICO_STATUS_REG != PICO_STATE_HAS_FILE) {
        return 0;
    }

    // 2. Captura o tamanho do arquivo enviado pelo Pico (16 bits fatiados em 2 bytes)
    uint16_t tamanho_arquivo = ((uint16_t)PICO_SIZE_HIGH << 8) | PICO_SIZE_LOW;
    
    if (tamanho_arquivo == 0) {
        return 0;
    }

    // 3. Loop de leitura dos dados do arquivo
    for (uint16_t i = 0; i < tamanho_arquivo; i++) {
        
        // Defesa extra: se o Pico bater em EOF antes do esperado, interrompe
        if (PICO_STATUS_REG == PICO_STATE_EOF) {
            return i; // Retorna a quantidade de bytes que conseguiu ler
        }
        
        // Lê o byte do registrador de dados do Pico e joga na RAM do m68k
        // Cada leitura aqui dispara o ciclo de barramento que a PIO do Pico está esperando
        destino_ram[i] = PICO_DATA_REG;
    }

    return tamanho_arquivo;
}