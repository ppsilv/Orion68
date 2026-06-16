#include <stdio.h>
#include <stdint.h>
#include "ff.h"        // Cabeçalho principal da FatFs
#include "diskio.h"    // Interface de baixo nível da FatFs

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

    printf("\n--- Montando Sistema de Arquivos FAT ---\n");
    
    // f_mount(Objeto, Caminho lógico, Forçar montagem imediata [1])
    res = f_mount(&FatFs, "", 1);
    if (res != FR_OK) {
        printf("Erro ao montar FAT: %d\n", res);
        return;
    }
    printf("FAT montado com sucesso!\n");
    printf("--- Listando Diretorio Raiz (/) ---\n");
    
    // Abre o diretório raiz
    res = f_opendir(&Dir, "/");
    if (res != FR_OK) {
        printf("Erro ao abrir diretorio: %d\n", res);
        return;
    }

    // Loop para ler cada entrada do diretório
    for (;;) {

        res = f_readdir(&Dir, &Fno);                   // Lê a próxima entrada
        if (res != FR_OK || Fno.fname[0] == 0) break;  // Erro ou Fim do diretório

        // Verifica se é um diretório ou um arquivo
        if (Fno.fattrib & AM_DIR) {
            printf("-[%s] ", Fno.fname);               // É uma pasta
        } else {
            printf("-%s  %ld bytes", Fno.fname, (unsigned long)Fno.fsize); // É um arquivo
        }

        res = f_readdir(&Dir, &Fno);                   // Lê a próxima entrada
        if (res != FR_OK || Fno.fname[0] == 0) break;  // Erro ou Fim do diretório

        // Verifica se é um diretório ou um arquivo
        if (Fno.fattrib & AM_DIR) {
            printf("-[%s] ", Fno.fname);               // É uma pasta
        } else {
            printf("-%s  %ld bytes", Fno.fname, (unsigned long)Fno.fsize); // É um arquivo
        }

        res = f_readdir(&Dir, &Fno);                   // Lê a próxima entrada
        if (res != FR_OK || Fno.fname[0] == 0) break;  // Erro ou Fim do diretório

        // Verifica se é um diretório ou um arquivo
        if (Fno.fattrib & AM_DIR) {
            printf("-[%s]\n", Fno.fname);               // É uma pasta
        } else {
            printf("-%s  %ld\n", Fno.fname, (unsigned long)Fno.fsize); // É um arquivo
        }

    }

    // Fecha o diretório
    f_closedir(&Dir);
    printf("-----------------------------------\n");
}
