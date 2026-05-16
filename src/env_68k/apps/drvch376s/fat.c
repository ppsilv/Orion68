#include <stdio.h>
#include <stdlib.h>

#include <mc68000.h>

#define CH376_DATA      ((volatile unsigned char *) 0x04601)
#define CH376_CMD       ((volatile unsigned char *) 0x04603)

/* Códigos de Resposta Comuns */
#define USB_INT_SUCCESS 0x14
#define USB_INT_CONNECT 0x15
#define USB_INT_DISCONNECT 0x16
#define USB_INT_BUF_OVER 0x17
#define ERR_MISS_FILE   0x41
#define ERR_FOUND_NAME  0x42

/* Macros de Auxílio */
#define CH376_DELAY()   for(volatile int i=0; i<20; i++) // Ajuste conforme seu Clock

// Se você ligou o CH376S no D0-D7 (LDS), os endereços serão ÍMPARES
// Se ligou no D8-D15 (UDS), os endereços serão PARES.
// Assumindo LDS (D0-D7) e A0 do chip no A1 do 68k:

// Comandos Básicos
#define CMD_GET_IC_VER    0x01
#define CMD_CHECK_EXIST   0x06
#define CMD_SET_USB_MODE  0x15

void long_pause() {
    // Aproximadamente 210ms a 16MHz
    // Usando 187000 para arredondar a margem de erro do C
    volatile uint32_t i;
    for(i = 0; i < 187000; i++);
}

void medium_pause() {
    // Aproximadamente 100ms
    volatile uint32_t i;
    for(i = 0; i < 89000; i++);
}

void short_pause() {
    // Pausa técnica para comandos rápidos (micro-pausa)
    volatile uint32_t i;
    for(i = 0; i < 500; i++);
}

void check_module_exists() {
    unsigned char test_val = 0x55;
    unsigned char res;

    printf("Testando CH376S no I/O 0x460x...\n");

    *CH376_CMD = CMD_CHECK_EXIST;
    *CH376_DATA = test_val;
    
    res = *CH376_DATA; // O chip DEVE inverter para 10101010 (0xAA)

    if (res == 0xAA) {
        printf("SINAL VERDE: O 68000 leu 0xAA. Barramento OK!\n");
    } else {
        printf("ERRO: Leu 0x%02X. Verifique GAL e fiaçao.\n", res);
    }
}

void reset_ch376_module(){
    
}


void write_cmd(unsigned char cmd) {
    CH376_DELAY();
    *CH376_CMD = cmd;
    CH376_DELAY();
}

void write_data(unsigned char data) {
    *CH376_DATA = data;
}

unsigned char read_data() {
    CH376_DELAY();
    return *CH376_DATA;
}

/* Espera o chip terminar uma operação (Interrupt Status) */
unsigned char wait_interrupt() {
    unsigned char status;
    // No modo paralelo, o CH376S avisa que terminou via registrador de status
    // Ou você pode ler o pino INT se tiver ligado na GAL/CPU
    while (1) {
        write_cmd(0x22); // CMD_GET_STATUS
        status = read_data();
        if (status < 0x80) return status; // Status de interrupção válido
    }
}

/* Inicializa o modo (0x06 para Pendrive, 0x03 para SD se você fizer o mod) */
unsigned char set_usb_mode(unsigned char mode) {
    write_cmd(0x15); // CMD_SET_USB_MODE
    write_data(mode);
    CH376_DELAY();
    return read_data(); 
}

/* Monta o disco (Obrigatório antes de abrir arquivos) */
unsigned char disk_mount() {
    write_cmd(0x31); // CMD_DISK_MOUNT
    return wait_interrupt();
}

/* Abre um arquivo pelo nome (Ex: "DISK_A.DSK") */
unsigned char file_open(const char *name) {
    write_cmd(0x2F); // CMD_SET_FILE_NAME
    while (*name) {
        write_data(*name++);
    }
    write_data(0); // Null terminator
    
    write_cmd(0x32); // CMD_FILE_OPEN
    return wait_interrupt();
}

/* Posiciona o ponteiro (Crucial para o CP/M ler setores específicos) */
void file_seek(unsigned long offset) {
    write_cmd(0x39); // CMD_FILE_SEEK
    write_data((unsigned char)(offset));
    write_data((unsigned char)(offset >> 8));
    write_data((unsigned char)(offset >> 16));
    write_data((unsigned char)(offset >> 24));
}

