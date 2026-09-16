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

// Conversão de decimal usando lookup table (sem divisão)
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

// Conversão de octal usando shifts (sem divisão)
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


// Nova função para preencher com zeros à esquerda
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

// Implementação principal do printf - COM SUPORTE A MODIFICADORES
void kprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    int chars_printed = 0;
    char buffer[32];

    while (*format) {
        if (*format == '%') {
            format++;

            // Verificar modificadores
            int zero_pad = 0;
            int width = 0;

            // Processar flags e width
            while (*format >= '0' && *format <= '9') {
                if (*format == '0' && width == 0) {
                    zero_pad = 1;
                } else {
                    width = width * 10 + (*format - '0');
                }
                format++;
            }

            // Processar length modifiers (simplificado)
            if (*format == 'l') {
                format++; // Ignorar 'l' por enquanto
            }

            switch (*format) {
                case 'c': {
                    char c = (char)va_arg(args, int);
                    kernel_putchar(c);
                    chars_printed++;
                    break;
                }

                case 'd':
                case 'i': {
                    int num = va_arg(args, int);
                    itodec(num, buffer);
                    if (zero_pad && width > 0) {
                        pad_zeros(buffer, width);
                    }
                    kernel_puts(buffer);
                    chars_printed += strlen(buffer);
                    break;
                }

                case 'u': {
                    unsigned int num = va_arg(args, unsigned int);
                    itoudec(num, buffer);
                    if (zero_pad && width > 0) {
                        pad_zeros(buffer, width);
                    }
                    kernel_puts(buffer);
                    chars_printed += strlen(buffer);
                    break;
                }

                case 'f':
                case 'F': {
                    // Ignorar floats
                    kernel_puts("[float]");
                    chars_printed += 7;
                    va_arg(args, double);
                    break;
                }

                case 's': {
                    char *str = va_arg(args, char*);
                    if (str) {
                        kernel_puts(str);
                        chars_printed += strlen(str);
                    } else {
                        kernel_puts("(null)");
                        chars_printed += 6;
                    }
                    break;
                }

                case 'x':
                case 'X': {
                    unsigned int num = va_arg(args, unsigned int);
                    itox(num, buffer);
                    if (zero_pad && width > 0) {
                        pad_zeros(buffer, width);
                    }
                    kernel_puts(buffer);
                    chars_printed += strlen(buffer);
                    break;
                }

                case 'o': {
                    unsigned int num = va_arg(args, unsigned int);
                    itooct(num, buffer);
                    if (zero_pad && width > 0) {
                        pad_zeros(buffer, width);
                    }
                    kernel_puts(buffer);
                    chars_printed += strlen(buffer);
                    break;
                }

                case '%': {
                    kernel_putchar('%');
                    chars_printed++;
                    break;
                }

                default:
                    char num_str[10]={0};
                    kernel_putchar('%');
                    if (zero_pad) kernel_putchar('0');
                    if (width > 0) {
                        itodec(width, num_str);
                        kernel_puts(num_str);
                    }
                    kernel_putchar(*format);
                    chars_printed += 2 + (width > 0 ? strlen(num_str) : 0);
                    break;
            }
        } else {
            kernel_putchar(*format);
            chars_printed++;
        }

        format++;
    }

    va_end(args);
    //return chars_printed;
}


void kputc(char c) {
    kernel_putchar(c);
}

void kputs(const char *s) {
    kernel_puts(s);
    kernel_putchar('\n');
}