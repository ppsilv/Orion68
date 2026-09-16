Sim — a FatFs tem `f_chmod()`, que seta os atributos do
FAT (os mesmos bits do DOS/Windows: `AM_RDO`, `AM_HID`, `AM_SYS`, `AM_ARC`).

FRESULT f_chmod (
    const TCHAR* path,   /* Caminho do arquivo/diretório */
    BYTE attr,           /* Atributos a setar (dos bits abaixo) */
    BYTE mask            /* Máscara: quais bits devem ser alterados */
);


Pra marcar como somente-leitura:


f_chmod("firmware.bin", AM_RDO, AM_RDO);
```

O `mask` existe pra você poder mudar só um bit específico sem mexer nos
outros — por exemplo, se quisesse também ligar `AM_HID` (esconder) ao mesmo
tempo:


f_chmod("firmware.bin", AM_RDO | AM_HID, AM_RDO | AM_HID);


**  Ponto importante pro seu caso:
**  o bit `AM_RDO` (read-only) é uma convenção respeitada pelo
**  driver/sistema de arquivos que consulta o atributo antes de agir
**  — não é uma trava de hardware. Ou seja, protege contra deleção acidental
    só se a sua implementação de f_unlink()
    comando de delete no shell do oriondos checar esse bit e recusar a operação.
    A própria FatFs, por padrão, já faz isso: f_unlink() retorna FR_DENIED
    se o arquivo tiver `AM_RDO setado — então, se você não mudou esse
    comportamento no ff.c, já ganha a proteção de graça.

Vale conferir se seu shell chama f_unlink() diretamente (aí a proteção já vale)
ou se tem alguma camada própria que ignora o retorno de erro.



Tira o atributo antes de apagar, em duas chamadas:


f_chmod("MSDOS.SYS", 0, AM_RDO | AM_HID | AM_SYS);
f_unlink("MSDOS.SYS");

f_chmod("IO.SYS", 0, AM_RDO | AM_HID | AM_SYS);
f_unlink("IO.SYS");

f_chmod("DRVSPACE.BIN", 0, AM_RDO | AM_HID | AM_SYS);
f_unlink("DRVSPACE.BIN");

O primeiro argumento de f_chmod depois do path é attr (o que você quer ligar) e o
segundo é mask (quais bits mexer). Passando attr = 0 e mask = AM_RDO|AM_HID|AM_SYS
você desliga esses três bits especificamente, sem tocar em nada mais (como AM_ARC).

Se seu shell não tem um comando pra rodar f_chmod direto, o jeito mais rápido é
fazer um mini-programa (nos moldes do orionbur) só pra isso, ou — se preferir não
mexer em nada agora — resolve na hora, no PC mesmo: no Windows/DOS, attrib -r -h -s
MSDOS.SYS (idem pros outros dois) tira os atributos, aí você apaga normalmente antes
de levar o cartão de volta pro oriondos.

Bora. Baseado no que você já tem rodando (ELF carregado com `argc`/`argv`,0
FatFs via trap), aqui vai uma estrutura inicial no estilo `attrib` do DOS —
`+r`/`-r`, `+h`/`-h`, `+s`/`-s`, `+a`/`-a`, e listagem dos atributos se rodar
sem flags.

```c
/* ============================================================
 * attrib.c - Gerenciador de atributos de arquivo (FAT) para o oriondos
 * Projeto Orion68
 *
 * Uso:
 *   attrib arquivo.ext                 -> mostra os atributos atuais
 *   attrib +r -h -s arquivo.ext        -> liga RDO, desliga HID e SYS
 *   attrib +r+h+s arquivo.ext          -> tambem aceita colado
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

typedef struct {
    char letra;
    BYTE bit;
} attr_map_t;

static const attr_map_t ATTR_TABLE[] = {
    { 'r', AM_RDO },
    { 'h', AM_HID },
    { 's', AM_SYS },
    { 'a', AM_ARC },
};
#define ATTR_TABLE_LEN (sizeof(ATTR_TABLE) / sizeof(ATTR_TABLE[0]))

/* Converte o bit em letra maiuscula (ligado) ou '-' (desligado),
 * no estilo "dir /a" do DOS: R H S A */
static void fmt_attr_string(BYTE attr, char *out)
{
    uint8_t i;
    for (i = 0; i < ATTR_TABLE_LEN; i++) {
        out[i] = (attr & ATTR_TABLE[i].bit)
                     ? (char) (ATTR_TABLE[i].letra - 'a' + 'A')
                     : '-';
    }
    out[ATTR_TABLE_LEN] = '\0';
}

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
```

Pontos de atenção pro seu ambiente específico:

- **`f_stat` precisa estar habilitada** no `ffconf.h` da sua build da FatFs
(`FF_USE_FIND`/campos de `FILINFO` já cobrem isso, mas `f_stat` em si é sempre
compilada — só confirma que não está sendo excluída em algum `#if`).
- O parser de flags é propositalmente simples (sem `getopt`, sem libs extras)
pra combinar com o resto do seu código — mas ele **não valida bounds de `argv[i]`**
além do que os próprios `while`/`for` já cobrem naturalmente (string terminada
em `\0`), então deve estar dentro do seu padrão de "funciona sem checar limites
explicitamente", sem risco de estourar buffer aqui porque não há buffer de
tamanho fixo sendo preenchido a partir do input.
- Se quiser, o próximo passo natural é fazer seu `ls` mostrar essa mesma
`fmt_attr_string()` como coluna — dá pra extrair ela pra um `.h` compartilhado
entre os dois programas.





