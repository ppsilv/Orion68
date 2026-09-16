#include <stdio.h>
#include <stdarg.h>
#include <string.h>




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

#include <stdarg.h>
#include <string.h>


/*
 * Converte double em string decimal, sem usar math.h (sem log10/pow),
 * so' multiplicacao/divisao por 10 em loop. 'prec' e' o numero de casas
 * decimais (6 se -1 for passado, igual ao printf padrao).
 * Escreve em 'buf' incluindo o sinal '-' se negativo.
 */
static void ftoa(double val, char *buf, int prec)
{
    if (prec < 0) prec = 6;

    int neg = 0;
    if (val < 0) { neg = 1; val = -val; }

    long ip = (long)val;              /* parte inteira */
    double frac = val - (double)ip;   /* parte fracionaria, 0.0 a 1.0 */

    long pow10 = 1;
    for (int i = 0; i < prec; i++) pow10 *= 10;

    for (int i = 0; i < prec; i++) frac *= 10.0;
    long fp = (long)(frac + 0.5);     /* arredonda */

    if (fp >= pow10) {                /* carry do arredondamento (ex: 0.999999 -> 1.000000) */
        fp -= pow10;
        ip += 1;
    }

    char ibuf[16];
    itodec(ip, ibuf);

    int pos = 0;
    if (neg) buf[pos++] = '-';
    for (char *p = ibuf; *p; p++) buf[pos++] = *p;

    if (prec > 0) {
        buf[pos++] = '.';
        /* escreve fp com 'prec' digitos, zero-padded a' esquerda */
        char fbuf[16];
        for (int i = prec - 1; i >= 0; i--) {
            fbuf[i] = '0' + (fp % 10);
            fp /= 10;
        }
        for (int i = 0; i < prec; i++) buf[pos++] = fbuf[i];
    }
    buf[pos] = 0;
}

/*
 * Escreve 'str' aplicando padding conforme width/flags, sem depender
 * de pad_zeros externo -- decide aqui mesmo, char a char, se preenche
 * com zero ou espaco, e de que lado (esquerda ou direita).
 */
static int emit_padded(const char *str, int width, int zero_pad, int left_align, int force_sign, int is_negative)
{
    int len = (int)strlen(str);
    int sign_len = (is_negative || force_sign) ? 1 : 0;
    int total = len + sign_len;
    int pad = (width > total) ? (width - total) : 0;
    int n = 0;

    if (!left_align && pad > 0) {
        char padchar = zero_pad ? '0' : ' ';
        /* Se for zero-pad com sinal, o sinal vem ANTES dos zeros (ex: -007, nao 00-7) */
        if (zero_pad && sign_len) {
            putchar(is_negative ? '-' : '+');
            n++;
            for (int i = 0; i < pad; i++) { putchar(padchar); n++; }
            puts(str);
            n += len;
            return n;
        }
        for (int i = 0; i < pad; i++) { putchar(padchar); n++; }
    }

    if (sign_len && !(zero_pad && !left_align)) {
        putchar(is_negative ? '-' : '+');
        n++;
    }

    puts(str);
    n += len;

    if (left_align && pad > 0) {
        for (int i = 0; i < pad; i++) { putchar(' '); n++; }
    }

    return n;
}

int printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    int chars_printed = 0;
    char buffer[32];

    while (*format) {
        if (*format == '%') {
            format++;

            int zero_pad = 0;
            int left_align = 0;
            int force_sign = 0;
            int width = 0;
            int precision = -1;
            int is_long = 0;

            /* Flags (podem vir em qualquer ordem, ex: %-05d) */
            for (;;) {
                if (*format == '0') { zero_pad = 1; format++; }
                else if (*format == '-') { left_align = 1; format++; }
                else if (*format == '+') { force_sign = 1; format++; }
                else break;
            }

            /* Width */
            while (*format >= '0' && *format <= '9') {
                width = width * 10 + (*format - '0');
                format++;
            }

            /* Precisao: .N (usada por %f; ignorada pelos demais por enquanto) */
            if (*format == '.') {
                format++;
                precision = 0;
                while (*format >= '0' && *format <= '9') {
                    precision = precision * 10 + (*format - '0');
                    format++;
                }
            }

            /* Length modifier: l aplicado de verdade (long/unsigned long) */
            if (*format == 'l') {
                is_long = 1;
                format++;
            }

            switch (*format) {
                case 'c': {
                    char c = (char)va_arg(args, int);
                    putchar(c);
                    chars_printed++;
                    break;
                }

                case 'd':
                case 'i': {
                    long num = is_long ? va_arg(args, long) : (long)va_arg(args, int);
                    int neg = (num < 0);
                    itodec(neg ? -num : num, buffer);
                    chars_printed += emit_padded(buffer, width, zero_pad, left_align, force_sign, neg);
                    break;
                }

                case 'u': {
                    unsigned long num = is_long ? va_arg(args, unsigned long) : (unsigned long)va_arg(args, unsigned int);
                    itoudec(num, buffer);
                    chars_printed += emit_padded(buffer, width, zero_pad, left_align, 0, 0);
                    break;
                }

                case 'f':
                case 'F': {
                    double num = va_arg(args, double);
                    char fbuf[48];
                    ftoa(num, fbuf, precision);
                    int neg = (fbuf[0] == '-');
                    chars_printed += emit_padded(neg ? fbuf + 1 : fbuf, width, zero_pad, left_align, force_sign, neg);
                    break;
                }

                case 's': {
                    char *str = va_arg(args, char*);
                    if (!str) str = "(null)";
                    chars_printed += emit_padded(str, width, 0 /* zero_pad nao vale pra string */, left_align, 0, 0);
                    break;
                }

                case 'x':
                case 'X': {
                    unsigned long num = is_long ? va_arg(args, unsigned long) : (unsigned long)va_arg(args, unsigned int);
                    itox(num, buffer);
                    chars_printed += emit_padded(buffer, width, zero_pad, left_align, 0, 0);
                    break;
                }

                case 'p': {
                    void *ptr = va_arg(args, void*);
                    puts("0x");
                    chars_printed += 2;
                    itox((unsigned long)ptr, buffer);
                    /* endereco sempre com zero-pad ate 8 digitos (32-bit), sem sinal */
                    chars_printed += emit_padded(buffer, 8, 1, 0, 0, 0);
                    break;
                }

                case 'o': {
                    unsigned long num = is_long ? va_arg(args, unsigned long) : (unsigned long)va_arg(args, unsigned int);
                    itooct(num, buffer);
                    chars_printed += emit_padded(buffer, width, zero_pad, left_align, 0, 0);
                    break;
                }

                case '%': {
                    putchar('%');
                    chars_printed++;
                    break;
                }

                default: {
                    char num_str[10] = {0};
                    putchar('%');
                    if (zero_pad) putchar('0');
                    if (left_align) putchar('-');
                    if (force_sign) putchar('+');
                    if (width > 0) {
                        itodec(width, num_str);
                        puts(num_str);
                    }
                    putchar(*format);
                    chars_printed += 2 + (width > 0 ? strlen(num_str) : 0)
                                    + zero_pad + left_align + force_sign;
                    break;
                }
            }
        } else {
            putchar(*format);
            chars_printed++;
        }

        format++;
    }

    va_end(args);
    return chars_printed;
}
