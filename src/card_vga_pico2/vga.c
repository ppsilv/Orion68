
/*
    Version 1.2.25.16.00: Protothreads eliminated now CORE 0 read bus CORE executes bus task arrived.

*/
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "string.h"
#include "vga_drv.h"
#include "vga_primitives.h"
#include "colors.h"
#include "vga_bus_read.h"
#include "hardware/pio.h"
#include "eeprom.h"
#include "hardware/watchdog.h"

vga_t *vga = NULL ;

#define MAGIC_WARM_BOOT 0xDEADBEEF
#define SCRATCH_REASON_REG watchdog_hw->scratch[0]
#define SCRATCH_MODE_REG   watchdog_hw->scratch[1]
static bool is_coldstart = false;

static repeating_timer_t timer;
static int last_toggle_time = 1;
static bool system_run;
screenMode_t video_mode;


extern PIO bus_pio1;
extern uint bus_sm;
extern bool bus_try_get_event(uint8_t *value,uint8_t *reg,PIO pio, uint sm);

//VGA variables
volatile uint16_t cursor_x = 0;
volatile uint16_t cursor_y = 0;
volatile uint8_t  text_color = 0;
volatile uint8_t  bg_color = 0;
sys_config_t vga_nvc_config;

//Prototypes
void drawPixel(short x, short y, color_t color) ;
void drawHLine(int x, int y, int w, color_t color) ;
void fillRect(short x, short y, short w, short h, color_t color);
bool bus_try_get_event32(uint32_t *value,PIO pio, uint sm);

static bool timer_callback(repeating_timer_t *rt)
{
    if(last_toggle_time == 1){
        put_cursor(1);
        last_toggle_time = 0;
    }
    else{
        put_cursor(0);
        last_toggle_time = 1;
    }
  return true;
}
static void create_timer(bool btimer)
{
    if(btimer){
        cancel_repeating_timer(&timer);
        int16_t tempo = vga->get_blink_interval();
        add_repeating_timer_ms(tempo, timer_callback, NULL, &timer);
    }else{
        cancel_repeating_timer(&timer);
    }
}

void video_welcome_screen(){
    vga->setTextCursorPos(0,0);
    vga->setTextColor(RED, BLACK);
    vga->printString("Orion Vpico2 vga312k   VGA BIOS VRP2350\n");
    vga->setTextColor(YELLOW, BLACK);
    vga->printString("Version 1.2.25.16.00RA\n"); /*1.0 version 21 week 18 day*/
    vga->setTextColor(CYAN, BLACK);
    if ( video_mode < MODE_TEXT_80_S){
        vga->printString("Copyright (C) 2026 pdsilva(aka pgordao).\nV1.2 Vpico2vga312k\n");
    }else{
        vga->printString("Copyright (C) 2026 pdsilva(aka pgordao).V1.2 Vpico2vga312k -2620\n");
    }
    vga->setTextCursorPos(0,4);
    vga->setTextColor(GREEN, BLACK);
}

