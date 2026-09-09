/* ============================================================
 * ls.c - Lista arquivos do diretorio (FAT) para o oriondos
 * Projeto Orion68
 *
 * Uso:
 *   ls              -> lista o diretorio atual, sem ocultos/sistema
 *   ls -a           -> lista tudo, incluindo HID e SYS
 *   ls caminho      -> lista o diretorio informado
 *   ls -a caminho   -> combina os dois
 * ============================================================ */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <fatfs/ff.h>
#include "fsattrib.h"

FATFS FatFs;

void vputs(const char *s);

/* Formata o tamanho em decimal manualmente (sem %lu/snprintf,
 * mesma convencao usada no orionbur) */
static void fmt_dec_u320(uint32_t v, char *out)
{
    char tmp[10];
    int8_t n = 0;
    int8_t i;

    if (v == 0) {
        out[0] = '0';
        out[1] = '\0';
        return;
    }

    while (v > 0 && n < 10) {
        tmp[n++] = (char) ('0' + (v % 10));
        v /= 10;
    }

    for (i = 0; i < n; i++) {
        out[i] = tmp[n - 1 - i];
    }
    out[n] = '\0';
}
static void fmt_dec_u32(uint32_t v, char *out, int8_t width)
{
    char tmp[10];
    int8_t n = 0;
    int8_t i;
    int8_t pad;

    if (v == 0) {
        tmp[n++] = '0';
    } else {
        while (v > 0 && n < 10) {
            tmp[n++] = (char) ('0' + (v % 10));
            v /= 10;
        }
    }

    pad = width - n;
    if (pad < 0) pad = 0;

    for (i = 0; i < pad; i++) {
        out[i] = ' ';
    }

    for (i = 0; i < n; i++) {
        out[pad + i] = tmp[n - 1 - i];
    }
    out[pad + n] = '\0';
}
static int list_dir(const char *path, int show_hidden)
{
    DIR dir;
    FILINFO fno;
    FRESULT fr;
    char attrbuf[ATTR_TABLE_LEN + 1];
    char sizebuf[11];
    int total_files = 0;

    fr = f_opendir(&dir, path);
    if (fr != FR_OK) {
        printf("ls: nao foi possivel abrir '%s'\n", path);
        return 1;
    }

    for (;;) {
        fr = f_readdir(&dir, &fno);
        if (fr != FR_OK || fno.fname[0] == 0) {
            break; /* fim da listagem ou erro */
        }

        if (!show_hidden && (fno.fattrib & (AM_HID | AM_SYS))) {
            continue; /* pula ocultos/sistema, como o DOS sem /a */
        }

        fmt_attr_string(fno.fattrib, attrbuf);

        if (fno.fattrib & AM_DIR) {
            printf("%s  <DIR>       %s\n", attrbuf, fno.fname);
        } else {
            fmt_dec_u32((uint32_t) fno.fsize, sizebuf,6);
            printf("%s  %10s  %s\n", attrbuf, sizebuf, fno.fname);
        }

        total_files++;
    }

    f_closedir(&dir);

    printf("\n%d item(s)\n", total_files);
    return 0;
}
static void ideinit()
{
    FRESULT fr;
    fr = f_mount(&FatFs, "", 0);
    if (fr != FR_OK) {
        printf("PANIC: Erro ao montar FAT\n");
    }else{
        printf(": FAT success mounted!\n");
    }
}
int main(int argc, char *argv[])
{
    const char *path = ".";
    int show_hidden = 0;
    int i;
    vputs("Calling ideinit\n");
    ideinit();
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            show_hidden = 1;
        } else {
            path = argv[i];
        }
    }

    printf("RHSA  %-10s  Nome\n", "Tamanho");
    return list_dir(path, show_hidden);
}
