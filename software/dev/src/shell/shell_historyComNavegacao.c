/*
 * shell_history.c
 *
 * Historico de comandos com navegacao via seta pra cima (0x10) e
 * seta pra baixo (0x11) -- byte unico, sem sequencia de escape, pelo
 * que voce reportou do seu terminal.
 *
 * INTEGRACAO: troque a leitura de caractere (getchar_serial) e escrita
 * (putchar_serial/puts_serial) pelas funcoes reais que seu shell ja
 * usa hoje pra ler/escrever no console.
 */

Feito. Resumo do que mudou e do que fica pendente pra você:

**Macros que você pediu:** `KEY_LEFT = 0x12`, `KEY_RIGHT = 0x13` — são **chutes seguindo a sequência** de `0x10`/`0x11` que você confirmou, mas marquei bem grande no comentário que é só palpite. Também coloquei `KEY_DELETE = 0x7F` (tecla Delete, diferente do Backspace) já que edição no meio pede as duas.

**A peça nova que exige uma decisão sua:** `video_cursor_move(int delta)` — função que só **reposiciona** o cursor sem apagar nada (diferente do backspace que apaga). Isso é a parte que mais depende do seu hardware específico: se sua placa de vídeo tem um registrador de posição de cursor direto, é uma escrita nesse registrador; se não tiver isso, pode ser mais trabalhoso (calcular offset em memória de vídeo). Essa função **precisa existir** de verdade pro cursor esquerda/direita funcionar visualmente — sem ela, o buffer edita certo internamente, mas a tela não vai refletir onde o cursor realmente está.

Quando o sistema voltar e você conseguir testar os valores reais das teclas, é só ajustar as três macros — a lógica de `memmove`/redesenho não muda.

#include <string.h>
#include <stdint.h>

#define HIST_SIZE   16    /* quantos comandos guardar -- ajuste como quiser */
#define LINE_MAX    128   /* tamanho maximo de uma linha de comando */

#define KEY_UP     0x10
#define KEY_DOWN   0x11
#define KEY_LEFT   0x12   /* PLACEHOLDER -- ajuste pro valor real quando testar.
                             Seu padrao ate agora sugere ASCII ja traduzido
                             fora da faixa imprimivel (0x10=up, 0x11=down),
                             entao 0x12/0x13 seguindo a sequencia e' um
                             palpite razoavel, mas SO' um palpite. */
#define KEY_RIGHT  0x13   /* PLACEHOLDER -- idem KEY_LEFT */
#define KEY_DELETE 0x7F   /* PLACEHOLDER -- tecla Delete, DIFERENTE de
                             Backspace (KEY_BS). Ajuste tambem. */
#define KEY_BS     0x08   /* AJUSTE se seu backspace for outro codigo (ex: 0x7F) */
#define KEY_ENTER  '\r'   /* ou '\n', conforme seu terminal manda */

/*
 * Move o cursor de VIDEO (nao o de historico) 'delta' colunas, sem
 * alterar nenhum caractere na tela -- so' reposiciona onde o proximo
 * putchar vai desenhar. Implemente isso de acordo com seu driver de
 * video real (registrador de cursor de hardware, ou uma funcao
 * video_set_col() que voce ja tenha). Isso e' DIFERENTE do
 * "backspace que apaga" que voce ja usa -- aqui NADA e' apagado, so'
 * reposiciona.
 */
extern void video_cursor_move(int delta);

extern int  getchar_serial(void);
extern void putchar_serial(char c);
extern void puts_serial(const char *s);

static char history[HIST_SIZE][LINE_MAX];
static int  hist_count = 0;     /* quantos comandos ja foram guardados no total */
static int  hist_cursor = -1;   /* -1 = "linha nova" (fora do historico) */

/*
 * Apaga visualmente 'len' caracteres da linha atual no terminal e
 * escreve 'text' no lugar. Como terminal nao tem "desfazer", isso e'
 * feito na base do backspace + espaco + backspace.
 */
static void redraw_line(int old_len, const char *text)
{
    for (int i = 0; i < old_len; i++) putchar_serial('\b');
    for (int i = 0; i < old_len; i++) putchar_serial(' ');
    for (int i = 0; i < old_len; i++) putchar_serial('\b');
    puts_serial(text);
}

/*
 * Grava um comando executado no historico (chame isso depois que o
 * usuario der Enter e o comando for de fato executado -- nao grave
 * linhas vazias, e evite gravar duplicata consecutiva se quiser um
 * comportamento mais "bash-like", isso e' opcional).
 */
static void history_add(const char *cmd)
{
    if (cmd[0] == '\0')
        return; /* nao guarda linha vazia */

    strncpy(history[hist_count % HIST_SIZE], cmd, LINE_MAX - 1);
    history[hist_count % HIST_SIZE][LINE_MAX - 1] = '\0';
    hist_count++;
    hist_cursor = -1; /* toda vez que roda um comando novo, reseta a navegacao */
}

