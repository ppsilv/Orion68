#ifndef SLOTS_H
#define SLOTS_H

#include <stdint.h>


/*
 * 10 slots de 256K a partir de 0x00092000.
 *
 * CONFIRME antes de usar: isso precisa bater com o resto do seu mapa
 * de memoria (kernel.bin em 0x82000, a regiao antiga USERRAM de
 * 512K do linker.ld original, etc -- essas faixas NAO podem se
 * sobrepor). Ajuste SLOT_BASE/SLOT_SIZE/SLOT_COUNT conforme o que
 * sobrar de RAM livre na sua placa.
 */
#define SLOT_BASE       0x00092000UL
#define SLOT_SIZE       0x00040000UL   /* 256K por slot 1024K*/
#define SLOT_COUNT      10

/*
#define SLOT_SIZE_256   0x00040000UL   // 256K por slot 1024K
#define SLOT_COUNT_256  4
#define SLOT_SIZE_128   0x00020000UL   // 128K por slot 1024K
#define SLOT_COUNT_128  7
#define SLOT_SIZE_64    0x00010000UL   //  64K por slot  640K
#define SLOT_COUNT_64   10
#define SLOT_SIZE_32    0x00008000UL   //  32K por slot  640K
#define SLOT_COUNT_32   20
#define SLOT_SIZE_16    0x00004000UL   //  16K por slot  640K
#define SLOT_COUNT_16   40
*/


void     Slots_Init(void);
int      Slots_Alloc(void);          /* devolve indice 0..SLOT_COUNT-1, ou -1 se nao tiver slot livre */
void     Slots_Free(int slot_index);
uint32_t Slots_BaseAddr(int slot_index);

#endif
