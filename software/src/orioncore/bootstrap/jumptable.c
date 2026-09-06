#include <stdint.h>

/*
 * Popula 'count' entradas de 6 bytes a partir de 'base_addr', cada
 * entrada sendo:
 *
 *   DC.W 0x4EF9   -- opcode do JMP (xxx).L (absolute long addressing)
 *   DC.L target   -- endereco absoluto de 32 bits pra onde salta
 *
 * ATENCAO -- CONFIRME ISSO ANTES DE USAR:
 * Do jeito que voce descreveu, as 254 entradas apontam TODAS pro
 * MESMO endereco (0x80000) -- inclusive a primeira entrada, que fica
 * fisicamente EM 0x80000, aponta pra ela mesma (jump table
 * autoreferenciada). Se a intencao real for cada entrada pular pra
 * um endereco DIFERENTE (padrao classico de tabela de vetores, cada
 * slot levando a um handler distinto), veja a segunda funcao abaixo
 * (build_jump_table_sequential) -- e' so' trocar qual delas voce chama.
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

/*
 * Variante com enderecos DIFERENTES por entrada -- caso voce quisesse
 * uma tabela de vetores de verdade (entrada 0 pula pra 'first_target',
 * entrada 1 pula pra 'first_target + stride', etc). Nao usada pelo
 * pedido original, deixada aqui so' de referencia caso seja o que
 * voce realmente precisava.
 */
//void build_jump_table_sequential(uint32_t base_addr, uint32_t first_target,
//                                  uint32_t stride, int count)
//{
//    uint8_t *p = (uint8_t *) base_addr;
//    uint32_t target = first_target;
//
//    for (int i = 0; i < count; i++) {
//        *p++ = 0x4E;
//        *p++ = 0xF9;
//        *p++ = (uint8_t)((target >> 24) & 0xFF);
//        *p++ = (uint8_t)((target >> 16) & 0xFF);
//        *p++ = (uint8_t)((target >> 8)  & 0xFF);
//        *p++ = (uint8_t)( target        & 0xFF);
//        target += stride;
//    }
//}

