#include <stdio.h>
#include <stdlib.h>

#include <mc68000.h>

#define LEDS_ADDRESS 0x4400
#define LEDS (*(volatile unsigned char *)LEDS_ADDRESS)

/*
UART1 - 0x4000 até 0x20FF = UART_CS1                  
UART2 - 0x4100 até 0x21FF = UART_CS2                                                   
UART3 - 0x4200 até 0x22FF = UART_CS3                                                   
UART4 - 0x4300 até 0x23FF = UART_CS4
*/

#define UART_KEYBOARD 0x4300

#define RHR   0   // receive holding register (read)
#define THR   0   // transmit holding register (write)
#define IER   2   // interrupt enable register
#define ISR   4   // interrupt status register (read)
#define FCR   4   // FIFO control register (write)
#define LCR   6   // line control register
#define MCR   8   // modem control register
#define LSR   10  // line status register
#define MSR   12  // modem status register
#define SPR   14  // scratchpad register (reserved for system use)
#define DLL   0   // divisor latch LSB
#define DLM   2   // divisor latch MSB
// aliases for register names (used by different manufacturers)cd ..
#define RBR   RHR // receive buffer register
#define IIR   ISR // interrupt identification register
#define SCR   SPR // scratch register

void delay(unsigned int time) {
    for (volatile unsigned int i = 0; i < time; i++);
}

// Função para verificar stack
void check_stack(void) {
    unsigned long stack_val;
    asm volatile (
        "move.l %%sp, %0\n\t"
        : "=r" (stack_val)
    );
    printf("Stack pointer: 0x%08X\n", stack_val);
}

/* Definindo o endereço base da sua Video Card */
//#define SCREEN_REG 0xB8001
#define S1_REG 0xB8003
#define S2_REG 0xB8005
#define S3_REG 0xB8007
//#define CONFIG_REG 0xB8009


//Register address
#define WRITE_SCREEN   0xB8001        //Endereço real  3 0x03  o pico enxerga 0x00
#define REG_02         0xB8003        //Endereço real  3 0x03  o pico enxerga 0x01
#define REG_03         0xB8005        //Endereço real  5 0x05  o pico enxerga 0x02
#define REG_04         0xB8007        //Endereço real  7 0x07  o pico enxerga 0x03
#define CONFIG_REG     0xB8009        //Endereço real  9 0x09  o pico enxerga 0x04
#define REG_06         0xB800b        //Endereço real 11 0x0b  o pico enxerga 0x05
#define REG_07         0xB800d        //Endereço real 13 0x0d  o pico enxerga 0x06
#define REG_08         0xB800f        //Endereço real 15 0x0f  o pico enxerga 0x07
#define REG_09         0xB8011        //Endereço real 17 0x11  o pico enxerga 0x08
#define REG_0A         0xB8013        //Endereço real 19 0x13  o pico enxerga 0x09
#define REG_0B         0xB8015        //Endereço real 21 0x15  o pico enxerga 0x0A
#define REG_0C         0xB8017        //Endereço real 23 0x17  o pico enxerga 0x0B
#define REG_0D         0xB8019        //Endereço real 25 0x19  o pico enxerga 0x0C
#define REG_0E         0xB801b        //Endereço real 27 0x1b  o pico enxerga 0x0D
#define REG_0F         0xB801d        //Endereço real 29 0x1d  o pico enxerga 0x0E
#define REG_10         0xB801f        //Endereço real 31 0x1f  o pico enxerga 0x0F
#define REG_11         0xB8021        //Endereço real 33 0x21  o pico enxerga 0x10
#define REG_12         0xB8023        //Endereço real 35 0x23  o pico enxerga 0x11
#define REG_13         0xB8025        //Endereço real 37 0x25  o pico enxerga 0x12
#define SET_MODE       0xB8027        //Endereço real 39 0x27  o pico enxerga 0x13 (0=Texto+Scroll, 1=Texto Fixo, 2=320x200, 3=640x200)
#define SET_TXT_COLOR  0xB8029        //Endereço real 41 0x29  o pico enxerga 0x14
#define CHANGE_CUR_POS 0xB802b        //Endereço real 43 0x2b  o pico enxerga 0x15
#define REG_X_HIGH     0xB802d        //Endereço real 45 0x2d  o pico enxerga 0x16
#define REG_X_LOW      0xB802f        //Endereço real 47 0x2f  o pico enxerga 0x17
#define REG_Y_HIGH     0xB8031        //Endereço real 49 0x31  o pico enxerga 0x18
#define REG_Y_LOW      0xB8033        //Endereço real 51 0x33  o pico enxerga 0x19
#define CHANGE_BUFFER  0xB8035        //Endereço real 53 0x35  o pico enxerga 0x1A
#define SELECT_SCREEN  0xB8037        //Endereço real 55 0x37  o pico enxerga 0x1B
#define SET_HORIZONTAL 0xB8039        //Endereço real 57 0x39  o pico enxerga 0x1C
#define SET_VERTICAL   0xB803b        //Endereço real 59 0x3b  o pico enxerga 0x1D
#define RUN_CMD        0xB803d        //Endereço real 61 0x3d  o pico enxerga 0x1E
#define CORINGA        0xB803f        //Endereço real 63 0x3f  o pico enxerga 0x1F





