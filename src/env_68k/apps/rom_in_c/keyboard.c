// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <mc68000.h>

// 🛠️🇧🇷
//🚀🛠️

#define UART_KEYBOARD 0x4300

#define RHR 0  // receive holding register (read)
#define THR 0  // transmit holding register (write)
#define IER 2  // interrupt enable register
#define ISR 4  // interrupt status register (read)
#define FCR 4  // FIFO control register (write)
#define LCR 6  // line control register
#define MCR 8  // modem control register
#define LSR 10 // line status register
#define MSR 12 // modem status register
#define SPR 14 // scratchpad register (reserved for system use)
#define DLL 0  // divisor latch LSB
#define DLM 2  // divisor latch MSB
// aliases for register names (used by different manufacturers)cd ..
#define RBR RHR // receive buffer register
#define IIR ISR // interrupt identification register
#define SCR SPR // scratch register

unsigned char shift_status = 0;
unsigned char ctrl_status = 0;
unsigned char alt_status = 0;
unsigned char modifier = 0;
unsigned char _CapsFlag = 0;

unsigned char key_buffer[12];
unsigned char key_bufferA[12];
unsigned char key_bufferB[12];
unsigned char cmd, length, type, new_packet = 0;

unsigned char get_keypress();
void set_keyboard_leds(unsigned char led_status);

void delay(unsigned int time)
{
    for (volatile unsigned int i = 0; i < time; i++)
        ;
}

#define BAUD_DIV_L 0x08 //(BAUD_DIV&$FF)
#define BAUD_DIV_U 0x00 //((BAUD_DIV>>8)&$FF)

/* -----------------------------------------------------------------
    * Exemplo: Preencher um buffer na RAM com o caractere 'A' (0x41)
    * Buffer localizado em $00002000, tamanho de 512 bytes
    * -----------------------------------------------------------------

    * 1. Empilha o 3º argumento: count (unsigned int = 32 bits)
    move.l  #512, -(sp)          * Empilha o tamanho (512 bytes)

    * 2. Empilha o 2º argumento: ch (int). 
    * Nota: O GCC espera 32 bits aqui para manter o alinhamento da pilha!
    move.l  #$41, -(sp)          * Empilha o caractere 'A' (expandido para long)

    * 3. Empilha o 1º argumento: dest (ponteiro = 32 bits)
    move.l  #$00002000, -(sp)    * Empilha o endereço inicial do buffer

    * 4. Salta para a função na ROM do PDS317
    jsr     mymemset             * O processador executa o memset

    * 5. Limpa a pilha (Stack Cleanup)
    * Empilhamos 3 longs (4 + 4 + 4 = 12 bytes). Precisamos devolver o SP ao normal.
    add.l   #12, sp              * Desaloca os argumentos da pilha

    * A partir daqui, o buffer em $2000 está limpo e o registrador D0 
    * contém o ponteiro de retorno (o próprio endereço $00002000).
*/
void *mymemset(void *dest, int ch, unsigned int count)
{
    unsigned char *ptr = (unsigned char *)dest;

    while (count--) {
        *ptr++ = (unsigned char)ch;
    }

    return dest;
}

void init_uart()
{
    volatile unsigned char *uart_reg = (volatile unsigned char *)UART_KEYBOARD;

    // 1. Entra no modo DLAB para configurar Baud Rate
    *(uart_reg + LCR) = 0x83;
    *(uart_reg + DLL) = 8; // Divisor para 115200 com 14.7456MHz
    *(uart_reg + DLM) = 0;
    // 2. Sai do modo DLAB e define 8N1 (MUITO IMPORTANTE: usar 0x03)
    *(uart_reg + LCR) = 0x03;

    // 3. Configura FIFO (Habilita, limpa buffers e seta trigger de 14 bytes)
    *(uart_reg + FCR) = 0xC7;

    // 4. Limpa registradores de controle
    *(uart_reg + MCR) = 0x00;
    *(uart_reg + IER) = 0x00; // Garante que interrupções estão desligadas
}

unsigned char read_kbd()
{
    volatile unsigned char *uart_reg = (volatile unsigned char *)UART_KEYBOARD;

    while (!(*(uart_reg + LSR) & 0x01))
        ;
    return (unsigned char)*(uart_reg + RHR);
}
unsigned char uart_read()
{
    return read_kbd();
}
void write_kbd(unsigned char data)
{
    volatile unsigned char *uart_reg = (volatile unsigned char *)UART_KEYBOARD;
    // Tente ler a versão do chip
    while (!(*(uart_reg + LSR) & 0x20))
    {
    };
    *(uart_reg + THR) = data;
}

