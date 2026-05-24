/* No seu arquivo .ld 
 * gostei desse exemplo porque posso separar por arquivo
 *
 *
 *
 *
 *
 * */
MEMORY
{
    ROM_BASE (rx) : ORIGIN = 0x00FC0000, LENGTH = 256K
    ROM_IO   (rx) : ORIGIN = 0x00FC2000, LENGTH = 4K   /* Bloco dedicado para I/O */
}

SECTIONS
{
    /* Força TUDO que vier do arquivo rom_io.o para o endereço 0x00FC2000 */
    .secao_io : {
        rom_io.o(.text .text.*)
    } > ROM_IO

    /* O restante do código vai para a base da ROM */
    .text : {
        *(.text .text.*)
    } > ROM_BASE
}

