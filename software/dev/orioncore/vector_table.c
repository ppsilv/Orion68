#include <stdint.h>
#include "vector_table.h"

/*
 * Popula 'count' entradas de 6 bytes a partir de 'base_addr', cada
 * entrada sendo:
 *
 *   DC.W 0x4EF9   -- opcode do JMP (xxx).L (absolute long addressing)
 *   DC.L target   -- endereco absoluto de 32 bits pra onde salta
 *
 * Escrevo byte a byte (nao como uint16_t/uint32_t direto) de proposito:
 * isso elimina qualquer duvida de endianness -- m68k e' big-endian,
 * entao mesmo compilando nativo pra m68k isso já sairia certo, mas
 * assim fica explicito e' facil de revisar sem pensar duas vezes.
 */
void build_jump_table(uint32_t base_addr, uint32_t target_addr, int count)
{
    uint8_t *p = (uint8_t *) base_addr;
    //RTE = 0x4E73  0x000805FA=0x4E 0x000805FB=0x73
    for (int i = 0; i < count; i++) {
        /* DC.W 0x4EF9 -- opcode JMP absoluto de 32 bits */
        *p++ = 0x4E;
        *p++ = 0xF9;

        /* DC.L target_addr -- endereco absoluto, big-endian 0x000805FA */
        *p++ = (uint8_t)((target_addr >> 24) & 0xFF);
        *p++ = (uint8_t)((target_addr >> 16) & 0xFF);
        *p++ = (uint8_t)((target_addr >> 8)  & 0xFF);
        *p++ = (uint8_t)( target_addr        & 0xFF);
    }
    //Address  0x000805FA = RTE
    p = (uint8_t *)0x000805FA;
    *p++ = 0x4E;
    *p++ = 0x73;
}

#include <stdint.h>

#define VETOR_BASE  0x00080000UL
#define VETOR_STRIDE 6

/**
 * Atualiza o endereço alvo de um vetor específico na tabela de jump
 * 
 * @param vetor     Índice do vetor (0-based)
 * @param funcao    Endereço da função destino (32 bits)
 * 
 * Exemplo: setaVetorFuncao(1, 0x000A00FF) 
 *          Atualiza o vetor 1 (endereço 0x80006) com o alvo 0x000A00FF
 * Use: 
 *      setaVetorFuncao(uint8_t vetor, uint32_t funcao);
 */
void setaVetorFuncao(uint8_t vetor, uint32_t funcao) {
    /* Calcula o endereço base do vetor */
    uint32_t endereco_vetor = VETOR_BASE + (vetor * VETOR_STRIDE);
    
    /* Ponteiro para a área de memória */
    uint8_t *p = (uint8_t *)endereco_vetor;
    
    /* Os bytes 0x4E e 0xF9 (opcode JMP) permanecem inalterados,
       mas vamos reposicionar o ponteiro após o opcode para escrever o endereço */
    p += 2;  /* Pula os bytes 4E F9 */
    
    /* Escreve o endereço em big-endian (32 bits) */
    *p++ = (uint8_t)((funcao >> 24) & 0xFF);
    *p++ = (uint8_t)((funcao >> 16) & 0xFF);
    *p++ = (uint8_t)((funcao >> 8)  & 0xFF);
    *p++ = (uint8_t)( funcao        & 0xFF);
}

/*
 * Variante com enderecos DIFERENTES por entrada -- caso voce quisesse
 * uma tabela de vetores de verdade (entrada 0 pula pra 'first_target',
 * entrada 1 pula pra 'first_target + stride', etc). Nao usada pelo
 * pedido original, deixada aqui so' de referencia caso seja o que
 * voce realmente precisava.
 *
void build_jump_table_sequential(uint32_t base_addr, uint32_t first_target,
                                  uint32_t stride, int count)
{
    uint8_t *p = (uint8_t *) base_addr;
    uint32_t target = first_target;

    for (int i = 0; i < count; i++) {
        *p++ = 0x4E;
        *p++ = 0xF9;
        *p++ = (uint8_t)((target >> 24) & 0xFF);
        *p++ = (uint8_t)((target >> 16) & 0xFF);
        *p++ = (uint8_t)((target >> 8)  & 0xFF);
        *p++ = (uint8_t)( target        & 0xFF);
        target += stride;
    }
}
 */
/*
 * Chamada correspondente EXATAMENTE ao que voce pediu:
 * 254 entradas, a partir de 0x80000, todas apontando pra 0x80000.
 */
void populate_orioncore_jumptable(void)
{
    build_jump_table(0x00080000UL, 0x00080000UL, 254);
}