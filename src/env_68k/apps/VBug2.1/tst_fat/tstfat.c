#include <stdio.h>
#include <stdint.h>
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
extern int ata_read_sector(int sector, char *buffer);

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

void abrir_arquivo(){
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