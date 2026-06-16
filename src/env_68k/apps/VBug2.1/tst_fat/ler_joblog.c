#include <stdio.h>
#include <stdint.h>
#include "ff.h"        // Cabeçalho principal da FatFs
#include "diskio.h"    // Interface de baixo nível da FatFs

// Reutilizando as variáveis globais da FatFs
extern FATFS FatFs;
FIL Arq;               // Objeto de controle do arquivo (File Object)

// Buffer temporário para ler os blocos do arquivo de texto.
// 128 bytes é um tamanho seguro e amigável para a memória do 68k na bancada.
#define BUFFER_SIZE 128
char buffer_leitura[BUFFER_SIZE];

void ler_e_exibir_joblog(void) {
    FRESULT res;
    UINT bytes_lidos;

    printf("\n--- Abrindo Arquivo JOBLOG.TXT ---\n");

    // f_open(Objeto_File, Caminho, Modo_de_Acesso)
    // FA_READ: Abre o arquivo apenas para leitura
    res = f_open(&Arq, "/START.BAK", FA_READ);
    
    if (res != FR_OK) {
        // Se retornar erro 4 (FR_NO_FILE), o arquivo não existe na raiz do cartão
        printf("Erro ao abrir /START.BAK. Codigo: %d\n", res);
        //return;
    }
    res = f_open(&Arq, "START.BAK", FA_READ);
    
    if (res != FR_OK) {
        // Se retornar erro 4 (FR_NO_FILE), o arquivo não existe na raiz do cartão
        printf("Erro ao abrir START.BAK. Codigo: %d\n", res);
        return;
    }
    
    printf("Arquivo aberto com sucesso! Conteudo:\n");
    printf("--------------------------------------------------\n");

    // Loop de leitura: Lê o arquivo em blocos até chegar ao fim (EOF)
    do {
        // f_read(Objeto_File, Buffer_Destino, Quantos_Bytes_Ler, Ponteiro_Bytes_Efetivamente_Lidos)
        // Subtraímos 1 de BUFFER_SIZE para garantir espaço para o '\0' terminador de string.
        res = f_read(&Arq, buffer_leitura, BUFFER_SIZE - 1, &bytes_lidos);

        if (res != FR_OK) {
            printf("\n[Erro durante a leitura do arquivo: %d]\n", res);
            break;
        }

        if (bytes_lidos > 0) {
            // Insere o terminador de string logo após o último byte lido
            // Isso evita que o printf imprima lixo que sobrou no buffer anterior
            buffer_leitura[bytes_lidos] = '\0';
            
            // Cospe o bloco de texto direto no terminal serial
            printf("%s", buffer_leitura);
        }

    } while (bytes_lidos > 0); // Enquanto ler mais que 0 bytes, o arquivo não acabou

    printf("\n--------------------------------------------------\n");

    // Obrigatório: Fechar o arquivo para liberar o objeto na FatFs
    f_close(&Arq);
    printf("Arquivo JOBLOG.TXT fechado.\n");
}