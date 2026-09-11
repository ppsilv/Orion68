// ============================================================
// kprintf.c - Printf para o kernel Orion68DOS
// Versão CORRETA e COMPILÁVEL!
// ============================================================

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <kernel.h>
#include <string.h>

// ============================================================
// FUNÇÃO DE SAÍDA (substitua pelo seu hardware!)
// ============================================================
#define PICO_VGA_BASE 0x00FF8000
#define WRITE_SCREEN   (*((volatile unsigned char *)(PICO_VGA_BASE + 0x01)))

static void kernel_putchar(char ch) {
    WRITE_SCREEN = ch;
}

void kernel_puts(const char *s) {
    while (*s) {
        kernel_putchar(*s++);
    }
}
void kernel_puthex(uint32_t v) {
    const char *hex = "0123456789ABCDEF";
    kernel_putchar('0');
    kernel_putchar('x');
    for (int i = 28; i >= 0; i -= 4) {
        kernel_putchar(hex[(v >> i) & 0xF]);
    }
}
// ============================================================
// CONVERSÃO DE NÚMEROS
// ============================================================

static char *itoa_kernel(int value, char *str, int base) {
    char *ptr = str;
    char *start = str;
    int num = value;
    bool negative = false;
    
    if (base < 2 || base > 36) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }
    
    if (num < 0 && base == 10) {
        negative = true;
        num = -num;
    }
    
    if (num == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return str;
    }
    
    while (num > 0) {
        int digit = num % base;
        if (digit < 10) {
            *ptr++ = '0' + digit;
        } else {
            *ptr++ = 'a' + (digit - 10);
        }
        num /= base;
    }
    
    if (negative) {
        *ptr++ = '-';
    }
    
    *ptr = '\0';
    
    // Inverte a string
    ptr--;
    while (start < ptr) {
        char temp = *start;
        *start++ = *ptr;
        *ptr-- = temp;
    }
    
    return str;
}

static char *ultoa_kernel(unsigned long value, char *str, int base) {
    char *ptr = str;
    char *start = str;
    unsigned long num = value;
    
    if (base < 2 || base > 36) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }
    
    if (num == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return str;
    }
    
    while (num > 0) {
        int digit = num % base;
        if (digit < 10) {
            *ptr++ = '0' + digit;
        } else {
            *ptr++ = 'a' + (digit - 10);
        }
        num /= base;
    }
    
    *ptr = '\0';
    
    // Inverte
    ptr--;
    while (start < ptr) {
        char temp = *start;
        *start++ = *ptr;
        *ptr-- = temp;
    }
    
    return str;
}

// Conversão de unsigned decimal (sem divisão)
char *itoudec(unsigned int value, char *str) {
    char *ptr = str;

    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return str;
    }

    // Potências de 10 para unsigned
    static const unsigned int powers[] = {
        1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1
    };

    int started = 0;

    for (int i = 0; i < 10; i++) {
        unsigned int power = powers[i];
        int digit = 0;

        while (value >= power) {
            value -= power;
            digit++;
        }

        if (digit != 0 || started) {
            *ptr++ = '0' + digit;
            started = 1;
        }
    }

    *ptr = '\0';
    return str;
}
char *itooct(unsigned int value, char *str) {
    char *ptr = str;
    int started = 0;

    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return str;
    }

    for (int shift = 30; shift >= 0; shift -= 3) {
        int triplet = (value >> shift) & 0x7;
        if (triplet != 0 || started) {
            *ptr++ = '0' + triplet;
            started = 1;
        }
    }

    *ptr = '\0';
    return str;
}
char *itodec(int value, char *str) {
    char *ptr = str;

    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return str;
    }

    if (value < 0) {
        *ptr++ = '-';
        value = -value;
    }

    unsigned int uvalue = value;

    // Potências de 10 pré-calculadas
    static const unsigned int powers[] = {
        1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1
    };

    int started = 0;

    for (int i = 0; i < 10; i++) {
        unsigned int power = powers[i];
        int digit = 0;

        // Contar subtrações (em vez de divisão)
        while (uvalue >= power) {
            uvalue -= power;
            digit++;
        }

        if (digit != 0 || started) {
            *ptr++ = '0' + digit;
            started = 1;
        }
    }

    *ptr = '\0';
    return str;
}
// Conversão de hexadecimal usando shifts (sem divisão)
char *itox(unsigned int value, char *str) {
    char *ptr = str;
    int started = 0;

    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return str;
    }

    for (int shift = 28; shift >= 0; shift -= 4) {
        int nibble = (value >> shift) & 0xF;
        if (nibble != 0 || started) {
            *ptr++ = "0123456789abcdef"[nibble];
            started = 1;
        }
    }

    *ptr = '\0';
    return str;
}
void pad_zeros(char *str, int width) {
    int len = strlen(str);
    if (len >= width) return;

    // Mover conteúdo para a direita
    for (int i = len; i >= 0; i--) {
        str[i + (width - len)] = str[i];
    }

    // Preencher com zeros à esquerda
    for (int i = 0; i < (width - len); i++) {
        str[i] = '0';
    }
}

// ============================================================
// kprintf - A FUNÇÃO PRINCIPAL (COMPILÁVEL!)
// ============================================================

void kprintf(const char *fmt, ...) {
    va_list args;
    char buffer[32];
    const char *p;
    
    va_start(args, fmt);
    
    for (p = fmt; *p; p++) {
        if (*p != '%') {
            kernel_putchar(*p);
            continue;
        }
        
        p++;
        if (!*p) break;
        
        switch (*p) {
            case 'c': {
                char c = (char)va_arg(args, int);
                kernel_putchar(c);
                break;
            }
            
            case 'd':
            case 'i': {
                int val = va_arg(args, int);
                itoa_kernel(val, buffer, 10);
                kernel_puts(buffer);
                break;
            }
            
            case 'u': {
                unsigned int val = va_arg(args, unsigned int);
                ultoa_kernel(val, buffer, 10);
                kernel_puts(buffer);
                break;
            }
            
            case 'x': {
                unsigned int val = va_arg(args, unsigned int);
                ultoa_kernel(val, buffer, 16);
                kernel_puts(buffer);
                break;
            }
            
            case 'X': {
                unsigned int val = va_arg(args, unsigned int);
                ultoa_kernel(val, buffer, 16);
                char *s = buffer;
                while (*s) {
                    if (*s >= 'a' && *s <= 'f') {
                        *s = *s - 'a' + 'A';
                    }
                    s++;
                }
                kernel_puts(buffer);
                break;
            }
            
            case 'p': {
                void *ptr = va_arg(args, void*);
                kernel_putchar('0');
                kernel_putchar('x');
                ultoa_kernel((unsigned long)ptr, buffer, 16);
                kernel_puts(buffer);
                break;
            }
            
            case 's': {
                char *str = va_arg(args, char*);
                if (str == NULL) {
                    kernel_puts("(null)");
                } else {
                    kernel_puts(str);
                }
                break;
            }
            
            case '%': {
                kernel_putchar('%');
                break;
            }
            
            default: {
                kernel_putchar('%');
                kernel_putchar(*p);
                break;
            }
        }
    }
    
    va_end(args);
}

void kputc(char c) {
    kernel_putchar(c);
}

void kputs(const char *s) {
    kernel_puts(s);
    kernel_putchar('\n');
}