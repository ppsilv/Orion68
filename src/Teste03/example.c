// rotinas.c

// Dizemos que essa função pertence à seção ".rom_print"
__attribute__((section(".rom_print"))) 
void rom_imprimir_char(char c) {
    // Seu código de envio serial / vídeo aqui
}

// Dizemos que essa função pertence à seção ".rom_teclado"
__attribute__((section(".rom_teclado"))) 
char rom_ler_teclado(void) {
    return 'A';
}
