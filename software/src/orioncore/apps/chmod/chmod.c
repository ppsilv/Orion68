/* ============================================================
 * chmod.c - Gerenciador de atributos de arquivo (FAT) para o oriondos
 * Projeto Orion68
 *
 * Uso:
 *   chmod arquivo.ext                 -> mostra os atributos atuais
 *   chmod +r -h -s arquivo.ext        -> liga RDO, desliga HID e SYS
 *   chmod +r+h+s arquivo.ext          -> tambem aceita colado
 *
 * Flags suportadas (mesma letra do DOS):
 *   r = AM_RDO (somente leitura)
 *   h = AM_HID (oculto)
 *   s = AM_SYS (sistema)
 *   a = AM_ARC (arquivo - "modificado desde o ultimo backup")
 * ============================================================ */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <fatfs/ff.h>
#include "fsattrib.h"


FATFS FatFs;

void vputs(const char *s);
//
//typedef struct {
//    char letra;
//    BYTE bit;
//} attr_map_t;
//
//static const attr_map_t ATTR_TABLE[] = {
//    { 'r', AM_RDO },
//    { 'h', AM_HID },
//    { 's', AM_SYS },
//    { 'a', AM_ARC },
//};
//#define ATTR_TABLE_LEN (sizeof(ATTR_TABLE) / sizeof(ATTR_TABLE[0]))

/* Converte o bit em letra maiuscula (ligado) ou '-' (desligado),
 * no estilo "dir /a" do DOS: R H S A */
//static void fmt_attr_string(BYTE attr, char *out)
//{
//    uint8_t i;
//    for (i = 0; i < ATTR_TABLE_LEN; i++) {
//        out[i] = (attr & ATTR_TABLE[i].bit)
//                     ? (char) (ATTR_TABLE[i].letra - 'a' + 'A')
//                     : '-';
//    }
//    out[ATTR_TABLE_LEN] = '\0';
//}

/* Mostra os atributos atuais de um arquivo. Retorna 0 em sucesso. */
static int show_attr(const char *path)
{
    FILINFO fno;
    FRESULT fr;
    char buf[ATTR_TABLE_LEN + 1];

    fr = f_stat(path, &fno);
    if (fr != FR_OK) {
        printf("attrib: nao foi possivel acessar '%s'\n", path);
        return 1;
    }

    fmt_attr_string(fno.fattrib, buf);
    printf("%s  %s\n", buf, path);
    return 0;
}

/* Interpreta uma string tipo "+r-h+s" e preenche 'set_mask'
 * (bits a ligar) e 'clr_mask' (bits a desligar).
 * Retorna 0 se a string era valida (so +/- seguidos de rhsa). */
static int parse_flag_arg(const char *arg, BYTE *set_mask, BYTE *clr_mask)
{
    char sinal = 0;
    uint8_t i;

    while (*arg) {
        if (*arg == '+' || *arg == '-') {
            sinal = *arg;
            arg++;
            continue;
        }

        if (sinal == 0) {
            return 1; /* letra sem +/- antes: argumento invalido */
        }

        for (i = 0; i < ATTR_TABLE_LEN; i++) {
            if (*arg == ATTR_TABLE[i].letra) {
                if (sinal == '+') {
                    *set_mask |= ATTR_TABLE[i].bit;
                } else {
                    *clr_mask |= ATTR_TABLE[i].bit;
                }
                break;
            }
        }
        if (i == ATTR_TABLE_LEN) {
            return 1; /* letra desconhecida */
        }

        arg++;
    }

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
    BYTE set_mask = 0;
    BYTE clr_mask = 0;
    const char *path = NULL;
    int i;
    FRESULT fr;

    if (argc < 2) {
        printf("Uso: attrib [+r|-r] [+h|-h] [+s|-s] [+a|-a] arquivo\n");
        printf("     attrib arquivo            (mostra atributos atuais)\n");
        return 1;
    }
    vputs("Calling ideinit\n");
    ideinit();

    /* Separa os argumentos de flag do argumento de path.
     * Convencao: qualquer arg comecando com + ou - e' flag,
     * o resto e' o caminho do arquivo (so aceita um path por vez). */
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '+' || argv[i][0] == '-') {
            if (parse_flag_arg(argv[i], &set_mask, &clr_mask)) {
                printf("attrib: argumento invalido '%s'\n", argv[i]);
                return 1;
            }
        } else {
            if (path != NULL) {
                printf("attrib: mais de um arquivo especificado\n");
                return 1;
            }
            path = argv[i];
        }
    }

    if (path == NULL) {
        printf("attrib: nenhum arquivo especificado\n");
        return 1;
    }

    /* Sem nenhuma flag: so mostra os atributos atuais */
    if (set_mask == 0 && clr_mask == 0) {
        return show_attr(path);
    }

    /* set_mask liga bits, clr_mask desliga -- nao podem se sobrepor
     * (ex: "+r-r" na mesma chamada nao faz sentido) */
    if (set_mask & clr_mask) {
        printf("attrib: flag conflitante (+ e - para o mesmo atributo)\n");
        return 1;
    }

    if (set_mask) {
        fr = f_chmod(path, set_mask, set_mask);
        if (fr != FR_OK) {
            printf("attrib: erro ao ligar atributo(s) em '%s'\n", path);
            return 1;
        }
    }

    if (clr_mask) {
        fr = f_chmod(path, 0, clr_mask);
        if (fr != FR_OK) {
            printf("attrib: erro ao desligar atributo(s) em '%s'\n", path);
            return 1;
        }
    }

    return show_attr(path);
}
