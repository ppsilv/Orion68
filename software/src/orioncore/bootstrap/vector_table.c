#include <stdint.h>

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

    for (int i = 0; i < count; i++) {
        /* DC.W 0x4EF9 -- opcode JMP absoluto de 32 bits */
        *p++ = 0x4E;
        *p++ = 0xF9;

        /* DC.L target_addr -- endereco absoluto, big-endian */
        *p++ = (uint8_t)((target_addr >> 24) & 0xFF);
        *p++ = (uint8_t)((target_addr >> 16) & 0xFF);
        *p++ = (uint8_t)((target_addr >> 8)  & 0xFF);
        *p++ = (uint8_t)( target_addr        & 0xFF);
    }
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