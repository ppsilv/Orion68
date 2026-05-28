#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mc68000.h>
#include "keyboard.h"

// 🛠️🇧🇷



unsigned char shift_status = 0;
unsigned char ctrl_status = 0;
unsigned char alt_status = 0;


static volatile unsigned char mod_ctrl = 0;
static volatile unsigned char mod_shift = 0;
static volatile unsigned char mod_caps = 0;
static volatile unsigned char mod_alt = 0;
static volatile unsigned char mod_altgr = 0;
static volatile unsigned char _CapsFlag = 0;

unsigned char key_buffer[12];
unsigned char cmd, length, type, new_packet = 0;

unsigned char get_keypress();
void set_keyboard_leds(unsigned char led_status);

void *mymemset(void *dest, int ch, unsigned int count)
{
    unsigned char *ptr = (unsigned char *)dest;

    while (count--)
    {
        *ptr++ = (unsigned char)ch;
    }

    return dest;
}

static void init_uart()
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
    unsigned char ch;
    while (!(*(uart_reg + LSR) & 0x01)) ;
    ch = (unsigned char)*(uart_reg + RHR);
   // printf("%02x ",ch);
    return ch;
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
unsigned char get_0x88()
{
    unsigned char size = read_kbd();
    // Não se esqueça eu lia hardcoded de 0 a 12
    // agora com size já sendo lido tenho que começar da posição 1
    // pois o codigo espera os comandos nas posições A=4 e B=2
    for (int i = 1; i < size; i++)    {
        key_buffer[i] = read_kbd(); //*(uart_reg + RHR);
    }
    if (key_buffer[2] == 0x0 && key_buffer[4] == 0x0) /*normal*/       {
        //Esse if é executado em todo key up
        mod_ctrl = 0;
        mod_shift = 0;
        mod_alt = 0;
        mod_altgr = 0;
        //printf("1-key_buffer[2] [%02x] - key_buffer[4] [%02x]\n",key_bufferA[2],key_bufferA[4]);
        return NO_KEY;
    }
    if (key_buffer[2] == 0x01 || key_buffer[2] == 0x10) /*control*/       {
        mod_ctrl = 1;
        //printf("2-key_buffer[2] [%02x] - key_buffer[4] [%02x]\n",key_bufferA[2],key_bufferA[4]);
        return KEY_CTRL;
    }
    if (key_buffer[2] == 0x02 || key_buffer[2] == 0x20) /*shift*/       {
        mod_shift = 2;
        //printf("3-key_buffer[2] [%02x] - key_buffer[4] [%02x]\n",key_bufferA[2],key_bufferA[4]);
        return KEY_SHIFT;
    }
    if (key_buffer[2] == 0x04 ) /*Alt*/       {
        mod_alt = 1;
        //printf("4-key_buffer[2] [%02x] - key_buffer[4] [%02x]\n",key_bufferA[2],key_bufferA[4]);
        return KEY_ALT;
    }        
    if (key_buffer[2] == 0x40) /*Altgr*/       {
        mod_altgr = 1;
        //printf("5-key_buffer[2] [%02x] - key_buffer[4] [%02x]\n",key_bufferA[2],key_bufferA[4]);
        return KEY_ALTGR;
    }        
    if (key_buffer[4] == 0x39) /*capslock*/       {
        if (mod_caps == 0)            {
            mod_caps = 2;
            set_keyboard_leds(mod_caps);
        }
        else            {
            mod_caps = 0;
            set_keyboard_leds(mod_caps);
        }
        //printf("6-key_buffer[2] [%02x] - key_buffer[4] [%02x]\n",key_bufferA[2],key_bufferA[4]);
        return KEY_CAPS;
    }
    return VALID_KEY;
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
            continue;
        }
        if (cmd == 0x82)
            continue;
        if (cmd == 0x87)        {
            get_0x87();
            continue;
        }
        if (cmd == 0x88)        {
            return get_0x88();
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
            return NO_KEY;
        }
    }
    return NO_KEY;
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
unsigned char get_kbd_key(unsigned char code);

void reset_por_software(){
    __asm__ __volatile__(
        "jmp 0x000.l\n\t" // JMP absoluto longo para 0x000000
    );
}

void init_kbd(){
    printf("iniciando uart\n");
    init_uart();
}


void main1()
{
    unsigned char ch;

    while (1)    {
        if ( get_packet() == NO_KEY )
            continue;
        ch = get_kbd_key(key_buffer[4]);
        mymemset((void *)key_buffer, 0, sizeof(key_buffer));
        if ( ch >= 0x20 ){
            printf("%c",ch);
        }else if( (ch == 0x0A) || (ch == 0x0D)){ 
            ch = 0x0A;
            printf("%c",ch);
            ch = 0x0D;
            printf("%c",ch);
        }
        if (ch == 0x1b)
            return;
    }
}


unsigned char get_kbd_key(unsigned char code)
{
    unsigned char RetKey = 0; // default is 0 (No key pressed)

    if (mod_shift == 0x00 && mod_caps == 0x00)    { // No modifier
        RetKey = OE_BASE_KEYMAP[code];
    }
    if (mod_shift == 0x02) { // Left & Right Shift modifier
        if (mod_caps == 2)
            RetKey = OE_BASE_KEYMAP[code];
        else    
            RetKey = OE_SHIFT_KEYMAP[code];
        return RetKey;    
    }
    if ((mod_ctrl == 0x01) || (mod_ctrl == 0x10))    { // CTRL modifier
        RetKey = OE_CTRL_KEYMAP[code];
    }
    if (mod_alt == 0x04)    { // Left ALT modifier
        RetKey = OE_ALT_KEYMAP[code];
    }
    if (mod_altgr == 0x40)    { // Right ALT (ALT GR) modifier
        RetKey = OE_ALTGR_KEYMAP[code];
    }
    if (mod_caps) { // Left & Right Shift modifier
        RetKey = OE_SHIFT_KEYMAP[code];
    }
    return RetKey;
}




unsigned char get_key(){
    unsigned char ch;


    if ( get_packet() == NO_KEY )
        return 0;
    ch = get_kbd_key(key_buffer[4]);
    mymemset((void *)key_buffer, 0, sizeof(key_buffer));
    if( (ch == 0x0A) || (ch == 0x0D)){ 
        ch = 0x0A;
        printf("%c",ch);
        ch = 0x0D;
        printf("%c",ch);
        return 0;
    }

    return ch;
}
