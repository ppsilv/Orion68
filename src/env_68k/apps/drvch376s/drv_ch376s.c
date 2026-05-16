#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <mc68000.h>

#define CH376_DATA      ((volatile uint8_t *) 0x04601)
#define CH376_CMD       ((volatile uint8_t *) 0x04603)


#define CMD_GET_IC_VER    0x01
#define CMD_SET_BAUDRATE  0x02
#define CMD_RESET_ALL     0x05
#define CMD_CHECK_EXIST   0x06
#define CMD_GET_FILE_SIZE 0x0C
#define CMD_SET_USB_MODE  0x15
#define CMD_GET_STATUS    0x22
#define CMD_RD_USB_DATA0  0x27
#define CMD_WR_USB_DATA   0x2C
#define CMD_WR_REQ_DATA   0x2D
#define CMD_WR_OFS_DATA   0x2E
#define CMD_SET_FILE_NAME 0x2F
#define CMD_DISK_CONNECT  0x30
#define CMD_DISK_MOUNT    0x31
#define CMD_FILE_OPEN     0x32
#define CMD_FILE_ENUM_GO  0x33
#define CMD_FILE_CREATE   0x34
#define CMD_FILE_ERASE    0x35
#define CMD_FILE_CLOSE    0x36
#define CMD_DIR_INFO_READ 0x37
#define CMD_DIR_INFO_SAVE 0x38
#define CMD_BYTE_LOCATE   0x39
#define CMD_BYTE_READ     0x3A
#define CMD_BYTE_RD_GO    0x3B
#define CMD_BYTE_WRITE    0x3C
#define CMD_BYTE_WR_GO    0x3D
#define CMD_DISK_CAPACITY 0x3E
#define CMD_DISK_QUERY    0x3F
#define CMD_DIR_CREATE    0x40

#define USB_INT_SUCCESS    0x14
#define USB_INT_CONNECT    0x15
#define USB_INT_DISCONNECT 0x16
#define USB_INT_BUF_OVER   0x17
#define USB_INT_USB_READY  0x18
#define USB_INT_DISK_READ  0x1D
#define USB_INT_DISK_WRITE 0x1E
#define USB_INT_DISK_ERR   0x1F
#define YES_OPEN_DIR       0x41
#define ERR_MISS_FILE      0x42
#define ERR_FOUND_NAME     0x43
#define ERR_DISK_DISCON    0x82
#define ERR_LARGE_SECTOR   0x84
#define ERR_TYPE_ERROR     0x92
#define ERR_BPB_ERROR      0xA1
#define ERR_DISK_FULL      0xB1
#define ERR_FDT_OVER       0xB2
#define ERR_FILE_CLOSE     0xB4

#define DATA_TYPE_USB     0x06


#define true 1
#define false 0
//typedef uint8_t bool;

static void long_pause() {
    // Aproximadamente 210ms a 16MHz
    // Usando 187000 para arredondar a margem de erro do C
    volatile uint32_t i;
    for(i = 0; i < 187000; i++);
}

static void medium_pause() {
    // Aproximadamente 100ms
    volatile uint32_t i;
    for(i = 0; i < 89000; i++);
}
/*
static void short_pause() {
    // Pausa técnica para comandos rápidos (micro-pausa)
    volatile uint32_t i;
    for(i = 0; i < 500; i++);
}
*/
static inline void send_command( uint8_t cmd){
    *CH376_CMD = cmd;
}
static inline void send_data( uint8_t data){
    *CH376_DATA = data;
}
static inline uint8_t read_data(){
    uint8_t ret = *CH376_DATA;
    return ret;
}

static bool read_status_byte(){
    send_command(CMD_GET_STATUS);
    if( read_data() == 0){
        return true;
    }
    return false;
}

//Publics functions
void reset_ch376_module(){
    send_command(CMD_RESET_ALL);
    long_pause();
    long_pause();
}

bool check_module_exists() {
    uint8_t test_val = 0x55;
    uint8_t res;

    printf("Testando CH376S no I/O 0x460x...\n");

    send_command(CMD_CHECK_EXIST);
    send_data(test_val);
    
    res = (uint8_t)read_data(); // O chip DEVE inverter para 10101010 (0xAA)

    if (res == 0xAA) {
        printf("SINAL VERDE: O 68000 leu 0xAA. Barramento OK!\n");
    } else {
        printf("ERRO: Leu 0x%02X. Verifique GAL e fiaçao.\n", res);
        return false;
    }
    return true;
}
bool set_usb_host_mode(){
    reset_ch376_module ();
    send_command(CMD_SET_USB_MODE);
    send_data(DATA_TYPE_USB);
    long_pause();
    long_pause();
    long_pause();
    long_pause();
    uint8_t res = read_status_byte();
    printf("Resultado de set usb mode: %02x\n",res);
    if( res == USB_INT_CONNECT ){
        printf("SUCCESS: disk found...\n");
        return true;
    }
    printf("ERROR: No disk...\n");
    return false;
}

bool connect_to_usb_drive(){
    reset_ch376_module();
    if(set_usb_host_mode()){
        printf("connect_to_usb_drive:CH376 found and connected.\n");
        return true;
    }
    printf("connect_to_usb_drive:CH376 found and NOT connected.\n");
    return false;
}



bool connect_to_disk(){
    send_command(CMD_DISK_CONNECT);
    if ( read_status_byte() == false ){
        return true;
    }
    printf("connect_to_disk: ERROR connecting to USB Disk.\n");
    return false;
}

bool mount_disk(){
    send_command(CMD_DISK_MOUNT);
    if ( read_status_byte() == false ){
        printf("mount_disk: ERROR mounting to USB Disk.\n");
        return false;
    }
    return true;
}

bool configure_memorystick(){
    reset_ch376_module();
    if(check_module_exists() == false){
        printf("No module found...\n");
        return false;
    }
    for(int i=0; i<5; i++){
        if(connect_to_usb_drive() == true){
            goto SAIDA_OK;
        }
        long_pause();
        long_pause();
        long_pause();
        long_pause();
    }    
    printf("No module found...\n");
    return false;

SAIDA_OK:
    if ( connect_to_disk() ){
        if ( mount_disk() ){
            printf("Disk mounted...\n");
            return true;
        }
    }
    return false;
}
void get_module_version(){
    send_command(CMD_GET_IC_VER);
    medium_pause();
    uint8_t res = read_data();
    printf("Found CH376S V [%02x]\n",res & 0x1F);
}
void main(){
    reset_ch376_module();
    get_module_version();
    configure_memorystick();
    get_module_version();
    get_module_version();
    get_module_version();
    get_module_version();

}