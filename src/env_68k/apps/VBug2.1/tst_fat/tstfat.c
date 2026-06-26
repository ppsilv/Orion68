#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ff.h"        // Cabeçalho principal da FatFs
#include "diskio.h"    // Interface de baixo nível da FatFs
#include "drv_kbd.h"

// Estrutura do seu driver que guarda a partição encontrada no MBR
// (Assumindo a estrutura que você já fez funcionar)
typedef unsigned long sector_t;
struct partition {
    sector_t base;                                                                                                                               
    sector_t size;
    uint8_t fstype;
    uint8_t flags;
};
struct ata_drive {
    struct partition parts[4];
};
extern struct ata_drive drives[]; 
//extern int ata_read_sector(int sector, char *buffer);

FATFS FatFs;      // Objeto de controle do sistema de arquivos (Work area)
DIR Dir;          // Objeto de diretório
FILINFO Fno;      // Estrutura que recebe os metadados do arquivo/pasta



// Funções stubs obrigatórias para compilar o diskio.c (mantenha o básico)
//DSTATUS disk_status (BYTE pdrv) { return (pdrv == 0) ? 0 : STA_NOINIT; }
//DSTATUS disk_initialize (BYTE pdrv) { return (pdrv == 0) ? 0 : STA_NOINIT; }


void listar_diretorio_raiz(void) {
    FRESULT res;
    int i=0;
    printf("\n--- Montando Sistema de Arquivos FAT ---\n");
    res = f_mount(&FatFs, "", 1);
    if (res != FR_OK) {
        printf("Erro ao montar FAT: %d\n", res);
        return;
    }
    printf("FAT montado com sucesso!\n");
    printf("--- Listando Diretorio Raiz (/) ---\n");
    
    res = f_opendir(&Dir, "/");
    if (res != FR_OK) {
        printf("Erro ao abrir diretorio: %d\n", res);
        return;
    }

    // Agora o loop lê APENAS UMA VEZ por iteração
    for (;;) {
        res = f_readdir(&Dir, &Fno);                   // Lê a próxima entrada
        if (res != FR_OK || Fno.fname[0] == 0) break;  // Erro ou Fim do diretório

        // Ignora arquivos ocultos ou do sistema se quiser, ou imprime tudo
        if (Fno.fname[0] == '.') continue; 

        // Verifica se é um diretório ou um arquivo
        if (Fno.fattrib & AM_DIR) {
            printf(" [DIR]  %s\n", Fno.fname);
        } else {
            // Imprime o nome original e o tamanho
            printf(" FILE   %s  (%ld bytes)\n", Fno.fname, (unsigned long)Fno.fsize);
        }
        i++;
        if( i>=9 ){
            printf("Press to continue: ");
            get_char();
            i=0;
        }
    }

    f_closedir(&Dir);
    printf("-----------------------------------\n");
}

void open_dir(){
    FRESULT res;

    printf("\n--- Montando Sistema de Arquivos FAT ---\n");
    res = f_mount(&FatFs, "", 1);
    if (res != FR_OK) {
        printf("Erro ao montar FAT: %d\n", res);
        return;
    }
    printf("FAT montado com sucesso!\n");
    printf("--- Listando Diretorio Raiz (/) ---\n");
    
    res = f_opendir(&Dir, "/");
    if (res != FR_OK) {
        printf("Erro ao abrir diretorio: %d\n", res);
        return;
    }

    for (;;) {
        res = f_readdir(&Dir, &Fno);
        if (res != FR_OK || Fno.fname[0] == 0) break;

        if (!(Fno.fattrib & AM_DIR)) {
            printf("Achei o arquivo: %s. Tentando abrir...\n", Fno.fname);
            
            // Tenta abrir usando o exato ponteiro de string que veio do disco
            FIL teste_arq;
            FRESULT teste_res = f_open(&teste_arq, Fno.fname, FA_READ);
            
            if (teste_res == FR_OK) {
                printf("-> BINGO! Abriu direto da listagem!\n");
                f_close(&teste_arq);
            } else {
                printf("-> Erro mesmo direto da listagem: %d\n", teste_res);
            }
        }
    }

    f_closedir(&Dir);
    printf("-----------------------------------\n");

}

// Reutilizando as variáveis globais da FatFs
extern FATFS FatFs;
static __attribute__((aligned(2)))FIL Arq;               // Objeto de controle do arquivo (File Object)

// Buffer temporário para ler os blocos do arquivo de texto.
// 128 bytes é um tamanho seguro e amigável para a memória do 68k na bancada.
#define BUFFER_SIZE 128
char buffer_leitura[BUFFER_SIZE];

void ler_e_exibir_joblog(char * filename) {
    FRESULT res;
    UINT bytes_lidos;
    char arquivo[16];
    sprintf(arquivo,"%s",filename);

    res = f_mount(&FatFs, "", 1);
    if (res != FR_OK) {
        printf("Erro ao montar FAT: %d\n", res);
        return;
    }
    printf("FAT montado com sucesso!\n");
    printf("drives[0].parts[0].base[%d]\n",drives[0].parts[0].base);


    printf("\n--- Abrindo Arquivo %s ---\n",arquivo);

    // f_open(Objeto_File, Caminho, Modo_de_Acesso)
    // FA_READ: Abre o arquivo apenas para leitura
    memset(&Arq,0,sizeof(FIL));
    res = f_open(&Arq, arquivo, FA_READ);
    
    if (res != FR_OK) {
        // Se retornar erro 4 (FR_NO_FILE), o arquivo não existe na raiz do cartão
        printf("Erro ao abrir %s Codigo: %d\n",filename, res);
        return;
    }    
    printf("Arquivo aberto com sucesso! Conteudo:\n");
    printf("--------------------------------------------------\n");

    f_lseek(&Arq, 0);
    // Loop de leitura: Lê o arquivo em blocos até chegar ao fim (EOF)
    do {
        res = f_read(&Arq, buffer_leitura, BUFFER_SIZE - 1, &bytes_lidos);
        printf("bytes_lidos %d res %d\n",bytes_lidos,res);
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
    printf("Arquivo [%s] fechado.\n",arquivo);
}