//Commands
#define CMD_SYSTEM_ENABLE   0xA5
#define CMD_CLEAR_SCREEN    0xA4
#define CMD_SET_CUR_POS     0xA3
#define CMD_SET_TXT_COLOR   0xA2
#define CMD_GO_HOME         0xA1

unsigned char *vga_run_cmd = (unsigned char *)RUN_CMD;

#define BAUD_DIV_L  0x08 //(BAUD_DIV&$FF)
#define BAUD_DIV_U  0x00 //((BAUD_DIV>>8)&$FF)


void init_uart999(){
    unsigned char *uart_reg = (unsigned char *)UART_KEYBOARD;
    *(uart_reg+LCR) = 0x83;          // 8 data bits, no parity, 1 stop bit, DLAB=1
    *(uart_reg+DLL) = 8; //BAUD_DIV_L;    // set divisor latch low byte
    *(uart_reg+DLM) = 0; //BAUD_DIV_U;    // set divisor latch high byte
    *(uart_reg+LCR) = 0x03;          // disable divisor latch     
    *(uart_reg+FCR) = 0xC7;          // enable FIFO
    // 4. Limpa registradores de controle
    *(uart_reg + MCR) = 0x00;          
    *(uart_reg + IER) = 0x00; // Garante que interrupções estão desligadas    
}

void init_uart(){
    volatile unsigned char *uart_reg = (volatile unsigned char *)UART_KEYBOARD;
    
    // 1. Entra no modo DLAB para configurar Baud Rate
    *(uart_reg + LCR) = 0x83;          
    *(uart_reg + DLL) = 8;    // Divisor para 115200 com 14.7456MHz
    *(uart_reg + DLM) = 0; 
    // 2. Sai do modo DLAB e define 8N1 (MUITO IMPORTANTE: usar 0x03)
    *(uart_reg + LCR) = 0x03;          

    // 3. Configura FIFO (Habilita, limpa buffers e seta trigger de 14 bytes)
    *(uart_reg + FCR) = 0xC7;          
    
    // 4. Limpa registradores de controle
    *(uart_reg + MCR) = 0x00;          
    *(uart_reg + IER) = 0x00; // Garante que interrupções estão desligadas
}       
/*
void read_uart_correta(){
    volatile unsigned char *uart_reg = (volatile unsigned char *)UART_KEYBOARD;
    unsigned char ch=0;
//    unsigned char ch1=0x41;
    unsigned char buf[16];
    unsigned char size=0,start=0;
    printf("Aguardando Loopback na UART 3...\n");

    while(1){
        // 1. Espera o transmissor ficar livre (Bit 5 = 1)
//        while( !(*(uart_reg + LSR) & 0x20) ); 
//        
//        // 2. Envia o caractere
//        *(uart_reg + THR) = ch1++;  
//        if( ch1 > 0x80)
//            ch1 = 0x41;

        // 3. Espera o dado chegar no receptor (Bit 0 = 1)
        // Se ficar travado aqui, a UART não está recebendo o próprio sinal no RX
        while( !(*(uart_reg + LSR) & 0x01) ){};
        ch = *(uart_reg + RHR);  
        //printf("%02x ", ch);
        if(ch == 0x57)
            start=1;
        if(start){
            buf[size]=ch;
            size++;
            if(size == 3){
                if( buf[2] != 0x88){
                    size = 0;
                    start= 0;
                    continue;
                }
            }
            if(size >=11){ 
                size = 0;
                start= 0;
                if( buf[2] == 0x88){
                    for(int i=0;i <= 11;i++){
                        printf("%02x",buf[i]);
                    }
                    printf("\n");
                }
            }
        }
        // 4. Se chegou aqui, o bit subiu!
        delay(0x00FF); 
    }
}
*/
// Tabela simplificada de Tradução HID para ASCII
// Índice é o ScanCode, valor é o caractere
const char hid2ascii[] = {
    0, 0, 0, 0, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2', 
    '3', '4', '5', '6', '7', '8', '9', '0', 0x0D, 0x1B, 0x08, 0x09, ' '
};