unsigned char get_0x81()
{
    int i;
 
    type = read_kbd(); //*(uart_reg + RHR);
    length = read_kbd(); //*(uart_reg + RHR);
    if (length > 8)    {
        for (i = 0; i < length; i++)        {
            // while (!(*(uart_reg + LSR) & 0x01)) ;
            //*(uart_reg + RHR); // descarta
            read_kbd();
        }
    }
    if (length == 8)    {
        for (i = 0; i < 8; i++)        {
            //, while (!(*(uart_reg + LSR) & 0x01))            ;
            key_buffer[i] = read_kbd(); //*(uart_reg + RHR);
            // printf("[%02x ",key_buffer[i]);
        }
        if (key_buffer[2] != 0)        {
            return key_buffer[2]; // Retorna o ScanCode real!
        }
    }
    else    {
        // Caso venha um tamanho inesperado, limpa para não desalinhar
        for (i = 0; i < length; i++)        {
            //while (!(*(uart_reg + LSR) & 0x01))    ;
            //*(uart_reg + RHR);
            read_kbd();
        }
    }
    return 0;
}
void get_0x87()
{
    int i;
    length = read_kbd(); //*(uart_reg + RHR);

    // Esvazia os bytes desse aviso rapidamente
    for (i = 0; i < length; i++) {
        // while (!(*(uart_reg + LSR) & 0x01));
        //*(uart_reg + RHR);
        read_kbd();
    }
}
void get_0x88()
{
    unsigned char size = read_kbd();
    // Não se esqueça eu lia hardcoded de 0 a 12
    // agora com size já sendo lido tenho que começar da posição 1
    // pois o codigo espera os comandos nas posições A=4 e B=2
    for (int i = 1; i < size; i++)    {
        if (new_packet == 0)        {
            key_bufferA[i] = read_kbd(); //*(uart_reg + RHR);
        }
        else        {
            key_bufferB[i] = read_kbd(); //*(uart_reg + RHR);
        }
    }
    if (new_packet == 0)    {
        new_packet = 1;
        if (key_bufferA[2] == 0x02)        {
            modifier = 2;
        }
        if (key_bufferA[4] == 0x39)        {
            if (_CapsFlag == 0)            {
                modifier = 2;
                set_keyboard_leds(modifier);
                _CapsFlag = 1;
            }
            else            {
                modifier = 0;
                set_keyboard_leds(modifier);
                _CapsFlag = 0;
            }
        }
        // printf("key_bufferA[4] [%02x] - ",key_bufferA[4]);
        // printf("key_bufferA[2] [%02x]\n",key_bufferA[2]);
    }
}
unsigned char get_packet()
{
    while (1)
    {
        // 1. Sincroniza no primeiro byte do cabeçalho (0x57)
        if (read_kbd() != 0x57)        {
            new_packet = 0;
            continue;
        }

        // 2. Confirma o segundo byte (0xAB)
        if (read_kbd() != 0xAB)        {
            continue;
        }

        // 3. Lê o Comando (Pelo seu dump, pode vir 0x88, 0x87 ou 0x81)
        cmd = read_kbd(); // *(uart_reg + RHR);
        // if ( cmd != 0x82)
        //     printf("cmd %02x\n",cmd);

        if (cmd == 0x81)        {
            get_0x81();
        }
        if (cmd == 0x82)
            continue;
        if (cmd == 0x87)        {
            get_0x87();
        }
        if (cmd == 0x88)        {
            get_0x88();
        }
        if (new_packet == 1)        {
            /*
            if(key_bufferA[0] == 0x0b && key_bufferA[1] == 0x10){
                for(int i = 0; i < 12; i++) {
                    printf("%02x|",key_bufferA[i]);
                }
                printf("\n");
            }
            if(key_bufferB[0] == 0x0b && key_bufferB[1] == 0x10){
                for(int i = 0; i < 12; i++) {
                    printf("%02x|",key_bufferB[i]);
                }
                printf("\n");
            }
            */
            return 0;
        }
    }
}
void set_keyboard_leds(unsigned char led_status)
{
    volatile unsigned char *uart_reg = (volatile unsigned char *)UART_KEYBOARD;

    unsigned char buf[11];

    buf[0] = 0x57;
    buf[1] = 0xAB;
    buf[2] = 0x12; // Comando de escrita de dados HID
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 0x00;
    buf[6] = 0x00;
    buf[7] = led_status; // O bitmask dos LEDs (0x01, 0x02, 0x04)
    buf[8] = 0x00;
    buf[9] = 0x0F; // Constante de preenchimento (comum nesse protocolo)

    // Checksum: soma de buf[0] até buf[9]
    unsigned char ck = 0;
    for (int i = 2; i < 10; i++)    {
        ck += buf[i];
    }
    buf[10] = ck;

    // Envio para a UART
    for (int i = 0; i < 11; i++)    {
        while (!(*(uart_reg + LSR) & 0x20))            ;
        *(uart_reg + THR) = buf[i];
    }
}
unsigned char get_kbd_key(char mod, unsigned char code);

