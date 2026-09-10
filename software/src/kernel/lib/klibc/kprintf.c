// ============================================================
// kprintf.c - Printf para o kernel Orion68DOS
// Versão CORRETA e COMPILÁVEL!
// ============================================================

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <kernel.h>

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