unsigned char get_keypress() {
    volatile unsigned char *uart_reg = (volatile unsigned char *)UART_KEYBOARD;
    //unsigned char header1;
    //unsigned char header2;
    unsigned char scancode;
    int i;

    while(1) {
        // 1. Procura o primeiro byte do cabeçalho (0x57)
        while(!(*(uart_reg + LSR) & 0x01)); 
        if (*(uart_reg + RHR) != 0x57) {
            while((*(uart_reg + LSR) & 0x01)){
                *(uart_reg + RHR);
            }
            continue;
        }

        // 2. Procura o segundo byte (0xAB)
        while(!(*(uart_reg + LSR) & 0x01)); 
        if (*(uart_reg + RHR) != 0xAB) {
            while((*(uart_reg + LSR) & 0x01)){
                *(uart_reg + RHR);
            }
            continue;
        }
        // 2. Procura o segundo byte (0x88)
        while(!(*(uart_reg + LSR) & 0x01)); 
        if (*(uart_reg + RHR) != 0x88){
            while((*(uart_reg + LSR) & 0x01)){
                *(uart_reg + RHR);
            }
            continue;
        }

        // 3. Se chegou aqui, o pacote é legítimo. 
        // Vamos pular os bytes: Tipo (1), Modificador (1) e Reservado (1)
        for(i = 0; i <= 3; i++) {
            while(!(*(uart_reg + LSR) & 0x01));
            *(uart_reg + RHR); // Lê e descarta
        }

        // 4. O sexto byte é o que nos interessa: ScanCode!
        while(!(*(uart_reg + LSR) & 0x01));
        scancode = *(uart_reg + RHR);

        // 5. Filtro de "Key Up": O CH9350 manda 0x00 quando soltamos a tecla.
        // Se você quiser apenas a tecla pressionada, ignore o 0x00.
        if (scancode != 0) {
            return scancode;
        }
    }
}
 /*
void get_version(){
    volatile unsigned char *uart_reg = (volatile unsigned char *)UART_KEYBOARD;
    // Tente ler a versão do chip
    while( !(*(uart_reg + LSR) & 0x20) ){}; *(uart_reg + THR) = 0xAB;
    while( !(*(uart_reg + LSR) & 0x20) ){}; *(uart_reg + THR) = 0x01;
    while( !(*(uart_reg + LSR) & 0x20) ){}; *(uart_reg + THR) = 0x57;
    while( !(*(uart_reg + LSR) & 0x20) ){}; *(uart_reg + THR) = 0x03; // Checksum (57+AB+01)
}*/
void set_keyboard_leds(unsigned char led_status) {
    volatile unsigned char *uart_reg = (volatile unsigned char *)UART_KEYBOARD;
    
    unsigned char buf[11];
    
    buf[0] = 0x57;
    buf[1] = 0xAB;
    buf[2] = 0x12; // Comando de escrita de dados HID
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 0x00;
    buf[6] = 0x00;
    buf[7] = 0x02; // Report Type (Output)
    buf[8] = led_status; // O bitmask dos LEDs (0x01, 0x02, 0x04)
    buf[9] = 0x0F; // Constante de preenchimento (comum nesse protocolo)
    
    // Checksum: soma de buf[0] até buf[9]
    unsigned char ck = 0;
    for(int i = 0; i < 10; i++) {
        ck += buf[i];
    }
    buf[10] = ck;

    // Envio para a UART
    for(int i = 0; i < 11; i++) {
        while( !(*(uart_reg + LSR) & 0x20) ); 
        *(uart_reg + THR) = buf[i];
    }
}
unsigned char keyboard_led_state=0;
void toggle_caps_lock() {
    // Inverte apenas o bit 1 (Caps Lock)
    keyboard_led_state ^= 0x02; 
    set_keyboard_leds(keyboard_led_state);
}
/*
void set_transparent_mode() {
    volatile unsigned char *uart_reg = (volatile unsigned char *)UART_KEYBOARD;

    // Array com o comando completo: Header(57 AB), Cmd(01), Param(00), Checksum(01)
    unsigned char cmd[] = {0x57, 0xAB, 0x01, 0x00, 0x01};

    for(int i = 0; i < 5; i++) {
        // Aguarda o Line Status Register (LSR) indicar que o 
        // Transmitter Holding Register (THR) está vazio (bit 5 - 0x20)
        while (!(uart_reg[LSR] & 0x20)); 
        
        // Escreve o byte no Transmit Holding Register
        uart_reg[THR] = cmd[i];
    }
}*/
void main_loop() {
    init_uart();
    //set_transparent_mode();
    printf("Desligando led...");
    delay(0x8FF);
    printf("\nOrion68K Online. Digite algo:\n");
    set_keyboard_leds(0x02);
//    get_version();
//    while(1){
//        read_uart_correta();
//    }
    while(1) {
        unsigned char code = get_keypress();
        
        // Traduz para ASCII usando a tabela
        if (code < sizeof(hid2ascii)) {
            char c = hid2ascii[code];
            if (c != 0) {
                printf("%c", c); // Ecoa no terminal do console
            }
        }
    }
}
void vga_set_txt_mode(unsigned char mode){
    unsigned char *config_reg_txt_mode = (unsigned char *)SET_MODE;

    *config_reg_txt_mode = (unsigned char)mode;

}

