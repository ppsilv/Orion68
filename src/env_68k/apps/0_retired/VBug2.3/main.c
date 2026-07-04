#include <stdio.h>
#include <stdlib.h>
//#include <printf.h>
#include <malloc.h>
#include <string.h>

#include "libs/fatfs/ff.h"
#include "ostimer.h"
//#include "system/syscall_file.h"
#include "shell/decodecmd.h"
#include "shell/conio.h"
#include "libs/fatfs/diskio.h"
//#include "drivers/sys/rtc.h"
//#include "drivers/sys/duart.h"
//#include "drivers/expansion.h"
//#include "drivers/display.h"
//#include "drivers/keyboard.h"

#define LINELEN 256
#define DOS_VERSION "0.3.7"

uint32_t g_timer_ticks = 0;
char g_cmd_buffer[LINELEN];

extern char _end; // Definido no seu linker.ld (final do código carregado)
static char *heap_ptr = &_end;

void* _sbrk(int incr) {
    char *prev_heap_ptr = heap_ptr;
    // Verifica se não ultrapassou a RAM (ex: 0x000FFFFF)
    if ((heap_ptr + incr) > (char*)0x000FFFFF) {
        return (void*)-1; // Out of memory
    }
    heap_ptr += incr;
    return (void*)prev_heap_ptr;
}

void display_prompt(void)
{
    char path[FF_MAX_LFN];

    f_getcwd(path, FF_MAX_LFN);
    if ('0' <= path[0] && path[0] <= '9')
        path[0] += 'A'-'0';
    printf("%s>", path);
}
FATFS FatFs;      // Objeto de controle do sistema de arquivos (Work area)
DIR Dir;          // Objeto de diretório
FILINFO Fno;      // Estrutura que recebe os metadados do arquivo/pasta

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
            get_key();
            i=0;
        }
    }

    f_closedir(&Dir);
    printf("-----------------------------------\n");
}


int main(void)
{
    // Use non OS SRAM for ROM malloc code
    malloc_init((const void *)HEAP_START, (const void *)(SRAM_END - STACK_SIZE), 256, 16, 4);

    // Ensure keyboard interrupts are disabled
    //clear_keyboard_config(0);

    //rtc_init();
    //initialise_disk();
    //init_syscall();
    //ENABLE_INTERRUPTS();

    // Initialise video card if present on bus
    //init_expansion_bus();
    //uint8_t num_boards = num_exp_boards_installed();

    printf("\nPDS317.\nA Motorola 68000 based computer\n\nOrion68DOS version %s\nBuilt on %s %s\n\n", DOS_VERSION, __DATE__, __TIME__);

    //printf("\nChecking for keyboard: ");
    DISABLE_INTERRUPTS();
    bool keyboard_present = 1; //init_keyboard();

//    if (keyboard_present)
//    {
//        cpu_delay(20000);
//        keyboard_flush();
//    }

   // ENABLE_INTERRUPTS();
//    if (keyboard_present)
//    {
//        enable_keyboard_interrupt();
//        printf("PS/2 keyboard detected\n");
//    }
//    else
//        printf("No keyboard detected\n");
//
//    printf("Checking for installed boards: ");
//    if (num_boards == 0)
//        printf("No boards installed\n");
//    else
//    {
//        if (num_boards == 1)
//            printf("%d board installed\n", num_boards);
//        else
//            printf("%d boards installed\n", num_boards);
//
//        for (int i = 0; i < MAX_EXP_DEVICES; i++)
//        {
//            if (is_exp_board_present(i))
//                printf("    Slot %d: %s\n", i+1, get_expid_description(i));
//        }
//    }
//    rtc_print_time();
//    printf("\n");

    // set the initial drive letter
    strcpy(g_cmd_buffer, "0:");
    execute_cmd(g_cmd_buffer);

    display_prompt();
//    enable_cursor();

    listar_diretorio_raiz();
    while (1)
    {
        if (getline(g_cmd_buffer, LINELEN) != -1)
        {
//            disable_cursor();
            execute_cmd(g_cmd_buffer);

            display_prompt();
//            enable_cursor();
        }

//        update_timers();
    }

	return 0;
}

