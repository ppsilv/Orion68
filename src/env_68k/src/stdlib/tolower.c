// Converte uma string inteira para minúsculas
void tolower_str(char *str) {
    while (*str) {
        if (*str >= 'A' && *str <= 'Z') {
            *str += ('a' - 'A'); // Soma 32 para converter
        }
        str++;
    }
}