void reset_por_software(){
    __asm__ __volatile__(
        "jmp 0x000.l\n\t" // JMP absoluto longo para 0x000000
    );
}

unsigned char get_key(){
    unsigned char ch;
    get_packet();
    // if (key_bufferA[4] >= 0x20){
    ch = get_kbd_key(modifier, key_bufferA[4]);
    if (ch == 0x1b)
        reset_por_software();
    if (ch >= 0x20)    {
        // printf("%c",ch);
        mymemset((void *)key_bufferA, 0, sizeof(key_bufferA));
        mymemset((void *)key_bufferB, 0, sizeof(key_bufferB));
    }
    if (_CapsFlag == 0)    {
        modifier = 0;
    }
    return ch;
}
/*
void main()
{
    unsigned char ch;
    init_uart();
    // set_transparent_mode();
    // printf("Desligando led...");
    delay(0x8FF);
    // printf("\nOrion68K Online. Digite algo:\n");
    // set_keyboard_leds(0x02);

    while (1)    {
        get_packet();
        // if (key_bufferA[4] >= 0x20){
        ch = get_kbd_key(modifier, key_bufferA[4]);
        if (ch == 0x1b)
            reset_por_software();
        if (ch >= 0x20)        {
            // printf("%c",ch);
            mymemset((void *)key_bufferA, 0, sizeof(key_bufferA));
            mymemset((void *)key_bufferB, 0, sizeof(key_bufferB));
        }
        if (_CapsFlag == 0)        {
            modifier = 0;
        }
    }
}
*/
/*
Esse código está enviando um comando de inicialização
crucial para mudar o comportamento interno do CH9350:
ele está tirando o chip do modo padrão e forçando-o a
entrar no Modo Transparente (ou Modo de Transmissão
Direta).Analisando a estrutura do array cmd[] =
{0x57, 0xAB, 0x01, 0x00, 0x01}, o que cada byte faz
de acordo com o protocolo do CH9350 é o seguinte:0x57
0xAB $\rightarrow$ É o cabeçalho de sincronismo
obrigatório que você já conhece.0x01 $\rightarrow$ É
o código do comando para "Definir Modo de Trabalho"
(Set Work Mode).0x00 $\rightarrow$ É o parâmetro do
modo. O valor 0x00 configura o chip para o Modo
Transparente USB HID.0x01 $\rightarrow$ É o Checksum
desse pacote (pulando o cabeçalho, a soma de 0x01 +
0x00 é igual a 0x01).O que esse modo muda no seu
Orion68K?Por padrão de fábrica, o CH9350 tenta ser
"esperto": ele tenta decodificar os relatórios USB
HID do teclado internamente e enviar apenas dados
mastigados na serial.Quando você envia esse comando e
ativa o Modo Transparente (0x00):Desativa o Filtro
Interno: Você está dizendo para o chip: "Não tente
processar ou filtrar nada por conta própria. Tudo o
que o teclado USB mandar, repasse bruto para a minha
serial".Explica o tamanho dos pacotes: É exatamente
por causa desse comando que o comportamento do chip
mudou e ele começou a te enviar aqueles pacotes longos
de 12 bytes em vez de apenas o scancode puro! No modo
transparente, ele encapsula o relatório USB HID inteiro
do teclado (que tem 8 bytes) dentro do frame serial
dele (adicionando os bytes de status, tamanho,
sequenciador e o checksum de rodapé que você descobriu).
*/
void set_transparent_mode()
{
    volatile unsigned char *uart_reg = (volatile unsigned char *)UART_KEYBOARD;
    // Array com o comando completo: Header(57 AB), Cmd(01), Param(00), Checksum(01)
    unsigned char cmd[] = {0x57, 0xAB, 0x01, 0x00, 0x01};
    for (int i = 0; i < 5; i++)    {
        while (!(uart_reg[LSR] & 0x20))            ;
        uart_reg[THR] = cmd[i];
    }
}