/* Lê um bloco de dados (O CP/M trabalha com 128 bytes) */
unsigned char file_read(unsigned char *buffer, unsigned char len) {
    write_cmd(0x3A); // CMD_BYTE_READ
    write_data(len);
    write_data(0);   // High byte do len (sempre 0 para blocos pequenos)
    
    unsigned char status = wait_interrupt();
    if (status == 0x1D) { // USB_INT_DISK_READ (Dados prontos)
        write_cmd(0x27); // CMD_RD_USB_DATA0
        unsigned char count = read_data();
        for (int i = 0; i < count; i++) {
            *buffer++ = read_data();
        }
        write_cmd(0x3B); // CMD_BYTE_RD_GO (Continua se for maior que o buffer)
        wait_interrupt();
    }
    return status;
}

unsigned char file_write(unsigned char *buffer, unsigned char len) {
    write_cmd(0x3C); // CMD_BYTE_WRITE
    write_data(len);
    write_data(0);   // High byte (0 para 128 bytes)

    unsigned char status = wait_interrupt();
    
    // O chip pede os dados (Status 0x1E - USB_INT_DISK_WRITE)
    if (status == 0x1E) { 
        write_cmd(0x2D); // CMD_WR_USB_DATA7
        write_data(len); // Informa quantos bytes vai enviar agora
        
        for (int i = 0; i < len; i++) {
            write_data(*buffer++);
        }
        
        write_cmd(0x3D); // CMD_BYTE_WR_GO (Manda o chip gravar de fato)
        status = wait_interrupt(); // Espera finalizar a gravação
    }
    return status; // Deve retornar USB_INT_SUCCESS (0x14)
}

// Estrutura simplificada do diretório FAT (32 bytes)
typedef struct {
    char name[11];      // Nome 8.3 (SEM o ponto, ex: "TESTE   TXT")
    unsigned char attr; // Atributos (Diretório, Somente Leitura, etc)
    char reserved[10];
    unsigned short time;
    unsigned short date;
    unsigned short start_cluster;
    unsigned long size; // Tamanho do arquivo em bytes
} FAT_DIR_INFO;

void listar_diretorio();
void set_file_name(const char *name);
void ler_arquivo_bob() ;
void main() {
    unsigned char res;
    
    test_ch376();

    res = set_usb_mode(0x06);
    for(volatile int i=0; i<0xFFFF; i++);
    if ( res == 0x51) { // 0x51 = Sucesso no comando
        printf("CH376S configurado como Host!\n");
        write_cmd(0x22); // CMD_GET_STATUS
        read_data();     // Lê e descarta o 0x51 que está "preso" lá

int tentativas = 0;
do {
    res = disk_mount();
    if (res == USB_INT_SUCCESS) break;
    for(volatile int i=0; i<0xFFFF; i++); // Espera um pouco
    tentativas++;
} while (tentativas < 3);
    unsigned char status=0;

        if ( res == USB_INT_SUCCESS) {
            printf("Pendrive montado!\n");
            // Agora é só abrir seu arquivo de disco do CP/M...
            set_file_name("/backup.sh");
            write_cmd(0x32);
            status = wait_interrupt();
            printf("Status abrindo BOB.TXT: 0x%02X\n", status);
            ler_arquivo_bob();
        }else{
            printf("disk_mount respondeu [%02X]\n",res);
        }
    }else{
        printf("ch376s not configured received [%02X]\n",res);
    }
}


#define CMD_GET_STATUS      0x22
#define CMD_SET_FILE_NAME   0x2F
#define CMD_FILE_OPEN       0x32
#define CMD_FILE_ENUM_GO    0x33
#define CMD_RD_USB_DATA     0x27

#define USB_INT_SUCCESS     0x14
#define USB_INT_DISK_READ   0x1D

// Envia o nome do arquivo/curinga para o chip
void set_file_name(const char *name) {
    write_cmd(CMD_SET_FILE_NAME);
    while (*name) {
        write_data(*name);
        name++;
    }
    write_data(0); // Terminador nulo exigido pelo CH376S
}