int verify_start() {

    if (SCRATCH_REASON_REG == MAGIC_WARM_BOOT) {
        is_coldstart = true;
    } else {
        // Coldstart: Primeira vez que liga
        SCRATCH_REASON_REG = MAGIC_WARM_BOOT;
    }
}
#include "pico/multicore.h"
void core1_entry() {
    uint32_t packet;
    uint8_t data, reg;
    static char buf[256]={0};

    while(1) {
        // Fica esperando chegar um pacote do Core 0
        packet = multicore_fifo_pop_blocking();

        data = packet & 0xFF;
        reg = (packet >> 8) & 0xFF;

        // O seu SWITCH original entra aqui purinho, sem protothread!
            switch(reg){    
                case D_RUN_CMD:
                    switch(data){
                        case CMD_SET_CUR_POS:
                            //sprintf(buf,"x:%02X y:%02X\n",cursor_x,cursor_y);
                            vga->setTextCursorPos(cursor_x,cursor_y);
                            break;
                        case CMD_CLEAR_SCREEN:
                            vga->clrscr();
                            cursor_x = cursor_y = 0;
                            break;
                        case CMD_GO_HOME:
                            cursor_x = cursor_y = 0;
                            vga->set_vga_home();
                            break;    
                        default:
                            vga->printString("VGA-Panic: Comando inexistente...\n");
                    }
                    break;
                case D_SET_MODE:                //0x12
                    sprintf(buf,"\n\nSe voce ver isso, nao funcionou data: [%d]\n",data);
                    vga->printString(buf);
                    change_mode((screenMode_t)data);
                    break;
                case D_SET_TXT_COLOR:
                    bg_color = data & 0x0F;
                    text_color = (data>>4) & 0x0F;
                    sprintf(buf,"text [%d] bg[%d]\n",text_color,bg_color);
                    vga->printString(buf);
                    vga->setTextColor(text_color, bg_color);  
                    break;    
                case D_WRITE_SCREEN:    
                        put_cursor(0);
                        vga->setTextCursorVisible(false);
                        put_cursor(0);
                        if( data == 0x08 ){
                            vga->dec_cursor_x();                                
                        }else{
                            if( data > 0x80 ){
                                sprintf(buf,"%d",data);
                                vga->printString(buf);
                            }else{
                                vga->pchar(data);  
                            }
                        }
                        vga->setTextCursorVisible(true);                        
                        put_cursor(1);
                        break;
                case D_REG_X_HIGH:
                        //cursor_x = (data <<8)|cursor_x;
                        // Isola o lixo limpando os 8 bits baixos antigos antes de injetar o HIGH
                        cursor_x = (uint16_t)(data << 8) | (cursor_x & 0x00FF);
                        break;
                case D_REG_X_LOW:
                        //cursor_x = data;// | cursor_x;
                        // Preserva o HIGH atual e atualiza apenas os 8 bits baixos
                        cursor_x = (cursor_x & 0xFF00) | data;
                        //sprintf(buf,"X-L data:%02X cursor_x:%02X",data,cursor_x);
                        //vga->printString(buf);
                        break;
                case D_REG_Y_HIGH:
                        //cursor_y = (data <<8)|cursor_y;
                        cursor_y = (uint16_t)(data << 8) | (cursor_y & 0x00FF);
                        break;
                case D_REG_Y_LOW:
                        //cursor_y = data;// | cursor_y;
                        cursor_y = (cursor_y & 0xFF00) | data;
                        //sprintf(buf,"Y-L data:%02X cursor_y:%02X",data,cursor_y);
                        //vga->printString(buf);
                        break;
                case D_CHANGE_BUFFER: 
                        vga->printString("NOT impl");
                        break;
                case D_SELECT_SCREEN: 
                        vga->printString("NOT impl");
                        break;
                case D_SET_HORIZONTAL:
                        vga->printString("NOT impl");
                        break;
                case D_SET_VERTICAL:  
                        vga->printString("NOT impl");
                        break;
            } //switch(reg)
    } //while(1)
}

int main(){

    // set the clock
    set_sys_clock_khz(200000, true);

    // start bus read
    initReadBus_Pio();

    // start the serial i/o
    stdio_init_all() ;
    set_sys_clock_khz(150000, true);
      vga = create_screen( MODE_TEXT_80_S ); //, 0, 0, font );
      video_mode = MODE_TEXT_80_S;
      

    drawHLine(0,48,640,YELLOW);
    drawHLine(0,49,640,YELLOW);
    drawHLine(0,50,640,YELLOW);
    drawHLine(0,51,640,YELLOW);

    video_welcome_screen();

    // === config threads ========================
    // for core 0
    create_timer(CURSOR_BLINK_ON); //Com o timer para o cursor ele não engasga como quando controlado pela protothread
    // Lança a função do Core 1 no segundo núcleo
    multicore_launch_core1(core1_entry);
    while (!pio_sm_is_rx_fifo_empty(bus_pio1, bus_sm)) {
        pio_sm_get(bus_pio1, bus_sm); 
    }    

    while(1) {
        uint8_t data, reg;
        if (bus_try_get_event(&data, &reg, bus_pio1, bus_sm)) {
            // Compacta data e reg em um único uint32_t para mandar via FIFO de hardware
            uint32_t packet = ((uint32_t)reg << 8) | data;

            // Envia para o Core 1 de forma ultra-rápida.
            // Se a FIFO encher, ele espera (bloqueia), mas como a FIFO de hardware
            // é rápida, o Core 0 quase nunca para.
            multicore_fifo_push_blocking(packet);
        }
    }


} // end main

