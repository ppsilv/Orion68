/* orion68k.ld - Script do Linker */

MEMORY
{
    /* Definindo os blocos físicos da sua ROM */
    ROM_BASE (rx)    : ORIGIN = 0x00FC0000, LENGTH = 256K
    
    /* Blocos específicos com o endereço exato que você quer para cada função */
    ADDR_PRINT (rx)   : ORIGIN = 0x00FC1000, LENGTH = 0x100
    ADDR_KEYBOARD (rx): ORIGIN = 0x00FC1200, LENGTH = 0x100
}

SECTIONS
{
    /* O código geral do sistema e outras funções vão para o início da ROM */
    .text : {
        *(.text)
    } > ROM_BASE

    /* Força a seção da função rom_imprimir_char a ir para o endereço 0x00FC1000 */
    .sec_print : {
        *(.rom_print)
    } > ADDR_PRINT

    /* Força a seção da função rom_ler_teclado a ir para o endereço 0x00FC1200 */
    .sec_teclado : {
        *(.rom_teclado)
    } > ADDR_KEYBOARD
}
