



// Converte uma string inteira para maiúsculas
void toupper_str(char *str) {
    while (*str) {
        if (*str >= 'a' && *str <= 'z') {
            *str -= ('a' - 'A'); // Subtrai 32 para converter
        }
        str++;
    }
}
