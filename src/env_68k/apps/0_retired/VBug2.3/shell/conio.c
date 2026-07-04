#include "shell/conio.h"
#include "stdio.h"
//#include "drivers/sys/duart.h"
//#include "drivers/display.h"
#include "drv_uart.h"
#include <string.h>
//#include "system/debug.h"

uint8_t get_key()
{
    uint8_t key = 0xFF;

    while (key == 0xFF)
    {
        key = uart0_read();
    }

    return key;
}
/*
bool has_key()
{
    return serial_has_char() || keyboard_has_key();
}
*/
void putchar(char c)
{
    uart0_write((char)c);

}

void _putchar_hex(char c)
{
    printf("%02X", (int)c);
}