#include <string.h>
#include <stdint.h>

#define VBUFFER_SIZE     153600
#define SCREEN_LINES     400
#define BYTES_PER_LINE   (VBUFFER_SIZE / SCREEN_LINES) // 384 bytes por linha gráfica

// Ajuste FONT_HEIGHT para a altura do seu caractere em pixels (ex: 16 pixels para 80x25)
#define FONT_HEIGHT      16
#define BYTES_PER_ROW    (BYTES_PER_LINE * FONT_HEIGHT) // 6.144 bytes por linha de texto

extern uint8_t vga_video_data_array0[VBUFFER_SIZE];

void scroll_up_graphics(void) {
    // 1. Quantidade de bytes do framebuffer que sobem (153.600 - 6.144 = 147.456 bytes)
    size_t bytes_para_copiar = VBUFFER_SIZE - BYTES_PER_ROW;

    // 2. DMA copia da 2ª linha de texto em diante para o início do buffer
    dma_memcpy(vga_video_data_array0,
               vga_video_data_array0 + BYTES_PER_ROW,
               bytes_para_copiar,
               true);

    // 3. Limpa a última linha de texto na parte inferior da tela (preenche com 0x00 = preto)
    memset(vga_video_data_array0 + bytes_para_copiar, 0x00, BYTES_PER_ROW);
}

void __not_in_flash_func(dma_memcpy)(void *dest, const void *src, size_t num, bool block) {
    if (num == 0) return;
    if (num < DMA_MIN_XFER) {
        // memmove, not memcpy: some callers (VRAM copy Esc[Z4, scrolls)
        // pass overlapping ranges, which the incrementing DMA handled
        // correctly for src > dest; memmove is safe for all overlaps.
        memmove(dest, src, num);
        return;
    }

    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;

    dma_channel_config c = dma_channel_get_default_config(memcpy_dma_chan);
    channel_config_set_read_increment(&c, true);
    channel_config_set_write_increment(&c, true);

    // Mismatched alignment: word-mode would corrupt — single byte DMA.
    // Works for both blocking and non-blocking (src is caller-owned).
    if ((((uintptr_t)d ^ (uintptr_t)s) & 3) != 0) {
        channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
        dma_channel_configure(memcpy_dma_chan, &c, d, s, num, true);
        if (block) dma_channel_wait_for_finish_blocking(memcpy_dma_chan);
        return;
    }

    // Head: CPU-copy 0-3 bytes until both pointers are 4-byte aligned.
    while (num > 0 && ((uintptr_t)d & 3) != 0) {
        *d++ = *s++;
        num--;
    }
    if (num == 0) return;

    size_t words = num >> 2;
    size_t tail = num & 3;

    if (block) {
        // Synchronous: word-DMA the bulk (if any), then CPU-copy the tail.
        if (words > 0) {
            channel_config_set_transfer_data_size(&c, DMA_SIZE_32);
            dma_channel_configure(memcpy_dma_chan, &c, d, s, words, true);
            dma_channel_wait_for_finish_blocking(memcpy_dma_chan);
            size_t bulk = words << 2;
            d += bulk;
            s += bulk;
        }
        while (tail > 0) {
            *d++ = *s++;
            tail--;
        }
    } else if (tail == 0) {
        // Async fast path: pure word DMA (num >= 4 guaranteed here).
        channel_config_set_transfer_data_size(&c, DMA_SIZE_32);
        dma_channel_configure(memcpy_dma_chan, &c, d, s, words, true);
    } else {
        // Async with tail bytes: single byte DMA covers the whole range.
        channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
        dma_channel_configure(memcpy_dma_chan, &c, d, s, num, true);
    }
}
