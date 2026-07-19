#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "orion_bus.pio.h" // Cabeçalho gerado automaticamente pelo pioasm

extern uint8_t *arquivo_buffer;
extern bool arquivo_pronto;
extern uint32_t arquivo_tamanho;
extern uint32_t ponteiro_leitura;


// Função executada para responder o m68k o mais rápido possível
void gerenciar_barramento_m68k(PIO pio, uint sm) {
    // Verifica se a PIO nos enviou um pedido de endereço (RX FIFO não está vazio)
    if (!pio_sm_is_rx_fifo_empty(pio, sm)) {
        printf("Algo chhegou pela pio\n");
        // Pega o valor enviado pela instrução 'in pins, 4' (Pinos A4-A1)
        uint32_t reg_selecionado = pio_sm_get(pio, sm);
        
        // Isola apenas os 4 bits que nos interessam (máscara 0x0F)
        reg_selecionado &= 0x0F; 

        uint8_t byte_resposta = 0x00;

        if (reg_selecionado == 0x00) {
            // --- REGISTRADOR DE STATUS (0xFF9100) ---
            if (!arquivo_pronto) {
                byte_resposta = 0x00; // Nenhum arquivo
            } else if (ponteiro_leitura < arquivo_tamanho) {
                byte_resposta = 0x01; // Arquivo pronto para leitura!
            } else {
                byte_resposta = 0x02; // Fim do arquivo atingido
            }
        } 
        else if (reg_selecionado == 0x01) {
            // --- REGISTRADOR DE DADOS (0xFF9102) ---
            if (arquivo_pronto && ponteiro_leitura < arquivo_tamanho) {
                // Pega o byte atual da RAM recebida pelo Wi-Fi
                byte_resposta = arquivo_buffer[ponteiro_leitura];
                ponteiro_leitura++; // Avança o ponteiro para o próximo byte
            } else {
                byte_resposta = 0xFF; // Fallback caso tente ler além do fim
            }
        }

        // Devolve a resposta instantaneamente para o TX FIFO da PIO cuspir no barramento
        printf("Enviando para o m68k[%02x]\n",byte_resposta);
        pio_sm_put(pio, sm, byte_resposta);
    }
}

