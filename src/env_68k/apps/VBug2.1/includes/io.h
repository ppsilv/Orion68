// Define um tipo chamado "putchar_func" que aceita um char
typedef void (*putchar_func)(unsigned char);
typedef unsigned char (*getchar_func)(void);

// Declara a variável global que vai apontar para a saída ativa
extern putchar_func cconout;
extern getchar_func cconin;

// Declara as funções principais de entrada e saída do sistema
void cconin_init(getchar_func entrada_padrao);
void cconout_init(putchar_func saida_padrao);
void print_string(const char *str);
void set_console_output(putchar_func nova_saida);