unsigned char get_kbd_key(char mod, unsigned char code)
{
    unsigned char RetKey = 0; // default is 0 (No key pressed)

//                                      0     1    2    3    4    5    6    7    8    9   10   11  12   13   14   15
    unsigned char OE_BASE_KEYMAP[] = {0x00, 0x00, 0x00, 0x00, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',              // 0x
                                      'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2',                  // 1x
                                      '3', '4', '5', '6', '7', '8', '9', '0', 0x0D, 0x1B, 0x08, 0x09, ' ', '-', '=', '[',              // 2x  ENTER, ESC, BACKSPACE, TAB
                                      ']', '\\', 0xFF, ';', '\'', '`', ',', '.', '/', 0x02, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6,        // 3x  CAPS = 2, F0-F6
                                      0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0x1F, 0xF0, 0xFE, 0x18, 0x14, 0x15, 0x7F, 0x17, 0x16, 0x13,  // 4x F7-F12, PrintScreen, ScrollLock, Pause, Insert, Home, PageUp, DelFwd,End, PageDown, Rightarrow
                                      0x12, 0x11, 0x10, 0x05, '/', '*', '-', '+', 0x0D, '1', '2', '3', '4', '5', '6', '7',             // 5x Left, Down, Up, NumLock, Keypad Symbols
                                      '8', '9', '0', '.', 0xFF, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      // 6x Unknown, Application, Not used symbols
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 7x Not used symbols

    unsigned char OE_SHIFT_KEYMAP[] = {0x00, 0x00, 0x00, 0x00, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',              // 0x
                                       'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '!', '@',                  // 1x
                                       '#', '$', '%', '^', '&', '*', '(', ')', 0x0D, 0x1B, 0x08, 0x09, ' ', '_', '+', '{',              // 2x  Shift-ENTER, ESC, BACKSPACE, TAB, Shift-Space
                                       '}', '|', 0xFF, ':', '"', '~', '<', '>', '?', 0x02, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6,          // 3x  CAPS = 2, F0-F6
                                       0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0x1F, 0xF0, 0xFE, 0x18, 0x14, 0x15, 0x7F, 0x17, 0x16, 0x13,  // 4x  F7-F12, PrintScreen, ScrollLock, Pause, Insert, Home, PageUp, DelFwd,End, PageDown, Rightarrow
                                       0x12, 0x11, 0x10, 0x05, '/', '*', '-', '+', 0x0D, '1', '2', '3', '4', '5', '6', '7',             // 5x  Left, Down, Up, NumLock, Keypad Symbols
                                       '8', '9', '0', '.', 0xFF, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      // 6x  Unknown, Application, Not used symbols
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 7x  Not used symbols

    unsigned char OE_CTRL_KEYMAP[] = {0x00, 0x00, 0x00, 0x00, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC,  // 0x
                                      0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xE1, 0xE2,  // 1x
                                      0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xE0, 0x0F, 0x00, 0x08, 0x00, ' ', 0x00, 0x00, 0x00,   // 2x  CTRL-ENTER, --, BACKSPACE, Space
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6,  // 3x  F0-F6
                                      0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13,  // 4x  F7-F12 / Rightarrow
                                      0x12, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 5x  Left, Down, Up, NumLock, Keypad Symbols
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 6x
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 7x

    unsigned char OE_ALT_KEYMAP[] = {0x00, 0x00, 0x00, 0x00, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC,  // 0x
                                     0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xBC, 0xBD,  // 1x
                                     0xBE, 0xBF, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xBB, 0x0D, 0x00, 0x08, 0x00, ' ', 0x00, 0x00, 0x00,   // 2x  ENTER, --, BACKSPACE, Space
                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6,  // 3x  F0-F6
                                     0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13,  // 4x  F7-F12 / Rightarrow
                                     0x12, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 5x  Left, Down, Up, NumLock, Keypad Symbols
                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 6x
                                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 7x

    unsigned char OE_ALTGR_KEYMAP[] = {0x00, 0x00, 0x00, 0x00, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C,  // 0x
                                       0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9C, 0x9D,  // 1x
                                       0x9E, 0x9F, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0x9B, 0x0D, 0x00, 0x08, 0x00, ' ', 0x00, 0x00, 0x00,   // 2x  ENTER, --, BACKSPACE, Space
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6,  // 3x  F0-F6
                                       0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13,  // 4x  F7-F12 / Rightarrow
                                       0x12, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 5x  Left, Down, Up, NumLock, Keypad Symbols
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 6x
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 7x

    if (mod == 0x00)    { // No modifier
        RetKey = OE_BASE_KEYMAP[code];
        if (_CapsFlag == 2)        {
            if ((RetKey >= 'a') && (RetKey <= 'z'))
                RetKey = RetKey - 32;
        }
    }
    if ((mod == 0x02) || (mod == 0x20))    { // Left & Right Shift modifier
        RetKey = OE_SHIFT_KEYMAP[code];
        if (_CapsFlag == 2)       {
            if ((RetKey >= 'A') && (RetKey <= 'Z'))
                RetKey = RetKey + 32;
        }
    }
    if ((mod == 0x01) || (mod == 0x10))    { // Left & Right CTRL modifier
        RetKey = OE_CTRL_KEYMAP[code];
    }
    if (mod == 0x04)    { // Left ALT modifier
        RetKey = OE_ALT_KEYMAP[code];
    }
    if (mod == 0x40)    { // Right ALT (ALT GR) modifier
        RetKey = OE_ALTGR_KEYMAP[code];
    }
    return RetKey;
}