void vga_set_x(unsigned short x) {
    unsigned char *config_reg_x_low = (unsigned char *)REG_X_LOW;
    unsigned char *config_reg_x_high = (unsigned char *)REG_X_HIGH;
    if( x < 80 ){
        *config_reg_x_low  = (unsigned char)x;
    }else{
        *config_reg_x_low  = (unsigned char)(x & 0xFF);
        *config_reg_x_high = (unsigned char)(x >> 8);
    }
}
void vga_set_y(unsigned short y) {
    unsigned char *config_reg_y_low = (unsigned char *)REG_Y_LOW;
    unsigned char *config_reg_y_high = (unsigned char *)REG_Y_HIGH;
    if ( y < 80 ){
        *config_reg_y_low  = (unsigned char)y;
    }else{
        *config_reg_y_low  = (unsigned char)(y & 0xFF);
        *config_reg_y_high = (unsigned char)(y >> 8);
    }
}
void vga_set_txt_color(unsigned char color){
    unsigned char *config_reg_txt_color = (unsigned char *)SET_TXT_COLOR;

    *config_reg_txt_color = (unsigned char)color;
}
void vga_go_home(){
    *vga_run_cmd = CMD_GO_HOME;
}
unsigned short read_uint() {
    unsigned short val = 0;
    char c;
    while (1) {
        c = getchar(); // Lê um caractere da serial
        if (c == '\r' || c == '\n' || c == ' ') break; // Para no Enter ou Espaço
        if (c >= '0' && c <= '9') {
            putchar(c); // Ecoa o que você digitou
            val = val * 10 + (c - '0');
        }
    }
    putchar('\n');
    return val;
}

