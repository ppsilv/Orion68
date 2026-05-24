* ===================================================================
*             MAPA DE SÍMBOLOS DA ROM DO COMPUTADOR PDS317
* ===================================================================
* Inclua este arquivo no topo dos seus programas usando: 
* include "pds317_rom.inc" (ou usando a diretiva do seu assembler)

delay                  EQU   $8034
mymemset               EQU   $804C
init_uart              EQU   $8064
read_kbd               EQU   $8090
uart_read              EQU   $80A0
write_kbd              EQU   $80A2
get_0x81               EQU   $80B6
get_0x87               EQU   $8122
set_keyboard_leds      EQU   $8148
get_0x88               EQU   $81B6
get_packet             EQU   $8256
reset_por_software     EQU   $82C2
set_transparent_mode   EQU   $82CA
get_kbd_key            EQU   $8308
get_key                EQU   $843e

