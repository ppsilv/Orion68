#include <stdint.h>
#include <stddef.h>


//crc32 **********************************************************
// Tabela de 1 KB gerada em tempo de execução
static uint32_t crc32_table[256];

// Polinômio padrão IEEE 802.3 (Refletido)
#define CRC32_POLYNOMIAL 0xEDB88320UL

/**
 * Inicializa a tabela de busca do CRC-32.
 * Chame esta função UMA VEZ no início do programa (main).
 */
void crc32_init(void) {
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t c = i;
        for (int k = 0; k < 8; k++) {
            c = (c & 1) ? (CRC32_POLYNOMIAL ^ (c >> 1)) : (c >> 1);
        }
        crc32_table[i] = c;
    }
}

/**
 * Atualiza o CRC a cada bloco ou byte recebido.
 * Pode ser chamado iterativamente se você receber o arquivo em partes.
 */
uint32_t crc32_update(uint32_t crc, const uint8_t *buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
        crc = crc32_table[(crc ^ buffer[i]) & 0xFF] ^ (crc >> 8);
    }
    return crc;
}

/**
 * Função para calcular o CRC-32 final de um buffer completo.
 */
uint32_t crc32_calculate(const uint8_t *buffer, size_t length) {
    // Inicia com 0xFFFFFFFF e faz o XOR final com 0xFFFFFFFF
    return crc32_update(0xFFFFFFFFUL, buffer, length) ^ 0xFFFFFFFFUL;
}

