#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "timers.h"
#include "orion68.h"
#include "picow.h"

// Definição dos registradores mapeados na memória do m68k
// Usamos 'volatile uint8_t' para obrigar o m68k a ler o hardware toda vez
#define PICO_DATA_REG    (*(volatile uint8_t *)0xFF9101)
#define PICO_STATUS_REG  (*(volatile uint8_t *)0xFF9103)
#define PICO_SIZE_HIGH   (*(volatile uint8_t *)0xFF9105)
#define PICO_SIZE_LOW    (*(volatile uint8_t *)0xFF9107)
#define PICO_CRC_REG3    (*(volatile uint8_t *)0xFF9109)
#define PICO_CRC_REG2    (*(volatile uint8_t *)0xFF910B)
#define PICO_CRC_REG1    (*(volatile uint8_t *)0xFF910D)
#define PICO_CRC_REG0    (*(volatile uint8_t *)0xFF910F)

#define PICO_SET_COLOR    (*(volatile uint8_t *)0xFF9127)
#define PICO_WRITE_STR    (*(volatile uint8_t *)0xFF9129)
#define PICO_WRITE_CH     (*(volatile uint8_t *)0xFF912B)

// Estados do STATUS REGISTER que definimos na PIO do Pico
#define PICO_STATE_IDLE       0x00
#define PICO_STATE_HAS_FILE   0x01
#define PICO_STATE_EOF        0x02

extern void video_puts(const char *s);

#define MIO_BASE_ADDRESS    0xFF9100
#define MIO_SECTOR_LOW_REG      (*(volatile uint8_t *)(MIO_BASE_ADDRESS + (SECTOR_LOW_REG*2) +1))
#define MIO_SECTOR_HIGH_REG     (*(volatile uint8_t *)(MIO_BASE_ADDRESS + (SECTOR_HIGH_REG*2)+1))
#define MIO_SECTOR_SEC_LOAD_REG (*(volatile uint8_t *)(MIO_BASE_ADDRESS + (SECTOR_SEC_LOAD_REG*2)+1))
//#define MIO_SECTOR_READ_REG     (*(volatile uint8_t *)(MIO_BASE_ADDRESS + (SECTOR_READ_REG*2)+1))
#define MIO_SECTOR_READ_REG     (*(volatile uint8_t *)0xFF911B)

#define KBD_DATA   (*((volatile unsigned char *)(0x00FF9113)))

void pico_write_ch(uint8_t ch){
    PICO_WRITE_CH = ch;    
}

/*NÃO USE ISSO TEM UMA ARMADILHA ESCONDIDA NISSO 
  QUE ME FEZ PERDER UM DIA INTEIRO PROCURANDO DE
  TÃO SUTIL QUE ELE É.
  QUANDO ALGUÉM CHAMA A CPU FICA ESPERANDO ETERNA
  MENTE O PRESSIONAR DE UMA TECLA.
  A SUTILESA: KBD É UM PERIFÉRICO DO PICOW QUE
  QUANDO A CPU TENTA LER ELA FICA ESPERANDO POR
  UM DTACK QUE SÓ VEM SE TIVER UMA TECLA PRESSIONADA
  NADA MAIS FUNCIONA*/
char getkbd(){
    return KBD_DATA;
}