// Lê os dados que o chip buscou no disco
void read_usb_data(unsigned char *buffer, unsigned char len) {
    write_cmd(CMD_RD_USB_DATA);
    unsigned char available = read_data(); // O primeiro byte é o tamanho disponível
    if (available > len) available = len;  // Proteção de buffer
    
    for (unsigned char i = 0; i < available; i++) {
        buffer[i] = read_data();
    }
}

void listar_diretorio() {
    unsigned char status;
    FAT_DIR_INFO file_info;

    printf("Listando arquivos no Pendrive:\n");
    printf("--------------------------------\n");

    // 1. Primeiro, garantimos que estamos na RAIZ
    set_file_name("/");
    write_cmd(CMD_FILE_OPEN);
    status = wait_interrupt();
    // Aqui ele deve retornar 0x14 (abriu o diretório /)

    // 2. Agora, configuramos o padrão de busca (Curinga)
    // Importante: Envie apenas o "*" ou "/*" dependendo da versão
    set_file_name("*"); 
    
    // 3. Comando Mágico: ENUM_GO (ou FILE_OPEN com o nome setado para *)
    write_cmd(CMD_FILE_OPEN); 
    status = wait_interrupt();

    // AGORA o status DEVE ser 0x1D (DISK_READ)
    printf("Status após busca com *: 0x%02X\n", status);

    while (status == 0x14) {
        // Lê os 32 bytes da entrada FAT
        read_usb_data((unsigned char*)&file_info, 32);

        // Exibe o nome se não for entrada vazia ou deletada
        if ((unsigned char)file_info.name[0] != 0xE5 && file_info.name[0] != 0x00) {
             printf("%.8s.%.3s  |  %ld bytes\n", file_info.name, &file_info.name[8], file_info.size);
        }

        // Pede o próximo
        write_cmd(CMD_FILE_ENUM_GO);
        status = wait_interrupt();
    }
}
unsigned char wait_interrupt_debug() ;
void ler_arquivo_bob() {
    volatile unsigned char *ch_cmd  = (volatile unsigned char *) 0x4603;
    volatile unsigned char *ch_data = (volatile unsigned char *) 0x4601;
    unsigned char status;

    printf("Pedindo 64 bytes...\n");

    *ch_cmd = 0x3A;      // Comando Read
    for(volatile int i=0; i<200; i++); // ESPERA
    
    *ch_data = 0x40;     // 64 bytes (LSB)
    for(volatile int i=0; i<200; i++); // ESPERA CRUCIAL
    
    *ch_data = 0x00;     // 0 bytes (MSB)
    for(volatile int i=0; i<200; i++); // ESPERA
    
    printf("Parametros enviados. Aguardando status...\n");

    // Se travar AQUI, o chip não entendeu o comando ou o pendrive "engasgou"
    status = wait_interrupt_debug(); 
    printf("Status após CMD_BYTE_READ: 0x%02X\n", status);

    while (status == 0x14) { // 0x1D = USB_INT_DISK_READ
        *ch_cmd = 0x27;      // CMD_RD_USB_DATA
        unsigned char len = *ch_data; 
        
        printf("(Lendo %d bytes): ", len);
        for (int i = 0; i < len; i++) {
            unsigned char c = *ch_data;
            // Só imprime se for caractere visível (evita lixo no terminal)
            if (c >= 32 && c <= 126) printf("%c", c);
            else printf("[%02X]", c);
        }
        printf("\n");

        *ch_cmd = 0x3B;      // CMD_BYTE_RD_GO
        status = wait_interrupt();
    }
    
    if (status == 0x1d) printf("\nLeitura concluída com sucesso!\n");
    else printf("\nErro na leitura: 0x%02X\n", status);
}

unsigned char wait_interrupt_debug() {
    volatile unsigned char *ch_cmd = (volatile unsigned char *) 0x4603;
    volatile unsigned char *ch_data = (volatile unsigned char *) 0x4601;
    unsigned long loop_count = 0;

    // Se o bit 7 for 1, o chip está OCUPADO ou PROCESSANDO
    while ((*ch_cmd & 0x80)) {
        loop_count++;
        if (loop_count > 500000) { // Um tempo razoável para o 68k a 8MHz/10MHz
            printf("\n[DEBUG] O CH376S ignorou o 68000. Timeout!\n");
            return 0xFF; 
        }
    }

    *ch_cmd = 0x22; // CMD_GET_STATUS
    return *ch_data;
}