void imprime_char(char ch)
{
    unsigned char *screen_reg = (unsigned char *)WRITE_SCREEN;
    *screen_reg = ch;

}

void read_str(char *str) {
    char c;
    char *p = str;

    while (1) {
        c = getchar(); // Lê um caractere
        // Para no Enter (Unix usa \n, alguns terminais mandam \r)
        if (c == '\n' || c == '\r') {
            break;
        }
        // Armazena o caractere no ponteiro e avança
        *p = c;
        p++;
        imprime_char(c);
    }
    *p = '\0'; // FINALIZA A STRING (Essencial em C!)
    putchar('\n');
    imprime_char('\0');
    imprime_char('\n');
}
void clrscr()
{
    *vga_run_cmd = CMD_CLEAR_SCREEN;
}



void imprime_tbl_ascii()
{
    unsigned char *screen_reg = (unsigned char *)WRITE_SCREEN;

    for(unsigned char i=0x20;i<0x80;i++){
        *screen_reg = i;
        delay(50);
    }

}

void show_menu(){
    int choice;
    unsigned short pos_x;
    unsigned short pos_y;

    choice = 'A';
    while (choice != 1000){
        printf("\n--- TCPBOX68K VIDEO TEST ---\n");
        printf("1 - Clear Screen\n");
        printf("2 - Set X Position\n");
        printf("3 - Set text and bg color \n");
        printf("4 - Envia string\n");
        printf("5 - Imprime tbl ascii\n");
        printf("6 - Imprime um caractere\n");
        printf("7 - Home\n");
        printf("8 - Troca modo tela");

        printf("0 - sai do programa\n");
        printf("\nEscolha: ");
        choice = getchar();
        printf("choice [%x04]\n",choice);
        switch(choice){
            case '1':
                printf("Limpando a tela\n");
                clrscr();
                break;
            case '2':
                printf("Digite a posicao X (0-639): ");
                pos_x = read_uint();
                vga_set_x(pos_x);
                printf("X definido para %d\n", pos_x);
                printf("Digite a posicao Y (0-319): ");
                pos_y = read_uint();
                vga_set_y(pos_y);
                printf("Y definido para %d\n", pos_y);
           // case '4': //Change cursor position.
                *vga_run_cmd =CMD_SET_CUR_POS;
                break;
            case '3':
                unsigned char textColor,bgColor,color;
                printf("Digite   a  cor  do  texto [0-8]: ");
                textColor = read_uint();
                printf("Digite a cor do background [0-8]: ");
                bgColor = read_uint();
                color = (unsigned char )((textColor<<4)&0xF0) | bgColor;
                printf("textColor[%x][%x] bgColor[%x] color[%x]",((textColor<<4)&0xF0),textColor,bgColor,color);
                vga_set_txt_color(color);
                break;
            case '4':
                printf("Escolhido opcao 4\n");
                char cstr[256];
                read_str(cstr);
                break;
            case '5':
                printf("\nVamos imprimir a tabela ascii\n");
                imprime_tbl_ascii();
                break;
            case '6':
                printf("Digite um caractere: ");
                char ch = getchar();
                imprime_char(ch);
                break;
            case '7':
                printf("Go home");
                vga_go_home();
                break;
            case '8':
                unsigned char mode;
                printf("Digite 0=320x200 1=640x400: ");
                mode = read_uint();
                vga_set_txt_mode(mode);
                break;
            case '0':
                return;

                break;
        }
    }
}


int main() {
    //char str[10]={0};
    check_stack();  // ✅ Verificar stack no início
/* Criando ponteiros de 8 bits (unsigned char) para os endereços */

    unsigned char *config_reg = (unsigned char *)CONFIG_REG;

    printf("\nTesting keyboard...\n");
    main_loop();



    printf("\n--- Teste de Video Card com Pi Pico V2 2603 ---\n");

    printf("Vou testar a placa de video: enviando SYSTEM_ENABLE \n");
    *config_reg = CMD_SYSTEM_ENABLE;



    show_menu();

    return 0;
}