/*
 * Le uma linha de comando do console, com edicao basica (backspace)
 * e navegacao de historico (setas). Retorna quando Enter e' pressionado.
 *
 * 'buf' precisa ter pelo menos LINE_MAX bytes.
 */
/*
 * Redesenha, a partir da posicao ATUAL do cursor de video, o trecho
 * de 'buf' de cursor_pos ate 'len', e depois reposiciona o cursor de
 * video de volta pro lugar certo (logo apos o caractere inserido/
 * apagado, nao no fim da linha reescrita).
 *
 * 'extra_erase' e' quanto sobrou "fantasma" na tela alem do texto
 * novo (usado no delete-no-meio/backspace-no-meio, onde a linha
 * ficou 1 caractere mais curta e sobra um caractere velho no fim que
 * precisa ser apagado).
 */
static void redraw_tail(const char *buf, int cursor_pos, int len, int extra_erase)
{
    puts_serial(&buf[cursor_pos]);            /* reescreve o resto da linha */
    for (int i = 0; i < extra_erase; i++)
        putchar_serial(' ');                   /* apaga sobra visual, se houver */

    /* volta o cursor de video pra logo depois do ponto de edicao --
     * andamos (len - cursor_pos) pra frente escrevendo, mais
     * 'extra_erase' escrevendo espaco, entao voltamos tudo isso. */
    video_cursor_move(-((len - cursor_pos) + extra_erase));
}

void readline_with_history(char *buf)
{
    int len = 0;
    int cursor_pos = 0;   /* onde o cursor esta DENTRO da linha, 0..len */
    buf[0] = '\0';
    hist_cursor = -1;

    for (;;) {
        int c = getchar_serial();

        if (c == KEY_ENTER) {
            putchar_serial('\n');
            buf[len] = '\0';
            return;
        }

        if (c == KEY_LEFT) {
            if (cursor_pos > 0) {
                cursor_pos--;
                video_cursor_move(-1);
            }
            continue;
        }

        if (c == KEY_RIGHT) {
            if (cursor_pos < len) {
                cursor_pos++;
                video_cursor_move(1);
            }
            continue;
        }

        if (c == KEY_BS) {
            /* apaga o caractere ANTES do cursor -- pode ser no meio */
            if (cursor_pos > 0) {
                memmove(&buf[cursor_pos - 1], &buf[cursor_pos], len - cursor_pos);
                len--;
                cursor_pos--;
                buf[len] = '\0';

                video_cursor_move(-1);         /* volta 1 pra reescrever a partir dali */
                redraw_tail(buf, cursor_pos, len, 1); /* reescreve o resto + apaga sobra */
            }
            continue;
        }

        if (c == KEY_DELETE) {
            /* apaga o caractere SOB o cursor (nao move o cursor) */
            if (cursor_pos < len) {
                memmove(&buf[cursor_pos], &buf[cursor_pos + 1], len - cursor_pos - 1);
                len--;
                buf[len] = '\0';
                redraw_tail(buf, cursor_pos, len, 1);
            }
            continue;
        }

        if (c == KEY_UP) {
            if (hist_cursor + 1 < hist_count && hist_cursor + 1 < HIST_SIZE) {
                hist_cursor++;
                int idx = (hist_count - 1 - hist_cursor + HIST_SIZE) % HIST_SIZE;
                redraw_line(len, history[idx]);
                strcpy(buf, history[idx]);
                len = strlen(buf);
                cursor_pos = len; /* historico sempre entra com cursor no fim */
            }
            continue;
        }

        if (c == KEY_DOWN) {
            if (hist_cursor > 0) {
                hist_cursor--;
                int idx = (hist_count - 1 - hist_cursor + HIST_SIZE) % HIST_SIZE;
                redraw_line(len, history[idx]);
                strcpy(buf, history[idx]);
                len = strlen(buf);
                cursor_pos = len;
            } else if (hist_cursor == 0) {
                hist_cursor = -1;
                redraw_line(len, "");
                buf[0] = '\0';
                len = 0;
                cursor_pos = 0;
            }
            continue;
        }

        /* caractere normal -- INSERE na posicao do cursor, nao so' no fim */
        if (len < LINE_MAX - 1 && c >= 0x20 && c < 0x7F) {
            memmove(&buf[cursor_pos + 1], &buf[cursor_pos], len - cursor_pos);
            buf[cursor_pos] = (char) c;
            len++;
            buf[len] = '\0';

            redraw_tail(buf, cursor_pos, len, 0);
            /* redraw_tail ja deixou o cursor de video no lugar certo
             * (logo apos o char inserido) -- so' falta avancar
             * cursor_pos no BUFFER pra bater com isso */
            cursor_pos++;
        }
    }
}

/*
 * Exemplo de uso no loop principal do shell:
 *
 *   char cmdline[LINE_MAX];
 *   for (;;) {
 *       puts_serial("orion> ");
 *       readline_with_history(cmdline);
 *       history_add(cmdline);      // grava ANTES ou DEPOIS de executar,
 *                                  // tanto faz, contanto que seja sempre
 *       executa_comando(cmdline);
 *   }
 */
