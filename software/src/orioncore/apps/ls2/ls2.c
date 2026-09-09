/* ============================================================
 * ls.c - Lista arquivos do diretorio (FAT) para o oriondos
 * Projeto Orion68
 *
 * Uso:
 *   ls              -> lista o diretorio atual, sem ocultos/sistema
 *   ls -a           -> lista tudo, incluindo HID e SYS
 *   ls -w           -> lista so os nomes, em colunas (estilo DOS /w)
 *   ls caminho      -> lista o diretorio informado
 *   ls -a -w caminho -> combina as opcoes
 * ============================================================ */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <fatfs/ff.h>
#include "fsattrib.h"

FATFS FatFs;

void vputs(const char *s);

/* Largura de coluna do modo -w e quantas colunas cabem numa tela
 * de 80 colunas (igual ao "dir /w" do DOS: 5 colunas de 16) */
#define WIDE_COL_WIDTH 16
#define WIDE_COLS      (80 / WIDE_COL_WIDTH)

/* Formata o tamanho em decimal manualmente (sem %lu/snprintf,
 * mesma convencao usada no orionbur) */
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

/* Monta o nome pro modo -w: diretorios entre [colchetes], como o
 * DOS faz. Trunca se nao couber na coluna, pra nao estourar o
 * buffer nem desalinhar as colunas se um nome vier maior que o
 * esperado (LFN, por exemplo). */
static void fmt_wide_nameold(const FILINFO *fno, char *out, int8_t width)
{
    int8_t maxlen = width - 1; /* deixa 1 espaco de respiro entre colunas */
    int8_t len;
    int8_t is_dir = (fno->fattrib & AM_DIR) != 0;

    len = (int8_t) strlen(fno->fname);
    if (is_dir) {
        if (len > maxlen - 2) len = maxlen - 2; /* espaco pros colchetes */
        out[0] = '[';
        memcpy(out + 1, fno->fname, len);
        out[1 + len] = ']';
        out[2 + len] = '\0';
    } else {
        if (len > maxlen) len = maxlen;
        memcpy(out, fno->fname, len);
        out[len] = '\0';
    }
}
/* Monta o nome pro modo -w: diretorios entre [colchetes], como o
 * DOS faz. Trunca se nao couber na coluna, e ja preenche com
 * espacos até 'width' porque o printf do cc65 nao tem %-*s. */
static void fmt_wide_name(const FILINFO *fno, char *out, int8_t width)
{
    int8_t maxlen = width - 1; /* deixa 1 espaco de respiro entre colunas */
    int8_t len;
    int8_t total;
    int8_t is_dir = (fno->fattrib & AM_DIR) != 0;
    int8_t i;

    len = (int8_t) strlen(fno->fname);
    if (is_dir) {
        if (len > maxlen - 2) len = maxlen - 2; /* espaco pros colchetes */
        out[0] = '[';
        memcpy(out + 1, fno->fname, len);
        out[1 + len] = ']';
        total = (int8_t) (2 + len);
    } else {
        if (len > maxlen) len = maxlen;
        memcpy(out, fno->fname, len);
        total = len;
    }

    for (i = total; i < width; i++) {
        out[i] = ' ';
    }
    out[width] = '\0';
}
static int list_dir(const char *path, int show_hidden, int wide)
{
    DIR dir;
    FILINFO fno;
    FRESULT fr;
    char attrbuf[ATTR_TABLE_LEN + 1];
    char sizebuf[11];
    char namebuf[WIDE_COL_WIDTH + 3]; /* nome + colchetes + '\0' */
    int total_files = 0;
    int8_t col = 0;

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
        if (wide) {
            fmt_wide_name(&fno, namebuf, WIDE_COL_WIDTH);
            printf("%s", namebuf);
            col++;
            if (col == WIDE_COLS) {
                //printf("\n");
                col = 0;
            }
        }else {
            fmt_attr_string(fno.fattrib, attrbuf);

            if (fno.fattrib & AM_DIR) {
                printf("%s  <DIR>       %s\n", attrbuf, fno.fname);
            } else {
                fmt_dec_u32((uint32_t) fno.fsize, sizebuf, 6);
                printf("%s  %10s  %s\n", attrbuf, sizebuf, fno.fname);
            }
        }

        total_files++;
    }

    f_closedir(&dir);

    if (wide && col != 0) {
        printf("\n"); /* fecha a ultima linha se nao deu redondo com WIDE_COLS */
    }

    printf("\n%d item(s)\n", total_files);
    return 0;
}

static void ideinit()
{
    FRESULT fr;
    fr = f_mount(&FatFs, "", 0);
    if (fr != FR_OK) {
        printf("PANIC: Erro ao montar FAT\n");
    } else {
        printf(": FAT success mounted!\n");
    }
}
void help(){
    printf("--------------------------------------\n");
    printf("Usage: ls -a = lists hidden files.\n");
    printf("       ls -w = lists files in columns.\n");
    printf("       ls -h = show help.\n");
    printf("       ls = lists files in ONE column.\n");
    printf("--------------------------------------\n");
}
int main(int argc, char *argv[])
{
    const char *path = ".";
    int show_hidden = 0;
    int wide = 0;
    int i;
    vputs("Calling ideinit\n");
    ideinit();
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            help();
            return 0;
        } else if (strcmp(argv[i], "-a") == 0) {
            show_hidden = 1;
        } else if (strcmp(argv[i], "-w") == 0) {
            wide = 1;
        } else {
            path = argv[i];
        }
    }

    if (!wide) {
        printf("RHSA  %-10s  Nome\n", "Tamanho");
    }
    return list_dir(path, show_hidden, wide);
}