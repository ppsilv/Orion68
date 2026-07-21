#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "orion_bus.pio.h" // Cabeçalho gerado automaticamente pelo pioasm

extern uint8_t *arquivo_buffer;
extern bool arquivo_pronto;
extern uint8_t arquivo_ok;
extern uint8_t arquivo_tamh;
extern uint8_t arquivo_tamL;
extern uint32_t arquivo_tamanho;
extern uint32_t ponteiro_leitura;

#define REG_DATA_PREPARE        0x0
#define REG_STATE_PREPARE       0x1
#define REG_SHIGH_PREPARE       0x3
#define REG_SLOW_PREPARE        0x5
#define REG_DATA_READ           0x7
#define REG_STATE_READ          0x9
#define REG_SHIGH_READ          0xB
#define REG_SLOW_READ           0xD

// Definições de bits para o status_registro (Reg 0x01)
#define STATUS_BUSY 0x00
#define STATUS_READY 0x01
 

void __not_in_flash_func(gerenciar_barramento_m68k)(PIO pio, uint sm){
    if (!pio_sm_is_rx_fifo_empty(pio, sm)) {
        sio_hw->gpio_set = (1 << 19);

        uint32_t reg = pio_sm_get_blocking(pio, sm);
        pio_sm_clear_fifos(pio, sm);
       // printf("reg [%08x]\n",reg);
        
        switch(reg){
            case 0x00:
                        pio_sm_put_blocking(pio, sm, 0xA5);
                        break;
            case 0x01:
                        pio_sm_put_blocking(pio, sm, arquivo_ok);
                        break;
            case 0x02:
                        pio_sm_put_blocking(pio, sm, arquivo_tamh);
                        break;
            case 0x03:
                        pio_sm_put_blocking(pio, sm, arquivo_tamL);
                        break;
            default:                        
                    pio_sm_put_blocking(pio, sm, 0x0A);
        } 
        sio_hw->gpio_clr = (1 << 19);
        return;



        uint8_t byte_resposta = 0x0;

        switch (reg) {
            case 0x00: // --- m68k leu 0xFF9101: REGISTRADOR DE DADOS ---
                //if (arquivo_pronto && ponteiro_leitura < arquivo_tamanho) {
                    byte_resposta = arquivo_buffer[ponteiro_leitura];
                    ponteiro_leitura++; // Avança o ponteiro do arquivo
                //} 
                break;

            case 0x01: // --- m68k leu 0xFF9103: REGISTRADOR DE STATUS ---
                if (!arquivo_pronto) {
                    byte_resposta = 0x00; // PICO_STATE_IDLE
                } else if (ponteiro_leitura < arquivo_tamanho) {
                    byte_resposta = 0x01; // PICO_STATE_HAS_FILE
                } else {
                    byte_resposta = 0x02; // PICO_STATE_EOF
                }
                break;

            case 0x02: // --- m68k leu 0xFF9105: SIZE HIGH (Byte Alto) ---
                //if (arquivo_pronto) {
                    byte_resposta = (uint8_t)((arquivo_tamanho >> 8) & 0xFF);
                //}
                break;

            case 0x03: // --- m68k leu 0xFF9107: SIZE LOW (Byte Baixo) ---
                //if (arquivo_pronto) {
                    byte_resposta = (uint8_t)(arquivo_tamanho & 0xFF);
                //}
                break;

            default:
                byte_resposta = 0xFF;
                break;
        }
        pio_sm_put(pio, sm, byte_resposta);
        sio_hw->gpio_clr = (1 << 19);

    }
}


// Função executada para responder o m68k o mais rápido possível
void gerenciar_barramento_m68k_OLD(PIO pio, uint sm) {
    // Verifica se a PIO nos enviou um pedido de endereço (RX FIFO não está vazio)
    if (!pio_sm_is_rx_fifo_empty(pio, sm)) {
        printf("Algo chegou pela pio: ");
        // Pega o valor enviado pela instrução 'in pins, 4' (Pinos A4-A1)
        uint32_t reg_selecionado = pio_sm_get(pio, sm);
        
        // Isola apenas os 4 bits que nos interessam (máscara 0x0F)
        reg_selecionado &= 0x0F; 
        printf(" reg_selecionado=[%02x]\n",reg_selecionado);
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

