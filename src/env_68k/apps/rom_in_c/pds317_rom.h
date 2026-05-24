/* =================================================================== */
/*             MAPA DE PONTEIROS DA ROM DO COMPUTADOR PDS317          */
/*              (Gerado automaticamente via gera_includes.py)         */
/* =================================================================== */

#ifndef PDS317_ROM_H
#define PDS317_ROM_H

#define ROM_xmodemReceive            ((int (*)(void))0x8034)
#define ROM_xmodemTransmit           ((int (*)(void))0x8036)
#define ROM_delay                    ((int (*)(void))0x8074)
#define ROM_mymemset                 ((int (*)(void))0x808C)
#define ROM_init_uart                ((int (*)(void))0x80A4)
#define ROM_read_kbd                 ((int (*)(void))0x80D0)
#define ROM_uart_read                ((int (*)(void))0x80E0)
#define ROM_write_kbd                ((int (*)(void))0x80E2)
#define ROM_get_0x81                 ((int (*)(void))0x80F6)
#define ROM_get_0x87                 ((int (*)(void))0x8162)
#define ROM_set_keyboard_leds        ((int (*)(void))0x8188)
#define ROM_get_0x88                 ((int (*)(void))0x81F6)
#define ROM_get_packet               ((int (*)(void))0x8296)
#define ROM_reset_por_software       ((int (*)(void))0x8302)
#define ROM_set_transparent_mode     ((int (*)(void))0x830A)
#define ROM_get_kbd_key              ((int (*)(void))0x8348)
#define ROM_get_key                  ((int (*)(void))0x847E)

#endif /* PDS317_ROM_H */
